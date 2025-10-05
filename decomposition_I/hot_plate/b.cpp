#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <random>
#include <string>

// b.cpp: Hot Plate serial con tamaño N vía CLI (requisito 2b)
// Uso: ./hp_b <N> [max_iters]

int main(int argc,char* argv[]){
    if(argc<2){ std::cerr<<"Uso: ./hp_b <N> [max_iters]\n"; return 1; }
    size_t N=std::stoull(argv[1]); int max_iters=(argc>=3)? std::stoi(argv[2]):1000;
    std::vector<double> cur(N*N,0.0), next(N*N,0.0);
    // puntos aleatorios calientes
    std::mt19937_64 rng(7); std::uniform_int_distribution<size_t> d(1,N-2);
    for(int k=0;k<5 && N>4;++k){ cur[d(rng)*N + d(rng)] = 10.0; }
    auto t1=std::chrono::high_resolution_clock::now();
    int iter=0; int hot=0;
    for(;iter<max_iters;++iter){
        hot=0;
        for(size_t i=1;i<N-1;++i){
            for(size_t j=1;j<N-1;++j){
                size_t idx=i*N+j; double c=cur[idx];
                double up=cur[(i-1)*N+j], down=cur[(i+1)*N+j];
                double left=cur[i*N+(j-1)], right=cur[i*N+(j+1)];
                double newv=(up+down+left+right+4*c)/8.0; next[idx]=newv;
                double avg=(up+down+left+right)/4.0; if(std::fabs(c-avg)>0.1) hot++;
            }
        }
        std::swap(cur,next); if(hot==0) break;
    }
    auto t2=std::chrono::high_resolution_clock::now();
    double ms=std::chrono::duration<double,std::milli>(t2-t1).count();
    std::cout<<"[hot_plate b.cpp] N="<<N<<" iterations="<<iter+1<<" hot_final="<<hot<<" time_ms="<<ms<<"\n";
    return 0;
}
