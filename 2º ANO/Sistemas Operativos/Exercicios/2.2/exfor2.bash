#!/bin/bash
# exfor2 - Fase 2: pede limites, valida, mostra só ímpares

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

echo -n "Limite inferior: "
read low
echo -n "Limite superior: "
read high

if ! is_int "$low"; then
  echo "Erro: limite inferior não é inteiro."
  exit 1
fi

if ! is_int "$high"; then
  echo "Erro: limite superior não é inteiro."
  exit 1
fi

if [ "$low" -gt "$high" ]; then
  echo "Erro: limite inferior é maior que o limite superior."
  exit 1
fi

for ((i=low; i<=high; i=i+1)); do
  if [ $((i % 2)) -ne 0 ]; then
    echo "$i"
  fi
done