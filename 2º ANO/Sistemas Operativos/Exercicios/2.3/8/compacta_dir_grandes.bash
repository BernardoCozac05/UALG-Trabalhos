#!/bin/bash
# compacta_dir_grandes.sh - comprime diretórios com tamanho > limiar (KB)

if [ $# -ne 1 ]; then
  echo "Uso: $0 <limiar_KB>"
  exit 1
fi

lim="$1"
case "$lim" in
  ""|*[!0-9]*) echo "Erro: limiar tem de ser inteiro (KB)."; exit 1 ;;
esac

for d in *; do
  [ -d "$d" ] || continue

  # du -sk devolve: "<KB> <nome>"
  set -- $(du -sk "$d" 2>/dev/null)
  size="$1"

  case "$size" in
    ""|*[!0-9]*) continue ;;
  esac

  if [ "$size" -gt "$lim" ]; then
    echo "A comprimir: $d (tamanho ${size}KB)"
    tar -czf "${d}.tar.gz" "$d"
  fi
done