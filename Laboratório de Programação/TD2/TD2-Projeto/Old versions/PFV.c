#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define BUF_SIZE 8192

/* ==== dynvec genérico ==== */
typedef struct {
    void   *data;
    size_t  elem_size, length, capacity;
} dynvec;
typedef int (*cmpf)(const void*, const void*);
static dynvec *dynvec_create(size_t sz) {
    dynvec *v = malloc(sizeof *v);
    if (!v) {
        fprintf(stderr, "Erro de memória dynvec_create\n");
        exit(EXIT_FAILURE);
    }
    v->elem_size = sz;
    v->length    = 0;
    v->capacity  = 16;
    v->data      = malloc(sz * v->capacity);
    if (!v->data) {
        fprintf(stderr, "Erro de memória dynvec_create data\n");
        exit(EXIT_FAILURE);
    }
    return v;
}
static void dynvec_free(dynvec *v) {
    free(v->data);
    free(v);
}
static size_t dynvec_length(const dynvec *v) {
    return v->length;
}
static void *dynvec_get(const dynvec *v, size_t i) {
    return (char*)v->data + i * v->elem_size;
}
static void dynvec_push(dynvec *v, const void *elem) {
    if (v->length == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * v->elem_size);
        if (!v->data) {
            fprintf(stderr, "Erro de memória dynvec_push\n");
            exit(EXIT_FAILURE);
        }
    }
    memcpy((char*)v->data + v->length * v->elem_size, elem, v->elem_size);
    v->length++;
}

/* ==== MergeSort genérico ==== */
static void merge_seg(void *base, void *tmp,
                      size_t sz, size_t l, size_t m, size_t r,
                      cmpf cmp)
{
    char *src = tmp, *dst = base;
    size_t i = l, j = m;
    for (size_t k = l; k < r; k++) {
        if (i < m && (j >= r || cmp(src + i*sz, src + j*sz) <= 0)) {
            memcpy(dst + k*sz, src + i*sz, sz);
            i++;
        } else {
            memcpy(dst + k*sz, src + j*sz, sz);
            j++;
        }
    }
}
static void mergesort_rec(dynvec *v, void *tmp,
                          size_t l, size_t r, cmpf cmp)
{
    if (r - l < 2) return;
    size_t m = l + (r - l)/2;
    mergesort_rec(v, tmp, l, m, cmp);
    mergesort_rec(v, tmp, m, r, cmp);
    memcpy((char*)tmp + l*v->elem_size,
           (char*)v->data + l*v->elem_size,
           (r - l) * v->elem_size);
    merge_seg(v->data, tmp, v->elem_size, l, m, r, cmp);
}
static void mergesort_dynvec(dynvec *v, cmpf cmp) {
    if (v->length < 2) return;
    void *tmp = malloc(v->length * v->elem_size);
    if (!tmp) {
        fprintf(stderr, "Erro de memória mergesort\n");
        exit(EXIT_FAILURE);
    }
    mergesort_rec(v, tmp, 0, v->length, cmp);
    free(tmp);
}

/* ==== Tokenização UTF-8 + lowercase ==== */
// Reconhece ASCII A–Z, a–z e acentos U+00C0–U+00FF
static int is_letter_utf8(unsigned char c1, unsigned char c2, int *len) {
    if ((c1 >= 'A' && c1 <= 'Z') || (c1 >= 'a' && c1 <= 'z')) {
        *len = 1; return 1;
    }
    if (c1 == 0xC3 && c2 >= 0x80 && c2 <= 0xBF) {
        *len = 2; return 1;
    }
    *len = 1;
    return 0;
}
static void append_lower(char *buf, int *bidx,
                         unsigned char c1, unsigned char c2, int sz)
{
    if (sz == 1) {
        buf[(*bidx)++] = (char)tolower(c1);
    } else {
        buf[(*bidx)++] = (char)c1;
        buf[(*bidx)++] = (char)((c2 >= 0x80 && c2 <= 0x9E) ? c2 + 0x20 : c2);
    }
}

/* ==== Contagem ==== */
typedef struct {
    char *word;
    int   count;
} WC;
static int cmp_strp(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}
static int cmp_wc(const void *a, const void *b) {
    const WC *A = a, *B = b;
    if (A->count != B->count) return B->count - A->count;
    return strcmp(A->word, B->word);
}

int main(void) {
    setlocale(LC_ALL, "");

    int N;
    if (scanf("%d", &N) != 1) return 0;
    getchar();  // descarta newline

    dynvec *words = dynvec_create(sizeof(char*));
    char bufw[BUF_SIZE];
    int bidx = 0;
    char line[4096];

    // Ler e tokenizar todo stdin
    while (fgets(line, sizeof line, stdin)) {
        unsigned char *p = (unsigned char*)line;
        size_t L = strlen(line);
        for (size_t i = 0; i < L; ) {
            int sl;
            if (i + 1 < L && is_letter_utf8(p[i], p[i+1], &sl)) {
                if (bidx + sl >= BUF_SIZE - 1) {
                    // palavra muito longa: descarta conteúdo atual
                    bidx = 0;
                }
                append_lower(bufw, &bidx, p[i], p[i+1], sl);
                i += sl;
            } else {
                if (bidx > 0) {
                    bufw[bidx] = '\0';
                    char *w = strdup(bufw);
                    if (!w) { fprintf(stderr, "Erro strdup\n"); exit(EXIT_FAILURE); }
                    dynvec_push(words, &w);
                    bidx = 0;
                }
                i++;
            }
        }
    }
    if (bidx > 0) {
        bufw[bidx] = '\0';
        char *w = strdup(bufw);
        if (!w) { fprintf(stderr, "Erro strdup\n"); exit(EXIT_FAILURE); }
        dynvec_push(words, &w);
    }

    // Agrupar e contar
    mergesort_dynvec(words, cmp_strp);
    dynvec *counts = dynvec_create(sizeof(WC));
    size_t W = dynvec_length(words);
    for (size_t i = 0; i < W; ) {
        char *w0 = *(char**)dynvec_get(words, i);
        size_t j = i + 1;
        while (j < W && strcmp(w0, *(char**)dynvec_get(words, j)) == 0) j++;
        WC rec = { strdup(w0), (int)(j - i) };
        if (!rec.word) { fprintf(stderr, "Erro strdup\n"); exit(EXIT_FAILURE); }
        dynvec_push(counts, &rec);
        free(w0);
        i = j;
    }
    dynvec_free(words);

    // Ordenar por frequência e imprimir top N
    mergesort_dynvec(counts, cmp_wc);
    size_t C = dynvec_length(counts);
    size_t lim = N < (int)C ? N : C;
    for (size_t i = 0; i < lim; i++) {
        WC *e = dynvec_get(counts, i);
        printf("%d %s\n", e->count, e->word);
        free(e->word);
    }
    dynvec_free(counts);
    return 0;
}
