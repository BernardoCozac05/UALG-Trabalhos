#ifndef DYNVEC_H
#define DYNVEC_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct dynvec dynvec;

dynvec *dynvec_create(size_t elem_size);
void dynvec_free(dynvec *v);
void dynvec_push(dynvec *v, void *elem);
void dynvec_print(dynvec *v, int n);

int dynvec_length(dynvec *v);
void *dynvec_get(dynvec *v, int index);
void dynvec_set(dynvec *v, size_t index, void *element);

int dynvec_busca_binaria(dynvec *v, int esquerda, int direita, void *chave, int (*cmp)(const void *, const void *));
void insertion_sort_dynvec(dynvec *v, int (*cmp)(const void *, const void *));
void dynvec_mergesort(dynvec *v, int (*cmp)(const void *, const void *));

// static void dynvec_resize(dynvec *v, size_t new_capacity)

#endif // DYNVEC_H
