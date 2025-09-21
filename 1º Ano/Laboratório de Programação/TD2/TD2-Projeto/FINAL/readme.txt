Para Testar o programa com o 2 Script(Pollos)

Compilação:
gcc -Wall Pollos.c -lm -o Pollos

Execução:
{ echo 63; cat padre_amaro.txt; } | ./Pollos

Extrair o output para .txt:
{ echo 63; cat padre_amaro.txt; } | ./Pollos > realoutput.txt

Comparar ao output fornecido pelo TD2:
diff realoutput.txt wantedoutput.txt

-------------------------------------------------------------------

Para Testar o programa com o 1 Script(Pollos1s)

Compilação:
gcc -Wall Pollos1s.c -lm -o Pollos1s

Execução:
{ echo 30; cat padre_amaro.txt; } | ./Pollos1s

Extrair o output para .txt:
{ echo 30; cat padre_amaro.txt; } | ./Pollos1s > realoutput1s.txt

Comparar ao output fornecido pelo TD2:
diff realoutput1s.txt wantedoutput1s.txt


Nota:
Pollos.c - Codigo totalmente funcional utilizando o segundo script, sem new line no final (Melhor Opção)
Pollos_commented.c - Codigo totalmente funcional utilizando o segundo script, sem new line no final, completamente comentario a explicar tudo
Pollos1nl.c - Codigo totalmente funcional utilizando o segundo script, com new line no final
Pollos2nl.c - Codigo totalmente funcional utilizando o segundo script, com 2 new line no final
Pollos1s.c - Codigo totalmente funcional utilizando o primeiro script, sem new line no final 
Pollos(old).c - Uma versão antiga, provavelmente não funcional a 100%, mas chega perto