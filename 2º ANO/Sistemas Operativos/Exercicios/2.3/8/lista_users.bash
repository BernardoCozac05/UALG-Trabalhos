#!/bin/bash
# lista_users.sh - mostra login e nome (GECOS) a partir de /etc/passwd

while IFS=: read -r login x uid gid gecos home shell; do
  nome="$gecos"
  case "$nome" in
    *,*) nome="${nome%%,*}" ;;
  esac
  printf "%-20s %s\n" "$login" "$nome"
done < /etc/passwd