#!/bin/bash
# soma - função soma de dois inteiros + validação do nº de argumentos

soma() {
  echo $(( $1 + $2 ))
}

if [ $# -ne 2 ]; then
  echo "Uso: $0 <int1> <int2>"
  exit 1
fi

# validação simples: tenta avaliar como aritmética
a="$1"
b="$2"

# Se não forem números inteiros, o bash costuma dar erro em aritmética;
# fazemos um teste indireto:
tmp=$((a + 0)) 2>/dev/null
if [ $? -ne 0 ]; then
  echo "Erro: o 1º argumento não é inteiro."
  exit 1
fi

tmp=$((b + 0)) 2>/dev/null
if [ $? -ne 0 ]; then
  echo "Erro: o 2º argumento não é inteiro."
  exit 1
fi

res="$(soma "$a" "$b")"
echo "$a + $b = $res"