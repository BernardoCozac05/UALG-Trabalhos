/*
 * wordfreq_fast.c – TD2 (versão linha‑de‑comando, C89‑compatível)
 * ----------------------------------------------------------------
 *   Uso:  wordfreq N < texto.txt
 *
 *   Compila sem avisos em GCC 3.x (C89), ex.:  gcc -Wall wordfreq_fast.c -o wordfreq
 */

 #define _XOPEN_SOURCE 700
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <errno.h>
 #include <locale.h>
 
 /*=========================== dynvec ===========================*/
 typedef struct { void *data; size_t length, cap, elem_size; } dynvec;
 
 static dynvec *dynvec_create(size_t elem_size)
 {
     dynvec *v = (dynvec*)malloc(sizeof *v);
     if (!v) { perror("malloc dynvec"); exit(EXIT_FAILURE); }
     v->length = 0; v->cap = 16; v->elem_size = elem_size;
     v->data = malloc(v->cap * elem_size);
     if (!v->data) { perror("malloc data"); exit(EXIT_FAILURE); }
     return v;
 }
 static void dynvec_reserve(dynvec *v, size_t need)
 {
     if (need <= v->cap) return;
     while (v->cap < need) v->cap *= 2;
     v->data = realloc(v->data, v->cap * v->elem_size);
     if (!v->data) { perror("realloc dynvec"); exit(EXIT_FAILURE); }
 }
 static void dynvec_push(dynvec *v, const void *elem)
 {
     dynvec_reserve(v, v->length + 1);
     memcpy((char *)v->data + v->length * v->elem_size, elem, v->elem_size);
     v->length++;
 }
 static void dynvec_free(dynvec *v, void (*elem_free)(void *))
 {
     size_t i;
     if (!v) return;
     if (elem_free)
         for (i = 0; i < v->length; ++i)
             elem_free((char *)v->data + i * v->elem_size);
     free(v->data);
     free(v);
 }
 
 /*======================= estrutura Word =======================*/
 typedef struct { char *word; size_t count; } Word;
 
 static int cmp_alpha(const void *a, const void *b)
 { return strcmp(((const Word *)a)->word, ((const Word *)b)->word); }
 
 static int cmp_freq(const void *a, const void *b)
 {
     const Word *wa = (const Word*)a, *wb = (const Word*)b;
     if (wa->count != wb->count)   return (wa->count < wb->count) ? 1 : -1;
     {
         size_t la = strlen(wa->word), lb = strlen(wb->word);
         if (la != lb)             return (la < lb) ? 1 : -1;
     }
     return strcmp(wa->word, wb->word);
 }
 static void free_word(void *p) { free(((Word *)p)->word); }
 
 static char *dup_slice(const char *src, size_t n)
 {
     char *s = (char*)malloc(n + 1);
     if (!s) { perror("malloc token"); exit(EXIT_FAILURE); }
     memcpy(s, src, n);
     s[n] = '\0';
     return s;
 }
 
 /*===================== tokenização auxiliar ===================*/
 static void process_char(unsigned char ch,
                          char **tok, size_t *len, size_t *cap,
                          dynvec *raw)
 {
     if (isalpha(ch) || (ch >= 0xC0 && ch <= 0xFF)) {
         if (*len + 1 >= *cap) {
             *cap = *cap ? *cap * 2 : 32;
             *tok = (char*)realloc(*tok, *cap);
             if (!*tok) { perror("realloc token"); exit(EXIT_FAILURE); }
         }
         (*tok)[(*len)++] = (char)tolower(ch);
     } else if (*len) {
         Word w = { dup_slice(*tok, *len), 1 };
         dynvec_push(raw, &w);
         *len = 0;
     }
 }
 
 /*============================== main ==========================*/
 int main(int argc, char *argv[])
 {
     /* ---------- N (argumento) ---------- */
     if (argc < 2) {
         fputs("Uso: wordfreq N < texto.txt\n", stderr);
         return EXIT_FAILURE;
     }
     {
         char *endptr; errno = 0;
         unsigned long tmp = strtoul(argv[1], &endptr, 10);
         if (errno || *endptr || tmp == 0) {
             fputs("N inválido\n", stderr);
             return EXIT_FAILURE;
         }
         /* N pode agora ser usado */
         size_t N = (size_t)tmp;
 
         /* ---------- locale ---------- */
         setlocale(LC_CTYPE, "");
 
         /* ---------- leitura ---------- */
         dynvec *raw = dynvec_create(sizeof(Word));
         char *tok = NULL; size_t cap = 0, len = 0;
         int in_text = 0;
         char buf[4096];
         while (fgets(buf, sizeof buf, stdin)) {
             if (!in_text) {
                 if (strncmp(buf, "*** START OF THIS PROJECT GUTENBERG EBOOK", 41) == 0)
                     in_text = 1;
                 continue;
             }
             if (strncmp(buf, "*** END OF THIS PROJECT GUTENBERG EBOOK", 39) == 0)
                 break;
             {
                 unsigned char *p;
                 for (p = (unsigned char *)buf; *p; ++p)
                     process_char(*p, &tok, &len, &cap, raw);
             }
         }
         if (len) {
             Word w = { dup_slice(tok, len), 1 };
             dynvec_push(raw, &w);
         }
         free(tok);
 
         /* ---------- 1ª ordenação ---------- */
         qsort(raw->data, raw->length, sizeof(Word), cmp_alpha);
 
         /* ---------- compressão ---------- */
         dynvec *dict = dynvec_create(sizeof(Word));
         if (raw->length) {
             Word *arr = (Word*)raw->data;
             Word curr = arr[0];
             size_t i;
             for (i = 1; i < raw->length; ++i) {
                 if (strcmp(curr.word, arr[i].word) == 0) {
                     curr.count++; free(arr[i].word);
                 } else { dynvec_push(dict, &curr); curr = arr[i]; }
             }
             dynvec_push(dict, &curr);
         }
         dynvec_free(raw, NULL);
 
         /* ---------- 2ª ordenação ---------- */
         qsort(dict->data, dict->length, sizeof(Word), cmp_freq);
 
         /* ---------- saída ---------- */
         {
             size_t out = N < dict->length ? N : dict->length;
             Word *arr = (Word*)dict->data;
             size_t i;
             for (i = 0; i < out; ++i)
                 printf("%lu %s\n", (unsigned long)arr[i].count, arr[i].word);
         }
         dynvec_free(dict, free_word);
     }
     return 0;
 }
 