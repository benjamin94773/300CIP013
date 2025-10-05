#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <omp.h>
#include <string>
#include <cstdlib>

/*
 * c_opt.cpp: Versión optimizada paralela de Operaciones de Matrices.
 * Objetivo: Implementar la mejora #1 del informe:
 *   - Paralelizar internamente la multiplicación usando data parallel (collapse / tiling)
 *   - (Opcional) Paralelizar suma y transpuesta también a nivel de datos
 *   - Evitar el cuello de botella de functional decomposition (3 secciones)
 * Uso:
 *   ./mo_c_opt <m> <n> <k> [tile=0]
 * Si tile > 0 se aplica bloqueo (tiling) en (i,j,t) para mejorar localidad cache.
 * Hilos controlados por OMP_NUM_THREADS o OMP runtime.
 */

// Genera matriz aleatoria m x n
std::vector<double> rnd(size_t m,size_t n,unsigned seed){
    std::mt19937_64 r(seed); std::uniform_real_distribution<double>d(-1,1);
    std::vector<double>A(m*n); for(auto &v:A) v=d(r); return A; }

// Suma paralela C=A+B
void add_parallel(const std::vector<double>&A,const std::vector<double>&B,std::vector<double>&C){
    size_t N=C.size();
    #pragma omp parallel for schedule(static)
    for(long long i=0;i<(long long)N;++i) C[i]=A[i]+B[i];
}

// Transpuesta paralela T = A^T (A: m x n, T: n x m)
void transpose_parallel(const std::vector<double>&A,std::vector<double>&T,size_t m,size_t n){
    #pragma omp parallel for collapse(2) schedule(static)
    for(long long i=0;i<(long long)m;++i)
        for(long long j=0;j<(long long)n;++j)
            T[j*m + i] = A[i*n + j];
}

// Multiplicación paralela C = A * B   (A: m x n, B: n x k, C: m x k)
// Estrategias:
//  - Si tile == 0: bucles i,j paralelos (collapse(2)), bucle interno en t
//  - Si tile > 0: triple bucle bloqueado (ii, jj, tt) + bucle interno
void multiply_parallel(const std::vector<double>&A,const std::vector<double>&B,std::vector<double>&C,
                       size_t m,size_t n,size_t k,int tile){
    if(tile <= 0){
        #pragma omp parallel for collapse(2) schedule(static)
        for(long long i=0;i<(long long)m;++i){
            for(long long j=0;j<(long long)k;++j){
                double sum = 0.0;
                for(size_t t=0;t<n;++t) sum += A[i*n + t] * B[t*k + j];
                C[i*k + j] = sum;
            }
        }
    } else {
        int T = tile;
        // Bloques  (ii, jj) y dentro bloque reducción en t (tt)
        #pragma omp parallel for collapse(2) schedule(static)
        for(long long ii=0; ii<(long long)m; ii+=T){
            for(long long jj=0; jj<(long long)k; jj+=T){
                int i_end = std::min<long long>(ii+T, m);
                int j_end = std::min<long long>(jj+T, k);
                for(int i=ii; i<i_end; ++i){
                    for(int j=jj; j<j_end; ++j){
                        double sum = 0.0;
                        for(int tt=0; tt<(int)n; ++tt) sum += A[i*n + tt] * B[tt*k + j];
                        C[i*k + j] = sum;
                    }
                }
            }
        }
    }
}

int main(int argc,char* argv[]){
    if(argc < 4){
        std::cerr << "Uso: ./mo_c_opt <m> <n> <k> [tile=0]\n";
        return 1;
    }
    size_t m = std::stoull(argv[1]);
    size_t n = std::stoull(argv[2]);
    size_t k = std::stoull(argv[3]);
    int tile = (argc>=5)? std::stoi(argv[4]) : 0;

    auto A = rnd(m,n,1001);
    auto Bsame = rnd(m,n,1002);      // para suma mismas dimensiones que A
    auto Bmul = rnd(n,k,1003);       // para multiplicación (n x k)
    std::vector<double> C_add(m*n), C_mul(m*k), A_T(n*m);

    auto now = [](){ return std::chrono::high_resolution_clock::now(); };
    auto dt_ms = [](auto a, auto b){return std::chrono::duration<double,std::milli>(b-a).count();};

    auto t1 = now(); add_parallel(A,Bsame,C_add); auto t2 = now();
    auto t3 = now(); multiply_parallel(A,Bmul,C_mul,m,n,k,tile); auto t4 = now();
    auto t5 = now(); transpose_parallel(A,A_T,m,n); auto t6 = now();

    double add_ms = dt_ms(t1,t2);
    double mul_ms = dt_ms(t3,t4);
    double tr_ms  = dt_ms(t5,t6);
    // FLOPs aproximados de multiplicación: 2*m*n*k (mult+sum)
    double gflops = (2.0 * m * n * k) / (mul_ms * 1e6); // (ops / 1e9) * (1e3/ms) => divide por 1e6

    std::cout << "[matrix_ops c_opt.cpp] m="<<m<<" n="<<n<<" k="<<k
              << " tile="<<tile
              << " add_ms="<<add_ms
              << " mul_ms="<<mul_ms
              << " trans_ms="<<tr_ms
              << " GFLOPS_mul="<<gflops
              << "\n";
    return 0;
}
