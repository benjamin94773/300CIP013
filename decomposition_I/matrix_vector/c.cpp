#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <omp.h>
#include <string>

// c.cpp: Matrix-Vector product paralelo (OpenMP) row-wise.
// Uso: ./mv_c <m> <n> [threads]

std::vector<double> gen_matrix(size_t m, size_t n) {
    std::mt19937_64 rng(5); std::uniform_real_distribution<double> dist(-1,1);
    std::vector<double> A(m*n); for(auto &v:A) v=dist(rng); return A; }
std::vector<double> gen_vec(size_t n) {
    std::mt19937_64 rng(6); std::uniform_real_distribution<double> dist(-1,1);
    std::vector<double> x(n); for(auto &v:x) v=dist(rng); return x; }

int main(int argc,char* argv[]){
    if(argc<3){ std::cerr<<"Uso: ./mv_c <m> <n> [threads]\n"; return 1; }
    size_t m=std::stoull(argv[1]); size_t n=std::stoull(argv[2]);
    int threads = (argc>=4)? std::stoi(argv[3]) : omp_get_max_threads();
    auto A=gen_matrix(m,n); auto x=gen_vec(n); std::vector<double> y(m,0.0);
    auto t1=std::chrono::high_resolution_clock::now();
#pragma omp parallel for num_threads(threads) schedule(static)
    for(long long i=0;i<(long long)m;++i){ double acc=0; const double* row=&A[i*n]; for(size_t j=0;j<n;++j) acc+=row[j]*x[j]; y[i]=acc; }
    auto t2=std::chrono::high_resolution_clock::now();
    double ms=std::chrono::duration<double,std::milli>(t2-t1).count();
    std::cout<<"[c.cpp] m="<<m<<" n="<<n<<" threads="<<threads<<" time_ms="<<ms<<"\n";
    return 0;
}
