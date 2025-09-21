#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#define MAX_LINE 100
#define DYNVEC_INIT 4

// ################################################################################################################

typedef struct palavra
{
    char data[20];
    int contagem;
} palavra;

typedef struct dynvec
{
    void *data;
    size_t elem_size;
    size_t capacity;
    size_t length;
} dynvec;



// ################################################ DYNVEC ############################################################
// ################################################ DYNVEC ############################################################
// ################################################ DYNVEC ############################################################
// ################################################ DYNVEC ############################################################

dynvec *dynvec_create(size_t elem_size)
{

    dynvec *v = (dynvec *)malloc(sizeof(dynvec));

    if (!v)
    {
        return NULL;
    }

    v->capacity = DYNVEC_INIT;
    v->elem_size = elem_size;
    v->length = 0;
    v->data = malloc(v->elem_size * v->capacity);

    if (!(v->data))
    {
        free(v);
        return NULL;
    }

    return v;
}

void dynvec_free(dynvec *v)
{

    if (!v)
    {
        return;
    }

    free(v->data);
    free(v);
}

static void dynvec_resize(dynvec *v, size_t new_size)
{
    if (!v)
    {
        return;
    }

    if (v->capacity > new_size)
    {
        return;
    }

    void *new_data = realloc(v->data, new_size * v->elem_size);

    if (!new_data)
    {
        return;
    }

    v->data = new_data;
    v->capacity = new_size;
    return;
}

void dynvec_push(dynvec *v, void *elem)
{

    if (!v)
    {
        return;
    }

    if (v->length == v->capacity)
    {
        dynvec_resize(v, 2 * v->capacity);
    }

    memcpy((char *)v->data + v->length * v->elem_size, elem, v->elem_size);

    v->length++;
    return;
}

int return_contagem(palavra *pa)
{

    return pa->contagem;
}

char *return_string(palavra *pa)
{

    return pa->data;
}

void dynvec_print(dynvec *v, int n)
{
    if (!v || !v->data)
    {
        printf("Erro: Vetor inválido!\n");
        return;
    }

    for (size_t i = 0; i < v->length && i < (size_t)n; i++)
    {
        palavra *value = (palavra *)((char *)v->data + i * v->elem_size);
        printf("%d %s\n", return_contagem(value), return_string(value));
    }
}

// ##############################################################################

int dynvec_length(dynvec *v)
{
    if (v == NULL)
    {
        return -1;
    }
    return v->length - 1;
}

void *dynvec_get(dynvec *v, int index)
{

    if (index >= (int)v->length)
    {
        return NULL;
    }
    return (char *)v->data + (index * v->elem_size);
}

void dynvec_set(dynvec *v, size_t index, void *value)
{
    if (index < v->length)
    {
        memcpy((char *)v->data + index * v->elem_size, value, v->elem_size);
    }
    return;
}

// ##############################################################################

int dynvec_busca_binaria(dynvec *v, int esquerda, int direita, void *chave, int (*cmp)(const void *, const void *))
{
    int resultado = -1;
    while (esquerda <= direita)
    {
        int meio = esquerda + (direita - esquerda) / 2;
        if (!cmp((char *)(v->data + meio * v->elem_size), chave))
        {
            resultado = meio;
            direita = meio - 1;
        }
        else if (cmp((char *)(v->data + meio * v->elem_size), chave) < 0)
        {
            esquerda = meio + 1;
        }
        else
        {
            direita = meio - 1;
        }
    }
    return resultado;
}

// ########################################################################################

void insertion_sort_dynvec(dynvec *v, int (*cmp)(const void *, const void *))
{

    int i, j;

    void *k = malloc(v->elem_size);

    if (k == NULL)
    {
        return;
    }

    for (i = 1; i < (int)v->length; i++)
    {
        memcpy(k, dynvec_get(v, i), v->elem_size);

        for (j = i; 0 < j && cmp(k, (char *)v->data + (j - 1) * v->elem_size) < 0; j--)
        {

            // v[j] = v[j-1];
            // memcpy((char *)v->data + j * v->elem_size, (char *)v->data + (j-1) * v->elem_size, v->elem_size);
            dynvec_set(v, j, dynvec_get(v, j - 1));
        }

        // v[j] = k;
        // memcpy((char *)v->data + j * v->elem_size, k, v->elem_size);
        dynvec_set(v, j, k);
    }
    free(k);
}

