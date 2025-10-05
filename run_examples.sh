#!/usr/bin/env bash
set -euo pipefail

run() { echo; echo ">$ $*"; "$@"; }

run ./bin/mv_a
run ./bin/mv_b 800 800
run ./bin/mv_c 800 800 8
run ./bin/hp_a
run ./bin/hp_b 256 500
run ./bin/hp_c 256 500 8
run ./bin/mo_a
run ./bin/mo_b 120 120 120
run ./bin/mo_c 120 120 120
run ./bin/component_labeling 256 256 0.4 4
run ./bin/game_of_life 256 256 100 0.3

echo; echo "Ejemplos completados"