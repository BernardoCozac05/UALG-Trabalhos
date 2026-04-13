#!/bin/bash
# excase1 - menu com case

echo "1) Data do sistema"
echo "2) Data e hora do sistema"
echo "3) Sair"
echo -n "Opção: "
read opt

case "$opt" in
  1)
    date +%F
    ;;
  2)
    date
    ;;
  3)
    exit 0
    ;;
  *)
    echo "opções válidas [1 2 3]"
    exit 1
    ;;
esac