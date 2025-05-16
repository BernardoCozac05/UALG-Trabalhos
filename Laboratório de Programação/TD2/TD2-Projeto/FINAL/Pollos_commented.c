/* Inclusão das bibliotecas padrão necessárias para entrada/saída,


 * alocação de memória, manipulação de strings, tipos de caracteres,
 * configuração de localidade e limites de inteiros. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <limits.h>

/* Define tamanhos de buffer para leitura de linhas e armazenamento de tokens (palavras).
 * LINE_BUFFER_SIZE: Tamanho máximo de uma linha lida da entrada.
 * TOKEN_BUFFER_SIZE: Tamanho máximo de uma palavra (token) extraída. */
#define LINE_BUFFER_SIZE 4096
#define TOKEN_BUFFER_SIZE 8192

/*
 * Estrutura para representar um vetor dinâmico genérico.
 * Armazena um array de dados (void*), o tamanho de cada elemento,
 * o número atual de elementos (comprimento) e a capacidade alocada.
 * Permite armazenar elementos de qualquer tipo e crescer conforme necessário.
 */
typedef struct {
    void   *data_array;        /* Ponteiro para o bloco de memória que armazena os elementos */
    size_t  element_size;     /* Tamanho em bytes de cada elemento no vetor */
    size_t  current_length;   /* Número atual de elementos armazenados no vetor */
    size_t  current_capacity; /* Número máximo de elementos que o vetor pode armazenar atualmente */
} DynamicVector;

/*
 * Define um tipo para ponteiros de função de comparação genérica.
 * Esta assinatura é compatível com as funções qsort e bsearch da biblioteca padrão C,
 * permitindo que as funções de ordenação e busca funcionem com diferentes tipos de dados.
 * A função deve retornar:
 *  - um valor negativo se o primeiro elemento for menor que o segundo.
 *  - zero se os elementos forem iguais.
 *  - um valor positivo se o primeiro elemento for maior que o segundo.
 */
typedef int  (*ComparisonFunction)(const void*, const void*);

/*
 * Cria e inicializa um novo vetor dinâmico (DynamicVector).
 * Parâmetros:
 *   - element_size_param: O tamanho (em bytes) de cada elemento que será armazenado no vetor.
 * Retorna:
 *   - Um ponteiro para a estrutura DynamicVector recém-criada e inicializada.
 * Comportamento:
 *   - Aloca memória para a estrutura DynamicVector.
 *   - Aloca memória inicial para o buffer de dados (data_array) com uma capacidade padrão (16).
 *   - Define o tamanho do elemento, comprimento inicial (0) e capacidade inicial.
 *   - Termina o programa com erro (exit(EXIT_FAILURE)) se:
 *     - o tamanho do elemento for 0.
 *     - a alocação de memória para a estrutura ou para o buffer de dados falhar.
 *     - ocorrer um overflow no cálculo do tamanho da alocação inicial.
 */
static DynamicVector* dynamic_vector_create(size_t element_size_param) {
    /* Verifica se o tamanho do elemento é válido (não pode ser zero) */
    if (element_size_param == 0) {
        fprintf(stderr, "dynamic_vector_create: element size cannot be zero\n");
        exit(EXIT_FAILURE);
    }
    /* Aloca memória para a estrutura do vetor dinâmico */
    DynamicVector *vector = malloc(sizeof *vector);
    if (!vector) {
        perror("dynamic_vector_create: malloc failed for DynamicVector struct");
        exit(EXIT_FAILURE);
    }
    /* Inicializa os campos da estrutura */
    vector->element_size = element_size_param;
    vector->current_length    = 0; /* Começa vazio */
    vector->current_capacity  = 16; /* Capacidade inicial padrão */
    /* Calcula o tamanho total em bytes para a alocação inicial */
    size_t initial_allocation_size = vector->element_size * vector->current_capacity;
    /* Verifica se houve overflow no cálculo do tamanho da alocação */
     if (vector->current_capacity > 0 && initial_allocation_size / vector->current_capacity != vector->element_size) {
         fprintf(stderr, "dynamic_vector_create: initial size calculation overflow\n");
         free(vector); /* Libera a memória já alocada para a estrutura */
         exit(EXIT_FAILURE);
     }
    /* Aloca memória para o buffer de dados onde os elementos serão armazenados */
    vector->data_array = malloc(initial_allocation_size);
    if (!vector->data_array) {
        perror("dynamic_vector_create: malloc failed for data buffer");
        free(vector); /* Libera a memória já alocada para a estrutura */
        exit(EXIT_FAILURE);
    }
    /* Retorna o ponteiro para o vetor dinâmico criado */
    return vector;
}

/*
 * Libera a memória alocada para um vetor dinâmico.
 * Parâmetros:
 *   - vector: Ponteiro para o DynamicVector a ser liberado.
 * Comportamento:
 *   - Se o ponteiro do vetor não for nulo:
 *     - Libera a memória alocada para o buffer de dados (data_array).
 *     - Libera a memória alocada para a própria estrutura DynamicVector.
 *   - É seguro chamar esta função com um ponteiro nulo (não faz nada).
 */
