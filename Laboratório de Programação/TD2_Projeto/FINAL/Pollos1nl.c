#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <limits.h>

/* Define buffer sizes for reading lines and storing word tokens */
#define LINE_BUFFER_SIZE 4096
#define TOKEN_BUFFER_SIZE 8192

/*
 * Estrutura para representar um vetor dinâmico genérico.
 * Armazena um array de dados, o tamanho de cada elemento,
 * o número atual de elementos e a capacidade alocada.
 */
typedef struct {
    void   *data_array;
    size_t  element_size;
    size_t  current_length;
    size_t  current_capacity;
} DynamicVector;

/*
 * Define um tipo para ponteiros de função de comparação genérica,
 * compatível com qsort e bsearch.
 */
typedef int  (*ComparisonFunction)(const void*, const void*);

/*
 * Cria e inicializa um novo vetor dinâmico (DynamicVector).
 * Aloca memória para a estrutura e para o buffer de dados inicial.
 * Termina o programa se a alocação falhar ou se o tamanho do elemento for 0.
 */
static DynamicVector* dynamic_vector_create(size_t element_size_param) {
    if (element_size_param == 0) {
        fprintf(stderr, "dynamic_vector_create: element size cannot be zero\n");
        exit(EXIT_FAILURE);
    }
    DynamicVector *vector = malloc(sizeof *vector);
    if (!vector) {
        perror("dynamic_vector_create: malloc failed for DynamicVector struct");
        exit(EXIT_FAILURE);
    }
    vector->element_size = element_size_param;
    vector->current_length    = 0;
    vector->current_capacity  = 16;
    size_t initial_allocation_size = vector->element_size * vector->current_capacity;
     if (vector->current_capacity > 0 && initial_allocation_size / vector->current_capacity != vector->element_size) {
         fprintf(stderr, "dynamic_vector_create: initial size calculation overflow\n");
         free(vector);
         exit(EXIT_FAILURE);
     }
    vector->data_array = malloc(initial_allocation_size);
    if (!vector->data_array) {
        perror("dynamic_vector_create: malloc failed for data buffer");
        free(vector);
        exit(EXIT_FAILURE);
    }
    return vector;
}

/*
 * Libera a memória alocada para um vetor dinâmico,
 * incluindo o buffer de dados e a própria estrutura.
 */
static void dynamic_vector_free(DynamicVector *vector) {
    if (vector) {
        free(vector->data_array);
        free(vector);
    }
}

/*
 * Retorna o número atual de elementos armazenados no vetor dinâmico.
 */
static size_t dynamic_vector_length(const DynamicVector *vector) {
    return vector ? vector->current_length : 0;
}

/*
 * Retorna um ponteiro para o elemento no índice especificado.
 * Termina o programa se o vetor for nulo ou o índice estiver fora dos limites.
 */
static void* dynamic_vector_get(const DynamicVector *vector, size_t index) {
    if (!vector || index >= vector->current_length) {
         fprintf(stderr, "dynamic_vector_get: index out of bounds or NULL vector\n");
         exit(EXIT_FAILURE);
    }
    return (char*)vector->data_array + index * vector->element_size;
}

/*
 * Adiciona um elemento ao final do vetor dinâmico.
 * Se necessário, realoca o buffer de dados (duplicando a capacidade)
 * para acomodar o novo elemento.
 * Termina o programa se a alocação/realocação falhar.
 */
