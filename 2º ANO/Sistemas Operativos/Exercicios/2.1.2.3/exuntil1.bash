#!/bin/bash
# exuntil1 - contador 1..20 com until

i=1
until [ $i -gt 20 ]; do
  echo "$i"
  i=$((i + 1))
done