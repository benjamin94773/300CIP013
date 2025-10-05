# Decomposition I – Ejecución de Programas (a, b, c)

Este documento explica cómo compilar y ejecutar cada subprograma (a.cpp, b.cpp, c.cpp) para los tres problemas.

Compilación genérica (Linux / contenedor):
```bash
g++ -O2 -std=c++17 -fopenmp <archivo>.cpp -o <binario>
```

## 1. Matrix-Vector Product
Archivos en `decomposition_I/matrix_vector/`

- a.cpp (1a): Versión serial con tamaños internos arbitrarios.
  ```bash
  g++ -O2 -std=c++17 decomposition_I/matrix_vector/a.cpp -o mv_a
  ./mv_a
  ```
- b.cpp (1b): Versión serial con argumentos m y n.
  ```bash
  g++ -O2 -std=c++17 decomposition_I/matrix_vector/b.cpp -o mv_b
  ./mv_b 1000 1200
  ```
- c.cpp (1c): Versión paralela OpenMP (row-wise). Argumentos m n [threads].
  ```bash
  g++ -O2 -std=c++17 -fopenmp decomposition_I/matrix_vector/c.cpp -o mv_c
  ./mv_c 1000 1200 8
  ```

## 2. Hot Plate Problem
Archivos en `decomposition_I/hot_plate/`

- a.cpp (2a): Versión serial fija 256x256.
  ```bash
  g++ -O2 -std=c++17 decomposition_I/hot_plate/a.cpp -o hp_a
  ./hp_a
  ```
- b.cpp (2b): Versión serial con tamaño N vía CLI.
  ```bash
  g++ -O2 -std=c++17 decomposition_I/hot_plate/b.cpp -o hp_b
  ./hp_b 256 800    # N=256, max_iters=800
  ```
- c.cpp (2c): Versión paralela OpenMP. Argumentos N [max_iters] [threads].
  ```bash
  g++ -O2 -std=c++17 -fopenmp decomposition_I/hot_plate/c.cpp -o hp_c
  ./hp_c 512 1000 8
  ```

## 3. Matrix Operations (Add, Mul, Transpose)
Archivos en `decomposition_I/matrix_ops/`

- a.cpp (3a): Serial con tamaños internos.
  ```bash
  g++ -O2 -std=c++17 decomposition_I/matrix_ops/a.cpp -o mo_a
  ./mo_a
  ```
- b.cpp (3b): Serial con tamaños m n k vía CLI.
  ```bash
  g++ -O2 -std=c++17 decomposition_I/matrix_ops/b.cpp -o mo_b
  ./mo_b 200 200 200
  ```
- c.cpp (3c): Paralelo (functional decomposition con OpenMP sections).
  ```bash
  g++ -O2 -std=c++17 -fopenmp decomposition_I/matrix_ops/c.cpp -o mo_c
  ./mo_c 200 200 200
  ```
  
- c_opt.cpp (3c optimizado extra, NO formaba parte original pero implementa mejora #1): paraleliza internamente la multiplicación (data parallel) y también suma y transpuesta.
  ```bash
  g++ -O2 -std=c++17 -fopenmp decomposition_I/matrix_ops/c_opt.cpp -o mo_c_opt
  ./mo_c_opt 200 200 200        # sin tiling
  ./mo_c_opt 300 300 300 32     # con tiling 32
  ```
  Salida incluye GFLOPS aproximados de la multiplicación.

## Recolección de Resultados
Para tu informe guarda (copia/pega o redirige) las salidas:
```bash
./mv_a > results_mv_a.txt
./mv_b 2000 2000 > results_mv_b.txt
./mv_c 2000 2000 8 > results_mv_c.txt
# ... y así para los demás
```

## Sugerencias de Tamaños para Benchmark
- Matrix-Vector: 1000x1000, 2000x2000, 4000x2000
- Hot Plate: N=256, 512, 1024 (ajusta max_iters si tarda mucho)
- Matrix Ops: 200, 300, 400 (cuidado: multiplicación crece O(n^3))

## Control de Hilos OpenMP
```bash
export OMP_NUM_THREADS=8
./mv_c 2000 2000
```
Si pasas `[threads]` en c.cpp de matrix-vector o hot plate, ese valor manda.

## Verificación Rápida de OpenMP Disponible
```bash
echo "#include <omp.h>\n#include <iostream>\nint main(){#ifdef _OPENMP std::cout<<_OPENMP;#endif}" > test.cpp
g++ -fopenmp test.cpp -o test && ./test
```

Listo. Ejecuta y pásame los resultados para ayudarte a redactar el informe.
