#!/bin/bash
# exif1 - verifica se um diretório existe

if [ $# -ne 1 ]; then
  echo "Uso: $0 <diretorio>"
  exit 1
fi

DIR="$1"

if [ -d "$DIR" ]; then
  echo "O diretório existe: $DIR"
else
  echo "O diretório NÃO existe: $DIR"
fi