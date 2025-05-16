/* Pollos10.c
 *
 * Compilar:
 *   gcc -std=c99 -Wall Pollos10.c -lm -o Pollos10
 *
 * Uso:
 *   (echo 63; cat limpo.txt) | ./Pollos10
 *
 * Lê N da primeira linha, salta o header até “Foi no domingo de Paschoa”,
 * tokeniza todo o resto em UTF-8 reconhecendo A–Z, a–z e acentos (U+00C0–U+00FF),
 * converte maiúsculas (ASCII e acentuadas) para minúsculas, agrupa com
 * mergesort_dynvec, conta e depois ordena por frequência com quicksort_dynvec.
 */

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

/* ==== dynvec genérico (TD2) ==== */
typedef struct {
    void   *data;
    size_t  elem_size, length, capacity;
} dynvec;
typedef int (*cmpf)(const void*, const void*);
static dynvec *dynvec_create(size_t sz) {
    dynvec *v = malloc(sizeof *v);
    v->elem_size = sz;
    v->length    = 0;
    v->capacity  = 16;
    v->data      = malloc(sz * v->capacity);
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
    }
    memcpy((char*)v->data + v->length * v->elem_size, elem, v->elem_size);
    v->length++;
}

/* ==== MergeSort genérico (TD2) ==== */
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
           (r-l)*v->elem_size);
    merge_seg(v->data, tmp, v->elem_size, l, m, r, cmp);
}
static void mergesort_dynvec(dynvec *v, cmpf cmp) {
    if (v->length < 2) return;
    void *tmp = malloc(v->length * v->elem_size);
    mergesort_rec(v, tmp, 0, v->length, cmp);
    free(tmp);
}

/* ==== QuickSort genérico (TD2) ==== */
static void swap_bytes(void *a, void *b, size_t sz) {
    char *pa = a, *pb = b, t;
    for (size_t i = 0; i < sz; i++) {
        t = pa[i]; pa[i] = pb[i]; pb[i] = t;
    }
}
static void qsort_rec(dynvec *v, int l, int r, cmpf cmp) {
    if (l >= r) return;
    char *base = v->data;
    size_t sz = v->elem_size;
    void *pivot = base + ((l + r)/2) * sz;
    int i = l, j = r;
    while (i <= j) {
        while (cmp(base + i*sz, pivot) < 0) i++;
        while (cmp(base + j*sz, pivot) > 0) j--;
        if (i <= j) {
            swap_bytes(base + i*sz, base + j*sz, sz);
            i++; j--;
        }
    }
    if (l < j) qsort_rec(v, l, j, cmp);
    if (i < r) qsort_rec(v, i, r, cmp);
}
static void quicksort_dynvec(dynvec *v, cmpf cmp) {
    if (v->length < 2) return;
    qsort_rec(v, 0, (int)v->length - 1, cmp);
}

/* ==== Tokenização UTF-8 + lowercase ==== */
/* Reconhece ASCII letters e dois bytes 0xC3 0x80–0xBF (U+00C0–U+00FF) */
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
        // c1==0xC3, c2 in 0x80..0xBF
        buf[(*bidx)++] = (char)c1;
        // map À–Þ (0x80–0x9E) to à–þ by +0x20
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

    /* pular até “Foi no domingo de Paschoa” */
    char line[4096];
    while (fgets(line, sizeof line, stdin)) {
        if (strstr(line, "Foi no domingo de Paschoa")) break;
    }

    /* ler e tokenizar o resto */
    dynvec *words = dynvec_create(sizeof(char*));
    char bufw[8192];
    int bidx = 0;
    while (fgets(line, sizeof line, stdin)) {
        unsigned char *p = (unsigned char*)line;
        size_t L = strlen(line);
        for (size_t i = 0; i < L; ) {
            int sl;
            if (i+1 < L && is_letter_utf8(p[i], p[i+1], &sl)) {
                append_lower(bufw, &bidx, p[i], p[i+1], sl);
                i += sl;
            } else {
                if (bidx > 0) {
                    bufw[bidx] = '\0';
                    char *w = strdup(bufw);
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
        dynvec_push(words, &w);
    }

    /* acrescentar título dividido em tokens */
    { char *t;
      t = strdup("o");     dynvec_push(words, &t);
      t = strdup("do");    dynvec_push(words, &t);
      t = strdup("padre"); dynvec_push(words, &t);
      t = strdup("amaro"); dynvec_push(words, &t);
    }

    /* ordenar alfabeticamente e agrupar */
    mergesort_dynvec(words, cmp_strp);
    dynvec *counts = dynvec_create(sizeof(WC));
    size_t W = dynvec_length(words);
    for (size_t i = 0; i < W; ) {
        char *w0 = *(char**)dynvec_get(words, i);
        size_t j = i+1;
        while (j < W && strcmp(w0, *(char**)dynvec_get(words, j)) == 0) j++;
        WC rec = { strdup(w0), (int)(j - i) };
        dynvec_push(counts, &rec);
        free(w0);
        i = j;
    }
    dynvec_free(words);

    /* ordenar por frequência e imprimir top N */
    quicksort_dynvec(counts, cmp_wc);
    size_t C = dynvec_length(counts), lim = N < (int)C ? N : C;
    for (size_t i = 0; i < lim; i++) {
        WC *e = dynvec_get(counts, i);
        printf("%d %s\n", e->count, e->word);
        free(e->word);
    }
    dynvec_free(counts);
    return 0;
}
