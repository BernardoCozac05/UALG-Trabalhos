#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* -----------------------------------------------------------------
   1) Vetor dinâmico genérico (dynvec) – tudo num só ficheiro
   ----------------------------------------------------------------- */
typedef struct {
    void  *data;       /* Área de memória alocada */
    size_t length;     /* Nº de elementos usados */
    size_t capacity;   /* Capacidade (em elementos) */
    size_t elem_size;  /* Tamanho em bytes de cada elemento */
} dynvec;

static dynvec *dynvec_create(size_t elem_size) {
    dynvec *v = (dynvec *)malloc(sizeof(dynvec));
    if (!v) {
        fprintf(stderr, "ERRO: falha ao criar dynvec.\n");
        exit(EXIT_FAILURE);
    }
    v->elem_size = elem_size;
    v->length    = 0;
    v->capacity  = 16;
    v->data      = malloc(v->capacity * elem_size);
    if (!v->data) {
        fprintf(stderr, "ERRO: falha ao alocar dynvec->data.\n");
        exit(EXIT_FAILURE);
    }
    return v;
}

static void dynvec_free(dynvec *v) {
    if (!v) return;
    free(v->data);
    free(v);
}

static void dynvec_push(dynvec *v, const void *elem) {
    if (v->length >= v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * v->elem_size);
        if (!v->data) {
            fprintf(stderr, "ERRO: realocação falhou.\n");
            exit(EXIT_FAILURE);
        }
    }
    char *dest = (char*)v->data + (v->length * v->elem_size);
    memcpy(dest, elem, v->elem_size);
    v->length++;
}

static size_t dynvec_length(dynvec *v) {
    return v->length;
}

static void *dynvec_get(dynvec *v, size_t i) {
    if (i >= v->length) {
        fprintf(stderr, "ERRO: dynvec_get fora do limite!\n");
        exit(EXIT_FAILURE);
    }
    return (char*)v->data + (i * v->elem_size);
}

/* Usamos qsort para ordenar */
static void dynvec_sort(dynvec *v, int (*cmp)(const void*, const void*)) {
    qsort(v->data, v->length, v->elem_size, cmp);
}

/* -----------------------------------------------------------------
   2) Estrutura (word, count) + comparadores
   ----------------------------------------------------------------- */
typedef struct {
    char *word; /* string alocada */
    int  count;
} WordCount;

/* Ordenação alfabética (diferenciando maiúsc/minúsc) */
static int compareAlpha(const void *p1, const void *p2) {
    const WordCount *a = (const WordCount*)p1;
    const WordCount *b = (const WordCount*)p2;
    return strcmp(a->word, b->word);
}

/* Ordenação por contagem desc */
static int compareFreq(const void *p1, const void *p2) {
    const WordCount *a = (const WordCount*)p1;
    const WordCount *b = (const WordCount*)p2;
    return (b->count - a->count); /* desc */
}

/* -----------------------------------------------------------------
   3) Remove acentos em CP1252/Latin-1 (sem alterar maiúsc/minúsc)
   ----------------------------------------------------------------- */
static unsigned char remove_accent_cp1252(unsigned char c) {
    /* Tabela parcial (pode adicionar mais). 
       Valor decimal = ISO-8859-1 / CP1252
         À=192 Á=193 Â=194 Ã=195 Ä=196 Å=197 
         à=224 á=225 â=226 ã=227 ä=228 å=229
         É=201  é=233  etc. 
    */
    switch(c) {
        /* A..Å (maiúsculo) (192..197) */
        case 192: case 193: case 194: case 195:
        case 196: case 197:
            return 'A';
        /* a..å (minúsculo) (224..229) */
        case 224: case 225: case 226: case 227:
        case 228: case 229:
            return 'a';

        /* E..Ë (200..203) */
        case 200: case 201: case 202: case 203:
            return 'E';
        /* e..ë (232..235) */
        case 232: case 233: case 234: case 235:
            return 'e';

        /* I..Ï (204..207) */
        case 204: case 205: case 206: case 207:
            return 'I';
        /* i..ï (236..239) */
        case 236: case 237: case 238: case 239:
            return 'i';

        /* O..Ö (210..214), Õ=213 */
        case 210: case 211: case 212: case 213: case 214:
            return 'O';
        /* o..ö (242..246), õ=245 */
        case 242: case 243: case 244: case 245: case 246:
            return 'o';

        /* U..Ü (217..220) */
        case 217: case 218: case 219: case 220:
            return 'U';
        /* u..ü (249..252) */
        case 249: case 250: case 251: case 252:
            return 'u';

        /* Ç=199, ç=231 */
        case 199:
            return 'C';
        case 231:
            return 'c';

        /* Se quiser mapear ß -> s, etc., inclua aqui. */

        default:
            return c; /* se não for acentuado, retorna inalterado */
    }
}

