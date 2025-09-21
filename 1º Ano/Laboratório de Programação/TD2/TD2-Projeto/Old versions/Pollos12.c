/* Pollos12.c
 * Programa de contagem de palavras UTF-8 genérico e otimizado
 * Utiliza dynvec genérico, mergesort e boas práticas de memória
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <locale.h>

 /* ---- dynvec genérico ---- */
 typedef struct {
     void   *data;
     size_t  elem_size;
     size_t  length;
     size_t  capacity;
 } dynvec;
 
 typedef int  (*cmpf)  (const void*, const void*);
 typedef void (*unaryf)(void*);
 
 static dynvec* dynvec_create(size_t esz) {
     dynvec *v = malloc(sizeof *v);
     if (!v) { perror("dynvec_create"); exit(EXIT_FAILURE); }
     v->elem_size = esz;
     v->length    = 0;
     v->capacity  = 16;
     v->data      = malloc(esz * v->capacity);
     if (!v->data) { perror("dynvec_create data"); free(v); exit(EXIT_FAILURE); }
     return v;
 }
 
 static void dynvec_free(dynvec *v) {
     free(v->data);
     free(v);
 }
 
 static size_t dynvec_length(const dynvec *v) {
     return v->length;
 }
 
 static void* dynvec_get(const dynvec *v, size_t i) {
     return (char*)v->data + i * v->elem_size;
 }
 
 static void dynvec_push(dynvec *v, const void *elem) {
     if (v->length == v->capacity) {
         size_t nc = v->capacity * 2;
         void *nd = realloc(v->data, nc * v->elem_size);
         if (!nd) { perror("dynvec_push"); exit(EXIT_FAILURE); }
         v->data     = nd;
         v->capacity = nc;
     }
     memcpy((char*)v->data + v->length * v->elem_size, elem, v->elem_size);
     v->length++;
 }
 
 
 /* ---- MergeSort genérico ---- */
 static void merge_segment(void *base, void *tmp, size_t esz, size_t l, size_t m, size_t r, cmpf cmp) {
     char *src = tmp, *dst = base;
     size_t i = l, j = m;
     for (size_t k = l; k < r; k++) {
         if (i < m && (j >= r || cmp(src + i*esz, src + j*esz) <= 0)) {
             memcpy(dst + k*esz, src + i*esz, esz);
             i++;
         } else {
             memcpy(dst + k*esz, src + j*esz, esz);
             j++;
         }
     }
 }
 
 static void mergesort_rec(dynvec *v, void *tmp, size_t l, size_t r, cmpf cmp) {
     if (r - l < 2) return;
     size_t m = l + (r - l)/2;
     mergesort_rec(v, tmp, l, m, cmp);
     mergesort_rec(v, tmp, m, r, cmp);
     memcpy((char*)tmp + l*v->elem_size, (char*)v->data + l*v->elem_size, (r - l)*v->elem_size);
     merge_segment(v->data, tmp, v->elem_size, l, m, r, cmp);
 }
 
 static void dynvec_sort(dynvec *v, cmpf cmp) {
     if (v->length < 2) return;
     void *tmp = malloc(v->length * v->elem_size);
     if (!tmp) { perror("mergesort"); exit(EXIT_FAILURE); }
     mergesort_rec(v, tmp, 0, v->length, cmp);
     free(tmp);
 }
 
 /* ---- Tokenização UTF-8 e contagem ---- */
 typedef struct { char *word; int count; } WC;
 
 // Reconhecimento de letra: apenas ASCII A–Z, a–z, ou UTF-8 2 bytes (C3 80..BF)
 static int is_letter_utf8(unsigned char c1, unsigned char c2, int *sz) {
     if ((c1 >= 'A' && c1 <= 'Z') || (c1 >= 'a' && c1 <= 'z')) {
         *sz = 1;
         return 1;
     }
     if (c1 == 0xC3 && c2 >= 0x80 && c2 <= 0xBF) {
         *sz = 2;
         return 1;
     }
     *sz = 1;
     return 0;
 }
 
 // Adiciona letra ao buffer, com lowercase de ASCII e de UTF-8 (C3 80..9E)
 static void append_char(char *buf, int *len, unsigned char c1, unsigned char c2, int sz) {
     if (sz == 1) {
         buf[(*len)++] = tolower((unsigned char)c1);
     } else {
         buf[(*len)++] = c1;
         buf[(*len)++] = (c2 >= 0x80 && c2 <= 0x9E) ? c2 + 0x20 : c2;
     }
 }
 
 static int cmp_str(const void *a, const void *b) {
     return strcmp(*(char**)a, *(char**)b);
 }
 static int cmp_wc(const void *a, const void *b) {
     const WC *A = a, *B = b;
     if (A->count != B->count) return B->count - A->count;
     return strcmp(A->word, B->word);
 }
 
 int main(void) {
     setlocale(LC_ALL, "");
     int n;
     if (scanf("%d", &n) != 1) return EXIT_FAILURE;
     getchar();
 
     dynvec *words = dynvec_create(sizeof(char*));
     char token[8192]; int tlen = 0;
     char line[4096];
 
     while (fgets(line, sizeof line, stdin)) {
         unsigned char *p = (unsigned char*)line;
         size_t L = strlen(line);
         for (size_t i = 0; i < L; ) {
             int sz;
             if (i + 1 < L && is_letter_utf8(p[i], p[i+1], &sz)) {
                 append_char(token, &tlen, p[i], p[i+1], sz);
                 i += sz;
             } else if (tlen > 0) {
                 token[tlen] = '\0';
                 char *w = strdup(token);
                 dynvec_push(words, &w);
                 tlen = 0;
                 i++;
             } else {
                 i++;
             }
         }
     }
     if (tlen > 0) {
         token[tlen] = '\0';
         char *w = strdup(token);
         dynvec_push(words, &w);
     }
 
     dynvec_sort(words, cmp_str);
     dynvec *counts = dynvec_create(sizeof(WC));
 
     size_t W = dynvec_length(words);
     for (size_t i = 0; i < W; ) {
         char *w0 = *(char**)dynvec_get(words, i);
         size_t j = i + 1;
         while (j < W && strcmp(w0, *(char**)dynvec_get(words, j)) == 0) j++;
         WC rec = { strdup(w0), (int)(j - i) };
         dynvec_push(counts, &rec);
         free(w0);
         i = j;
     }
     dynvec_free(words);
 
     dynvec_sort(counts, cmp_wc);
     size_t C = dynvec_length(counts);
     size_t lim = (n < (int)C ? n : C);
     for (size_t i = 0; i < lim; i++) {
         WC *e = dynvec_get(counts, i);
         printf("%d %s\n", e->count, e->word);
         free(e->word);
     }
     dynvec_free(counts);
     return EXIT_SUCCESS;
 }
 