#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

// a.cpp: Hot Plate serial versión fija 256x256 (requisito 2a)
// Regla: new = (up+down+left+right + 4*center)/8
// Conv: cuenta celdas calientes si |center - avg(neigh)| > 0.1

int main(){
    size_t N=256; int max_iters=1000; 
    std::vector<double> cur(N*N,0.0), next(N*N,0.0);
    // semilla caliente central
    cur[(N/2)*N + (N/2)] = 10.0; cur[(N/2)*N + (N/2-1)] = 5.0;
    auto t1=std::chrono::high_resolution_clock::now();
    int iter=0; int hot=0;
    for(;iter<max_iters;++iter){
        hot=0;
        for(size_t i=1;i<N-1;++i){
            for(size_t j=1;j<N-1;++j){
                size_t idx=i*N+j;
                double c=cur[idx];
                double up=cur[(i-1)*N+j], down=cur[(i+1)*N+j];
                double left=cur[i*N+(j-1)], right=cur[i*N+(j+1)];
                double newv=(up+down+left+right+4*c)/8.0; next[idx]=newv;
                double avg=(up+down+left+right)/4.0; if(std::fabs(c-avg)>0.1) hot++;
            }
        }
        std::swap(cur,next);
        if(hot==0) break;
    }
    auto t2=std::chrono::high_resolution_clock::now();
    double ms=std::chrono::duration<double,std::milli>(t2-t1).count();
    std::cout<<"[hot_plate a.cpp] N="<<N<<" iterations="<<iter+1<<" hot_final="<<hot<<" time_ms="<<ms<<"\n";
    return 0;
}
