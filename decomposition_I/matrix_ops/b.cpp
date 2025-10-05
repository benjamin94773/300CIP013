#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>

// b.cpp: Operaciones de matrices serial con tamaños vía CLI (requisito 3b)
// Uso: ./mo_b <m> <n> <k>

std::vector<double> rnd(size_t m,size_t n,unsigned seed){ std::mt19937_64 r(seed); std::uniform_real_distribution<double>d(-1,1); std::vector<double>A(m*n); for(auto &v:A)v=d(r); return A; }
void add(const std::vector<double>&A,const std::vector<double>&B,std::vector<double>&C){ for(size_t i=0;i<C.size();++i) C[i]=A[i]+B[i]; }
void mul(const std::vector<double>&A,const std::vector<double>&B,std::vector<double>&C,size_t m,size_t n,size_t k){ for(size_t i=0;i<m;++i) for(size_t j=0;j<k;++j){ double s=0; for(size_t t=0;t<n;++t) s+=A[i*n+t]*B[t*k+j]; C[i*k+j]=s; }}
void trans(const std::vector<double>&A,std::vector<double>&T,size_t m,size_t n){ for(size_t i=0;i<m;++i) for(size_t j=0;j<n;++j) T[j*m+i]=A[i*n+j]; }

int main(int argc,char* argv[]){ if(argc<4){ std::cerr<<"Uso: ./mo_b <m> <n> <k>\n"; return 1; }
 size_t m=std::stoull(argv[1]), n=std::stoull(argv[2]), k=std::stoull(argv[3]);
 auto A=rnd(m,n,11), B=rnd(m,n,12), X=rnd(n,k,13); std::vector<double>Cadd(m*n), Cmul(m*k), AT(n*m);
 auto t1=std::chrono::high_resolution_clock::now(); add(A,B,Cadd); auto t2=std::chrono::high_resolution_clock::now();
 auto t3=std::chrono::high_resolution_clock::now(); mul(A,X,Cmul,m,n,k); auto t4=std::chrono::high_resolution_clock::now();
 auto t5=std::chrono::high_resolution_clock::now(); trans(A,AT,m,n); auto t6=std::chrono::high_resolution_clock::now();
 double add_ms=std::chrono::duration<double,std::milli>(t2-t1).count();
 double mul_ms=std::chrono::duration<double,std::milli>(t4-t3).count();
 double tr_ms=std::chrono::duration<double,std::milli>(t6-t5).count();
 std::cout<<"[matrix_ops b.cpp] m="<<m<<" n="<<n<<" k="<<k<<" add_ms="<<add_ms<<" mul_ms="<<mul_ms<<" trans_ms="<<tr_ms<<"\n"; return 0; }
