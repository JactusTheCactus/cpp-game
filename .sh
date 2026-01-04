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
		&> "logs/build/$1.log" || :
}
rm -r logs bin 2> /dev/null || :
mkdir bin logs logs/build logs/run
compile game
find . -empty -delete