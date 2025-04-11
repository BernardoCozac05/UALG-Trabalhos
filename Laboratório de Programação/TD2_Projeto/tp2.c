#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*  ------------------------------------------------------------------
    Estrutura base do vetor dinâmico genérico (dynvec)
    ------------------------------------------------------------------ */
typedef struct {
    void  *data;       /* Ponteiro para a área de memória */
    size_t length;     /* Número atual de elementos armazenados */
    size_t capacity;   /* Capacidade alocada (em nº de elementos) */
    size_t elem_size;  /* Tamanho em bytes de cada elemento */
} dynvec;

/* 
 * Funções do dynvec 
 */

/* Cria um dynvec para armazenar elementos de tamanho elem_size */
static dynvec *dynvec_create(size_t elem_size) {
    dynvec *v = (dynvec *)malloc(sizeof(dynvec));
    if (!v) {
        fprintf(stderr, "ERRO: falha ao criar dynvec.\n");
        exit(EXIT_FAILURE);
    }
    v->elem_size = elem_size;
    v->length    = 0;
    v->capacity  = 16; /* capacidade inicial */
    v->data      = malloc(v->capacity * elem_size);
    if (!v->data) {
        fprintf(stderr, "ERRO: falha ao alocar data.\n");
        exit(EXIT_FAILURE);
    }
    return v;
}

/* Liberta a memória de data + o próprio dynvec */
static void dynvec_free(dynvec *v) {
    if (!v) return;
    free(v->data);
    free(v);
}

/* Adiciona um elemento ao final do dynvec (cópia) */
static void dynvec_push(dynvec *v, const void *elem) {
    if (v->length >= v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * v->elem_size);
        if (!v->data) {
            fprintf(stderr, "ERRO: realocação falhou.\n");
            exit(EXIT_FAILURE);
        }
    }
    /* Copia o elem para a posição [length] */
    char *dest = (char*)v->data + (v->length * v->elem_size);
    memcpy(dest, elem, v->elem_size);
    v->length++;
}

/* Acessa o i-ésimo elemento, retornando um ponteiro para dentro de data */
static void *dynvec_get(dynvec *v, size_t i) {
    if (i >= v->length) {
        fprintf(stderr, "ERRO: dynvec_get fora do limite.\n");
        exit(EXIT_FAILURE);
    }
    return (char*)v->data + (i * v->elem_size);
}

/* Retorna número de elementos */
static size_t dynvec_length(dynvec *v) {
    return v->length;
}

/* Função de ordenação genérica, usando qsort */
static void dynvec_sort(dynvec *v, int (*cmp)(const void*, const void*)) {
    qsort(v->data, v->length, v->elem_size, cmp);
}

/*  ------------------------------------------------------------------
    Estrutura e funções de contagem de palavras
    ------------------------------------------------------------------ */

/* struct para guardar (palavra, contagem) */
typedef struct {
    char *word;
    int  count;
} WordCount;

/* Função para comparar alfabeticamente (a < b => -1; a > b => +1) */
static int compareAlpha(const void *p1, const void *p2) {
    const WordCount *w1 = (const WordCount *)p1;
    const WordCount *w2 = (const WordCount *)p2;
    return strcmp(w1->word, w2->word);
}

/* Função para comparar por contagem decrescente */
static int compareFreq(const void *p1, const void *p2) {
    const WordCount *w1 = (const WordCount *)p1;
    const WordCount *w2 = (const WordCount *)p2;
    /* Para ordem decrescente: se w2->count > w1->count => + */
    return (w2->count - w1->count);
}

/* Duplica string */
static char *dupString(const char *s) {
    size_t len = strlen(s);
    char *out = (char *)malloc(len + 1);
    if (!out) {
        fprintf(stderr, "ERRO: malloc falhou\n");
        exit(EXIT_FAILURE);
    }
    strcpy(out, s);
    return out;
}

/*  ------------------------------------------------------------------
    PROGRAMA PRINCIPAL
    ------------------------------------------------------------------ */
int main(void) {
    /* 1) Ler n (quantas palavras mais frequentes exibir) */
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "ERRO: Falha ao ler n\n");
        return 1;
    }
    /* Descarta resto da linha, se houver */
    getchar();

    /* 2) dynvec para guardar (word, count=0) temporariamente */
    dynvec *tempVec = dynvec_create(sizeof(WordCount));

    /* 3) Ler do stdin linha a linha */
    char line[8192];
    while (fgets(line, sizeof(line), stdin)) {
        /* Separar as "palavras" só alfabéticas, 
           usando como delimitadores tudo o resto. */
        char *token = strtok(line,
            " \t\r\n.,!?;:\"()[]{}-_=+/\\0123456789“”’‘«»—…");
        while (token) {
            /* converter em minúsculas */
            for (char *p = token; *p; p++) {
                *p = (char)tolower((unsigned char)*p);
            }
            /* Guardar no dynvec */
            WordCount wc;
            wc.word  = dupString(token); /* duplicamos a string */
            wc.count = 0;                /* para já, zero */
            dynvec_push(tempVec, &wc);

            token = strtok(NULL,
               " \t\r\n.,!?;:\"()[]{}-_=+/\\0123456789“”’‘«»—…");
        }
    }

    /* Verificar se lemos alguma palavra */
    size_t totalWords = dynvec_length(tempVec);
    if (totalWords == 0) {
        fprintf(stderr, "Nenhuma palavra lida.\n");
        dynvec_free(tempVec);
        return 0;
    }

    /* 4) Ordenar alfabeticamente (1ª função de ordenação) */
    dynvec_sort(tempVec, compareAlpha);

    /* 5) Agrupar (estilo uniq -c) num segundo dynvec: 
          ao percorrer tempVec (ordenado), somamos as repetições. */
    dynvec *grouped = dynvec_create(sizeof(WordCount));
    size_t i = 0;
    while (i < totalWords) {
        WordCount *wcPtr = (WordCount *)dynvec_get(tempVec, i);
        char *currWord   = wcPtr->word;
        int   count      = 1;
        i++;
        /* Avança enquanto a mesma palavra se repetir */
        while (i < totalWords) {
            WordCount *wcNext = (WordCount *)dynvec_get(tempVec, i);
            if (strcmp(wcNext->word, currWord) == 0) {
                count++;
                i++;
            } else {
                break;
            }
        }
        /* Guardamos (currWord, count) em grouped */
        WordCount newWC;
        newWC.word  = currWord; /* Reaproveitamos o ponteiro da string */
        newWC.count = count;
        dynvec_push(grouped, &newWC);
    }

    /* Libertar o tempVec (mas sem libertar as strings, pois agora
       elas foram aproveitadas no grouped). Precisamos só libertar
       a estrutura base do tempVec. */
    free(tempVec->data);
    free(tempVec);

    /* 6) Ordenar grouped por contagem decrescente (2ª função de ordenação) */
    dynvec_sort(grouped, compareFreq);

    /* 7) Imprimir as n mais frequentes */
    size_t gLen = dynvec_length(grouped);
    size_t limit = (n < (int)gLen) ? n : gLen;
    for (size_t k = 0; k < limit; k++) {
        WordCount *pWC = (WordCount *)dynvec_get(grouped, k);
        printf("%d %s\n", pWC->count, pWC->word);
    }

    /* 8) Limpar (grouped):
       Cada pWC->word foi alocado com dupString.
       Precisamos libertar 1 a 1.
    */
    for (size_t k = 0; k < gLen; k++) {
        WordCount *pWC = (WordCount *)dynvec_get(grouped, k);
        free(pWC->word);
    }
    dynvec_free(grouped);

    return 0;
}
