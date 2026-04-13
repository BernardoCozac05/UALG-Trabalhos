#!/bin/bash
# fatorial - iterativo e recursivo no mesmo ficheiro

is_nat() {
  case "$1" in
    ""|*[!0-9]*) return 1 ;;
    *) return 0 ;;
  esac
}

fact_iter() {
  n="$1"
  res=1
  i=2
  while [ $i -le "$n" ]; do
    res=$((res * i))
    i=$((i + 1))
  done
  echo "$res"
}

fact_rec() {
  n="$1"
  if [ "$n" -le 1 ]; then
    echo 1
    return
  fi
  prev="$(fact_rec $((n - 1)))"
  echo $((n * prev))
}

if [ $# -ne 1 ]; then
  echo "Uso: $0 <n (inteiro >= 0)>"
  exit 1
fi

n="$1"
if ! is_nat "$n"; then
  echo "Erro: n tem de ser um inteiro >= 0."
  exit 1
fi

echo "1) Iterativo"
echo "2) Recursivo"
echo "3) Ambos"
echo -n "Opção: "
read opt

case "$opt" in
  1)
    echo "Iterativo: $(fact_iter "$n")"
    ;;
  2)
    echo "Recursivo: $(fact_rec "$n")"
    ;;
  3)
    echo "Iterativo: $(fact_iter "$n")"
    echo "Recursivo: $(fact_rec "$n")"
    ;;
  *)
    echo "Opções válidas [1 2 3]"
    exit 1
    ;;
esac