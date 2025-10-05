#include <iostream>
#include <vector>
#include <random>
#include <chrono>

// a.cpp: Matrix-Vector product (serial) con tamaños arbitrarios internos.
// Requisito 1(a): serial versión con matriz m x n y vector n arbitrarios.

std::vector<double> gen_matrix(size_t m, size_t n) {
    std::mt19937_64 rng(1);
    std::uniform_real_distribution<double> dist(-1,1);
    std::vector<double> A(m*n); for (auto &v:A) v = dist(rng); return A;
}
std::vector<double> gen_vec(size_t n) {
    std::mt19937_64 rng(2);
    std::uniform_real_distribution<double> dist(-1,1);
    std::vector<double> x(n); for (auto &v:x) v = dist(rng); return x;
}

int main(){
    size_t m=8, n=10; // tamaños arbitrarios
    auto A = gen_matrix(m,n);
    auto x = gen_vec(n);
    std::vector<double> y(m,0.0);
    auto t1 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<m;++i){
        double acc=0; const double* row=&A[i*n];
        for(size_t j=0;j<n;++j) acc += row[j]*x[j];
        y[i]=acc;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double,std::milli>(t2-t1).count();
    std::cout << "[a.cpp] m="<<m<<" n="<<n<<" time_ms="<<ms<<"\n";
    // Muestra primeros elementos
    std::cout << "y[0..min(4)]:";
    for(size_t i=0;i<std::min<size_t>(5,m);++i) std::cout<<' '<<y[i];
    std::cout<<"\n";
    return 0;
}
