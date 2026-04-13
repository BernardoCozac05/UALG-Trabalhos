#!/bin/bash
# parse_last_wtmp.bash
# Lê teste.wtmp (saída do last -f /var/log/wtmp > teste.wtmp)
# Para cada utilizador: nº de entradas e tempo total de utilização.
# Se "still logged in", conta até agora.
# Sem pipes.

FILE="teste.wtmp"
if [ $# -ge 1 ]; then
  FILE="$1"
fi

if [ ! -f "$FILE" ]; then
  echo "Erro: ficheiro não existe: $FILE"
  exit 1
fi

declare -A count
declare -A seconds

# Converte "HH:MM" ou "D+HH:MM" em segundos
dur_to_seconds() {
  s="$1"
  days=0

  case "$s" in
    *+*)
      days="${s%%+*}"
      rest="${s#*+}"
      ;;
    *)
      rest="$s"
      ;;
  esac

  hh="${rest%%:*}"
  mm="${rest#*:}"

  # FORÇAR BASE 10 (evita "08"/"09" serem interpretados como octal)
  days=$((10#$days))
  hh=$((10#$hh))
  mm=$((10#$mm))

  echo $((days*86400 + hh*3600 + mm*60))
}

# Devolve a posição (índice 1-based) do token do mês (Jan..Dec) na linha
month_index() {
  i=1
  for tok in "$@"; do
    case "$tok" in
      Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)
        echo "$i"
        return 0
        ;;
    esac
    i=$((i + 1))
  done
  echo 0
  return 1
}

now_epoch="$(date +%s)"
year="$(date +%Y)"

while IFS= read -r line; do
  [ -z "$line" ] && continue

  # Ignorar linhas de cabeçalho/rodapé
  case "$line" in
    wtmp\ begins*|btmp\ begins*|reboot*|shutdown*) continue ;;
  esac

  set -- $line
  user="$1"
  [ -z "$user" ] && continue

  # Contar entradas
  count["$user"]=$(( ${count["$user"]:-0} + 1 ))

  # Token final
  lasttok="${!#}"

  # Caso com duração no fim: "(HH:MM)" ou "(D+HH:MM)"
  case "$lasttok" in
    \(*\))
      d="${lasttok#(}"
      d="${d%)}"
      add="$(dur_to_seconds "$d")"
      seconds["$user"]=$(( ${seconds["$user"]:-0} + add ))
      continue
      ;;
  esac

  # Caso "still logged in" (sem "(...)"): calcular agora - login_time
  mi="$(month_index "$@")"
  if [ "$mi" -eq 0 ]; then
    # formato inesperado; ignora
    continue
  fi

  # tokens: ... <Mon> <Day> <HH:MM> ...
  eval mon="\${$mi}"
  eval day="\${$((mi+1))}"
  eval hm="\${$((mi+2))}"

  login_epoch="$(date -d "$year $mon $day $hm" +%s 2>/dev/null)"
  if [ -z "$login_epoch" ]; then
    continue
  fi

  add=$(( now_epoch - login_epoch ))
  [ "$add" -lt 0 ] && add=0
  seconds["$user"]=$(( ${seconds["$user"]:-0} + add ))

done < "$FILE"

echo "Utilizador | Entradas | Tempo total"
echo "-----------------------------------"

for u in "${!count[@]}"; do
  total="${seconds["$u"]:-0}"
  hh=$(( total / 3600 ))
  mm=$(( (total % 3600) / 60 ))
  ss=$(( total % 60 ))
  printf "%-9s | %-8s | %02d:%02d:%02d\n" "$u" "${count["$u"]}" "$hh" "$mm" "$ss"
done