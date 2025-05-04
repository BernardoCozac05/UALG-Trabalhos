Compilação:
gcc -Wall Pollos.c -lm -o Pollos

Execução:
{ echo 63; cat padre_amaro.txt; } | ./Pollos

Extrair o output para .txt:
{ echo 63; cat padre_amaro.txt; } | ./Pollos > realoutput.txt

Comparar ao output fornecido pelo TD2:
diff realoutput.txt wantedoutput.txt


Nota:
Pollos.c - Codigo totalmente funcional utilizando o segundo script, sem new line no final (Melhor Opção)
Pollos1nl.c - Codigo totalmente funcional utilizando o segundo script, com new line no final
Pollos2nl.c - Codigo totalmente funcional utilizando o segundo script, com 2 new line no final