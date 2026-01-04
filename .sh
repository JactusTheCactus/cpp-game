#!/usr/bin/env bash
set -euo pipefail
compile() {
	g++ \
		"src/$1.cpp" \
		-std=c++20 \
		-lncurses \
		-o "bin/$1" \
		> "logs/out/$1.log" \
		2> "logs/err/$1.log"
}
rm -r bin logs 2> /dev/null || :
mkdir bin logs logs/out logs/err
compile game
find logs -empty -delete