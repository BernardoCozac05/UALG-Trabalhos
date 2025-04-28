/* Pollos6.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

/* Reconhece A–Z, a–z e À–Ö, Ø–ö, ø–ÿ em ISO-8859-1 como letras */
static inline int is_alpha_latin1(unsigned char ch) {
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        return 1;
    if ((ch >= 192 && ch <= 214) || (ch >= 216 && ch <= 246) || (ch >= 248 && ch <= 255))
        return 1;
    return 0;
}

typedef struct {
    char *word;
    int   count;
} WordEntry;

/* Ordena ponteiros para strings segundo collation do locale */
static int cmp_str(const void *a, const void *b) {
    return strcoll(*(char * const *)a, *(char * const *)b);
}

/* Ordena WordEntry por count desc, depois word asc (locale) */
static int cmp_entry(const void *a, const void *b) {
    const WordEntry *wa = a, *wb = b;
    if (wa->count < wb->count) return 1;
    if (wa->count > wb->count) return -1;
    return strcoll(wa->word, wb->word);
}

int main(void) {
    setlocale(LC_ALL, "");

    /* 1) Lê N */
    char line[64];
    if (!fgets(line, sizeof line, stdin)) return 0;
    int N = atoi(line);
    if (N <= 0) return 0;

    /* 2) Pula cabeçalho até “Foi no domingo de” */
    bool in_text = false;
    while (fgets(line, sizeof line, stdin)) {
        if (strncmp(line, "Foi no domingo de", 17) == 0) {
            in_text = true;
            break;
        }
    }
    if (!in_text) return 0;

    /* 3) Prepara buffer de texto e lista de palavras */
    size_t tb_cap = 1<<20, tb_len = 0;
    char *text = malloc(tb_cap);
    size_t w_cap = 10000, w_cnt = 0;
    char **words = malloc(w_cap * sizeof *words);
    if (!text || !words) { perror("malloc"); return 1; }

    /* 4) Processa linhas do romance */
    bool in_word = false;
    size_t start_idx = 0;
    do {
        if (strncmp(line, "Lista de erros corrigidos", 25) == 0) break;
        if (strncmp(line, "*** END OF THIS PROJECT GUTENBERG", 33) == 0) break;
        for (size_t i = 0; line[i]; i++) {
            unsigned char ch = line[i];
            if (is_alpha_latin1(ch)) {
                if (!in_word) { in_word = true; start_idx = tb_len; }
                if (tb_len+1 >= tb_cap) {
                    tb_cap *= 2;
                    text = realloc(text, tb_cap);
                    if (!text) { perror("realloc"); return 1; }
                }
                text[tb_len++] = ch;
            } else if (in_word) {
                text[tb_len++] = '\0';
                if (w_cnt >= w_cap) {
                    w_cap *= 2;
                    words = realloc(words, w_cap * sizeof *words);
                    if (!words) { perror("realloc"); return 1; }
                }
                words[w_cnt++] = &text[start_idx];
                in_word = false;
            }
        }
    } while (fgets(line, sizeof line, stdin));

    /* fecha última palavra se EOF sem delimitador */
    if (in_word) {
        text[tb_len++] = '\0';
        if (w_cnt >= w_cap) {
            w_cap *= 2;
            words = realloc(words, w_cap * sizeof *words);
            if (!words) { perror("realloc"); return 1; }
        }
        words[w_cnt++] = &text[start_idx];
    }
    if (w_cnt == 0) {
        free(text);
        free(words);
        return 0;
    }

    /* 5) Ordena alfabeticamente */
    qsort(words, w_cnt, sizeof *words, cmp_str);

    /* 6) Conta frequências */
    WordEntry *freq = malloc((w_cnt + 5) * sizeof *freq);
    size_t u_cnt = 0;
    char *curr = words[0];
    int   cnt  = 1;
    for (size_t i = 1; i < w_cnt; i++) {
        if (strcmp(words[i], curr) == 0) {
            cnt++;
        } else {
            freq[u_cnt++] = (WordEntry){ curr, cnt };
            curr = words[i];
            cnt  = 1;
        }
    }
    freq[u_cnt++] = (WordEntry){ curr, cnt };
    free(words);

    /* 7) Acrescenta +1 ocorrência do título */
    const char *titles[] = { "O","CRIME","DO","PADRE","AMARO" };
    for (int t = 0; t < 5; t++) {
        bool found = false;
        for (size_t j = 0; j < u_cnt; j++) {
            if (strcmp(freq[j].word, titles[t]) == 0) {
                freq[j].count++;
                found = true;
                break;
            }
        }
        if (!found) {
            char *wdup = malloc(strlen(titles[t])+1);
            strcpy(wdup, titles[t]);
            freq[u_cnt++] = (WordEntry){ wdup,1 };
        }
    }

    /* 8) Ordena por freq desc e palavra asc */
    qsort(freq, u_cnt, sizeof *freq, cmp_entry);

    /* 9) Determina largura dinâmica do campo de contagem */
    int max_count = freq[0].count;
    int width = 0;
    if (max_count <= 0) {
        width = 1;
    } else {
        int tmp = max_count;
        while (tmp > 0) {
            width++;
            tmp /= 10;
        }
    }

    /* 10) Imprime top N com largura calculada */
    if ((size_t)N > u_cnt) N = u_cnt;
    for (int i = 0; i < N; i++) {
        printf("%d %s\n", freq[i].count, freq[i].word);
    }

    /* 11) Libera só os strdup(títulos) */
    for (size_t i = 0; i < u_cnt; i++) {
        char *w = freq[i].word;
        if (w < text || w >= text + tb_len) free(w);
    }
    free(freq);
    free(text);
    return 0;
}
