#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXTXT      4096
#define MAXCHOICE     10

typedef enum { NORMAL, WON, FAILED } TipoCena;

typedef struct {
    int id;
    char *descricao;
    TipoCena tipo;
    int nOpcoes;
    char *opcoesTexto[MAXCHOICE];
    int escolhas[MAXCHOICE];    // IDs das cenas de destino
} Cena;

typedef struct No {
    Cena *cena;
    int nVizinhos;
    struct No **vizinhos;
} No;

typedef struct {
    No *inicio;
    No *ativo;
} Historia;

/* Cria uma cena com descrição, tipo e número de opções */
Cena *criaCena(int id, const char *desc, TipoCena tipo, int nOpcoes) {
    if ((tipo == NORMAL && nOpcoes <= 0) ||
        ((tipo == WON || tipo == FAILED) && nOpcoes != 0) ||
        nOpcoes > MAXCHOICE)
        return NULL;
    Cena *c = malloc(sizeof(Cena));
    if (!c) return NULL;
    c->id = id;
    c->tipo = tipo;
    c->nOpcoes = nOpcoes;
    if (desc) {
        c->descricao = malloc(strlen(desc) + 1);
        if (!c->descricao) { free(c); return NULL; }
        strcpy(c->descricao, desc);
    } else {
        c->descricao = NULL;
    }
    for (int i = 0; i < MAXCHOICE; ++i) {
        c->opcoesTexto[i] = NULL;
        c->escolhas[i]    = -1;
    }
    return c;
}

/* Cria um nó do grafo que envolve uma cena com nVizinhos vizinhos */
No *criaNo(Cena *cena, int nVizinhos) {
    No *no = malloc(sizeof(No));
    if (!no) return NULL;
    no->cena     = cena;
    no->nVizinhos = nVizinhos;
    if (nVizinhos > 0) {
        no->vizinhos = malloc(nVizinhos * sizeof(No *));
        if (!no->vizinhos) { free(no); return NULL; }
        for (int i = 0; i < nVizinhos; ++i)
            no->vizinhos[i] = NULL;
    } else {
        no->vizinhos = NULL;
    }
    return no;
}

/* Cria a estrutura História apontando início e ativo para noInicio */
Historia *criaHistoria(No *noInicio) {
    Historia *h = malloc(sizeof(Historia));
    if (!h) return NULL;
    h->inicio = noInicio;
    h->ativo  = noInicio;
    return h;
}

/* Liberta toda a memória de uma cena */
void libertaCena(Cena *c) {
    if (!c) return;
    free(c->descricao);
    for (int i = 0; i < c->nOpcoes; ++i)
        free(c->opcoesTexto[i]);
    free(c);
}

/* Liberta toda a memória de um nó (não recursivo) */
void libertaNo(No *no) {
    if (!no) return;
    libertaCena(no->cena);
    free(no->vizinhos);
    free(no);
}

