#!/usr/bin/env bash
set -euo pipefail
compile() {
	local out="logs/out/$1.log"
	local err="logs/err/$1.log"
	local flags=(-std=c++20 -lncurses)
	local input="src/$1.cpp"
	local output="-o bin/$1"
	if ! g++ "$input" "${flags[@]}" "$output" > "$out" 2> "$err"; then
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
CPP=(game)
for i in "${CPP[@]}"; do
	compile "$i" &
done
wait
find logs -empty -delete