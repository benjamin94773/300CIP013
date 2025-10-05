#include <iostream>
#include <vector>
#include <random>
#include <chrono>

// a.cpp: Operaciones de matrices (suma, multiplicación, transpuesta) serial con tamaños internos.
// Requisito 3a.

std::vector<double> rnd(size_t m,size_t n,unsigned seed){ std::mt19937_64 r(seed); std::uniform_real_distribution<double>d(-1,1); std::vector<double>A(m*n); for(auto &v:A)v=d(r); return A; }

void add(const std::vector<double>&A,const std::vector<double>&B,std::vector<double>&C){ for(size_t i=0;i<C.size();++i) C[i]=A[i]+B[i]; }
void mul(const std::vector<double>&A,const std::vector<double>&B,std::vector<double>&C,size_t m,size_t n,size_t k){ for(size_t i=0;i<m;++i) for(size_t j=0;j<k;++j){ double s=0; for(size_t t=0;t<n;++t) s+=A[i*n+t]*B[t*k+j]; C[i*k+j]=s; }}
void trans(const std::vector<double>&A,std::vector<double>&T,size_t m,size_t n){ for(size_t i=0;i<m;++i) for(size_t j=0;j<n;++j) T[j*m+i]=A[i*n+j]; }

int main(){ size_t m=60,n=70,k=80; auto A=rnd(m,n,1),B=rnd(m,n,2),X=rnd(n,k,3); std::vector<double>Cadd(m*n),Cmul(m*k),AT(n*m);
 auto t1=std::chrono::high_resolution_clock::now(); add(A,B,Cadd); auto t2=std::chrono::high_resolution_clock::now();
 auto t3=std::chrono::high_resolution_clock::now(); mul(A,X,Cmul,m,n,k); auto t4=std::chrono::high_resolution_clock::now();
 auto t5=std::chrono::high_resolution_clock::now(); trans(A,AT,m,n); auto t6=std::chrono::high_resolution_clock::now();
 auto add_ms=std::chrono::duration<double,std::milli>(t2-t1).count();
 auto mul_ms=std::chrono::duration<double,std::milli>(t4-t3).count();
 auto tr_ms=std::chrono::duration<double,std::milli>(t6-t5).count();
 std::cout<<"[matrix_ops a.cpp] m="<<m<<" n="<<n<<" k="<<k<<" add_ms="<<add_ms<<" mul_ms="<<mul_ms<<" trans_ms="<<tr_ms<<"\n"; }
