#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynvec.h"
#include <stddef.h>
#include <ctype.h>
#include "palavra.h"

#define MAX_LINE 100


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <arquivo_de_teste>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (!file)
    {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    dynvec *vec = NULL;
    char line[MAX_LINE];
    size_t elem_size = sizeof(palavra);

    vec = dynvec_create(elem_size);
    int n;

    if (fgets(line, sizeof(line), file) != NULL)
    {
        n = atoi(line);
    }
    

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;

        is_in_dynvec(vec, line);
    }

    dynvec_mergesort(vec, comparar_contagem);
    dynvec_print(vec, n);
    dynvec_free(vec);
    fclose(file);

    return 0;
}
