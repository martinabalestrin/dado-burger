#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
EXECUTABLE="$BUILD_DIR/dado_burger"
COMPILER="${CC:-clang}"

mkdir -p "$BUILD_DIR"

sources=()
while IFS= read -r source; do
    sources+=("$source")
done < <(find "$ROOT_DIR/src" -type f -name '*.c' -print | sort)

if ((${#sources[@]} == 0)); then
    printf 'Nenhum arquivo .c encontrado em %s/src\n' "$ROOT_DIR" >&2
    exit 1
fi

"$COMPILER" -Wall -std=c99 "${sources[@]}" -lm -o "$EXECUTABLE"
cd "$ROOT_DIR"
exec "$EXECUTABLE" "$@"
