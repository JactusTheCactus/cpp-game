#!/usr/bin/env bash
set -euo pipefail
flag() {
	for f in "$@"
		do [[ -e ".flags/$f" ]] || return 1
	done
}
compile() {
	g++ \
		"src/$1.cpp" \
		-std=c++20 \
		-lncurses \
		-o "bin/$1" \
		1> "logs/out/$1.log" \
		2> "logs/err/$1.log" \
		|| :
}
rm -r bin logs 2> /dev/null || :
mkdir bin logs logs/out logs/err
compile game
find . -empty -delete