int main(void) {
    int nCenas;
    if (scanf("%d", &nCenas) != 1)
        return 0;
    // consome resto da linha
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    // vetor de nós indexado por ID de cena
    No **nodes = calloc(nCenas, sizeof(No *));
    if (!nodes) return 1;

    char line[MAXTXT + 16];
    for (int i = 0; i < nCenas; ++i) {
        // lê identificador "[id]" (ignora linhas em branco)
        int sceneId = -1;
        while (fgets(line, sizeof(line), stdin)) {
            if (line[0] == '[') {
                // extrai dígitos entre [ e ]
                char *p = line + 1, buf[32]; int bi = 0;
                while (*p && *p != ']' && bi < (int)sizeof(buf)-1) {
                    if (isdigit((unsigned char)*p))
                        buf[bi++] = *p;
                    p++;
                }
                buf[bi] = '\0';
                sceneId = atoi(buf);
                break;
            }
        }
        if (sceneId < 0 || sceneId >= nCenas)
            sceneId = i;

        // lê linha "<<<"
        while (fgets(line, sizeof(line), stdin))
            if (strncmp(line, "<<<", 3) == 0) break;

        // acumula descrição até encontrar ">>>"
        char *descBuf = malloc(1);
        size_t descLen = 0;
        TipoCena tipo = NORMAL;
        int nOp = 0;

        while (fgets(line, sizeof(line), stdin)) {
            if (strncmp(line, ">>>", 3) == 0) {
                // após ">>>" vem <X> ou <WON> ou <FAILED>
                char *p = line + 3, tag[16]; int ti = 0;
                while (*p && !isalnum((unsigned char)*p)) p++;
                while (*p && isalnum((unsigned char)*p) && ti < (int)sizeof(tag)-1)
                    tag[ti++] = *p++;
                tag[ti] = '\0';
                int allDigits = 1;
                for (int k = 0; k < ti; ++k)
                    if (!isdigit((unsigned char)tag[k])) { allDigits = 0; break; }
                if (allDigits) {
                    tipo = NORMAL;
                    nOp  = atoi(tag);
                } else if (strcmp(tag, "WON") == 0) {
                    tipo = WON;   nOp = 0;
                } else if (strcmp(tag, "FAILED") == 0) {
                    tipo = FAILED; nOp = 0;
                }
                break;
            }
            // concatena linha à descrição
            size_t len = strlen(line);
            descBuf = realloc(descBuf, descLen + len + 1);
            memcpy(descBuf + descLen, line, len);
            descLen += len;
            descBuf[descLen] = '\0';
        }

        // cria cena e nó
        Cena *c = criaCena(sceneId, descBuf, tipo, nOp);
        free(descBuf);
        No   *no = criaNo(c, nOp);
        nodes[sceneId] = no;

        // lê exatamente nOp opções, pulando quaisquer linhas que não comecem com '+'
        int parsed = 0;
        while (parsed < nOp && fgets(line, sizeof(line), stdin)) {
            char *p = line;
            while (*p && isspace((unsigned char)*p)) p++;
            if (*p != '+') continue;  // pula separadores "***" ou linhas vazias

            // form: "+ X. texto..."
            p++;
            while (*p && !isdigit((unsigned char)*p)) p++;
            char numbuf[16]; int bi = 0;
            while (*p && isdigit((unsigned char)*p) && bi < (int)sizeof(numbuf)-1)
                numbuf[bi++] = *p++;
            numbuf[bi] = '\0';
            int dest = atoi(numbuf);

            // avança até ao texto após ponto "."
            while (*p && *p != '.') p++;
            if (*p == '.') p++;
            while (*p && isspace((unsigned char)*p)) p++;

            // copia texto da opção (sem '\n' final)
            size_t tlen = strlen(p);
            if (tlen && p[tlen-1] == '\n') p[--tlen] = '\0';
            char *optTxt = malloc(tlen + 1);
            strcpy(optTxt, p);

            c->escolhas[parsed]    = dest;
            c->opcoesTexto[parsed] = optTxt;
            parsed++;
        }
    }

    // liga vizinhos no grafo conforme escolhas
    for (int i = 0; i < nCenas; ++i) {
        No *no = nodes[i];
        if (!no) continue;
        for (int j = 0; j < no->nVizinhos; ++j) {
            int dest = no->cena->escolhas[j];
            no->vizinhos[j] = (dest >= 0 && dest < nCenas) ? nodes[dest] : NULL;
        }
    }

    // cria história e simula escolhas
    Historia *hist = criaHistoria(nodes[0]);
    int outcome = 0;  // 1=WON, 2=FAILED, 3=WAITING

    // checa se cena inicial já é final
    if (hist->ativo->cena->tipo == WON)   outcome = 1;
    if (hist->ativo->cena->tipo == FAILED) outcome = 2;

    int escolha;
    while (outcome == 0 && scanf("%d", &escolha) == 1) {
        Cena *cc = hist->ativo->cena;
        if (cc->tipo != NORMAL) break;  // cena final atingida, ignora resto

        // procura transição válida cujo ID de destino == escolha
        No *next = NULL;
        for (int k = 0; k < cc->nOpcoes; ++k) {
            if (cc->escolhas[k] == escolha) {
                next = hist->ativo->vizinhos[k];
                break;
            }
        }
        if (!next) {
            outcome = 3;  // escolha inválida → WAITING
            break;
        }
        hist->ativo = next;
        if (next->cena->tipo == WON)   { outcome = 1; break; }
        if (next->cena->tipo == FAILED) { outcome = 2; break; }
    }

    if (outcome == 0)
        outcome = 3;  // sem escolhas suficientes → WAITING

    // imprime apenas o resultado final
    if (outcome == 1)       printf("WON\n");
    else if (outcome == 2)  printf("FAILED\n");
    else                    printf("WAITING\n");

    // liberta memória
    for (int i = 0; i < nCenas; ++i)
        libertaNo(nodes[i]);
    free(nodes);
    free(hist);
    return 0;
}
