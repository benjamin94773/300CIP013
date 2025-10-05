# Decomposition II – Parallel Component Labeling (PCAM)

## Description
Parallel connected-component labeling for a binary image using iterative minimum label propagation (4-connectivity).

## PCAM Design
- Partitioning: image divided by rows among threads.
- Communication: neighbor label reads; implicit barrier each iteration.
- Agglomeration: keep simple row chunks (low overhead, good locality).
- Mapping: static schedule balances equal row counts.

## Build
```bash
g++ -O2 -fopenmp component_labeling.cpp -o component_labeling
```

## Run
```bash
./component_labeling 512 512 0.4 8
```
Arguments: rows cols density threads.

## Example Output
```
Component Labeling Parallel (512x512) density=0.4 threads=8
Iterations=23 components=1573 time_ms=45.8
```
(Times vary.)