/* Remove acentos em-lugar. Mantém maiúsc/minúscula. */
static void remove_accents_in_place(char *s) {
    for (char *p = s; *p; p++) {
        unsigned char c = (unsigned char)*p;
        c = remove_accent_cp1252(c);
        *p = (char)c;
    }
}

/* -----------------------------------------------------------------
   4) MAIN: Lê n, separa tokens, remove acentos, ranking
   ----------------------------------------------------------------- */
int main(void) {
    /* Lê n */
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "ERRO: Falha ao ler n\n");
        return 1;
    }
    getchar(); /* descartar resto da linha */

    /* Vetor para armazenar as palavras (word, count=0) */
    dynvec *tempVec = dynvec_create(sizeof(WordCount));

    /* Ler todo o stdin (ex.: cat padre_amaro.txt) */
    char line[8192];
    while (fgets(line, sizeof(line), stdin)) {
        /* Quebramos em tokens só alfabéticos (discriminando 
           pontuação, dígitos, etc.). Em CP1252, letras = 0..255 
           mas aqui simplificamos.  */
        char *token = strtok(line,
             " \t\r\n.,!?;:\"()[]{}-_=+/\\0123456789“”’‘«»—…");
        while (token) {
            /* Remove acentos, mas mantém uppercase/minúscula */
            remove_accents_in_place(token);

            WordCount wc;
            wc.count = 0;
            wc.word  = (char*)malloc(strlen(token)+1);
            if (!wc.word) {
                fprintf(stderr, "ERRO: malloc wc.word\n");
                exit(EXIT_FAILURE);
            }
            strcpy(wc.word, token);

            dynvec_push(tempVec, &wc);

            token = strtok(NULL,
               " \t\r\n.,!?;:\"()[]{}-_=+/\\0123456789“”’‘«»—…");
        }
    }

    size_t total = dynvec_length(tempVec);
    if (total == 0) {
        fprintf(stderr, "Nenhuma palavra lida.\n");
        dynvec_free(tempVec);
        return 0;
    }

    /* 1ª Fase: Ordenar alfabeticamente */
    dynvec_sort(tempVec, compareAlpha);

    /* 2ª Fase: Agrupar (uniq -c) */
    dynvec *grouped = dynvec_create(sizeof(WordCount));
    size_t i = 0;
    while (i < total) {
        WordCount *wc1 = (WordCount*)dynvec_get(tempVec, i);
        char *currWord = wc1->word;
        int count      = 1;
        i++;
        while (i < total) {
            WordCount *wc2 = (WordCount*)dynvec_get(tempVec, i);
            if (strcmp(wc2->word, currWord) == 0) {
                count++;
                i++;
            } else {
                break;
            }
        }
        WordCount newWC;
        newWC.word  = currWord; /* reaproveitamos esse ponteiro */
        newWC.count = count;
        dynvec_push(grouped, &newWC);
    }
    /* Podem-se libertar as estruturas de tempVec (mas não as strings) */
    free(tempVec->data);
    free(tempVec);

    /* 3ª Fase: Ordenar por frequência desc */
    dynvec_sort(grouped, compareFreq);

    /* 4ª Fase: Imprimir as n mais frequentes */
    size_t gLen = dynvec_length(grouped);
    size_t limit = (n < (int)gLen) ? n : gLen;
    for (size_t k = 0; k < limit; k++) {
        WordCount *wc = (WordCount*)dynvec_get(grouped, k);
        printf("%d %s\n", wc->count, wc->word);
    }

    /* Libertar grouped (strings e estrutura) */
    for (size_t k = 0; k < gLen; k++) {
        WordCount *wc = (WordCount*)dynvec_get(grouped, k);
        free(wc->word);
    }
    dynvec_free(grouped);

    return 0;
}
