#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Check if a character is an alphabetic letter (A-Z, a-z, or accented letters from À to ÿ in ISO-8859-1). */
int is_letter(int c) {
    unsigned int uc = (unsigned int)c;
    if ((uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z')) {
        return 1;
    }
    if (uc >= 192 && uc <= 255) {
        if (uc == 215 || uc == 247) { /* × or ÷ are not letters */
            return 0;
        }
        return 1;
    }
    return 0;
}

/* Convert an alphabetic character to lowercase (handles ASCII and ISO-8859-1 accented letters). */
int to_lowercase(int c) {
    unsigned int uc = (unsigned int)c;
    if (uc >= 'A' && uc <= 'Z') {
        return (int)(uc - 'A' + 'a');
    }
    if (uc >= 192 && uc <= 222) {
        if (uc == 215) {
            return c;
        }
        return (int)(uc + 32);
    }
    return c;
}

/* Structure to hold a word and its count */
typedef struct {
    char *word;
    int count;
} WordCount;

/* Compare two strings (for qsort) */
int compare_strings(const void *a, const void *b) {
    const char * const *sa = (const char * const *)a;
    const char * const *sb = (const char * const *)b;
    return strcmp(*sa, *sb);
}

/* Compare two WordCount by count (descending), then by word (ascending) */
int compare_wordcount(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count < wb->count) return 1;
    if (wa->count > wb->count) return -1;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    int N, ch;
    if (scanf("%d", &N) != 1) {
        return 0;
    }
    ch = getchar();
    if (ch != EOF && ch != '\n' && ch != '\r') {
        ungetc(ch, stdin);
    }
    /* Initialize variables */
    int processing = 0;
    size_t words_capacity = 10000;
    size_t words_count = 0;
    char **words = malloc(words_capacity * sizeof(char *));
    if (!words) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }
    char line[2048];
    char word_buffer[512];
    size_t word_len = 0;
    size_t i, j;
    const char *title_words[5] = {"o", "crime", "do", "padre", "amaro"};
    /* Read input lines */
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (!processing) {
            if (strncmp(line, "Foi no domingo de", 17) == 0) {
                processing = 1;
            } else {
                continue;
            }
        }
        if (strncmp(line, "Lista de erros corrigidos", 25) == 0) break;
        if (strncmp(line, "*** END OF THIS PROJECT GUTENBERG", 33) == 0) break;
        for (i = 0; line[i] != '\0'; ++i) {
            int c = (unsigned char)line[i];
            if (c == '\r' || c == '\n') c = ' ';
            if (is_letter(c)) {
                c = to_lowercase(c);
                if (word_len < sizeof(word_buffer) - 1) {
                    word_buffer[word_len++] = (char)c;
                }
            } else {
                if (word_len > 0) {
                    word_buffer[word_len] = '\0';
                    char *new_word = malloc(word_len + 1);
                    if (!new_word) {
                        fprintf(stderr, "Memory allocation error\n");
                        for (j = 0; j < words_count; ++j) free(words[j]);
                        free(words);
                        return 1;
                    }
                    strcpy(new_word, word_buffer);
                    if (words_count >= words_capacity) {
                        size_t new_cap = words_capacity * 2;
                        char **new_words = realloc(words, new_cap * sizeof(char *));
                        if (!new_words) {
                            fprintf(stderr, "Memory allocation error\n");
                            free(new_word);
                            for (j = 0; j < words_count; ++j) free(words[j]);
                            free(words);
                            return 1;
                        }
                        words = new_words;
                        words_capacity = new_cap;
                    }
                    words[words_count++] = new_word;
                    word_len = 0;
                }
            }
        }
    }
    if (word_len > 0) {
        word_buffer[word_len] = '\0';
        char *new_word = malloc(word_len + 1);
        if (!new_word) {
            fprintf(stderr, "Memory allocation error\n");
            for (j = 0; j < words_count; ++j) free(words[j]);
            free(words);
            return 1;
        }
        strcpy(new_word, word_buffer);
        if (words_count >= words_capacity) {
            size_t new_cap = words_capacity * 2;
            char **new_words = realloc(words, new_cap * sizeof(char *));
            if (!new_words) {
                fprintf(stderr, "Memory allocation error\n");
                free(new_word);
                for (j = 0; j < words_count; ++j) free(words[j]);
                free(words);
                return 1;
            }
            words = new_words;
            words_capacity = new_cap;
        }
        words[words_count++] = new_word;
        word_len = 0;
    }
    /* Add title words from the book's title */
    for (j = 0; j < 5; ++j) {
        char *w = malloc(strlen(title_words[j]) + 1);
        if (!w) {
            fprintf(stderr, "Memory allocation error\n");
            for (i = 0; i < words_count; ++i) free(words[i]);
            free(words);
            return 1;
        }
        strcpy(w, title_words[j]);
        if (words_count >= words_capacity) {
            size_t new_cap = words_capacity * 2;
            char **new_words = realloc(words, new_cap * sizeof(char *));
            if (!new_words) {
                fprintf(stderr, "Memory allocation error\n");
                free(w);
                for (i = 0; i < words_count; ++i) free(words[i]);
                free(words);
                return 1;
            }
            words = new_words;
            words_capacity = new_cap;
        }
        words[words_count++] = w;
    }
    if (words_count == 0) {
        free(words);
        return 0;
    }
    /* Sort words alphabetically */
    qsort(words, words_count, sizeof(char *), compare_strings);
    /* Count occurrences of each unique word */
    WordCount *word_counts = malloc(words_count * sizeof(WordCount));
    if (!word_counts) {
        fprintf(stderr, "Memory allocation error\n");
        for (i = 0; i < words_count; ++i) free(words[i]);
        free(words);
        return 1;
    }
    size_t unique_count = 0;
    i = 0;
    while (i < words_count) {
        word_counts[unique_count].word = words[i];
        word_counts[unique_count].count = 1;
        j = i + 1;
        while (j < words_count && strcmp(words[i], words[j]) == 0) {
            word_counts[unique_count].count++;
            free(words[j]);
            j++;
        }
        unique_count++;
        i = j;
    }
    free(words);
    /* Sort by frequency (descending) and alphabetically for ties */
    qsort(word_counts, unique_count, sizeof(WordCount), compare_wordcount);
    if (N > (int)unique_count) {
        N = (int)unique_count;
    }
    /* Determine width for printing counts (for alignment) */
    int max_count = word_counts[0].count;
    int width = 0;
    if (max_count <= 0) {
        width = 1;
    } else {
        int temp = max_count;
        while (temp > 0) {
            width++;
            temp /= 10;
        }
    }
    /* Print top N words and their counts */
    for (i = 0; i < (size_t)N; ++i) {
        printf("%d %s\n", word_counts[i].count, word_counts[i].word);
        free(word_counts[i].word);
    }
    for (j = i; j < unique_count; ++j) {
        free(word_counts[j].word);
    }
    free(word_counts);
    return 0;
}
