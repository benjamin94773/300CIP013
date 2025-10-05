# Game of Life Performance

## Files
- `game_of_life.cpp` – Implements sequential, row-parallel, and tiled-parallel versions.
- `game_of_life_speedup.md` – Report discussing speedups and observations.

## Build
```bash
g++ -O2 -fopenmp game_of_life.cpp -o game_of_life
```

## Run
```bash
./game_of_life 512 512 200 0.3
```
Args: rows cols steps density.

## Output Example
```
Game of Life Performance (512x512, steps=200)
Sequential ms: 950
Row-parallel ms: 260 speedup=3.65
Tiled-parallel ms: 210 speedup=4.52
```
(Timings vary.)
