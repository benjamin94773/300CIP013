# Ley de Amdahl – Análisis

Sea f la fracción secuencial (no paralelizable), p = 1 - f la fracción paralelizable y N el número de núcleos. Speedup ideal:
S(N) = 1 / ( f + (1 - f)/N ).

## 1. 1% no paralelizable, 61 núcleos
f = 0.01, N = 61
S = 1 / (0.01 + 0.99/61) = 1 / (0.01 + 0.0162295) ≈ 1 / 0.0262295 ≈ 38.14.

## 2. 0.1% no paralelizable (f = 0.001)
- N = 30:
  S ≈ 1 / (0.001 + 0.999/30) = 1 / (0.001 + 0.0333) ≈ 28.41
- N = 30,000:
  S ≈ 1 / (0.001 + 0.999/30000) = 1 / (0.001 + 0.0000333) ≈ 1 / 0.0010333 ≈ 967.77
- N = 3,000,000:
  S ≈ 1 / (0.001 + 0.999/3000000) ≈ 1 / (0.001 + 0.000000333) ≈ 1 / 0.001000333 ≈ 999.67
Límite cuando N→∞: 1/f = 1000.

## 3. Overheads de broadcast: 0.0001 N vs 0.0005 log₂ N
Suponemos una fase paralela de tiempo 1 que idealmente escalaría como 1/N, más un overhead h(N). Tiempo total normalizado:
T(N) = 1/N + h(N). El speedup relativo al núcleo único (≈1) es S(N) ≈ 1 / T(N).

Implementaciones:
1) Lineal: h₁(N) = 0.0001 N
2) Logarítmica (árbol): h₂(N) = 0.0005 log₂ N

Objetivos: (a) cruce h₁(N)=h₂(N); (b) N que minimiza T(N) para cada modelo.

### (a) Punto de cruce
0.0001 N = 0.0005 log₂ N ⇒ N = 5 log₂ N.
Solución numérica: N ≈ 22 (5·log₂ 22 ≈ 22.29). Para N > ~22 conviene el método logarítmico (menor overhead).

### (b) N óptimo
Lineal: T(N)=1/N + aN, a=0.0001.
dT/dN= -1/N² + a=0 ⇒ N_opt=1/√a=100. T≈0.02 ⇒ S≈50.

Logarítmico: T(N)=1/N + b log₂ N, b=0.0005.
dT/dN= -1/N² + b/(N ln2)=0 ⇒ 1/N = b/ln2 ⇒ N_opt ≈ ln2 / 0.0005 ≈ 1386. T≈0.00594 ⇒ S≈168.

Después de esos puntos el término de overhead domina (lineal) o crece lentamente (log) mientras 1/N aporta menos. Conclusión: broadcast logarítmico supera al lineal para N > ~22 y ofrece mejor máximo teórico.

## 4. ¿Por qué supercomputadoras pese a Amdahl?
La ley de Amdahl asume problema fijo. Muchos problemas científicos aumentan tamaño con N (weak scaling) → Ley de Gustafson: S ≈ N - f (N - 1). Además se reduce f con rediseño algorítmico, solapando comunicación, usando paralelismo heterogéneo (vector, datos, tareas). Sistemas masivos permiten resolver problemas cualitativamente mayores (resolución, horizonte temporal) no solo acelerar uno pequeño. Factores de I/O, resiliencia y pipelines de trabajo amplían el paralelismo efectivo.

## 5. Trabajo y profundidad en reducción de mínimo en árbol binario
Dado N elementos:
- Trabajo total: W(N) = N - 1 comparaciones.
- Profundidad (camino crítico): D(N) = ceil(log₂ N) niveles.
Paralelismo medio: P ≈ (N - 1) / log₂ N.

## Conclusión
Principales observaciones derivadas de los cinco ejercicios:
1. Fracción secuencial pequeña (1%) aún limita severamente el speedup máximo (≈38× en 61 núcleos) confirmando la naturaleza asintótica de Amdahl.
2. Reducir f de 0.01 a 0.001 amplifica el límite teórico (de 100 a 1000) pero alcanzar ese límite práctico requiere millones de núcleos y aun así se aproxima lentamente (999.67 < 1000 con 3,000,000 núcleos).
3. Overheads de comunicación alteran el punto óptimo: un coste lineal fija un N óptimo finito (~100), mientras el logarítmico permite seguir escalando hasta ≈1386 núcleos antes de rendimientos marginales menores.
4. Supercomputadoras existen porque muchos problemas escalan en tamaño (weak scaling, Ley de Gustafson) y porque se puede reducir la fracción secuencial mediante rediseño algoritmico, solapamiento y heterogeneidad.
5. La reducción binaria (mínimo) ofrece paralelismo P ≈ (N / log₂ N), ilustrando gran potencial de aceleración si se controla la fracción secuencial externa.

En conjunto, Amdahl subraya la importancia de: (a) minimizar la fracción secuencial; (b) gestionar y modelar sobrecostes de comunicación para elegir N efectivo; (c) combinar análisis de Amdahl con Gustafson para justificar arquitecturas masivas; y (d) diseñar algoritmos con paralelismo intrínseco alto (profundidad logarítmica) para sostener escalabilidad.
