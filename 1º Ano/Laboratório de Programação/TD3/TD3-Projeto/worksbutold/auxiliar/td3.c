#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
#define MAXTXT 4096
#define MAXCHOICE 10
 
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
        nOpcoes > MAXCHOICE) {
        return NULL;
    }
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
        c->escolhas[i] = -1;
    }
    return c;
}
 
/* Cria um nó do grafo que envolve uma cena com nVizinhos vizinhos */
No *criaNo(Cena *cena, int nVizinhos) {
    No *no = malloc(sizeof(No));
    if (!no) return NULL;
    no->cena = cena;
    no->nVizinhos = nVizinhos;
    if (nVizinhos > 0) {
        no->vizinhos = malloc(nVizinhos * sizeof(No *));
        if (!no->vizinhos) { free(no); return NULL; }
        for (int i = 0; i < nVizinhos; ++i) {
            no->vizinhos[i] = NULL;
        }
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
    for (int i = 0; i < c->nOpcoes; ++i) {
        free(c->opcoesTexto[i]);
    }
    free(c);
}
 
/* Liberta toda a memória de um nó (não recursivo) */
void libertaNo(No *no) {
    if (!no) return;
    libertaCena(no->cena);
    free(no->vizinhos);
    free(no);
}
 
/* Adiciona o vizinho v na posição pos do vetor de vizinhos do nó */
void juntaVizinhoNo(No *no, int pos, No *v) {
    if (!no || pos < 0 || pos >= no->nVizinhos) return;
    no->vizinhos[pos] = v;
}
 
/* Devolve o nó vizinho que corresponde à escolha (ID de cena) dada, ou NULL se inválido */
No *proximoNo(No *no_ativo, int escolha) {
    if (!no_ativo || !no_ativo->cena) return NULL;
    Cena *c = no_ativo->cena;
    if (c->tipo != NORMAL) return NULL;
    for (int i = 0; i < c->nOpcoes; ++i) {
        if (c->escolhas[i] == escolha) {
            return no_ativo->vizinhos[i];
        }
    }
    return NULL;
}
 
/* Mostra na saída standard a descrição da cena no nó e as suas opções (se existirem) */
void mostraCenaNo(No *no) {
    if (!no || !no->cena) return;
    Cena *c = no->cena;
    // Imprime o texto descritivo da cena
    if (c->descricao) {
        printf("%s", c->descricao);
        if (c->descricao[0] && c->descricao[strlen(c->descricao)-1] != '\n') {
            printf("\n");
        }
    }
    // Se for cena normal, listar as opções disponíveis
    if (c->tipo == NORMAL) {
        for (int i = 0; i < c->nOpcoes; ++i) {
            // Formato: "+ X. <texto da opção>"
            int opcaoId = c->escolhas[i];
            char *opTexto = c->opcoesTexto[i];
            if (opTexto) {
                printf("+ %d. %s\n", opcaoId, opTexto);
            }
        }
    }
}
 
/* Devolve o estatuto (tipo) da cena contida no nó */
TipoCena estadoCenaNo(No *no) {
    if (!no || !no->cena) return FAILED;  // por precaução, assumimos erro como FAILED
    return no->cena->tipo;
}
 
/* Lê uma escolha do utilizador (ID da próxima cena a seguir) */
int escolheCenaNo(No *no) {
    (void) no;  // parâmetro não utilizado nesta implementação
    int escolha;
    if (scanf("%d", &escolha) != 1) {
        return -1;  // retorna -1 se não houve escolha (EOF)
    }
    return escolha;
}
 
int main(void) {
    int nCenas;
    if (scanf("%d", &nCenas) != 1) {
        return 0;
    }
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
                char *p = line + 1, buf[32];
                int bi = 0;
                while (*p && *p != ']' && bi < (int)sizeof(buf) - 1) {
                    if (isdigit((unsigned char)*p)) {
                        buf[bi++] = *p;
                    }
                    p++;
                }
                buf[bi] = '\0';
                sceneId = atoi(buf);
                break;
            }
        }
        if (sceneId < 0 || sceneId >= nCenas) {
            sceneId = i;
        }
 
        // lê linha "<<<"
        while (fgets(line, sizeof(line), stdin)) {
            if (strncmp(line, "<<<", 3) == 0) break;
        }
 
        // acumula descrição até encontrar ">>>"
        char *descBuf = malloc(1);
        descBuf[0] = '\0';
        size_t descLen = 0;
        TipoCena tipo = NORMAL;
        int nOp = 0;
 
        while (fgets(line, sizeof(line), stdin)) {
            if (strncmp(line, ">>>", 3) == 0) {
                // após ">>>" vem <X> ou <WON> ou <FAILED>
                char *p = line + 3, tag[16];
                int ti = 0;
                while (*p && !isalnum((unsigned char)*p)) p++;
                while (*p && isalnum((unsigned char)*p) && ti < (int)sizeof(tag) - 1) {
                    tag[ti++] = *p++;
                }
                tag[ti] = '\0';
                int allDigits = 1;
                for (int k = 0; k < ti; ++k) {
                    if (!isdigit((unsigned char)tag[k])) {
                        allDigits = 0;
                        break;
                    }
                }
                if (allDigits) {
                    tipo = NORMAL;
                    nOp = atoi(tag);
                } else if (strcmp(tag, "WON") == 0) {
                    tipo = WON;
                    nOp = 0;
                } else if (strcmp(tag, "FAILED") == 0) {
                    tipo = FAILED;
                    nOp = 0;
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
 
        // cria cena e nó correspondentes
        Cena *c = criaCena(sceneId, descBuf, tipo, nOp);
        free(descBuf);
        No *no = criaNo(c, nOp);
        nodes[sceneId] = no;
 
        // lê exatamente nOp opções (ignora linhas que não começam em '+')
        int parsed = 0;
        while (parsed < nOp && fgets(line, sizeof(line), stdin)) {
            char *p = line;
            while (*p && isspace((unsigned char)*p)) p++;
            if (*p != '+') continue;  // salta separadores "***" ou linhas vazias
 
            // formato da linha: "+ X. texto..."
            p++;
            while (*p && !isdigit((unsigned char)*p)) p++;
            char numbuf[16];
            int bi = 0;
            while (*p && isdigit((unsigned char)*p) && bi < (int)sizeof(numbuf) - 1) {
                numbuf[bi++] = *p++;
            }
            numbuf[bi] = '\0';
            int dest = atoi(numbuf);
 
            // avança para o texto após o ponto "."
            while (*p && *p != '.') p++;
            if (*p == '.') p++;
            while (*p && isspace((unsigned char)*p)) p++;
 
            // copia o texto da opção (removendo o '\n' final, se existir)
            size_t tlen = strlen(p);
            if (tlen && p[tlen - 1] == '\n') {
                p[--tlen] = '\0';
            }
            char *optTxt = malloc(tlen + 1);
            strcpy(optTxt, p);
 
            c->escolhas[parsed] = dest;
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
            No *viz = (dest >= 0 && dest < nCenas) ? nodes[dest] : NULL;
            juntaVizinhoNo(no, j, viz);
        }
    }
 
    // cria história e simula escolhas
    Historia *hist = criaHistoria(nodes[0]);
    int outcome = 0;  // 1 = WON, 2 = FAILED, 3 = WAITING
 
    // verifica se a cena inicial já é final
    if (estadoCenaNo(hist->ativo) == WON) outcome = 1;
    if (estadoCenaNo(hist->ativo) == FAILED) outcome = 2;
 
    int escolha;
    while (outcome == 0) {
        escolha = escolheCenaNo(hist->ativo);
        if (escolha == -1) break;
        if (estadoCenaNo(hist->ativo) != NORMAL) break;
        No *next = proximoNo(hist->ativo, escolha);
        if (!next) {
            outcome = 3;
            break;
        }
        hist->ativo = next;
        TipoCena novoEstado = estadoCenaNo(hist->ativo);
        if (novoEstado == WON) {
            outcome = 1;
            break;
        }
        if (novoEstado == FAILED) {
            outcome = 2;
            break;
        }
        /* 
         * (Opcionalmente, poderíamos chamar mostraCenaNo(hist->ativo) aqui 
         * para imprimir a nova cena, caso quiséssemos exibir a história passo a passo.)
         */
    }
 
    if (outcome == 0) {
        outcome = 3;  // sem escolhas suficientes -> WAITING
    }
 
    // imprime apenas o resultado final
    if (outcome == 1) {
        printf("WON\n");
    } else if (outcome == 2) {
        printf("FAILED\n");
    } else {
        printf("WAITING\n");
    }
 
    // liberta memória antes de terminar
    for (int i = 0; i < nCenas; ++i) {
        libertaNo(nodes[i]);
    }
    free(nodes);
    free(hist);
    return 0;
}
