#!/usr/bin/env bash
set -euo pipefail
compile() {
	local out="logs/out/$1.log"
	local err="logs/err/$1.log"
	local flags=(
		-std=c++20
		-lncurses
	)
	local build=(
		g++
		"src/$1.cpp"
		"${flags[@]}"
		"-o bin/$1"
	)
	if ! "${build[@]}" > "$out" 2> "$err"; then
		code "$err"
		return 1
	fi
}
COMMANDS=(
	code
	g++
)
for i in "${COMMANDS[@]}"; do
	if ! command -v "$i" > /dev/null; then
		echo "[ERR]: command '$i' not found" >& 2
		exit 1
	fi
done
find bin logs -delete
mkdir -p bin logs/{out,err}
compile game
find logs -empty -delete