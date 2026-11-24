#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
GUI_DIR="$ROOT_DIR/interface-grafica"
CALLER_DIR="$PWD"
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

args=()
if [[ "$build_only" == false ]]; then
    for path in "$1" "$2"; do
        case "$path" in
            /*) args+=("$path") ;;
            *)  args+=("$CALLER_DIR/$path") ;;
        esac
    done
fi

raylib_prefix="${RAYLIB_PREFIX:-}"
if [[ -z "$raylib_prefix" ]] && command -v brew >/dev/null 2>&1; then
    raylib_prefix="$(brew --prefix raylib 2>/dev/null || true)"
fi

if [[ -z "$raylib_prefix" || ! -f "$raylib_prefix/include/raylib.h" ||
      ! -f "$raylib_prefix/lib/libraylib.dylib" ]]; then
    printf 'Raylib não encontrada. Informe RAYLIB_PREFIX com o prefixo da instalação local.\n' >&2
    exit 1
fi

mkdir -p "$BUILD_DIR"
"$COMPILER" -Wall -std=c99 -I"$raylib_prefix/include" \
    "$GUI_DIR/gui/main.c" "$GUI_DIR/gui/telas.c" "$GUI_DIR/gui/componentes.c" \
    "$GUI_DIR/src/jogo/aplicacao.c" "$GUI_DIR/src/jogo/sessao.c" \
    "$GUI_DIR/src/nodoAvl/nodoAvl.c" "$GUI_DIR/src/tabelaHash/tabelaHash.c" \
    "$GUI_DIR/src/listaContDin/listaContDin.c" "$GUI_DIR/src/listaEnc/listaEnc.c" \
    "$GUI_DIR/src/cliente/cliente.c" "$GUI_DIR/src/fila/fila.c" \
    "$GUI_DIR/src/pilha/pilha.c" "$GUI_DIR/src/heap/heap.c" \
    "$GUI_DIR/src/ingrediente/ingrediente.c" "$GUI_DIR/src/medicoes/medicoes.c" \
    "$GUI_DIR/src/timer/timer.c" "$GUI_DIR/src/ordenacao/ordenacaoHistorico.c" \
    -L"$raylib_prefix/lib" -lraylib -lm -o "$EXECUTABLE"

if [[ "$build_only" == true ]]; then
    printf 'Build concluído: %s\n' "$EXECUTABLE"
    exit 0
fi

cd "$GUI_DIR"
exec "$EXECUTABLE" "${args[@]}"
