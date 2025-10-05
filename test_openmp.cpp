#include <iostream>
#include <omp.h>
int main(){
#ifdef _OPENMP
    std::cout << "OpenMP habilitado. Version: " << _OPENMP << "\n";
#pragma omp parallel
    {
        #pragma omp critical
        std::cout << "Hola desde hilo " << omp_get_thread_num() << " de " << omp_get_num_threads() << "\n";
    }
#else
    std::cout << "OpenMP NO está habilitado. Compila con -fopenmp.\n";
#endif
    return 0;
}
