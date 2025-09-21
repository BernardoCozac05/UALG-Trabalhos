#include "palavra.h"
#include "dynvec.h"

// Funções de Comparação ##########################################

int comparar_chaves(const void *a, const void *b)
{
    return strcmp(a,b);
}

int comparar_palavra(const void *a, const void *b)
{
    return strcmp(((palavra *)a)->data, ((palavra *)b)->data);
}

int comparar_contagem(const void *a, const void *b)
{
    return ((palavra *)b)->contagem - ((palavra *)a)->contagem;
}

// ######################################################################

palavra *create_palavra(char *pala)
{

    palavra *p = (palavra *)malloc(sizeof(palavra));

    if (!p)
    {
        return NULL;
    }

    p->contagem = 1;

    strncpy(p->data, pala, 19);
    p->data[19] = '\0';

    return p;
}

static void aumentar_contagem(dynvec *v, int index)
{
    palavra *p = dynvec_get(v, index);
    p->contagem++;
}

void is_in_dynvec(dynvec *v, char *str)
{
    char *token;
    char *delimeter = " !#$&'()*+,-./:;<=>?@[]^_`{|}~^\t\"";
    token = strtok(str, delimeter);

    while (token != NULL)
    {
        int resultado = dynvec_busca_binaria(v, 0, dynvec_length(v), token, comparar_chaves);

        if (resultado == -1)
        {
            palavra *p = NULL;
            p = create_palavra(token);

            dynvec_push(v, p);
            insertion_sort_dynvec(v, comparar_palavra);
        }
        else
        {
            aumentar_contagem(v, resultado);
        }

        token = strtok(NULL, delimeter);
    }
}

int return_contagem(palavra *p)
{

    return p->contagem;
}

char *return_string(palavra *p)
{

    return p->data;
}