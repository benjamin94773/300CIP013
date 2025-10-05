<#
Compila todos los programas con g++ y OpenMP.
Uso:
  powershell -ExecutionPolicy Bypass -File .\build_all.ps1
#>

$ErrorActionPreference = 'Stop'

function Build($src, $out) {
  Write-Host "Compilando $src -> $out" -ForegroundColor Cyan
  & g++ -O2 -std=c++17 -fopenmp $src -o $out
  if ($LASTEXITCODE -ne 0) { throw "Fallo compilando $src" }
}

Build "decomposition_I/matrix_vector/a.cpp" "mv_a.exe"
Build "decomposition_I/matrix_vector/b.cpp" "mv_b.exe"
Build "decomposition_I/matrix_vector/c.cpp" "mv_c.exe"

Build "decomposition_I/hot_plate/a.cpp" "hp_a.exe"
Build "decomposition_I/hot_plate/b.cpp" "hp_b.exe"
Build "decomposition_I/hot_plate/c.cpp" "hp_c.exe"

Build "decomposition_I/matrix_ops/a.cpp" "mo_a.exe"
Build "decomposition_I/matrix_ops/b.cpp" "mo_b.exe"
Build "decomposition_I/matrix_ops/c.cpp" "mo_c.exe"
Build "decomposition_II/component_labeling.cpp" "component_labeling.exe"
Build "alg_performance_II/game_of_life/game_of_life.cpp" "game_of_life.exe"

Write-Host "Compilación completa" -ForegroundColor Green
