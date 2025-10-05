#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>

// b.cpp: Matrix-Vector product (serial) con tamaños m y n pasados por CLI.
// Uso: ./mv_b <m> <n>

std::vector<double> gen_matrix(size_t m, size_t n) {
    std::mt19937_64 rng(3); std::uniform_real_distribution<double> dist(-1,1);
    std::vector<double> A(m*n); for(auto &v:A) v=dist(rng); return A; }
std::vector<double> gen_vec(size_t n) {
    std::mt19937_64 rng(4); std::uniform_real_distribution<double> dist(-1,1);
    std::vector<double> x(n); for(auto &v:x) v=dist(rng); return x; }

int main(int argc,char* argv[]){
    if(argc<3){ std::cerr<<"Uso: ./mv_b <m> <n>\n"; return 1; }
    size_t m=std::stoull(argv[1]); size_t n=std::stoull(argv[2]);
    auto A=gen_matrix(m,n); auto x=gen_vec(n); std::vector<double> y(m,0.0);
    auto t1=std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<m;++i){ double acc=0; const double* row=&A[i*n]; for(size_t j=0;j<n;++j) acc+=row[j]*x[j]; y[i]=acc; }
    auto t2=std::chrono::high_resolution_clock::now();
    double ms=std::chrono::duration<double,std::milli>(t2-t1).count();
    std::cout<<"[b.cpp] m="<<m<<" n="<<n<<" time_ms="<<ms<<"\n";
    return 0;
}
