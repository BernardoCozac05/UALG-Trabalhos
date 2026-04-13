#!/bin/bash
# exif2 - verifica se ficheiro existe e tem conteúdo; senão cria a partir do stdin até Ctrl+C

if [ $# -ne 1 ]; then
  echo "Uso: $0 <ficheiro>"
  exit 1
fi

FILE="$1"

if [ -f "$FILE" ] && [ -s "$FILE" ]; then
  echo "OK: o ficheiro existe e tem conteúdo: $FILE"
  exit 0
fi

echo "O ficheiro não existe ou está vazio. Vou criar/recriar: $FILE"
echo "Escreve linhas (Ctrl+C para terminar)."

# cria/limpa o ficheiro
: > "$FILE"

# lê do stdin e acrescenta ao ficheiro até Ctrl+C
while read line; do
  echo "$line" >> "$FILE"
done