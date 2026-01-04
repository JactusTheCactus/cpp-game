#!/usr/bin/env bash
set -euo pipefail
compile() {
	local out="logs/out/$1.log"
	local err="logs/err/$1.log"
	local LIBS=(-lncurses)
	local FLAGS=(
		-std=c++20
		${LIBS[@]}
	)
	local BUILD=(
		g++
		"src/$1.cpp"
		-o "bin/$1"
		${FLAGS[@]}
	)
	if ! ${BUILD[@]} > "$out" 2> "$err"; then
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
mkdir -p bin logs/out logs/err
SRC=(game)
PIDS=()
for i in "${SRC[@]}"; do
	compile "$i" &
	PIDS+=($!)
done
fail=0
for pid in "${PIDS[@]}"; do
	wait "$pid" || fail=1
done
(( fail )) && exit 1
find logs -empty -delete