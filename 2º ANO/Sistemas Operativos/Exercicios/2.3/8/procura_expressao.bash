#!/bin/bash
# procura_expressao.sh - lista ficheiros que contêm a expressão (procura recursiva)

if [ $# -lt 1 ]; then
  echo "Uso: $0 <expressao>"
  exit 1
fi

expr="$1"
shift
for w in "$@"; do
  expr="$expr $w"
done

grep -R -l "$expr" .