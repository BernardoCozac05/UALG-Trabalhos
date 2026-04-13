#!/bin/bash
# exfor1 - entra em /bin e /etc por 2 segundos, lista, e volta ao diretório inicial

INI="$(pwd)"

for d in /bin /etc; do
  if [ -d "$d" ]; then
    cd "$d"
    echo "== Diretório atual: $(pwd) =="
    ls
    sleep 2
  else
    echo "Aviso: diretório não existe: $d"
  fi
done

cd "$INI"
echo "Voltei ao diretório inicial: $(pwd)"