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
    // Verificações de consistência nos parâmetros da cena
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
    // Aloca e copia a descrição da cena
    if (desc) {
        c->descricao = malloc(strlen(desc) + 1);
        if (!c->descricao) {
            free(c);
            return NULL;
        }
        strcpy(c->descricao, desc);
    } else {
        c->descricao = NULL;
    }
    // Inicializa as opções e escolhas com valores padrão
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
        if (!no->vizinhos) {
            free(no);
            return NULL;
        }
        // Inicializa todos os vizinhos com NULL
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

/* Coloca o vizinho v na posição pos do vetor de vizinhos do nó no */
void juntaVizinhoNo(No *no, int pos, No *v) {
    if (!no || pos < 0 || pos >= no->nVizinhos) return;
    no->vizinhos[pos] = v;
}

/* Devolve o nó vizinho correspondente à escolha 'escolha' a partir do no_ativo.
   Em caso de escolha inválida, devolve NULL para sinalizar erro. */
No *proximoNo(No *no_ativo, int escolha) {
    if (!no_ativo) return NULL;
    Cena *c = no_ativo->cena;
    if (!c || c->nOpcoes <= 0) {
        // Se não houver opções (cena final), não há próximo
        return NULL;
    }
    // Procura entre as opções da cena ativa um ID que corresponda à escolha
    for (int i = 0; i < c->nOpcoes; ++i) {
        if (c->escolhas[i] == escolha) {
            return no_ativo->vizinhos[i];
        }
    }
    // Se não encontrou nenhuma opção com o ID escolhido, retorna NULL
    return NULL;
}

/* Mostra a descrição da cena no e as opções disponíveis (se houver) */
void mostraCenaNo(No *no) {
    if (!no || !no->cena) return;
    // Imprime a descrição da cena
    if (no->cena->descricao) {
        printf("%s\n", no->cena->descricao);
    }
    // Se for uma cena NORMAL, lista as opções de escolha disponíveis
    if (no->cena->tipo == NORMAL) {
        for (int i = 0; i < no->cena->nOpcoes; ++i) {
            int dest = no->cena->escolhas[i];
            char *texto = no->cena->opcoesTexto[i];
            if (texto) {
                printf("%d. %s\n", dest, texto);
            }
        }
    }
}

/* Retorna o tipo da cena contida no nó */
TipoCena estadoCenaNo(No *no) {
    if (!no || !no->cena) return NORMAL;
    return no->cena->tipo;
}

/* Lê uma opção de cena escolhida pelo utilizador (input) */
int escolheCenaNo(No *no) {
    (void) no;  // neste contexto, não precisamos usar 'no' para ler a escolha
    int escolha;
    if (scanf("%d", &escolha) != 1) {
        return -1;  // retorno -1 em caso de erro de leitura
    }
    return escolha;
}