static void dynamic_vector_free(DynamicVector *vector) {
    if (vector) {
        free(vector->data_array); /* Libera o buffer de dados */
        free(vector);           /* Libera a estrutura */
    }
}

/*
 * Retorna o número atual de elementos armazenados no vetor dinâmico.
 * Parâmetros:
 *   - vector: Ponteiro para o DynamicVector.
 * Retorna:
 *   - O número de elementos (current_length) se o vetor não for nulo.
 *   - 0 se o ponteiro do vetor for nulo.
 */
static size_t dynamic_vector_length(const DynamicVector *vector) {
    return vector ? vector->current_length : 0;
}

/*
 * Retorna um ponteiro para o elemento no índice especificado dentro do vetor dinâmico.
 * Parâmetros:
 *   - vector: Ponteiro para o DynamicVector.
 *   - index: O índice do elemento desejado (baseado em 0).
 * Retorna:
 *   - Um ponteiro void* para o início do elemento no índice especificado.
 *     O chamador deve converter este ponteiro para o tipo apropriado.
 * Comportamento:
 *   - Termina o programa com erro (exit(EXIT_FAILURE)) se:
 *     - o ponteiro do vetor for nulo.
 *     - o índice estiver fora dos limites válidos (index >= current_length).
 *   - Calcula o endereço do elemento usando aritmética de ponteiros:
 *     endereço_base + (índice * tamanho_do_elemento).
 */
static void* dynamic_vector_get(const DynamicVector *vector, size_t index) {
    /* Verifica se o vetor é válido e se o índice está dentro dos limites */
    if (!vector || index >= vector->current_length) {
         fprintf(stderr, "dynamic_vector_get: index out of bounds or NULL vector\n");
         exit(EXIT_FAILURE);
    }
    /* Calcula e retorna o endereço do elemento desejado */
    /* (char*) é usado para aritmética de ponteiros em bytes */
    return (char*)vector->data_array + index * vector->element_size;
}

/*
 * Adiciona um elemento ao final do vetor dinâmico (operação "push back").
 * Parâmetros:
 *   - vector: Ponteiro para o DynamicVector onde o elemento será adicionado.
 *   - element: Ponteiro para os dados do elemento a ser adicionado. O conteúdo
 *              apontado por 'element' é copiado para o vetor.
 * Comportamento:
 *   - Verifica se os ponteiros 'vector' e 'element' são válidos.
 *   - Se o vetor estiver cheio (current_length == current_capacity):
 *     - Duplica a capacidade atual (current_capacity).
 *     - Verifica overflows no cálculo da nova capacidade e do novo tamanho de alocação.
 *     - Realoca o buffer de dados (data_array) para a nova capacidade.
 *     - Termina o programa com erro se a realocação falhar.
 *     - Atualiza a capacidade (current_capacity) e o ponteiro do buffer (data_array).
 *   - Copia o conteúdo do 'element' fornecido para a próxima posição livre no buffer de dados.
 *   - Incrementa o número de elementos (current_length).
 *   - Termina o programa com erro se os ponteiros de entrada forem nulos ou se ocorrer overflow.
 */
