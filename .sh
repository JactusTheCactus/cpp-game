#!/usr/bin/env bash
set -euo pipefail
compile() {
	local out="logs/out/$1.log"
	local err="logs/err/$1.log"
	local FLAGS=(
		-std=c++20
		-lncurses
	)
	local BUILD=(
		g++
		"src/$1.cpp"
		"${FLAGS[@]}"
		"-o bin/$1"
	)
	if ! "${BUILD[@]}" > "$out" 2> "$err"; then
		code "$err"
		return 1
	fi
}
COMMANDS=(code g++)
for i in "${COMMANDS[@]}"; do
	if ! command -v "$i" > /dev/null; then
		echo "[ERR]: command '$i' not found" >&2
		exit 1
	fi
done
rm -rf bin logs
mkdir -p bin logs/{out,err}
SRC=(game)
for i in "${SRC[@]}"; do
	compile "$i" &
done
wait
find logs -empty -delete