/* Lê do input todas as cenas, cria os nós e devolve um vetor de nós */
No **leCenas(int nCenas) {
    // Vetor de nós indexado pelo ID de cada cena
    No **nodes = calloc(nCenas, sizeof(No *));
    if (!nodes) return NULL;
    char line[MAXTXT + 16];
    // Para cada cena, lê do input sua definição
    for (int i = 0; i < nCenas; ++i) {
        // Lê o identificador da cena (entre '[' e ']'), ignorando linhas em branco
        int sceneId = -1;
        while (fgets(line, sizeof(line), stdin)) {
            if (line[0] == '[') {
                // Extrai dígitos do identificador entre '[' e ']'
                char *p = line + 1;
                char buf[32];
                int bi = 0;
                while (*p && *p != ']' && bi < (int)sizeof(buf) - 1) {
                    if (isdigit((unsigned char)*p)) {
                        buf[bi++] = *p;
                    }
                    p++;
                }
                buf[bi] = '\0';
                sceneId = atoi(buf);
                break;  // identificador encontrado, sai do loop
            }
        }
        // Se o ID lido for inválido ou fora do intervalo, usa o índice atual 'i'
        if (sceneId < 0 || sceneId >= nCenas) {
            sceneId = i;
        }
        // Lê até encontrar a marca de início de descrição "<<<"
        while (fgets(line, sizeof(line), stdin)) {
            if (strncmp(line, "<<<", 3) == 0) break;
        }
        // Lê a descrição da cena, acumulando linhas até encontrar ">>>"
        char *descBuf = malloc(1);
        if (!descBuf) {
            // Falha na alocação da descrição; libera memória e retorna NULL
            for (int k = 0; k <= i; ++k) {
                libertaNo(nodes[k]);
            }
            free(nodes);
            return NULL;
        }
        descBuf[0] = '\0';
        size_t descLen = 0;
        TipoCena tipo = NORMAL;
        int nOp = 0;
        while (fgets(line, sizeof(line), stdin)) {
            if (strncmp(line, ">>>", 3) == 0) {
                // Após ">>>" vem um indicador: número de opções <X> ou <WON> ou <FAILED>
                char *p = line + 3;
                char tag[16];
                int ti = 0;
                // Ignora caracteres não alfanuméricos iniciais
                while (*p && !isalnum((unsigned char)*p)) p++;
                // Copia caracteres alfanuméricos para 'tag'
                while (*p && isalnum((unsigned char)*p) && ti < (int)sizeof(tag) - 1) {
                    tag[ti++] = *p++;
                }
                tag[ti] = '\0';
                // Verifica se tag é um número (só dígitos)
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
                break;  // sai do loop de descrição
            }
            // Concatena a linha atual à descrição (mantém quebras de linha)
            size_t len = strlen(line);
            char *newBuf = realloc(descBuf, descLen + len + 1);
            if (!newBuf) {
                // Falha na realocação; libera memória e retorna NULL
                free(descBuf);
                for (int k = 0; k <= i; ++k) {
                    libertaNo(nodes[k]);
                }
                free(nodes);
                return NULL;
            }
            descBuf = newBuf;
            memcpy(descBuf + descLen, line, len);
            descLen += len;
            descBuf[descLen] = '\0';
        }
        // Cria a cena e o nó correspondente
        Cena *c = criaCena(sceneId, descBuf, tipo, nOp);
        free(descBuf);  // descBuf já copiado para c->descricao, podemos liberar
        No *no = NULL;
        if (c) {
            no = criaNo(c, nOp);
        }
        nodes[sceneId] = no;
        // Lê as opções da cena (exatamente nOp linhas começando com '+')
        int parsed = 0;
        while (parsed < nOp && fgets(line, sizeof(line), stdin)) {
            // Ignora linhas que não começam com '+'
            char *p = line;
            while (*p && isspace((unsigned char)*p)) p++;
            if (*p != '+') {
                continue;
            }
            // Formato: "+ X. texto da opção"
            p++;  // avança o '+'
            // Extrai o número (ID de destino) após '+'
            while (*p && !isdigit((unsigned char)*p)) p++;
            char numbuf[16];
            int bi = 0;
            while (*p && isdigit((unsigned char)*p) && bi < (int)sizeof(numbuf) - 1) {
                numbuf[bi++] = *p++;
            }
            numbuf[bi] = '\0';
            int dest = atoi(numbuf);
            // Avança até o texto da opção após o ponto '.'
            while (*p && *p != '.') p++;
            if (*p == '.') p++;
            while (*p && isspace((unsigned char)*p)) p++;
            // Copia o texto da opção (removendo o '\n' final, se houver)
            size_t tlen = strlen(p);
            if (tlen > 0 && p[tlen - 1] == '\n') {
                p[--tlen] = '\0';
            }
            char *optTxt = malloc(tlen + 1);
            if (!optTxt) {
                // Falha na alocação do texto da opção; libera memória e retorna NULL
                if (no && no->cena) {
                    no->cena->nOpcoes = parsed;  // ajusta nOpcoes para liberar corretamente
                }
                libertaNo(no);
                for (int k = 0; k < nCenas; ++k) {
                    if (k != sceneId) {
                        libertaNo(nodes[k]);
                    }
                }
                free(nodes);
                return NULL;
            }
            strcpy(optTxt, p);
            // Armazena a opção na cena
            if (c) {
                c->escolhas[parsed] = dest;
                c->opcoesTexto[parsed] = optTxt;
            }
            parsed++;
        }
    }
    return nodes;
}

