#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <omp.h>
#include <string>

/*
 * Parallel Connected Component Labeling (Iterative Min-Propagation)
 * =================================================================
 * Model: 4-connectivity (up, down, left, right).
 * Algorithm:
 *  1. Initialize label[i] = i (unique) for foreground pixels (value 1), else -1.
 *  2. Iteratively relax: label[i] = min(label[i], labels of 4-neighbors) for foreground.
 *  3. Stop when no label changes in an iteration.
 * Parallelization (PCAM):
 *  P (Partitioning): domain decomposition by rows. The 2D image is split into horizontal strips.
 *  C (Communication): threads read neighbor labels (read-only), write their own cell. Border dependency across strips handled implicitly by reading global arrays; barrier between iterations.
 *  A (Agglomeration): simple – keep partition as is; cost dominated by memory access.
 *  M (Mapping): static scheduling distributes roughly equal rows to threads.
 */

struct LabelResult { int iterations; double elapsed_ms; int components; };

std::vector<int> generate_binary_image(size_t rows, size_t cols, double density) {
    std::mt19937_64 rng(1234);
    std::bernoulli_distribution bern(density);
    std::vector<int> img(rows * cols);
    for (auto &v : img) v = bern(rng) ? 1 : 0;
    return img;
}

LabelResult parallel_label(std::vector<int>& img, size_t rows, size_t cols, int threads) {
    size_t N = rows * cols;
    std::vector<int> labels(N, -1);
    // initialize labels
#pragma omp parallel for num_threads(threads) schedule(static)
    for (long long i = 0; i < (long long)N; ++i) if (img[i]) labels[i] = (int)i;

    bool changed = true;
    int iterations = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    while (changed) {
        changed = false;
#pragma omp parallel for num_threads(threads) schedule(static) reduction(|:changed)
        for (long long r = 0; r < (long long)rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                size_t idx = r * cols + c;
                if (labels[idx] < 0) continue; // background
                int current = labels[idx];
                int min_label = current;
                if (r > 0 && labels[idx - cols] >= 0) min_label = std::min(min_label, labels[idx - cols]);
                if (r + 1 < rows && labels[idx + cols] >= 0) min_label = std::min(min_label, labels[idx + cols]);
                if (c > 0 && labels[idx - 1] >= 0) min_label = std::min(min_label, labels[idx - 1]);
                if (c + 1 < cols && labels[idx + 1] >= 0) min_label = std::min(min_label, labels[idx + 1]);
                if (min_label < current) { labels[idx] = min_label; changed = true; }
            }
        }
        iterations++;
        if (iterations > (int)(rows + cols)) break; // safeguard worst-case
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    // Compress labels: second pass to assign contiguous component IDs
    std::vector<int> root(labels.begin(), labels.end());
    // Path compression style: propagate minimum again (optional)
#pragma omp parallel for num_threads(threads) schedule(static)
    for (long long i = 0; i < (long long)N; ++i) {
        if (root[i] < 0) continue;
        int lbl = root[i];
        while (lbl >= 0 && lbl != root[lbl]) lbl = root[lbl];
        root[i] = lbl;
    }
    // Map roots to compact IDs
    std::vector<int> unique;
    unique.reserve(N);
    for (auto v : root) if (v >= 0) unique.push_back(v);
    std::sort(unique.begin(), unique.end());
    unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
    int components = (int)unique.size();

    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
    std::cout << "Iterations=" << iterations << " components=" << components << " time_ms=" << ms << "\n";
    return {iterations, ms, components};
}

int main(int argc, char* argv[]) {
    size_t rows = 512, cols = 512; double density = 0.4; int threads = omp_get_max_threads();
    if (argc >= 3) { rows = std::stoull(argv[1]); cols = std::stoull(argv[2]); }
    if (argc >= 4) density = std::stod(argv[3]);
    if (argc >= 5) threads = std::stoi(argv[4]);

    std::cout << "Component Labeling Parallel (" << rows << "x" << cols << ") density=" << density << " threads=" << threads << "\n";
    auto img = generate_binary_image(rows, cols, density);
    parallel_label(img, rows, cols, threads);
    return 0;
}
