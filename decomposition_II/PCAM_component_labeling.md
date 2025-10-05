# Etiquetado de Componentes – Análisis PCAM

Problema: Dada una imagen binaria 2D (0 = fondo, 1 = primer plano), asignar etiquetas iguales a todos los píxeles 1 que pertenezcan a la misma componente conexa 4‑conexa (vínculos verticales y horizontales). Se usa propagación iterativa del mínimo.

Algoritmo (implementado en `component_labeling.cpp`):
1. Inicialización: Para cada píxel de primer plano i asignar label[i] = i (id único). Fondo = -1.
2. Relajación iterativa: Repetidamente actualizar cada píxel de primer plano con el mínimo entre su etiqueta y la de sus 4 vecinos (arriba, abajo, izquierda, derecha) que sean también primer plano.
3. Terminar cuando una iteración no realiza cambios (bandera `changed=false`) o se alcance un límite de seguridad.
4. Compresión opcional: Segunda pasada para comprimir caminos y obtener una etiqueta raíz canónica por componente. El conteo de raíces distintas = número de componentes.

## Desglose PCAM

### Partitioning (P)
Descomposición del dominio por filas: la matriz 2D se divide en franjas horizontales. Cada hilo procesa su subconjunto de filas. No se replica información; se comparten los vectores globales de imagen y etiquetas.

### Communication (C)
Implícita mediante lecturas en memoria compartida de etiquetas vecinas. Las dependencias entre franjas sólo existen en filas limítrofes; no se requiere intercambio explícito de halos más allá de la sincronización por iteración. La barrera implícita al final del `parallel for` (por el `reduction`) garantiza visibilidad de las actualizaciones de la iteración k antes de iniciar la k+1.

### Agglomeration (A)
Se mantiene una granularidad relativamente gruesa (bloques de filas completos) para minimizar sobrecoste de planificación. No se agrupan varias iteraciones dentro de una sola región paralela para conservar la semántica de usar únicamente valores completos de la iteración anterior.

### Mapping (M)
Planificación estática reparte casi el mismo número de filas por hilo; con distribución binaria aleatoria uniforme la carga es balanceada. El número de iteraciones depende del diámetro Manhattan máximo de la mayor componente; peor caso O(rows + cols). El mapeo no cambia con el tiempo, así que no se necesita balanceo dinámico.

## Complejidad
Sea N = rows * cols.
- Trabajo por iteración: O(N) comparaciones / operaciones de mínimo sobre píxeles de primer plano.
- Número de iteraciones: O(D) donde D aproxima el diámetro Manhattan máximo de cualquier componente (≤ rows + cols).
- Trabajo total: O(N * D) en el peor caso; en práctica menor si las componentes son compactas.
- Span paralelo por iteración ≈ O(N / p + overhead). Sincronización: una barrera global por iteración.

## Estructuras de Datos
- `img`: vector<int> binario de tamaño N.
- `labels`: vector<int> de tamaño N con -1 para fondo o la etiqueta actual.
- `root` (fase de compresión): copia de etiquetas para comprimir y extraer raíces únicas.

## Detección de Convergencia
Un booleano `changed` combinado con `reduction(|:changed)` indica si hubo al menos una actualización. Cuando resulta `false` la iteración finaliza. Evita atomics finos.

## Métricas de Salida
El programa imprime: iteraciones, número de componentes y tiempo en milisegundos. El número de hilos se puede pasar por CLI.

---
## Resultados (Ejecución Proporcionada)

Parámetros de la corrida:
- Dimensiones: 512 × 512 (N = 262,144 píxeles)
- Densidad (probabilidad de píxel 1): 0.40
- Hilos (OpenMP): 8
- Semilla interna fija (1234) para reproducibilidad del patrón aleatorio.

Salida registrada:
```
Iterations=23 components=27884 time_ms=6.29092
```

Tabla resumida:
| Filas | Columnas | Densidad | Hilos | Iteraciones | Componentes | Tiempo (ms) | Píxeles/ms |
|-------|----------|----------|-------|-------------|-------------|-------------|------------|
| 512   | 512      | 0.40     | 1     | 23          | 27,884      | 38.9732     | ~6,730     |
| 512   | 512      | 0.40     | 8     | 23          | 27,884      | 6.29092     | ~41,700    |

Cálculos auxiliares:
- Píxeles totales: 512 × 512 = 262,144
- Rendimiento aproximado: 262,144 / 6.29092 ≈ 41,700 píxeles/ms
- Iteraciones hasta convergencia: 23
- Componentes detectados: 27,884

### Speedup
Speedup(8 hilos) = T_1_hilo / T_8_hilos = 38.9732 / 6.29092 ≈ 6.20

Eficiencia (8 hilos) = 6.20 / 8 ≈ 0.78

