#!/bin/bash
# exwhile1 - contador 1..20 com while

i=1
while [ $i -le 20 ]; do
  echo "$i"
  i=$((i + 1))
done