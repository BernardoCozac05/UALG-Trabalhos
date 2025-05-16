#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

/* Otimizações para compiladores GCC/Clang: Ofast, unrolling de loops e vectorização */
#pragma GCC optimize("Ofast,unroll-loops,inline")
#pragma GCC target("avx2")

/* ==== Vetor Dinâmico Genérico (`dynvec`) ==== */

typedef struct {
    void   *data;        /* Dados brutos armazenados */
    size_t  elem_size;   /* Tamanho (bytes) de um elemento */
    size_t  length;      /* Número de elementos atuais */
    size_t  capacity;    /* Capacidade máxima antes de realocar */
} dynvec;

typedef int (*cmpf)(const void*, const void*);  /* Função de comparação genérica */

static dynvec *dynvec_create(size_t elem_size) {
    dynvec *v = malloc(sizeof *v);
    if (!v) { fprintf(stderr, "Erro de memória dynvec_create\n"); exit(EXIT_FAILURE); }
    v->elem_size = elem_size;
    v->length    = 0;
    v->capacity  = 16;
    v->data      = malloc(elem_size * v->capacity);
    if (!v->data) { fprintf(stderr, "Erro de memória dynvec_create data\n"); free(v); exit(EXIT_FAILURE); }
    return v;
}

static void dynvec_free(dynvec *v) {
    free(v->data);
    free(v);
}

static size_t dynvec_length(const dynvec *v) {
    return v->length;
}

static void *dynvec_get(const dynvec *v, size_t idx) {
    return (char*)v->data + idx * v->elem_size;
}

static void dynvec_push(dynvec *v, const void *elem) {
    if (v->length == v->capacity) {
        size_t new_cap = v->capacity * 2;
        void *new_data = realloc(v->data, new_cap * v->elem_size);
        if (!new_data) { fprintf(stderr, "Erro de memória dynvec_push\n"); exit(EXIT_FAILURE); }
        v->data     = new_data;
        v->capacity = new_cap;
    }
    memcpy((char*)v->data + v->length * v->elem_size, elem, v->elem_size);
    v->length++;
}

/* ==== MergeSort Genérico ==== */

/*
 * merge_segment: intercala dois subarrays ordenados de `tmp` para `base`.
 * Parâmetros:
 *   base(void*), tmp(void*), esz(size_t), left(size_t), mid(size_t), right(size_t), cmp(cmpf)
 */
static void merge_segment(void *base, void *tmp, size_t esz, size_t left, size_t mid, size_t right, cmpf cmp) {
    char *src = (char*)tmp;
    char *dst = (char*)base;
    size_t i = left, j = mid;
    for (size_t k = left; k < right; k++) {
        if (i < mid && (j >= right || cmp(src + i*esz, src + j*esz) <= 0)) {
            memcpy(dst + k*esz, src + i*esz, esz);
            i++;
        } else {
            memcpy(dst + k*esz, src + j*esz, esz);
            j++;
        }
    }
}

static void mergesort_rec(dynvec *v, void *tmp, size_t left, size_t right, cmpf cmp) {
    if (right - left < 2) return;
    size_t mid = left + (right - left)/2;
    mergesort_rec(v, tmp, left, mid, cmp);
    mergesort_rec(v, tmp, mid, right, cmp);
    memcpy((char*)tmp + left*v->elem_size, (char*)v->data + left*v->elem_size, (right - left) * v->elem_size);
    merge_segment(v->data, tmp, v->elem_size, left, mid, right, cmp);
}

static void mergesort_dynvec(dynvec *v, cmpf cmp) {
    if (v->length < 2) return;
    void *tmp = malloc(v->length * v->elem_size);
    if (!tmp) { fprintf(stderr, "Erro de memória mergesort\n"); exit(EXIT_FAILURE); }
    mergesort_rec(v, tmp, 0, v->length, cmp);
    free(tmp);
}

/* ==== Tokenização UTF-8 + Conversão para Minúsculas ==== */

static int is_letter_utf8(unsigned char c1, unsigned char c2, int *sz) {
    if ((c1 >= 'A' && c1 <= 'Z') || (c1 >= 'a' && c1 <= 'z')) { *sz = 1; return 1; }
    if (c1 == 0xC3 && c2 >= 0x80 && c2 <= 0xBF) { *sz = 2; return 1; }
    *sz = 1; return 0;
}

static void append_char(char *buffer, int *len, unsigned char c1, unsigned char c2, int sz) {
    if (sz == 1) buffer[(*len)++] = tolower(c1);
    else {
        buffer[(*len)++] = c1;
        buffer[(*len)++] = (c2 >= 0x80 && c2 <= 0x9E) ? c2+0x20 : c2;
    }
}

typedef struct { char *word; int count; } WC;

static int cmp_str(const void *a, const void *b) { return strcmp(*(char**)a, *(char**)b); }
static int cmp_wc(const void *a, const void *b) { const WC *A = a, *B = b; return A->count != B->count ? B->count - A->count : strcmp(A->word, B->word); }

int main(void) {
    setlocale(LC_ALL, "");
    int n;
    if (scanf("%d", &n) != 1) return EXIT_FAILURE;
    getchar();

    dynvec *words = dynvec_create(sizeof(char*));
    char token_buf[8192]; int token_len = 0;
    char line[4096];

    while (fgets(line, sizeof line, stdin)) {
        unsigned char *p = (unsigned char*)line;
        size_t L = strlen(line);
        for (size_t i = 0; i < L; ) {
            int sz;
            if (i+1 < L && is_letter_utf8(p[i], p[i+1], &sz)) {
                append_char(token_buf, &token_len, p[i], p[i+1], sz);
                i += sz;
            } else if (token_len > 0) {
                token_buf[token_len] = '\0';
                char *w = strdup(token_buf);
                dynvec_push(words, &w);
                token_len = 0;
                i++;
            } else i++;
        }
    }
    if (token_len > 0) { token_buf[token_len] = '\0'; dynvec_push(words, &(char*[]){strdup(token_buf)}); }

    mergesort_dynvec(words, cmp_str);
    dynvec *counts = dynvec_create(sizeof(WC));
    size_t W = dynvec_length(words);
    for (size_t i = 0; i < W; ) {
        char *w0 = *(char**)dynvec_get(words, i);
        size_t j = i + 1; while (j < W && strcmp(w0, *(char**)dynvec_get(words, j)) == 0) j++;
        WC rec = { strdup(w0), (int)(j - i) };
        dynvec_push(counts, &rec);
        free(w0);
        i = j;
    }
    dynvec_free(words);

    mergesort_dynvec(counts, cmp_wc);
    size_t C = dynvec_length(counts), lim = (n < (int)C ? n : C);
    for (size_t i = 0; i < lim; i++) { WC *e = dynvec_get(counts, i); printf("%d %s\n", e->count, e->word); free(e->word); }
    dynvec_free(counts);
    return EXIT_SUCCESS;
}
