#!/usr/bin/env bash
set -euo pipefail
compile() {
	local out="logs/out/$1.log"
	local err="logs/err/$1.log"
	g++ \
		"src/$1.cpp" \
		-std=c++20 \
		-lncurses \
		-o "bin/$1" \
		> "$out" \
		2> "$err" \
		|| {
			code "$err"
			exit 1
		}
}
COMMANDS=(
	code
	g++
)
for i in "${COMMANDS[@]}"
	do command -v "$i" > /dev/null || {
		echo "[ERR]: command '$i' not found" >& 2
		exit 1
	}
done
find bin logs -delete
mkdir -p bin logs/{out,err}
compile game
find logs -empty -delete