/* Liga os nós do vetor conforme as escolhas de cada cena, construindo o grafo */
void montaGrafo(No **nodes, int nCenas) {
    for (int i = 0; i < nCenas; ++i) {
        No *no = nodes[i];
        if (!no) continue;
        // Para cada opção da cena, define o vizinho (próximo nó) correspondente
        for (int j = 0; j < no->nVizinhos; ++j) {
            int dest = no->cena->escolhas[j];
            if (dest >= 0 && dest < nCenas) {
                juntaVizinhoNo(no, j, nodes[dest]);
            } else {
                juntaVizinhoNo(no, j, NULL);
            }
        }
    }
}

/* Executa a simulação da história com base nas escolhas do utilizador.
   Retorna 1 se o jogador venceu (WON), 2 se falhou (FAILED) ou 3 se a história ficou em espera (WAITING). */
int simulaHistoria(Historia *hist) {
    if (!hist || !hist->ativo) {
        return 3;
    }
    int outcome = 0;  // 1=WON, 2=FAILED, 3=WAITING
    // Verifica se a cena inicial já é final (WON ou FAILED)
    if (estadoCenaNo(hist->ativo) == WON) {
        outcome = 1;
    } else if (estadoCenaNo(hist->ativo) == FAILED) {
        outcome = 2;
    }
    int escolha;
    // Processa as escolhas enquanto não houver desfecho e houver input disponível
    while (outcome == 0 && scanf("%d", &escolha) == 1) {
        // Se a cena atual já for final, interrompe (ignora entradas extra)
        if (estadoCenaNo(hist->ativo) != NORMAL) {
            break;
        }
        // Obtém o próximo nó de acordo com a escolha do utilizador
        No *next = proximoNo(hist->ativo, escolha);
        if (!next) {
            // Escolha inválida → história fica em espera (WAITING)
            outcome = 3;
            break;
        }
        // Avança para o próximo nó (cena escolhida)
        hist->ativo = next;
        // Verifica se a nova cena é um fim de jogo (WON ou FAILED)
        if (estadoCenaNo(hist->ativo) == WON) {
            outcome = 1;
            break;
        }
        if (estadoCenaNo(hist->ativo) == FAILED) {
            outcome = 2;
            break;
        }
    }
    // Se consumiu todas as entradas sem chegar a um fim, resultado é WAITING
    if (outcome == 0) {
        outcome = 3;
    }
    return outcome;
}

int main(void) {
    int nCenas;
    if (scanf("%d", &nCenas) != 1) {
        return 0;  // termina se não conseguir ler o número de cenas
    }
    // Consome o resto da linha após o número de cenas
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
    // Lê todas as cenas e constrói o vetor de nós
    No **nodes = leCenas(nCenas);
    if (!nodes) {
        // Falha ao ler cenas ou alocar memória; encerra com erro
        return 1;
    }
    // Monta o grafo de cenas ligando os nós conforme as escolhas
    montaGrafo(nodes, nCenas);
    // Cria a história a partir da cena inicial (ID 0)
    Historia *hist = criaHistoria(nodes[0]);
    if (!hist) {
        // Falha na criação da história; libera memória e sai com erro
        for (int i = 0; i < nCenas; ++i) {
            libertaNo(nodes[i]);
        }
        free(nodes);
        return 1;
    }
    // Simula a história com base nas escolhas do input e determina o desfecho
    int outcome = simulaHistoria(hist);
    // Imprime apenas o resultado final (WON, FAILED ou WAITING)
    if (outcome == 1) {
        printf("WON\n");
    } else if (outcome == 2) {
        printf("FAILED\n");
    } else {
        printf("WAITING\n");
    }
    // Liberta a memória alocada para a história e todos os nós/cenas
    for (int i = 0; i < nCenas; ++i) {
        libertaNo(nodes[i]);
    }
    free(nodes);
    free(hist);
    return 0;
}
