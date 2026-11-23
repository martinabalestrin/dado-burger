#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
EXECUTABLE="$BUILD_DIR/dado_burger_gui"
COMPILER="${CC:-clang}"

build_only=false
if [[ $# -eq 1 && $1 == --build-only ]]; then
    build_only=true
elif [[ $# -ne 2 ]]; then
    printf 'Uso: %s <pedidos.txt> <historico.txt> ou %s --build-only\n' "$0" "$0" >&2
    exit 2
fi

raylib_prefix="${RAYLIB_PREFIX:-}"
if [[ -z "$raylib_prefix" ]] && command -v brew >/dev/null 2>&1; then
    raylib_prefix="$(brew --prefix raylib)"
fi

if [[ -z "$raylib_prefix" || ! -f "$raylib_prefix/include/raylib.h" ||
      ! -f "$raylib_prefix/lib/libraylib.dylib" ]]; then
    printf 'Raylib não encontrada. Informe RAYLIB_PREFIX com o prefixo da instalação local.\n' >&2
    exit 1
fi

mkdir -p "$BUILD_DIR"
"$COMPILER" -Wall -std=c99 -I"$raylib_prefix/include" \
    "$ROOT_DIR/gui/main.c" "$ROOT_DIR/gui/telas.c" "$ROOT_DIR/gui/componentes.c" \
    "$ROOT_DIR/src/jogo/aplicacao.c" "$ROOT_DIR/src/jogo/sessao.c" \
    "$ROOT_DIR/src/nodoAvl/nodoAvl.c" "$ROOT_DIR/src/tabelaHash/tabelaHash.c" \
    "$ROOT_DIR/src/listaContDin/listaContDin.c" "$ROOT_DIR/src/listaEnc/listaEnc.c" \
    "$ROOT_DIR/src/cliente/cliente.c" "$ROOT_DIR/src/fila/fila.c" \
    "$ROOT_DIR/src/pilha/pilha.c" "$ROOT_DIR/src/heap/heap.c" \
    "$ROOT_DIR/src/ingrediente/ingrediente.c" "$ROOT_DIR/src/medicoes/medicoes.c" \
    "$ROOT_DIR/src/timer/timer.c" "$ROOT_DIR/src/ordenacao/ordenacaoHistorico.c" \
    -L"$raylib_prefix/lib" -lraylib -lm -o "$EXECUTABLE"

if [[ "$build_only" == true ]]; then
    printf 'Build concluído: %s\n' "$EXECUTABLE"
    exit 0
fi

cd "$ROOT_DIR"
exec "$EXECUTABLE" "$@"
