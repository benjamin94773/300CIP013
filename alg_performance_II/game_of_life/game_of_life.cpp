#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <omp.h>

/*
 * Conway's Game of Life – Sequential and Parallel Versions
 * -------------------------------------------------------
 * Versions:
 *  1. Sequential (single-thread)
 *  2. Parallel row-wise (OpenMP for loop)
 *  3. Parallel with 2D domain decomposition (still OpenMP; tiling for cache) – pseudo "fully" parallel.
 * Decomposition strategies explained in comments below.
 */

struct LifeResult { double seq_ms; double row_ms; double tile_ms; };

inline int idx(int r, int c, int cols) { return r * cols + c; }

void random_board(std::vector<int>& board, int rows, int cols, double density, unsigned seed=42) {
    std::mt19937 rng(seed); std::bernoulli_distribution bern(density);
    for (auto &v : board) v = bern(rng) ? 1 : 0;
}

int live_neighbors(const std::vector<int>& board, int r, int c, int rows, int cols) {
    int sum = 0;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) sum += board[idx(nr,nc,cols)];
        }
    }
    return sum;
}

void step_sequential(const std::vector<int>& cur, std::vector<int>& next, int rows, int cols) {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            int ln = live_neighbors(cur, r, c, rows, cols);
            int alive = cur[idx(r,c,cols)];
            next[idx(r,c,cols)] = (alive && (ln==2 || ln==3)) || (!alive && ln==3);
        }
}

void step_row_parallel(const std::vector<int>& cur, std::vector<int>& next, int rows, int cols) {
#pragma omp parallel for schedule(static)
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int ln = live_neighbors(cur, r, c, rows, cols);
            int alive = cur[idx(r,c,cols)];
            next[idx(r,c,cols)] = (alive && (ln==2 || ln==3)) || (!alive && ln==3);
        }
    }
}

void step_tiled_parallel(const std::vector<int>& cur, std::vector<int>& next, int rows, int cols, int tile) {
#pragma omp parallel for collapse(2) schedule(static)
    for (int tr = 0; tr < rows; tr += tile) {
        for (int tc = 0; tc < cols; tc += tile) {
            int r_end = std::min(tr + tile, rows);
            int c_end = std::min(tc + tile, cols);
            for (int r = tr; r < r_end; ++r)
                for (int c = tc; c < c_end; ++c) {
                    int ln = live_neighbors(cur, r, c, rows, cols);
                    int alive = cur[idx(r,c,cols)];
                    next[idx(r,c,cols)] = (alive && (ln==2 || ln==3)) || (!alive && ln==3);
                }
        }
    }
}

LifeResult run_life(int rows, int cols, int steps, double density) {
    std::vector<int> board(rows*cols), cur, next;
    random_board(board, rows, cols, density);

    cur = board; next = cur;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int s = 0; s < steps; ++s) { step_sequential(cur, next, rows, cols); std::swap(cur,next); }
    auto t2 = std::chrono::high_resolution_clock::now();
    double seq_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    random_board(board, rows, cols, density, 43); cur = board; next = cur;
    auto t3 = std::chrono::high_resolution_clock::now();
    for (int s = 0; s < steps; ++s) { step_row_parallel(cur, next, rows, cols); std::swap(cur,next); }
    auto t4 = std::chrono::high_resolution_clock::now();
    double row_ms = std::chrono::duration<double, std::milli>(t4 - t3).count();

    random_board(board, rows, cols, density, 44); cur = board; next = cur;
    int tile = 32;
    auto t5 = std::chrono::high_resolution_clock::now();
    for (int s = 0; s < steps; ++s) { step_tiled_parallel(cur, next, rows, cols, tile); std::swap(cur,next); }
    auto t6 = std::chrono::high_resolution_clock::now();
    double tile_ms = std::chrono::duration<double, std::milli>(t6 - t5).count();

    return {seq_ms, row_ms, tile_ms};
}

int main(int argc, char* argv[]) {
    int rows = 512, cols = 512, steps = 200; double density = 0.3;
    if (argc >= 4) { rows = std::stoi(argv[1]); cols = std::stoi(argv[2]); steps = std::stoi(argv[3]); }
    if (argc >= 5) density = std::stod(argv[4]);

    auto res = run_life(rows, cols, steps, density);
    std::cout << "Game of Life Performance (" << rows << "x" << cols << ", steps=" << steps << ")\n";
    std::cout << "Sequential ms: " << res.seq_ms << "\n";
    std::cout << "Row-parallel ms: " << res.row_ms << " speedup=" << res.seq_ms / res.row_ms << "\n";
    std::cout << "Tiled-parallel ms: " << res.tile_ms << " speedup=" << res.seq_ms / res.tile_ms << "\n";
    return 0;
}
