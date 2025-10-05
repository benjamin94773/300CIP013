# Decomposition I – Resultados de las Versiones (a, b, c)

Este documento presenta únicamente los resultados obtenidos (tiempos y speedups cuando aplica) para cada uno de los tres problemas solicitados en Decomposition I, con sus versiones: (a) serial con tamaños/valores arbitrarios, (b) serial parametrizable por CLI, (c) paralela según el tipo de descomposición indicado.

Convención: Speedup S = T_serial / T_parallel usando como baseline la versión (b) con el mismo tamaño. Cuando no hay tamaño comparable no se reporta speedup.

---
## 1. Producto Matriz-Vector

### 1.a Serial (arbitrario)
- Tamaño interno usado: 8 × 10
- Tiempo: 0.00019 ms (referencial, demasiado pequeño para análisis)

### 1.b Serial con argumentos
| m × n | Tiempo (ms) |
|-------|-------------|
| 1000 × 1200 | 0.966461 |
| 8000 × 8000 | 59.1749 |

### 1.c Paralelo (data decomposition row-wise)
| m × n | Hilos | Tiempo (ms) | Speedup vs (b) |
|-------|-------|-------------|----------------|
| 1000 × 1200 | 8 (arg) | 1.46938 | 0.657 |
| 8000 × 8000 | 8 (arg) | 21.8567 | 2.71 |
| 8000 × 8000 | 4 (env) | 19.0349 | 3.11 |
| 8000 × 8000 | 8 (env) | 17.9474 | 3.30 |

---
## 2. Hot Plate Problem

### 2.a Serial (256×256 fijo)
| N | Iteraciones | Tiempo (ms) | Hot cells finale |
|---|-------------|-------------|------------------|
| 256 | 14 | 0.992861 | 0 |

### 2.b Serial con argumentos
| N | Iteraciones | Tiempo (ms) | Hot cells finale |
|---|-------------|-------------|------------------|
| 256 | 12 | 0.827036 | 0 |

### 2.c Paralelo (domain decomposition por filas)
| N | Hilos | Iteraciones | Tiempo (ms) | Speedup vs (b) |
|---|-------|-------------|-------------|----------------|
| 256 | 8 | 12 | 1.20248 | 0.688 |
| 512 | 8 | 12 | 1.78297 | — |

---
## 3. Operaciones de Matrices (Suma, Multiplicación, Transpuesta)

### 3.a Serial (arbitrario)
Tamaños internos (60×70×80) sólo verificación.
| Operación | Tiempo (ms) |
|-----------|-------------|
| Suma | 0.001793 |
| Multiplicación | 0.18314 |
| Transpuesta | 0.001463 |

### 3.b Serial con argumentos
Tamaño 200³ (m = n = k = 200)
| Operación | Tiempo (ms) |
|-----------|-------------|
| Suma | 0.033335 |
| Multiplicación | 5.68569 |
| Transpuesta | 0.06118 |

Tamaño 300³ (m = n = k = 300)
| Operación | Tiempo (ms) |
|-----------|-------------|
| Suma | 0.114855 |
| Multiplicación | 20.1363 |
| Transpuesta | 0.116599 |

### 3.c Paralelo (functional decomposition: add, mult, trans en secciones)
Tamaño 200³
| Operación | Tiempo (ms) | Speedup vs (b) |
|-----------|-------------|----------------|
| Suma | 0.07608 | 0.44 |
| Multiplicación | 6.506 | 0.87 |
| Transpuesta | 0.075238 | 0.81 |

Tamaño 300³
| Operación | Tiempo (ms) | Speedup vs (b) |
|-----------|-------------|----------------|
| Suma | 0.055519 | 2.07* |
| Multiplicación | 19.9038 | 1.01 |
| Transpuesta | 0.060809 | 1.92* |
*Valores >1 en operaciones O(n²) pueden deberse a variabilidad y tamaño de muestra simple.

---
## Resumen Numérico Global
| Problema | Caso | T_serial (b) ms | T_paralelo (c) ms | Speedup |
|----------|------|-----------------|------------------|---------|
| Matriz-Vector | 1000×1200 | 0.966461 | 1.46938 | 0.657 |
| Matriz-Vector | 8000×8000 | 59.1749 | 21.8567 | 2.71 |
| Hot Plate | 256 | 0.827036 | 1.20248 | 0.688 |
| Matrix Ops (Mult) | 200³ | 5.68569 | 6.506 | 0.87 |
| Matrix Ops (Mult) | 300³ | 20.1363 | 19.9038 | 1.01 |

Fin de los resultados solicitados.


