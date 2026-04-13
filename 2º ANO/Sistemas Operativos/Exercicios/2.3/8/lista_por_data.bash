#!/bin/bash
# lista_por_data.sh - lista ficheiros (neste diretório) com data entre duas datas
# Datas no formato: YYYY-MM-DD

echo -n "Data inicial (YYYY-MM-DD): "
read ini
echo -n "Data final   (YYYY-MM-DD): "
read fim

# validar minimamente: tem de ter 10 chars e 2 hífens (sem regex avançado)
case "$ini" in
  ????-??-??) ;;
  *) echo "Erro: data inicial inválida."; exit 1 ;;
esac
case "$fim" in
  ????-??-??) ;;
  *) echo "Erro: data final inválida."; exit 1 ;;
esac

fim_next="$(date -d "$fim + 1 day" +%Y-%m-%d 2>/dev/null)"
if [ -z "$fim_next" ]; then
  echo "Erro: data final inválida."
  exit 1
fi

# Ficheiros regulares apenas no diretório atual
find . -maxdepth 1 -type f -newermt "$ini" ! -newermt "$fim_next" -print