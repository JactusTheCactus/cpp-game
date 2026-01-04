#!/usr/bin/env bash
set -euo pipefail
compile() {
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
	local out="logs/out/$1.log"
	local err="logs/err/$1.log"
	if ! ${BUILD[@]} > "$out" 2> "$err"; then
		code "$err"
		return 1
	fi
}
COMMANDS=(code g++)
for i in "${COMMANDS[@]}"; do
	if ! command -v "$i" > /dev/null; then
		printf '\e[91m%s \e[0m%s \e[93m"%s" \e[0m%s\n' "[ERR]:" "command" "$i" "not found" >&2
		exit 1
	fi
done
rm -rf bin logs
mkdir -p bin logs/{out,err}
SRC=(game test)
for i in "${SRC[@]}"; do
	[[ -f "src/$i.cpp" ]] && compile "$i" &
done
wait
# [[ -f "./bin/test" ]] && ./bin/test > logs/test.log && code logs/test.log
find logs -empty -delete