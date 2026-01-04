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
rm -r bin logs 2> /dev/null || :
mkdir bin logs logs/out logs/err
compile game
find logs -empty -delete