// ##################################################################################################################

static void merge(dynvec *v, int l, int m, int r, int (*cmp)(const void *, const void *), void *tmp)
{
    int i = l, j = m + 1, k = 0;

    while (i <= m && j <= r)
    {
        if (cmp(dynvec_get(v, i), dynvec_get(v, j)) <= 0)
        {
            memcpy((char *)tmp + k * v->elem_size, dynvec_get(v, i), v->elem_size);
            i++;
        }
        else
        {
            memcpy((char *)tmp + k * v->elem_size, dynvec_get(v, j), v->elem_size);
            j++;
        }
        k++;
    }

    while (i <= m)
    {
        memcpy((char *)tmp + k * v->elem_size, dynvec_get(v, i), v->elem_size);
        i++;
        k++;
    }

    while (j <= r)
    {
        memcpy((char *)tmp + k * v->elem_size, dynvec_get(v, j), v->elem_size);
        j++;
        k++;
    }

    for (int t = 0; t < k; t++)
    {
        memcpy(dynvec_get(v, l + t), (char *)tmp + t * v->elem_size, v->elem_size);
    }
}

static void mergesort_rec(dynvec *v, int l, int r, int (*cmp)(const void *, const void *), void *tmp)
{
    if (l >= r)
        return;

    int m = l + (r - l) / 2;

    mergesort_rec(v, l, m, cmp, tmp);
    mergesort_rec(v, m + 1, r, cmp, tmp);
    merge(v, l, m, r, cmp, tmp);
}

void dynvec_mergesort(dynvec *v, int (*cmp)(const void *, const void *))
{
    int n = dynvec_length(v);
    if (n < 2)
        return;

    void *tmp = malloc(v->elem_size * (n + 1));
    if (!tmp)
        return;

    mergesort_rec(v, 0, n, cmp, tmp);
}


 

// Funções de Comparação ##########################################

int comparar_chaves(const void *a, const void *b)
{
    const palavra *p = (const palavra *)a;
    const char *chave = (const char *)b;
    return strcmp(p->data, chave);
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
    char *delimeter = " !#$&'()*+,-./:;<=>?@[]^_`{|}~^";
    token = strtok(str, delimeter);

    while (token != NULL)
    {
        palavra *p = NULL;
        p = create_palavra(token);

        int resultado = dynvec_busca_binaria(v, 0, dynvec_length(v), p, comparar_chaves);

        if (resultado == -1)
        {
            dynvec_push(v, p);
            insertion_sort_dynvec(v, comparar_palavra);
        }
        else
        {
            free(p);
            aumentar_contagem(v, resultado);
        }

        token = strtok(NULL, delimeter);
    }
}

/*
void is_in_dynvec(dynvec *v, char *str)
{

    char *token;
    token = strtok(str, " ");

    while (token != NULL)
    {
        palavra *pala = NULL;
        pala = create_palavra(token);

        int resultado = dynvec_busca_binaria(v, 0, dynvec_length(v), pala, comparar_chaves);

        if (resultado == -1)
        {
            dynvec_push(v, pala);
            insertion_sort_dynvec(v, comparar_palavra);
        }
        else
        {
            aumentar_contagem(v, resultado);
        }

        token = strtok(NULL, " ");
    }
}
*/

   // ################################################ MAIN ############################################################
    // ################################################ MAIN ############################################################
    // ################################################ MAIN ############################################################
    // ################################################ MAIN ############################################################




 /*   void remover_simbolos(char *str)
{
    size_t j = 0;
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (!ispunct(str[i]))
        {
            str[j] = str[i];
            j++;
        }
    }

    str[j] = '\0';
}
*/

int main()
{
    

    dynvec *vec = NULL;
    char line[MAX_LINE];
    size_t elem_size = sizeof(palavra);

    vec = dynvec_create(elem_size);
    int n;

    if (fgets(line, sizeof(line), stdin))
    {
        n = atoi(line);
    }

    //int i = 0;

    while (fgets(line, sizeof(line), stdin))
    {
        line[strcspn(line, "\n")] = 0;

        //remover_simbolos(line);
        is_in_dynvec(vec, line);

       /* i++;
        
        if (i == 10)
        {
            break;
        }
            */
        
    }

    dynvec_mergesort(vec, comparar_contagem);
    dynvec_print(vec, n);

    dynvec_free(vec);

    return 0;
}