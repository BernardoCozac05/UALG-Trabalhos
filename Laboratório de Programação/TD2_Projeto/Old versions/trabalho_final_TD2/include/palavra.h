#ifndef PALAVRA_H
#define PALAVRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynvec.h"



typedef struct palavra
{
    char data[20];
    int contagem;
} palavra;


// static void aumentar_conatgem
void is_in_dynvec(dynvec *v, char *str);
int return_contagem(palavra *p);
char *return_string(palavra *p);

// Funç~es de comparação
int comparar_chaves(const void *a, const void *b);
int comparar_contagem(const void *a, const void *b);
int comparar_palavra(const void *a, const void *b);


#endif // PALAVRA_H