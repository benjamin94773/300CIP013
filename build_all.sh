#!/usr/bin/env bash
set -euo pipefail
CXX=${CXX:-g++}
CXXFLAGS="-O2 -std=c++17 -fopenmp"

echo "Compilando matrix_vector a,b,c"; \
	$CXX -O2 -std=c++17 decomposition_I/matrix_vector/a.cpp -o bin/mv_a; \
	$CXX -O2 -std=c++17 decomposition_I/matrix_vector/b.cpp -o bin/mv_b; \
	$CXX $CXXFLAGS decomposition_I/matrix_vector/c.cpp -o bin/mv_c

echo "Compilando hot_plate a,b,c"; \
	$CXX -O2 -std=c++17 decomposition_I/hot_plate/a.cpp -o bin/hp_a; \
	$CXX -O2 -std=c++17 decomposition_I/hot_plate/b.cpp -o bin/hp_b; \
	$CXX $CXXFLAGS decomposition_I/hot_plate/c.cpp -o bin/hp_c

echo "Compilando matrix_ops a,b,c"; \
	$CXX -O2 -std=c++17 decomposition_I/matrix_ops/a.cpp -o bin/mo_a; \
	$CXX -O2 -std=c++17 decomposition_I/matrix_ops/b.cpp -o bin/mo_b; \
	$CXX $CXXFLAGS decomposition_I/matrix_ops/c.cpp -o bin/mo_c

echo "Compilando component_labeling"; $CXX $CXXFLAGS decomposition_II/component_labeling.cpp -o bin/component_labeling

echo "Compilando game_of_life"; $CXX $CXXFLAGS alg_performance_II/game_of_life/game_of_life.cpp -o bin/game_of_life

echo "DONE"