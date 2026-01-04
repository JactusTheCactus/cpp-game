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
find bin logs -delete
mkdir -p bin logs/{out,err}
compile game
find logs -empty -delete