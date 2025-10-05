# Parallel Algorithms Project

Autores: Benjamin Ortiz, Marco Riascos, Rui Yu Lei Wu, Julian Galivis

Proyecto que implementa ejercicios de descomposición y análisis de rendimiento en C++ (C++17) utilizando OpenMP. Incluye:
- Decomposition I: tres problemas (producto matriz‑vector, hot plate, operaciones de matrices) con versiones a (serial fija), b (serial parametrizable) y c (paralela). Incluye versión optimizada adicional para multiplicación.
- Decomposition II: etiquetado de componentes (component labeling) con análisis PCAM.
- Algorithms Performance I: ejercicios de la Ley de Amdahl (archivo con cálculos y conclusiones).
- Algorithms Performance II: Game of Life (secuencial, paralela por filas y paralela con tiling) + informe de speedup basado en mediciones reales.

## Estructura de Carpetas
```
decomposition_I/
  matrix_vector/a.cpp b.cpp c.cpp
  hot_plate/a.cpp b.cpp c.cpp
  matrix_ops/a.cpp b.cpp c.cpp c_opt.cpp
  informe.md (resultados numéricos Decomposition I)
decomposition_II/
  component_labeling.cpp
  PCAM_component_labeling.md (análisis + resultados)
alg_performance_I/
  amdahl_analysis.md
alg_performance_II/
  game_of_life/
    game_of_life.cpp
    game_of_life_speedup.md
build_all.sh / build_all.ps1
run_examples.sh / run_examples.ps1
README_RUN.md (guía de ejecución detallada)
```

## Requisitos
- g++ con soporte OpenMP (`-fopenmp`).
- (Opcional) PowerShell en Windows para usar `build_all.ps1` y `run_examples.ps1`.

## Compilación
### Linux / WSL / Contenedor
```bash
chmod +x build_all.sh
./build_all.sh
```

### Windows (PowerShell)
```powershell
powershell -ExecutionPolicy Bypass -File .\build_all.ps1
```

Se generan los binarios en `./bin/` (Linux) o en la raíz con sufijo `.exe` (PowerShell). Principales ejecutables:
`mv_a mv_b mv_c`, `hp_a hp_b hp_c`, `mo_a mo_b mo_c mo_c_opt`, `component_labeling`, `game_of_life`.

## Ejecución Básica
```bash
./bin/mv_b 1000 1200
./bin/mv_c 8000 8000 8
./bin/hp_b 256 800
./bin/hp_c 256 800 8
./bin/mo_b 300 300 300
./bin/mo_c_opt 300 300 300 0   # tile=0 (sin tiling)
./bin/component_labeling 512 512 0.4 8
./bin/game_of_life 512 512 200 0.3
```

## Ajuste de Hilos
```bash
export OMP_NUM_THREADS=8
./bin/game_of_life 512 512 200 0.3
```
En Windows (PowerShell):
```powershell
$env:OMP_NUM_THREADS=8
./game_of_life.exe 512 512 200 0.3
```

## Informes Incluidos
- `decomposition_I/informe.md`: tabla de resultados (a,b,c) para los tres problemas.
- `decomposition_II/PCAM_component_labeling.md`: análisis PCAM + speedup (1 vs 8 hilos) etiquetado de componentes.
- `alg_performance_I/amdahl_analysis.md`: resolución completa de los 5 ejercicios de Amdahl (en español).
- `alg_performance_II/game_of_life/game_of_life_speedup.md`: resultados reales (5 repeticiones) y speedups del Game of Life.

## Métricas Clave (Resumen Breve)
- MatVec grande (8000×8000) speedup ~3.3 (8 hilos).
- Hot Plate (256²) slowdown en paralelo (overhead) con 8 hilos.
- Multiplicación matrices optimizada (300³) speedup ~4.5 vs serial.
- Component Labeling speedup 1→8 hilos ≈6.2 (eficiencia ~0.78).
- Game of Life (512², 200 steps) speedup row-wise ≈3.9×; tiled similar (~3.8×) promedio.

## Cómo Reproducir Game of Life (mediciones)
```bash
for i in {1..5}; do ./bin/game_of_life 512 512 200 0.3 >> gol_runs.txt; done
grep -E 'Sequential|Row-parallel|Tiled-parallel' gol_runs.txt
```

## Qué Subir al Repositorio Git
Sube TODO el contenido del proyecto excepto binarios generados (los binarios pueden regenerarse). Recomendado añadir un `.gitignore` para excluir:
```
bin/
*.exe
*.o
*.out
*.log
gol_runs.txt
```

## Pasos para Publicar en GitHub
Si el repositorio ya existe remoto (URL proporcionada):
```bash
git init               # si aún no hay repositorio local
git remote add origin https://github.com/benjamin94773/300CIP013.git

# Crear .gitignore si no existe
printf "bin/\n*.exe\n*.o\n*.out\n*.log\n" > .gitignore

git add .
git commit -m "Proyecto paralelo: Decomposition I & II + Amdahl + Game of Life speedups"
git push -u origin main  # o 'master' según rama por defecto del repo
```
Si la rama remota es `master`:
```bash
git push -u origin master
```

## Notas
- Ajustar flags (`-O3 -march=native`) si se buscan métricas máximas de rendimiento locales.
- Cambiar semillas si se desea uniformidad total entre versiones paralelas.

Fin.


