#!/bin/bash
# lista_extensao.sh - lista ficheiros com extensão indicada

if [ $# -ne 1 ]; then
  echo "Uso: $0 <extensao>"
  exit 1
fi

ext="$1"
# permitir que o utilizador escreva ".txt" ou "txt"
case "$ext" in
  .*) ext="${ext#.}" ;;
esac

for f in *; do
  [ -f "$f" ] || continue
  case "$f" in
    *."$ext") echo "$f" ;;
  esac
done