static void dynamic_vector_push(DynamicVector *vector, const void *element) {
    if (!vector || !element) {
        fprintf(stderr, "dynamic_vector_push: received NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    if (vector->current_length == vector->current_capacity) {
        size_t new_capacity = vector->current_capacity * 2;
        if (new_capacity < vector->current_capacity) {
             fprintf(stderr, "dynamic_vector_push: capacity overflow\n");
             exit(EXIT_FAILURE);
        }
        size_t new_allocation_size = new_capacity * vector->element_size;
        if (vector->element_size > 0 && new_allocation_size / vector->element_size != new_capacity) {
             fprintf(stderr, "dynamic_vector_push: allocation size calculation overflow\n");
             exit(EXIT_FAILURE);
        }
        void *new_data_array = realloc(vector->data_array, new_allocation_size);
        if (!new_data_array) {
            perror("dynamic_vector_push: realloc failed");
            exit(EXIT_FAILURE);
        }
        vector->data_array     = new_data_array;
        vector->current_capacity = new_capacity;
    }
    memcpy((char*)vector->data_array + vector->current_length * vector->element_size, element, vector->element_size);
    vector->current_length++;
}

/*
 * Função auxiliar para o Mergesort.
 * Mescla dois segmentos ordenados (de left_idx a middle_idx e de middle_idx a right_idx)
 * que estão no buffer temporário (source_temp_buffer) de volta para o buffer original (destination_base).
 */
static void merge_segment(void *destination_base, void *source_temp_buffer, size_t element_size, size_t left_idx, size_t middle_idx, size_t right_idx, ComparisonFunction compare_func) {
    char *source_ptr = source_temp_buffer;
    char *destination_ptr = destination_base;
    size_t i = left_idx, j = middle_idx;
    for (size_t k = left_idx; k < right_idx; k++) {
        if (i < middle_idx && (j >= right_idx || compare_func(source_ptr + i*element_size, source_ptr + j*element_size) <= 0)) {
            memcpy(destination_ptr + k*element_size, source_ptr + i*element_size, element_size);
            i++;
        } else if (j < right_idx) {
            memcpy(destination_ptr + k*element_size, source_ptr + j*element_size, element_size);
            j++;
        } else {
             fprintf(stderr, "merge_segment: unexpected state at k=%zu\n", k);
             exit(EXIT_FAILURE);
        }
    }
}

/*
 * Implementação recursiva do algoritmo Mergesort (abordagem top-down).
 * Divide o vetor em metades, ordena recursivamente e mescla os resultados usando um buffer temporário.
 */
static void mergesort_recursive(DynamicVector *vector, void *temp_buffer, size_t left_idx, size_t right_idx, ComparisonFunction compare_func) {
    if (right_idx - left_idx < 2) return;
    size_t middle_idx = left_idx + (right_idx - left_idx) / 2;
    mergesort_recursive(vector, temp_buffer, left_idx, middle_idx, compare_func);
    mergesort_recursive(vector, temp_buffer, middle_idx, right_idx, compare_func);
    size_t segment_length = right_idx - left_idx;
    size_t segment_byte_size = segment_length * vector->element_size;
    if (vector->element_size > 0 && segment_length > 0 && segment_byte_size / segment_length != vector->element_size) {
         fprintf(stderr, "mergesort_recursive: size calculation overflow\n");
         exit(EXIT_FAILURE);
     }
    memcpy((char*)temp_buffer + left_idx * vector->element_size, (char*)vector->data_array + left_idx * vector->element_size, segment_byte_size);
    merge_segment(vector->data_array, temp_buffer, vector->element_size, left_idx, middle_idx, right_idx, compare_func);
}

/*
 * Função principal para ordenar um vetor dinâmico usando Mergesort.
 * Aloca um buffer temporário e chama a função recursiva.
 * Baseado nos conceitos do TD2.
 */
static void dynamic_vector_mergesort(DynamicVector *vector, ComparisonFunction compare_func) {
    if (!vector || vector->current_length < 2) return;
    size_t total_byte_size = vector->current_length * vector->element_size;
    if (vector->element_size > 0 && vector->current_length > 0 && total_byte_size / vector->current_length != vector->element_size) {
         fprintf(stderr, "dynamic_vector_mergesort: size calculation overflow\n");
         exit(EXIT_FAILURE);
     }
    void *temp_buffer = malloc(total_byte_size);
    if (!temp_buffer) {
        perror("dynamic_vector_mergesort: malloc failed for temporary buffer");
        exit(EXIT_FAILURE);
    }
    mergesort_recursive(vector, temp_buffer, 0, vector->current_length, compare_func);
    free(temp_buffer);
}

/*
 * Ordena um vetor dinâmico usando o algoritmo Insertion Sort.
 * Percorre o vetor e insere cada elemento na sua posição correta
 * dentro da porção já ordenada à esquerda.
 * Baseado nos conceitos do TD2.
 */
static void dynamic_vector_insertionsort(DynamicVector *vector, ComparisonFunction compare_func) {
    if (!vector || vector->current_length < 2 || !compare_func) return;
    void *current_element_buffer = malloc(vector->element_size);
    if (!current_element_buffer) {
        perror("dynamic_vector_insertionsort: malloc failed for current_element buffer");
        exit(EXIT_FAILURE);
    }
    char *data_array_ptr = (char *)vector->data_array;
    for (size_t i = 1; i < vector->current_length; i++) {
        memcpy(current_element_buffer, data_array_ptr + i * vector->element_size, vector->element_size);
        size_t j = i;
        while (j > 0 && compare_func(current_element_buffer, data_array_ptr + (j - 1) * vector->element_size) < 0) {
            memcpy(data_array_ptr + j * vector->element_size, data_array_ptr + (j - 1) * vector->element_size, vector->element_size);
            j--;
        }
        if (j != i) {
             memcpy(data_array_ptr + j * vector->element_size, current_element_buffer, vector->element_size);
        }
    }
    free(current_element_buffer);
}

/*
 * Estrutura para armazenar uma palavra (string) e a sua contagem de ocorrências.
 */
typedef struct {
    char *word_string;
    int   occurrences;
} WordCount;

/*
 * Verifica se um ou dois bytes representam uma letra ASCII ou
 * um caractere UTF-8 comum em Português/Espanhol (intervalo C3 80-BF).
 * Retorna 1 se for letra, 0 caso contrário.
 * Atualiza char_byte_size_ptr com o número de bytes do caractere (1 ou 2).
 */
static int is_utf8_letter(unsigned char first_byte, unsigned char second_byte, int *char_byte_size_ptr) {
    if ((first_byte >= 'A' && first_byte <= 'Z') || (first_byte >= 'a' && first_byte <= 'z')) {
        *char_byte_size_ptr = 1; return 1;
    }
    if (first_byte == 0xC3 && second_byte >= 0x80 && second_byte <= 0xBF) {
        *char_byte_size_ptr = 2; return 1;
    }
    *char_byte_size_ptr = 1;
    return 0;
}

/*
 * Adiciona um caractere (ASCII ou UTF-8 de 2 bytes) ao buffer de token,
 * convertendo para minúsculas quando aplicável (ASCII e alguns UTF-8).
 * Verifica se há espaço no buffer antes de adicionar.
 * Termina o programa se o buffer estiver cheio.
 */
static void append_char_to_buffer(char *buffer, int *current_length_ptr, size_t buffer_capacity, unsigned char first_byte, unsigned char second_byte, int char_byte_size) {
    if ((size_t)(*current_length_ptr + char_byte_size) >= buffer_capacity) {
         fprintf(stderr, "append_char_to_buffer: token buffer overflow averted (max size %zu). Increase TOKEN_BUFFER_SIZE.\n", buffer_capacity);
         exit(EXIT_FAILURE);
    }
    if (char_byte_size == 1) {
        buffer[(*current_length_ptr)++] = tolower(first_byte);
    } else {
        buffer[(*current_length_ptr)++] = first_byte;
        buffer[(*current_length_ptr)++] = (second_byte >= 0x80 && second_byte <= 0x9E) ? second_byte + 0x20 : second_byte;
    }
}

/*
 * Função de comparação para ponteiros de string (char**).
 * Usada para ordenar o vetor de palavras alfabeticamente.
 */
static int compare_string_pointers(const void *ptr_a, const void *ptr_b) {
    char *string_a = *(char**)ptr_a;
    char *string_b = *(char**)ptr_b;
    return strcmp(string_a, string_b);
}

/*
 * Função de comparação para estruturas WordCount.
 * Ordena primeiro por contagem decrescente, depois por palavra alfabética crescente.
 */
static int compare_word_counts(const void *ptr_a, const void *ptr_b) {
    const WordCount *word_count_a = ptr_a;
    const WordCount *word_count_b = ptr_b;
    if (word_count_a->occurrences != word_count_b->occurrences) {
        return (word_count_b->occurrences > word_count_a->occurrences) - (word_count_b->occurrences < word_count_a->occurrences);
    }
    return strcmp(word_count_a->word_string, word_count_b->word_string);
}

/*
 * Função auxiliar para liberar a memória alocada para as strings
 * dentro do vetor dinâmico de palavras, antes de liberar o próprio vetor.
 */
static void free_word_list_content(DynamicVector *word_list_vector) {
    if (!word_list_vector) return;
    for (size_t k = 0; k < dynamic_vector_length(word_list_vector); ++k) {
        char **word_string_ptr = dynamic_vector_get(word_list_vector, k);
        if (word_string_ptr && *word_string_ptr) {
            free(*word_string_ptr);
            *word_string_ptr = NULL;
        }
    }
}

/*
 * Função principal do programa.
 */
int main(void) {
    /* Define o locale para suportar corretamente caracteres UTF-8 */
    if (setlocale(LC_ALL, "") == NULL) {
        fprintf(stderr, "Warning: Failed to set locale. UTF-8 handling might be incorrect.\n");
    }

    /* Lê o número de resultados (N) a serem exibidos */
    int num_results_requested;
    if (scanf("%d", &num_results_requested) != 1) {
        fprintf(stderr, "Error: Failed to read integer N.\n");
        return EXIT_FAILURE;
    }

    /* Descarta o caractere newline remanescente após a leitura de N */
    int input_char;
    while ((input_char = getchar()) != '\n' && input_char != EOF);

    /* Cria vetores dinâmicos para armazenar palavras e contagens */
    DynamicVector *word_list = dynamic_vector_create(sizeof(char*));
    DynamicVector *word_counts = NULL;

    /* Buffers para leitura de linhas e formação de tokens (palavras) */
    char line_buffer[LINE_BUFFER_SIZE];
    char token_buffer[TOKEN_BUFFER_SIZE];
    int current_token_length = 0;

    /* Processa a entrada padrão (stdin) linha por linha */
    while (fgets(line_buffer, sizeof line_buffer, stdin)) {
        unsigned char *current_char_ptr = (unsigned char*)line_buffer;
        unsigned char *line_end_ptr = current_char_ptr + strlen(line_buffer);

        /* Percorre cada caractere da linha lida */
        while (current_char_ptr < line_end_ptr) {
            int current_char_byte_size = 1;
            unsigned char first_byte = *current_char_ptr;
            unsigned char second_byte = (current_char_ptr + 1 < line_end_ptr) ? *(current_char_ptr + 1) : 0;

            /* Se for uma letra (ASCII ou UTF-8 suportado), adiciona ao token */
            if (is_utf8_letter(first_byte, second_byte, &current_char_byte_size)) {
                append_char_to_buffer(token_buffer, &current_token_length, TOKEN_BUFFER_SIZE, first_byte, second_byte, current_char_byte_size);
                current_char_ptr += current_char_byte_size;
            } else {
                /* Se não for letra e houver um token formado, processa o token */
                if (current_token_length > 0) {
                    token_buffer[current_token_length] = '\0';
                    char *word_copy = strdup(token_buffer);
                    if (!word_copy) {
                        perror("main: strdup failed for word token");
                        free_word_list_content(word_list);
                        dynamic_vector_free(word_list);
                        exit(EXIT_FAILURE);
                    }
                    dynamic_vector_push(word_list, &word_copy);
                    current_token_length = 0;
                }
                /* Avança para o próximo caractere (não-letra) */
                current_char_ptr++;
            }
        }
    }

    /* Verifica se ocorreram erros durante a leitura de stdin */
    if (ferror(stdin)) {
        perror("main: error reading from stdin");
        free_word_list_content(word_list);
        dynamic_vector_free(word_list);
        exit(EXIT_FAILURE);
    }

    /* Processa o último token se ele existir após o fim da entrada */
    if (current_token_length > 0) {
        token_buffer[current_token_length] = '\0';
        char *final_word_copy = strdup(token_buffer);
         if (!final_word_copy) {
             perror("main: strdup failed for final token");
             free_word_list_content(word_list);
             dynamic_vector_free(word_list);
             exit(EXIT_FAILURE);
         }
        dynamic_vector_push(word_list, &final_word_copy);
    }

    /* Ordena a lista de todas as palavras alfabeticamente usando Mergesort */
    dynamic_vector_mergesort(word_list, compare_string_pointers);

    /* Cria o vetor para armazenar as contagens de palavras únicas */
    word_counts = dynamic_vector_create(sizeof(WordCount));

    /* Conta as ocorrências de cada palavra única na lista ordenada */
    size_t total_word_count = dynamic_vector_length(word_list);
    for (size_t i = 0; i < total_word_count; ) {
        char *current_word_string = *(char**)dynamic_vector_get(word_list, i);
        size_t j = i + 1;
        /* Encontra o fim da sequência de palavras idênticas */
        while (j < total_word_count && strcmp(current_word_string, *(char**)dynamic_vector_get(word_list, j)) == 0) {
            /* Libera a memória das cópias duplicadas */
            free(*(char**)dynamic_vector_get(word_list, j));
            *(char**)dynamic_vector_get(word_list, j) = NULL;
            j++;
        }

        /* Cria um registro WordCount para a palavra única */
        WordCount current_record;
        current_record.word_string  = current_word_string; /* Reutiliza a primeira ocorrência da string */
        size_t current_word_occurrences = j - i;
        /* Verifica overflow antes de converter para int */
        if (current_word_occurrences > INT_MAX) {
             fprintf(stderr, "Warning: Word count for '%s' (%zu) exceeds INT_MAX, capping at %d.\n", current_record.word_string, current_word_occurrences, INT_MAX);
             current_record.occurrences = INT_MAX;
        } else {
            current_record.occurrences = (int)current_word_occurrences;
        }

        /* Adiciona o registro ao vetor de contagens */
        dynamic_vector_push(word_counts, &current_record);
        /* Avança o índice para a próxima palavra única */
        i = j;
    }

    /* Libera a memória do vetor que continha todas as palavras */
    dynamic_vector_free(word_list);
    word_list = NULL;

    /* Ordena o vetor de contagens (WordCount) usando Insertion Sort */
    /* Critério: contagem decrescente, depois palavra crescente */
    dynamic_vector_insertionsort(word_counts, compare_word_counts);

    /* Determina o número de resultados a serem impressos (limitado por N e pelo total de palavras únicas) */
    size_t unique_word_count = dynamic_vector_length(word_counts);
    size_t output_limit;
    if (num_results_requested < 0) {
        output_limit = unique_word_count;
    } else {
        output_limit = ((size_t)num_results_requested < unique_word_count) ? (size_t)num_results_requested : unique_word_count;
    }

    /* Imprime os N resultados mais frequentes no formato desejado */
    int output_was_generated = 0;
    for (size_t i = 0; i < output_limit; i++) {
        WordCount *current_entry = dynamic_vector_get(word_counts, i);
        if (current_entry && current_entry->word_string) {
             printf("%4d %s\n", current_entry->occurrences, current_entry->word_string);
             output_was_generated = 1;
        }
        /* Libera a string da palavra após a impressão */
        free(current_entry->word_string);
        current_entry->word_string = NULL;
    }

    /* Adiciona uma linha em branco no final se algum output foi gerado */
    if (output_was_generated) {
       // printf("\n");
    }

    /* Libera a memória do vetor de contagens */
    dynamic_vector_free(word_counts);
    word_counts = NULL;

    /* Verifica erros na escrita para stdout */
    if (ferror(stdout)) {
        perror("main: error writing to stdout");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
