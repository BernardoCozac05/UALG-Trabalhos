#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#define MAX_LINE 100
#define VECTOR_INIT 4

typedef struct {
    char text[20];
    int count;
} Token;

typedef struct {
    void *items;
    size_t item_size;
    size_t capacity;
    size_t size;
} Vector;

Vector *vector_create(size_t item_size) {
    Vector *vec = malloc(sizeof *vec);
    if (!vec) return NULL;
    vec->capacity = VECTOR_INIT;
    vec->item_size = item_size;
    vec->size = 0;
    vec->items = malloc(item_size * vec->capacity);
    if (!vec->items) { free(vec); return NULL; }
    return vec;
}

void vector_free(Vector *vec) {
    if (!vec) return;
    free(vec->items);
    free(vec);
}

static void vector_resize(Vector *vec, size_t new_capacity) {
    if (!vec || vec->capacity >= new_capacity) return;
    void *new_items = realloc(vec->items, new_capacity * vec->item_size);
    if (new_items) {
        vec->items = new_items;
        vec->capacity = new_capacity;
    }
}

void vector_push(Vector *vec, void *elem) {
    if (!vec) return;
    if (vec->size == vec->capacity) vector_resize(vec, vec->capacity * 2);
    memcpy((char*)vec->items + vec->size * vec->item_size, elem, vec->item_size);
    vec->size++;
}

int get_count(Token *t) {
    return t->count;
}

char *get_text(Token *t) {
    return t->text;
}

void vector_print(Vector *vec, int n) {
    if (!vec || !vec->items) return;
    for (size_t i = 0; i < vec->size && i < (size_t)n; i++) {
        Token *t = (Token*)((char*)vec->items + i * vec->item_size);
        printf(" %d %s\n", get_count(t), get_text(t));
    }
}

int vector_length(Vector *vec) {
    if (!vec) return -1;
    return vec->size - 1;
}

void *vector_get(Vector *vec, int index) {
    if (!vec || index < 0 || index >= (int)vec->size) return NULL;
    return (char*)vec->items + index * vec->item_size;
}

void vector_set(Vector *vec, size_t index, void *value) {
    if (!vec || index >= vec->size) return;
    memcpy((char*)vec->items + index * vec->item_size, value, vec->item_size);
}

int vector_binary_search(Vector *vec, int left, int right, void *key, int (*cmp)(const void*, const void*)) {
    int result = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int c = cmp((char*)vec->items + mid * vec->item_size, key);
        if (c == 0) {
            result = mid;
            right = mid - 1;
        } else if (c < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

void insertion_sort_vector(Vector *vec, int (*cmp)(const void*, const void*)) {
    void *buffer = malloc(vec->item_size);
    if (!buffer) return;
    for (int i = 1; i < (int)vec->size; i++) {
        memcpy(buffer, vector_get(vec, i), vec->item_size);
        int j = i;
        while (j > 0 && cmp(buffer, (char*)vec->items + (j - 1) * vec->item_size) < 0) {
            vector_set(vec, j, vector_get(vec, j - 1));
            j--;
        }
        vector_set(vec, j, buffer);
    }
    free(buffer);
}

static void merge(Vector *vec, int l, int m, int r, int (*cmp)(const void*, const void*), void *tmp) {
    int i = l, j = m + 1, k = 0;
    while (i <= m && j <= r) {
        if (cmp(vector_get(vec, i), vector_get(vec, j)) <= 0)
            memcpy((char*)tmp + k++ * vec->item_size, vector_get(vec, i++), vec->item_size);
        else
            memcpy((char*)tmp + k++ * vec->item_size, vector_get(vec, j++), vec->item_size);
    }
    while (i <= m) memcpy((char*)tmp + k++ * vec->item_size, vector_get(vec, i++), vec->item_size);
    while (j <= r) memcpy((char*)tmp + k++ * vec->item_size, vector_get(vec, j++), vec->item_size);
    for (int t = 0; t < k; t++) vector_set(vec, l + t, (char*)tmp + t * vec->item_size);
}

static void mergesort_rec(Vector *vec, int l, int r, int (*cmp)(const void*, const void*), void *tmp) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergesort_rec(vec, l, m, cmp, tmp);
    mergesort_rec(vec, m + 1, r, cmp, tmp);
    merge(vec, l, m, r, cmp, tmp);
}

void vector_mergesort(Vector *vec, int (*cmp)(const void*, const void*)) {
    int n = vector_length(vec);
    if (n < 2) return;
    void *tmp = malloc(vec->item_size * (n + 1));
    if (!tmp) return;
    mergesort_rec(vec, 0, n, cmp, tmp);
    free(tmp);
}

int compare_key(const void *a, const void *b) {
    return strcmp(((const Token*)a)->text, (const char*)b);
}

int compare_token(const void *a, const void *b) {
    return strcmp(((const Token*)a)->text, ((const Token*)b)->text);
}

int compare_count(const void *a, const void *b) {
    return ((const Token*)b)->count - ((const Token*)a)->count;
}

Token *token_create(char *str) {
    Token *t = malloc(sizeof *t);
    if (!t) return NULL;
    t->count = 1;
    strncpy(t->text, str, 19);
    t->text[19] = '\0';
    return t;
}

void increment_count(Vector *vec, int index) {
    ((Token*)vector_get(vec, index))->count++;
}

void process_tokens(Vector *vec, char *line) {
    const char *delim = " !#$&'()*+,-./:;<=>?@[]^_`{|}~";
    char *token = strtok(line, delim);
    while (token) {
        Token *t = token_create(token);
        int idx = vector_binary_search(vec, 0, vec->size - 1, t->text, compare_key);
        if (idx == -1) {
            vector_push(vec, t);
            insertion_sort_vector(vec, compare_token);
        } else {
            free(t);
            increment_count(vec, idx);
        }
        token = strtok(NULL, delim);
    }
}

int main() {
    Vector *vec = vector_create(sizeof(Token));
    char buffer[MAX_LINE];
    int top_n;
    if (fgets(buffer, sizeof buffer, stdin)) top_n = atoi(buffer);
    while (fgets(buffer, sizeof buffer, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        process_tokens(vec, buffer);
    }
    vector_mergesort(vec, compare_count);
    vector_print(vec, top_n);
    vector_free(vec);
    return 0;
}
