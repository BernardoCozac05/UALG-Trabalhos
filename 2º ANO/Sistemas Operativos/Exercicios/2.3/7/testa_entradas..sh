#!/bin/bash
# testa_entradas.sh - verifica se o script foi chamado com argumentos

if [ $# -eq 0 ]; then
  echo "Erro: não foram fornecidos argumentos."
  echo "Uso: $0 <arg1> [arg2 ...]"
  exit 1
fi

echo "OK: foram fornecidos $# argumento(s)."