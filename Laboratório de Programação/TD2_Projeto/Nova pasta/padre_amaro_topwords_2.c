/* padre_amaro_topwords.c – contagem *case‑sensitive*
 * ------------------------------------------------------------------
 * Trabalho Final TD2 – «O Crime do Padre Amaro»
 *  • Lê N (inteiro) na 1ª linha e depois todo o texto (stdin)
 *  • Ignora acentos, dígitos e pontuação; conta apenas A–Z/a–z
 *  • ***Distingue*** maiúsculas de minúsculas ("O" ≠ "o")
 *  • Dicionário mantido em ordem alfabética com procura binária
 *  • 1ª ordenação alfabética: qsort (comparador cmp_alpha)
 *  • 2ª ordenação por frequência decrescente: quicksort próprio
 *    – em caso de empate de frequência, palavra MAIS LONGA primeiro;
 *      se comprimento também empatar, usa ordem alfabética crescente
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <errno.h>
 
 /* ===== Vector dinâmico genérico (extracto do TD2) ================= */
 
 typedef struct {
     void  *data;
     size_t length;
     size_t capacity;
     size_t elem_size;
 } dynvec;
 
 static dynvec *dynvec_create(size_t elem_size) {
     dynvec *v = malloc(sizeof *v);
     if (!v) { perror("malloc dynvec"); exit(EXIT_FAILURE); }
     v->elem_size = elem_size;
     v->length = 0;
     v->capacity = 16;
     v->data = malloc(v->capacity * elem_size);
     if (!v->data) { perror("malloc dynvec data"); exit(EXIT_FAILURE); }
     return v;
 }
 
 static void dynvec_free(dynvec *v) {
     if (v) { free(v->data); free(v); }
 }
 
 static void dynvec_reserve(dynvec *v, size_t new_cap) {
     if (new_cap <= v->capacity) return;
     while (v->capacity < new_cap) v->capacity *= 2;
     v->data = realloc(v->data, v->capacity * v->elem_size);
     if (!v->data) { perror("realloc dynvec"); exit(EXIT_FAILURE); }
 }
 
 /* devolve ponteiro para slot novo */
 static void *dynvec_push_back(dynvec *v, const void *elem) {
     dynvec_reserve(v, v->length + 1);
     void *dest = (char*)v->data + v->length * v->elem_size;
     memcpy(dest, elem, v->elem_size);
     v->length++;
     return dest;
 }
 
 /* pesquisa binária genérica – devolve índice ou length se não achou */
 static size_t dynvec_bsearch(const dynvec *v, const void *key,
                               int (*cmp)(const void*, const void*))
 {
     size_t l = 0, r = (v->length ? v->length - 1 : 0);
     while (v->length && l <= r) {
         size_t mid = l + (r - l) / 2;
         int c = cmp(key, (char*)v->data + mid * v->elem_size);
         if (c == 0)
             return mid;
         if (c < 0) {
             if (mid == 0) break; /* underflow */
             r = mid - 1;
         } else {
             l = mid + 1;
         }
     }
     return v->length; /* não encontrado */
 }
 
 /* ===== Estrutura e comparadores de palavras ====================== */
 
 typedef struct {
     char  *word;
     size_t count;
 } Word;
 
 static int cmp_alpha(const void *a, const void *b) {
     const Word *wa = a, *wb = b;
     return strcmp(wa->word, wb->word);
 }
 
 /* Comparador: frequência decrescente; em empate -> comprimento decrescente;
  * novo empate -> ordem alfabética crescente
  */
 static int cmp_freq(const void *a, const void *b) {
     const Word *wa = a, *wb = b;
     if (wa->count < wb->count) return  1;
     if (wa->count > wb->count) return -1;
     size_t la = strlen(wa->word), lb = strlen(wb->word);
     if (la < lb) return 1;   /* queremos palavra mais longa primeiro */
     if (la > lb) return -1;
     return strcmp(wa->word, wb->word);
 }
 
 static void quicksort_freq(Word *arr, long lo, long hi) {
     if (lo >= hi) return;
     Word pivot = arr[(lo + hi) / 2];
     long i = lo, j = hi;
     while (i <= j) {
         while (cmp_freq(&arr[i], &pivot) < 0) i++;
         while (cmp_freq(&arr[j], &pivot) > 0) j--;
         if (i <= j) {
             Word tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
             i++; j--; }
     }
     if (lo < j) quicksort_freq(arr, lo, j);
     if (i < hi) quicksort_freq(arr, i, hi);
 }
 
 /* duplica string tal‑qual (case‑sensitive) */
 static char *dup_token(const char *src, size_t len) {
     char *s = malloc(len + 1);
     if (!s) { perror("malloc"); exit(EXIT_FAILURE); }
     memcpy(s, src, len);
     s[len] = '\0';
     return s;
 }
 
 /* Insere palavra ou incrementa contagem (mantém ordem alfabética) */
 static void insert_or_increment(dynvec *v, char *token) {
     Word key = { token, 1 };
     size_t idx = dynvec_bsearch(v, &key, cmp_alpha);
     if (idx != v->length) {
         Word *w = (Word*)((char*)v->data + idx * sizeof(Word));
         w->count++;
         free(token);
         return;
     }
     /* inserir mantendo ordenação: desloca elementos à direita */
     dynvec_reserve(v, v->length + 1);
     Word *arr = v->data;
     size_t pos = v->length; /* por defeito no fim */
     for (size_t i = 0; i < v->length; ++i)
         if (cmp_alpha(&key, &arr[i]) < 0) { pos = i; break; }
     memmove(&arr[pos+1], &arr[pos], (v->length - pos) * sizeof(Word));
     arr[pos] = key;
     v->length++;
 }
 
 /* ============================= main ============================== */
 
 int main(void) {
     size_t topN;
     if (scanf("%zu", &topN) != 1) {
         fprintf(stderr, "Erro: primeira linha deve conter N.\n");
         return EXIT_FAILURE;
     }
 
     int c;
     char buf[256];
     size_t len = 0;
     dynvec *dict = dynvec_create(sizeof(Word));
 
     while ((c = getchar()) != EOF) {
         if (isalpha((unsigned char)c)) {
             if (len < sizeof buf - 1)
                 buf[len++] = (char)c; /* mantém maiúscula/minúscula */
         } else if (len) {
             char *tok = dup_token(buf, len);
             insert_or_increment(dict, tok);
             len = 0;
         }
     }
     if (len) {
         char *tok = dup_token(buf, len);
         insert_or_increment(dict, tok);
     }
 
     if (dict->length)
         quicksort_freq(dict->data, 0, (long)dict->length - 1);
 
     size_t n = (topN < dict->length) ? topN : dict->length;
     Word *arr = dict->data;
     for (size_t i = 0; i < n; ++i)
         printf("%zu %s\n", arr[i].count, arr[i].word);
 
     for (size_t i = 0; i < dict->length; ++i)
         free(arr[i].word);
     dynvec_free(dict);
     return 0;
 }