# Conway's Game of Life – Informe de Rendimiento y Speedup (Resultados Reales)

## Versiones Implementadas
1. Secuencial: bucles anidados simples.
2. Paralela por filas (Row-wise OpenMP): `#pragma omp parallel for` sobre las filas.
3. Paralela con Tiling 2D: doble bucle de bloques + `collapse(2)` para exponer más paralelismo y mejorar locality.

## Metodología
- Tamaño evaluado: 512×512, 200 pasos, densidad 0.3 (runs múltiples).
- 5 repeticiones iniciales (sin forzar explícitamente `OMP_NUM_THREADS`) y luego ejecuciones dedicadas con `OMP_NUM_THREADS=8` y `OMP_NUM_THREADS=4`.
- Cada ejecución del binario genera tiempos de las tres versiones en ms (mide una corrida de cada variante con seeds distintas).
- Métricas calculadas: promedios, desviación estándar (método: muestra, n-1), speedups y eficiencia relativa.

## Datos Crudos (5 Repeticiones Iniciales)
| Repetición | Seq (ms) | Row (ms) | Tiled (ms) | S_row (inline) | S_tile (inline) |
|------------|----------|----------|-----------|----------------|-----------------|
| 1 | 591.977 | 144.932 | 143.742 | 4.0845 | 4.1183 |
| 2 | 559.193 | 142.890 | 153.067 | 3.9134 | 3.6533 |
| 3 | 553.009 | 144.572 | 149.785 | 3.8252 | 3.6920 |
| 4 | 548.717 | 142.955 | 147.431 | 3.8384 | 3.7219 |
| 5 | 549.218 | 140.378 | 144.232 | 3.9124 | 3.8079 |

### Estadísticas (n=5)
| Métrica | Secuencial | Row-wise | Tiled |
|---------|-----------:|---------:|------:|
| Media (ms) | 560.4228 | 143.1454 | 147.6514 |
| Desv. Est. (ms) | 18.12 | 1.80 | 3.90 |
| % Reducción vs Seq ( (1 - T/Seq)·100 ) | — | 74.45% | 73.64% |

### Speedups (basados en medias)
- S_row = 560.4228 / 143.1454 ≈ **3.915**
- S_tiled = 560.4228 / 147.6514 ≈ **3.795**
- S_tiled/row = 143.1454 / 147.6514 ≈ **0.970** (tiled promedio más lento que row en este set)
- S_global = 560.4228 / min(143.1454, 147.6514) = 560.4228 / 143.1454 ≈ **3.915**

## Ejecuciones Controladas con Hilos Fijos
`OMP_NUM_THREADS=8`
| Seq (ms) | Row (ms) | Tiled (ms) | S_row | S_tiled | Tiled/Row |
|----------|----------|-----------|-------|---------|-----------|
| 550.987 | 145.122 | 149.565 | 3.797 | 3.684 | 0.970 |

`OMP_NUM_THREADS=4`
| Seq (ms) | Row (ms) | Tiled (ms) | S_row | S_tiled | Tiled/Row |
|----------|----------|-----------|-------|---------|-----------|
| 566.163 | 192.884 | 173.987 | 2.935 | 3.254 | 1.109 |

Observación: A 4 hilos, la versión con tiling supera a la row-wise (Tiled/Row > 1), mientras que a 8 hilos el patrón se revierte (Row ligeramente mejor). En las 5 primeras corridas, la variación natural de las seeds y estado de caché hace que el tiled no supere consistentemente al row-wise.

## Diferencias Algorítmicas
- Secuencial: dos bucles, cálculo directo de vecinos.
- Row-wise: paraleliza filas; buena escalabilidad inicial, baja sobrecarga, locality razonable por trayectoria lineal de memoria.
- Tiled: bloque 2D reduce potencialmente misses de caché agrupando celdas cercanas, pero añade lógica de control de bloques y puede introducir overhead extra cuando la localidad inherente ya es suficiente.

## Interpretación de Resultados
1. El speedup ~3.9× sugiere límite por ancho de banda de memoria / dependencia de lectura de vecinos (8 hilos no alcanzan linealidad auténtica).
2. El tiling no ofrece mejora promedio en 512×512 (200 pasos) con esta implementación específica; la ganancia potencial se diluye frente al overhead de iterar bloques y re-calcular límites.
3. Con 4 hilos el tiling sí aventaja (≈10.9% sobre row) indicando que en menor paralelismo la mejora de locality pesa más que el overhead adicional.
4. La desviación estándar de la versión secuencial (≈3.2% del promedio) muestra variabilidad aceptable; las variantes paralelas son estables (<3% en row, ~2.6% en tiled respecto a sus medias).

## Conclusión
La paralelización por filas proporciona el mejor tiempo promedio en este escenario (512×512, 200 pasos) con un speedup ≈3.9×. El tiling no supera al enfoque row-wise a 8 hilos, pero a 4 hilos revela ventaja relativa, sugiriendo que el beneficio de locality depende de la presión de concurrencia. El techo de rendimiento observado está dominado por la naturaleza intensiva en memoria del recuento de vecinos; mejoras significativas adicionales requerirían optimizaciones de acceso (packing, vectorización) o algoritmos alternativos.