static void dynamic_vector_push(DynamicVector *vector, const void *element) {
    /* Verifica se os ponteiros recebidos são válidos */
    if (!vector || !element) {
        fprintf(stderr, "dynamic_vector_push: received NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    /* Verifica se o vetor precisa ser redimensionado */
    if (vector->current_length == vector->current_capacity) {
        /* Calcula a nova capacidade (geralmente o dobro da atual) */
        size_t new_capacity = vector->current_capacity * 2;
        /* Verifica overflow no cálculo da nova capacidade (importante para size_t) */
        if (new_capacity < vector->current_capacity) { /* Overflow detectado */
             fprintf(stderr, "dynamic_vector_push: capacity overflow\n");
             exit(EXIT_FAILURE);
        }
        /* Calcula o novo tamanho total em bytes necessário para o buffer */
        size_t new_allocation_size = new_capacity * vector->element_size;
        /* Verifica overflow no cálculo do tamanho da alocação */
        if (vector->element_size > 0 && new_allocation_size / vector->element_size != new_capacity) {
             fprintf(stderr, "dynamic_vector_push: allocation size calculation overflow\n");
             exit(EXIT_FAILURE);
        }
        /* Tenta realocar o buffer de dados para o novo tamanho */
        void *new_data_array = realloc(vector->data_array, new_allocation_size);
        if (!new_data_array) {
            perror("dynamic_vector_push: realloc failed");
            exit(EXIT_FAILURE);
        }
        /* Atualiza o ponteiro do buffer e a capacidade do vetor */
        vector->data_array     = new_data_array;
        vector->current_capacity = new_capacity;
    }
    /* Copia os dados do elemento fornecido para o final do vetor */
    /* Calcula o endereço de destino usando aritmética de ponteiros */
    memcpy((char*)vector->data_array + vector->current_length * vector->element_size, element, vector->element_size);
    /* Incrementa o contador de elementos */
    vector->current_length++;
}

/*
 * Função auxiliar para o algoritmo Mergesort.
 * Mescla (intercala) dois segmentos ordenados de um buffer temporário (source_temp_buffer)
 * de volta para o buffer de destino (destination_base).
 * Parâmetros:
 *   - destination_base: Ponteiro para o início do buffer de destino (geralmente o data_array do vetor).
 *   - source_temp_buffer: Ponteiro para o início do buffer temporário que contém os dados a serem mesclados.
 *   - element_size: Tamanho de cada elemento.
 *   - left_idx: Índice inicial do primeiro segmento (inclusive).
 *   - middle_idx: Índice final do primeiro segmento (exclusive) e inicial do segundo segmento (inclusive).
 *   - right_idx: Índice final do segundo segmento (exclusive).
 *   - compare_func: Ponteiro para a função de comparação usada para determinar a ordem.
 * Comportamento:
 *   - Percorre os dois segmentos ( [left_idx, middle_idx) e [middle_idx, right_idx) ) no buffer temporário.
 *   - Compara os elementos atuais de cada segmento usando compare_func.
 *   - Copia o menor elemento para a posição correta no buffer de destino.
 *   - Avança o índice do segmento de onde o elemento foi copiado.
 *   - Repete até que todos os elementos de ambos os segmentos tenham sido copiados para o destino.
 *   - Termina o programa com erro se encontrar um estado inesperado (lógica de segurança).
 */
static void merge_segment(void *destination_base, void *source_temp_buffer, size_t element_size, size_t left_idx, size_t middle_idx, size_t right_idx, ComparisonFunction compare_func) {
    /* Ponteiros char* para facilitar a aritmética de ponteiros em bytes */
    char *source_ptr = source_temp_buffer;
    char *destination_ptr = destination_base;
    /* Índices para percorrer o primeiro (i) e o segundo (j) segmento no buffer temporário */
    size_t i = left_idx, j = middle_idx;
    /* Índice k para percorrer a posição de escrita no buffer de destino */
    for (size_t k = left_idx; k < right_idx; k++) {
        /* Verifica se o primeiro segmento ainda tem elementos E
           (o segundo segmento acabou OU o elemento atual do primeiro é menor/igual ao do segundo) */
        if (i < middle_idx && (j >= right_idx || compare_func(source_ptr + i*element_size, source_ptr + j*element_size) <= 0)) {
            /* Copia o elemento do primeiro segmento para o destino */
            memcpy(destination_ptr + k*element_size, source_ptr + i*element_size, element_size);
            i++; /* Avança no primeiro segmento */
        /* Verifica se o segundo segmento ainda tem elementos (caso contrário, a condição anterior seria verdadeira) */
        } else if (j < right_idx) {
            /* Copia o elemento do segundo segmento para o destino */
            memcpy(destination_ptr + k*element_size, source_ptr + j*element_size, element_size);
            j++; /* Avança no segundo segmento */
        } else {
            /* Condição de erro: não deveria chegar aqui se a lógica estiver correta */
             fprintf(stderr, "merge_segment: unexpected state at k=%zu\n", k);
             exit(EXIT_FAILURE);
        }
    }
}

/*
 * Implementação recursiva do algoritmo Mergesort (abordagem top-down).
 * Ordena o segmento do vetor definido por [left_idx, right_idx).
 * Parâmetros:
 *   - vector: Ponteiro para o DynamicVector a ser ordenado.
 *   - temp_buffer: Ponteiro para um buffer temporário de tamanho igual ao segmento a ser ordenado.
 *   - left_idx: Índice inicial do segmento (inclusive).
 *   - right_idx: Índice final do segmento (exclusive).
 *   - compare_func: Ponteiro para a função de comparação.
 * Comportamento:
 *   - Caso base: Se o segmento tiver 0 ou 1 elemento (right_idx - left_idx < 2), já está ordenado, retorna.
 *   - Divide: Calcula o índice do meio (middle_idx).
 *   - Conquista: Chama recursivamente mergesort_recursive para ordenar a metade esquerda [left_idx, middle_idx)
 *                e a metade direita [middle_idx, right_idx).
 *   - Combina: Copia o segmento [left_idx, right_idx) do vetor original para o buffer temporário.
 *              Chama merge_segment para mesclar as duas metades ordenadas do buffer temporário
 *              de volta para o vetor original na posição correta.
 *   - Verifica overflow no cálculo do tamanho do segmento em bytes antes de copiar.
 */
static void mergesort_recursive(DynamicVector *vector, void *temp_buffer, size_t left_idx, size_t right_idx, ComparisonFunction compare_func) {
    /* Caso base da recursão: segmento com 0 ou 1 elemento */
    if (right_idx - left_idx < 2) return;
    /* Calcula o índice do meio para dividir o segmento */
    size_t middle_idx = left_idx + (right_idx - left_idx) / 2;
    /* Ordena recursivamente a metade esquerda */
    mergesort_recursive(vector, temp_buffer, left_idx, middle_idx, compare_func);
    /* Ordena recursivamente a metade direita */
    mergesort_recursive(vector, temp_buffer, middle_idx, right_idx, compare_func);
    /* Calcula o tamanho do segmento atual em número de elementos e em bytes */
    size_t segment_length = right_idx - left_idx;
    size_t segment_byte_size = segment_length * vector->element_size;
    /* Verifica overflow no cálculo do tamanho em bytes */
    if (vector->element_size > 0 && segment_length > 0 && segment_byte_size / segment_length != vector->element_size) {
         fprintf(stderr, "mergesort_recursive: size calculation overflow\n");
         exit(EXIT_FAILURE);
     }
    /* Copia o segmento atual (agora com metades ordenadas) do vetor para o buffer temporário */
    memcpy((char*)temp_buffer + left_idx * vector->element_size, (char*)vector->data_array + left_idx * vector->element_size, segment_byte_size);
    /* Mescla as duas metades ordenadas do buffer temporário de volta para o vetor original */
    merge_segment(vector->data_array, temp_buffer, vector->element_size, left_idx, middle_idx, right_idx, compare_func);
}

/*
 * Função principal para ordenar um vetor dinâmico usando o algoritmo Mergesort.
 * Parâmetros:
 *   - vector: Ponteiro para o DynamicVector a ser ordenado.
 *   - compare_func: Ponteiro para a função de comparação que define a ordem dos elementos.
 * Comportamento:
 *   - Se o vetor for nulo ou tiver menos de 2 elementos, não faz nada.
 *   - Calcula o tamanho total em bytes necessário para o buffer temporário.
 *   - Verifica overflow no cálculo do tamanho.
 *   - Aloca memória para o buffer temporário.
 *   - Termina o programa com erro se a alocação falhar.
 *   - Chama a função recursiva mergesort_recursive para realizar a ordenação.
 *   - Libera a memória do buffer temporário após a conclusão da ordenação.
 *   - Baseado nos conceitos do TD2 (Trabalho Dirigido 2, provavelmente de uma disciplina).
 */
static void dynamic_vector_mergesort(DynamicVector *vector, ComparisonFunction compare_func) {
    /* Não ordena se o vetor for nulo ou tiver 0 ou 1 elemento */
    if (!vector || vector->current_length < 2) return;
    /* Calcula o tamanho total em bytes do vetor */
    size_t total_byte_size = vector->current_length * vector->element_size;
    /* Verifica overflow no cálculo do tamanho */
    if (vector->element_size > 0 && vector->current_length > 0 && total_byte_size / vector->current_length != vector->element_size) {
         fprintf(stderr, "dynamic_vector_mergesort: size calculation overflow\n");
         exit(EXIT_FAILURE);
     }
    /* Aloca o buffer temporário necessário para o Mergesort */
    void *temp_buffer = malloc(total_byte_size);
    if (!temp_buffer) {
        perror("dynamic_vector_mergesort: malloc failed for temporary buffer");
        exit(EXIT_FAILURE);
    }
    /* Chama a função recursiva que implementa o Mergesort */
    mergesort_recursive(vector, temp_buffer, 0, vector->current_length, compare_func);
    /* Libera o buffer temporário após a ordenação */
    free(temp_buffer);
}

/*
 * Ordena um vetor dinâmico usando o algoritmo Insertion Sort (Ordenação por Inserção).
 * Parâmetros:
 *   - vector: Ponteiro para o DynamicVector a ser ordenado.
 *   - compare_func: Ponteiro para a função de comparação que define a ordem dos elementos.
 * Comportamento:
 *   - Se o vetor for nulo, tiver menos de 2 elementos ou a função de comparação for nula, não faz nada.
 *   - Aloca um buffer temporário para armazenar o elemento atual sendo inserido.
 *   - Itera pelo vetor a partir do segundo elemento (índice 1).
 *   - Para cada elemento (chave), compara-o com os elementos anteriores (já ordenados).
 *   - Desloca os elementos maiores que a chave uma posição para a direita para abrir espaço.
 *   - Insere a chave na posição correta.
 *   - Libera o buffer temporário do elemento atual.
 *   - Baseado nos conceitos do TD2.
 * Nota: Insertion Sort é eficiente para vetores pequenos ou quase ordenados, mas ineficiente (O(n^2))
 *       para vetores grandes e desordenados em comparação com Mergesort (O(n log n)).
 */
static void dynamic_vector_insertionsort(DynamicVector *vector, ComparisonFunction compare_func) {
    /* Não ordena se o vetor for inválido, pequeno ou sem função de comparação */
    if (!vector || vector->current_length < 2 || !compare_func) return;
    /* Aloca buffer para guardar temporariamente o elemento a ser inserido */
    void *current_element_buffer = malloc(vector->element_size);
    if (!current_element_buffer) {
        perror("dynamic_vector_insertionsort: malloc failed for current_element buffer");
        exit(EXIT_FAILURE);
    }
    /* Ponteiro char* para facilitar a aritmética de ponteiros no array de dados */
    char *data_array_ptr = (char *)vector->data_array;
    /* Itera a partir do segundo elemento (índice 1) */
    for (size_t i = 1; i < vector->current_length; i++) {
        /* Copia o elemento atual (chave) para o buffer temporário */
        memcpy(current_element_buffer, data_array_ptr + i * vector->element_size, vector->element_size);
        /* j é o índice onde a chave será potencialmente inserida */
        size_t j = i;
        /* Enquanto j > 0 e a chave for menor que o elemento à sua esquerda (j-1) */
        while (j > 0 && compare_func(current_element_buffer, data_array_ptr + (j - 1) * vector->element_size) < 0) {
            /* Desloca o elemento da esquerda (j-1) para a posição atual (j) */
            memcpy(data_array_ptr + j * vector->element_size, data_array_ptr + (j - 1) * vector->element_size, vector->element_size);
            j--; /* Move o índice de inserção para a esquerda */
        }
        /* Se j foi modificado, significa que a chave precisa ser inserida na posição j */
        if (j != i) {
             memcpy(data_array_ptr + j * vector->element_size, current_element_buffer, vector->element_size);
        }
    }
    /* Libera o buffer temporário usado para a chave */
    free(current_element_buffer);
}

/*
 * Estrutura para armazenar uma palavra (string) e a sua contagem de ocorrências.
 * Usada para guardar os resultados finais após contar as palavras únicas.
 */
typedef struct {
    char *word_string; /* Ponteiro para a string da palavra (alocada dinamicamente) */
    int   occurrences; /* Número de vezes que a palavra ocorreu no texto */
} WordCount;

/*
 * Verifica se um ou dois bytes (representando um caractere) formam uma letra.
 * Suporta letras ASCII (A-Z, a-z) e caracteres acentuados comuns em Português/Espanhol
 * codificados em UTF-8 (sequência C3 seguida por 80-BF).
 * Parâmetros:
 *   - first_byte: O primeiro byte do caractere.
 *   - second_byte: O segundo byte do caractere (relevante apenas se first_byte for 0xC3).
 *   - char_byte_size_ptr: Ponteiro para uma variável int onde o tamanho do caractere
 *                         detectado (1 ou 2 bytes) será armazenado.
 * Retorna:
 *   - 1 se for uma letra (ASCII ou UTF-8 suportado).
 *   - 0 caso contrário.
 * Comportamento:
 *   - Atualiza o valor apontado por char_byte_size_ptr para 1 ou 2.
 */
static int is_utf8_letter(unsigned char first_byte, unsigned char second_byte, int *char_byte_size_ptr) {
    /* Verifica se é uma letra ASCII maiúscula ou minúscula */
    if ((first_byte >= 'A' && first_byte <= 'Z') || (first_byte >= 'a' && first_byte <= 'z')) {
        *char_byte_size_ptr = 1; return 1;
    }
    /* Verifica se é um caractere UTF-8 de 2 bytes comum (C3 80-BF) */
    /* Corresponde a À-ÖØ-öø-ÿ (com algumas lacunas) */
    if (first_byte == 0xC3 && second_byte >= 0x80 && second_byte <= 0xBF) {
        *char_byte_size_ptr = 2; return 1;
    }
    /* Se não for nenhum dos casos acima, considera como 1 byte (não-letra) */
    *char_byte_size_ptr = 1;
    return 0;
}

/*
 * Adiciona um caractere (ASCII ou UTF-8 de 2 bytes) ao buffer de token (palavra).
 * Converte o caractere para minúsculas, se aplicável (letras ASCII e algumas UTF-8).
 * Parâmetros:
 *   - buffer: O buffer de char onde o token está sendo construído.
 *   - current_length_ptr: Ponteiro para o comprimento atual do token no buffer.
 *   - buffer_capacity: O tamanho máximo do buffer.
 *   - first_byte: O primeiro byte do caractere a adicionar.
 *   - second_byte: O segundo byte do caractere (usado se char_byte_size for 2).
 *   - char_byte_size: O tamanho do caractere (1 ou 2 bytes), determinado por is_utf8_letter.
 * Comportamento:
 *   - Verifica se há espaço suficiente no buffer para adicionar o caractere.
 *     Termina o programa com erro se o buffer estiver cheio para evitar overflow.
 *   - Se for um caractere de 1 byte (ASCII):
 *     - Converte para minúsculas usando tolower() e adiciona ao buffer.
 *   - Se for um caractere de 2 bytes (UTF-8):
 *     - Adiciona o primeiro byte (0xC3) diretamente.
 *     - Adiciona o segundo byte, convertendo para minúsculas se for uma letra maiúscula
 *       acentuada (0x80 a 0x9E correspondem a À a Ö). A conversão é feita adicionando 0x20.
 *   - Atualiza o comprimento atual do token no buffer.
 */
static void append_char_to_buffer(char *buffer, int *current_length_ptr, size_t buffer_capacity, unsigned char first_byte, unsigned char second_byte, int char_byte_size) {
    /* Verifica se a adição do caractere excederia a capacidade do buffer */
    if ((size_t)(*current_length_ptr + char_byte_size) >= buffer_capacity) {
         /* Mensagem de erro indicando buffer cheio e sugerindo aumento */
         fprintf(stderr, "append_char_to_buffer: token buffer overflow averted (max size %zu). Increase TOKEN_BUFFER_SIZE.\n", buffer_capacity);
         exit(EXIT_FAILURE);
    }
    /* Se for um caractere ASCII de 1 byte */
    if (char_byte_size == 1) {
        /* Converte para minúscula e adiciona ao buffer */
        buffer[(*current_length_ptr)++] = tolower(first_byte);
    } else { /* Se for um caractere UTF-8 de 2 bytes */
        /* Adiciona o primeiro byte (geralmente 0xC3) */
        buffer[(*current_length_ptr)++] = first_byte;
        /* Adiciona o segundo byte, convertendo para minúscula se for letra maiúscula acentuada (0x80-0x9E -> 0xA0-0xBE) */
        buffer[(*current_length_ptr)++] = (second_byte >= 0x80 && second_byte <= 0x9E) ? second_byte + 0x20 : second_byte;
    }
}

/*
 * Função de comparação para ser usada com qsort ou mergesort
 * ao ordenar um array de ponteiros para strings (char**).
 * Compara as strings apontadas pelos ponteiros alfabeticamente.
 * Parâmetros:
 *   - ptr_a: Ponteiro void* para o primeiro ponteiro de string (char**).
 *   - ptr_b: Ponteiro void* para o segundo ponteiro de string (char**).
 * Retorna:
 *   - Resultado de strcmp aplicado às strings apontadas.
 */
static int compare_string_pointers(const void *ptr_a, const void *ptr_b) {
    /* Converte os ponteiros void* para ponteiros para ponteiros de char (char**) */
    /* Dereferencia uma vez para obter os ponteiros de char (char*) */
    char *string_a = *(char**)ptr_a;
    char *string_b = *(char**)ptr_b;
    /* Compara as strings usando strcmp */
    return strcmp(string_a, string_b);
}

/*
 * Função de comparação para ser usada com qsort, bsearch ou as funções de ordenação
 * deste programa ao ordenar um array de estruturas WordCount.
 * Critério de ordenação:
 *   1. Contagem de ocorrências (occurrences) em ordem decrescente.
 *   2. Em caso de empate na contagem, palavra (word_string) em ordem alfabética crescente.
 * Parâmetros:
 *   - ptr_a: Ponteiro void* para a primeira estrutura WordCount.
 *   - ptr_b: Ponteiro void* para a segunda estrutura WordCount.
 * Retorna:
 *   - Valor negativo se A < B, zero se A == B, valor positivo se A > B (considerando os critérios).
 */
static int compare_word_counts(const void *ptr_a, const void *ptr_b) {
    /* Converte os ponteiros void* para ponteiros para WordCount */
    const WordCount *word_count_a = ptr_a;
    const WordCount *word_count_b = ptr_b;
    /* Compara primeiro pela contagem de ocorrências (decrescente) */
    if (word_count_a->occurrences != word_count_b->occurrences) {
        /* Retorna -1 se B > A, 1 se B < A (para ordem decrescente) */
        return (word_count_b->occurrences > word_count_a->occurrences) - (word_count_b->occurrences < word_count_a->occurrences);
    }
    /* Se as contagens forem iguais, compara pelas strings (alfabética crescente) */
    return strcmp(word_count_a->word_string, word_count_b->word_string);
}

/*
 * Função auxiliar para liberar a memória alocada para as strings
 * dentro de um vetor dinâmico que armazena ponteiros para strings (char**).
 * É crucial chamar esta função antes de liberar o próprio vetor dinâmico
 * para evitar vazamentos de memória das strings individuais.
 * Parâmetros:
 *   - word_list_vector: Ponteiro para o DynamicVector contendo ponteiros char**.
 * Comportamento:
 *   - Itera por todos os elementos do vetor.
 *   - Para cada elemento (que é um char**), obtém o ponteiro para a string (char*).
 *   - Libera a memória da string usando free().
 *   - Define o ponteiro da string para NULL após liberar (boa prática).
 */
static void free_word_list_content(DynamicVector *word_list_vector) {
    if (!word_list_vector) return; /* Não faz nada se o vetor for nulo */
    /* Itera por todos os elementos do vetor */
    for (size_t k = 0; k < dynamic_vector_length(word_list_vector); ++k) {
        /* Obtém o ponteiro para o ponteiro da string (char**) no índice k */
        char **word_string_ptr = dynamic_vector_get(word_list_vector, k);
        /* Verifica se o ponteiro para o ponteiro e o ponteiro da string são válidos */
        if (word_string_ptr && *word_string_ptr) {
            free(*word_string_ptr); /* Libera a memória da string */
            *word_string_ptr = NULL; /* Define o ponteiro como NULL */
        }
    }
}

/*
 * Função principal do programa (entry point).
 * Responsável por: Ler a entrada, processar o texto, contar palavras,
 * ordenar os resultados e imprimir as N palavras mais frequentes.
 */
int main(void) {
    /* Define a localidade (locale) para a do sistema operacional.
     * Isso é importante para que funções como tolower() e isalpha() (implícita em is_utf8_letter)
     * funcionem corretamente com caracteres acentuados e UTF-8. */
    if (setlocale(LC_ALL, "") == NULL) {
        /* Emite um aviso se a configuração da localidade falhar */
        fprintf(stderr, "Warning: Failed to set locale. UTF-8 handling might be incorrect.\n");
    }

    /* Lê o número de resultados (N) que o utilizador deseja ver. */
    int num_results_requested;
    /* scanf retorna o número de itens lidos com sucesso. Deve ser 1. */
    if (scanf("%d", &num_results_requested) != 1) {
        fprintf(stderr, "Error: Failed to read integer N.\n");
        return EXIT_FAILURE;
    }

    /* Descarta o caractere de nova linha ('\n') que permanece no buffer de entrada
     * após a leitura do número N com scanf. Isso evita que fgets leia uma linha vazia
     * imediatamente a seguir. */
    int input_char;
    while ((input_char = getchar()) != '\n' && input_char != EOF);

    /* Cria um vetor dinâmico para armazenar ponteiros para todas as palavras (strings) lidas. */
    /* Cada elemento do vetor terá o tamanho de um ponteiro para char (sizeof(char*)). */
    DynamicVector *word_list = dynamic_vector_create(sizeof(char*));
    /* Ponteiro para o vetor de contagens, será criado mais tarde. */
    DynamicVector *word_counts = NULL;

    /* Buffers estáticos para leitura de linhas e formação de tokens (palavras). */
    char line_buffer[LINE_BUFFER_SIZE]; /* Buffer para armazenar cada linha lida de stdin */
    char token_buffer[TOKEN_BUFFER_SIZE]; /* Buffer para construir a palavra atual */
    int current_token_length = 0; /* Comprimento atual da palavra no token_buffer */

    /* Loop principal: processa a entrada padrão (stdin) linha por linha. */
    /* fgets lê uma linha inteira até encontrar '\n' ou EOF, ou até o buffer encher. */
    while (fgets(line_buffer, sizeof line_buffer, stdin)) {
        /* Ponteiros para percorrer a linha lida. unsigned char é usado para garantir
           que os valores dos bytes sejam tratados como não negativos. */
        unsigned char *current_char_ptr = (unsigned char*)line_buffer;
        unsigned char *line_end_ptr = current_char_ptr + strlen(line_buffer);

        /* Loop interno: percorre cada caractere da linha atual. */
        while (current_char_ptr < line_end_ptr) {
            int current_char_byte_size = 1; /* Tamanho do caractere atual (1 ou 2 bytes) */
            unsigned char first_byte = *current_char_ptr;
            /* Obtém o segundo byte se existir (necessário para UTF-8 de 2 bytes) */
            unsigned char second_byte = (current_char_ptr + 1 < line_end_ptr) ? *(current_char_ptr + 1) : 0;

            /* Verifica se o caractere atual (1 ou 2 bytes) é uma letra */
            if (is_utf8_letter(first_byte, second_byte, &current_char_byte_size)) {
                /* Se for letra, adiciona ao token_buffer (convertendo para minúscula) */
                append_char_to_buffer(token_buffer, &current_token_length, TOKEN_BUFFER_SIZE, first_byte, second_byte, current_char_byte_size);
                /* Avança o ponteiro pelo número de bytes do caractere (1 ou 2) */
                current_char_ptr += current_char_byte_size;
            } else {
                /* Se não for letra (é um separador ou outro caractere) */
                /* Verifica se havia uma palavra sendo formada no token_buffer */
                if (current_token_length > 0) {
                    /* Finaliza a string no token_buffer com o caractere nulo */
                    token_buffer[current_token_length] = '\0';
                    /* Cria uma cópia dinâmica (strdup) da palavra no token_buffer */
                    char *word_copy = strdup(token_buffer);
                    if (!word_copy) {
                        perror("main: strdup failed for word token");
                        /* Libera memória alocada antes de sair em caso de erro */
                        free_word_list_content(word_list);
                        dynamic_vector_free(word_list);
                        exit(EXIT_FAILURE);
                    }
                    /* Adiciona o ponteiro para a cópia da palavra ao vetor word_list */
                    dynamic_vector_push(word_list, &word_copy);
                    /* Reseta o comprimento do token para começar a próxima palavra */
                    current_token_length = 0;
                }
                /* Avança o ponteiro para o próximo caractere (o separador) */
                current_char_ptr++;
            }
        }
        /* Fim do loop interno (fim da linha) */
    }
    /* Fim do loop principal (fim da entrada - EOF) */

    /* Verifica se ocorreram erros durante a leitura de stdin (ex: erro de I/O) */
    if (ferror(stdin)) {
        perror("main: error reading from stdin");
        free_word_list_content(word_list);
        dynamic_vector_free(word_list);
        exit(EXIT_FAILURE);
    }

    /* Processa o último token se a entrada terminar enquanto uma palavra estava sendo formada */
    if (current_token_length > 0) {
        token_buffer[current_token_length] = '\0';
        char *final_word_copy = strdup(token_buffer);
         if (!final_word_copy) {
             perror("main: strdup failed for final token");
             free_word_list_content(word_list);
             dynamic_vector_free(word_list);
             exit(EXIT_FAILURE);
         }
        /* Adiciona a última palavra ao vetor */
        dynamic_vector_push(word_list, &final_word_copy);
    }

    /* Ordena alfabeticamente o vetor que contém ponteiros para todas as palavras lidas. */
    /* Isso agrupa palavras idênticas, facilitando a contagem. */
    dynamic_vector_mergesort(word_list, compare_string_pointers);

    /* Cria o vetor dinâmico para armazenar as contagens de palavras únicas (estruturas WordCount). */
    word_counts = dynamic_vector_create(sizeof(WordCount));

    /* Conta as ocorrências de cada palavra única na lista ordenada. */
    size_t total_word_count = dynamic_vector_length(word_list);
    /* Itera pela lista ordenada de ponteiros de palavras */
    for (size_t i = 0; i < total_word_count; ) {
        /* Obtém o ponteiro para a string da palavra atual */
        char *current_word_string = *(char**)dynamic_vector_get(word_list, i);
        /* j começa na próxima palavra */
        size_t j = i + 1;
        /* Encontra o fim da sequência de palavras idênticas */
        while (j < total_word_count && strcmp(current_word_string, *(char**)dynamic_vector_get(word_list, j)) == 0) {
            /* Libera a memória das cópias duplicadas da string, mantendo apenas a primeira (em i) */
            free(*(char**)dynamic_vector_get(word_list, j));
            *(char**)dynamic_vector_get(word_list, j) = NULL; /* Define como NULL após liberar */
            j++;
        }

        /* Cria um registro WordCount para a palavra única encontrada */
        WordCount current_record;
        /* Reutiliza o ponteiro da primeira ocorrência da string (já alocada) */
        current_record.word_string  = current_word_string;
        /* Calcula o número de ocorrências (diferença entre j e i) */
        size_t current_word_occurrences = j - i;
        /* Verifica se a contagem excede o limite de um int. Se exceder, limita a INT_MAX. */
        if (current_word_occurrences > INT_MAX) {
             fprintf(stderr, "Warning: Word count for '%s' (%zu) exceeds INT_MAX, capping at %d.\n", current_record.word_string, current_word_occurrences, INT_MAX);
             current_record.occurrences = INT_MAX;
        } else {
            /* Converte a contagem (size_t) para int */
            current_record.occurrences = (int)current_word_occurrences;
        }

        /* Adiciona o registro (palavra e contagem) ao vetor word_counts */
        dynamic_vector_push(word_counts, &current_record);
        /* Avança o índice i para a próxima palavra diferente (posição j) */
        i = j;
    }

    /* Libera a memória do vetor word_list (que continha ponteiros char**). */
    /* As strings duplicadas já foram liberadas no loop anterior. As strings únicas */
    /* agora pertencem aos registros WordCount e serão liberadas mais tarde. */
    dynamic_vector_free(word_list);
    word_list = NULL; /* Define o ponteiro como NULL após liberar */

    /* Ordena o vetor de contagens (WordCount) usando Insertion Sort. */
    /* Critério: contagem decrescente, depois palavra crescente (definido em compare_word_counts). */
    /* Insertion Sort foi escolhido aqui, talvez por simplicidade ou por esperar que */
    /* o número de palavras únicas não seja excessivamente grande. */
    dynamic_vector_insertionsort(word_counts, compare_word_counts);

    /* Determina o número de resultados a serem impressos. */
    /* É o mínimo entre N (solicitado) e o número total de palavras únicas encontradas. */
    size_t unique_word_count = dynamic_vector_length(word_counts);
    size_t output_limit;
    /* Se N for negativo, imprime todos os resultados */
    if (num_results_requested < 0) {
        output_limit = unique_word_count;
    } else {
        /* Caso contrário, limita a N ou ao total de palavras únicas, o que for menor */
        output_limit = ((size_t)num_results_requested < unique_word_count) ? (size_t)num_results_requested : unique_word_count;
    }

    /* Imprime os N resultados mais frequentes (ou todos, se N < 0 ou N >= total) */
    int output_was_generated = 0; /* Flag para verificar se algo foi impresso */
    for (size_t i = 0; i < output_limit; i++) {
        /* Obtém o ponteiro para o registro WordCount no índice i */
        WordCount *current_entry = dynamic_vector_get(word_counts, i);
        /* Verifica se a entrada e a string são válidas */
        if (current_entry && current_entry->word_string) {
             /* Imprime a contagem e a palavra no formato "<contagem> <palavra>" */
             /* %4d formata a contagem com pelo menos 4 espaços */
             printf("%4d %s", current_entry->occurrences, current_entry->word_string);
             /* Adiciona uma nova linha, exceto após o último resultado */
             if (i < output_limit - 1) { printf("\n"); }
             output_was_generated = 1; /* Marca que pelo menos uma linha foi impressa */
        }
        /* Libera a memória da string da palavra após a impressão. */
        /* A string foi originalmente alocada com strdup e reutilizada. */
        free(current_entry->word_string);
        current_entry->word_string = NULL; /* Define como NULL após liberar */
    }

    /* Adiciona uma linha em branco no final da saída se algum resultado foi gerado,
       embora esta linha esteja comentada no código original. */
    if (output_was_generated) {
       // printf("\n");
    }

    /* Libera a memória do vetor de contagens (word_counts). */
    /* As strings dentro dele já foram liberadas no loop de impressão. */
    dynamic_vector_free(word_counts);
    word_counts = NULL; /* Define como NULL após liberar */

    /* Verifica se ocorreram erros durante a escrita para stdout (ex: pipe quebrado) */
    if (ferror(stdout)) {
        perror("main: error writing to stdout");
        /* Mesmo em caso de erro de escrita, a memória já foi liberada. */
        return EXIT_FAILURE;
    }

    /* Retorna sucesso se tudo correu bem */
    return EXIT_SUCCESS;
}

