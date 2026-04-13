#!/bin/bash
# dobro - função dobro com validação de argumento numérico (inteiro)

is_int() {
  case "$1" in
    ""|*[!0-9-]*|- ) return 1 ;;
    -*)
      [ "$1" = "-" ] && return 1
      case "${1#-}" in
        ""|*[!0-9]*) return 1 ;;
        *) return 0 ;;
      esac
      ;;
    *) return 0 ;;
  esac
}

dobro() {
  if [ $# -ne 1 ]; then
    echo "Erro: a função dobro precisa de 1 argumento." 1>&2
    return 1
  fi
  if ! is_int "$1"; then
    echo "Erro: argumento não é inteiro." 1>&2
    return 1
  fi
  echo $((2 * $1))
}

if [ $# -ne 1 ]; then
  echo "Uso: $0 <inteiro>"
  exit 1
fi

res="$(dobro "$1")"
if [ $? -ne 0 ]; then
  exit 1
fi

echo "Dobro de $1 = $res"