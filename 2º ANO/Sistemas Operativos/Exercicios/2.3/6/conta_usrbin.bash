#!/bin/bash
# conta_usrbin.sh - contagem aproximada: scripts vs executáveis vs links vs outros

cd /usr/bin || exit 1

scripts=0
execs=0
links=0
outros=0

for f in *; do
  # links simbólicos
  if [ -L "$f" ]; then
    links=$((links + 1))
    continue
  fi

  # só ficheiros regulares
  if [ ! -f "$f" ]; then
    continue
  fi

  # ler primeira linha para testar shebang
  first=""
  if read -r first < "$f" 2>/dev/null; then
    case "$first" in
      "#!"*)
        scripts=$((scripts + 1))
        continue
        ;;
    esac
  fi

  # executável mas não-script
  if [ -x "$f" ]; then
    execs=$((execs + 1))
  else
    outros=$((outros + 1))
  fi
done

echo "Scripts (com #!): $scripts"
echo "Executáveis (x e sem #!): $execs"
echo "Links simbólicos: $links"
echo "Outros: $outros"