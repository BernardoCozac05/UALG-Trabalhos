// Importação de bibliotecas padrão necessárias
// stdio.h para funções de entrada/saída
#include <stdio.h>
// stdlib.h para alocação de memória e utilitários gerais
#include <stdlib.h>
// string.h para manipulação de strings
#include <string.h>
// ctype.h para classificações de caracteres (isdigit, isspace, etc.)
#include <ctype.h>

// Definições de constantes para limites de buffer e número máximo de escolhas
#define MAXTXT 4096     // Tamanho máximo de linha de texto
#define MAXCHOICE 10    // Número máximo de opções por cena

// Enumeração para representar o tipo de cada cena
// NORMAL: cena padrão com opções
// WON: cena de vitória (final positivo)
// FAILED: cena de falha (final negativo)
typedef enum { NORMAL, WON, FAILED } TipoCena;

// Estrutura que representa uma cena no jogo
// id: identificador único da cena
// descricao: texto descritivo da cena
// tipo: tipo da cena (NORMAL, WON, FAILED)
// nOpcoes: número de escolhas disponíveis
// opcoesTexto: textos das opções apresentadas ao jogador
// escolhas: IDs das cenas de destino correspondentes a cada escolha
typedef struct {
    int id;
    char *descricao;
    TipoCena tipo;
    int nOpcoes;
    char *opcoesTexto[MAXCHOICE];
    int escolhas[MAXCHOICE];
} Cena;

// Nó do grafo de cenas, ligando uma Cena a seus vizinhos
// cena: ponteiro para a estrutura Cena
// nVizinhos: número de conexões (opções)
// vizinhos: array de ponteiros para nós destino
typedef struct No {
    Cena *cena;
    int nVizinhos;
    struct No **vizinhos;
} No;

// Estrutura para controlar o progresso da história
// inicio: nó inicial da história
// ativo: nó atualmente ativo (onde o jogador está)
typedef struct {
    No *inicio;
    No *ativo;
} Historia;

// Função: criaCena
// Cria e inicializa dinamicamente uma Cena
// id: identificador da cena
// desc: texto descritivo (pode ser NULL)
// tipo: tipo da cena (NORMAL, WON, FAILED)
// nOpcoes: número de escolhas que a cena terá
// Retorna: ponteiro para Cena alocada ou NULL em caso de erro
Cena *criaCena(int id, const char *desc, TipoCena tipo, int nOpcoes) {
    // Validação de parâmetros: cenas NORMAL devem ter >0 opções,
    // finais (WON/FAILED) devem ter 0 opções, e nunca exceder MAXCHOICE
    if ((tipo == NORMAL && nOpcoes <= 0) ||
        ((tipo == WON || tipo == FAILED) && nOpcoes != 0) ||
        nOpcoes > MAXCHOICE) {
        return NULL;
    }

    // Aloca espaço para estrutura Cena
    Cena *c = malloc(sizeof(Cena));
    if (!c) return NULL;

    c->id = id;
    c->tipo = tipo;
    c->nOpcoes = nOpcoes;

    // Se houver descrição, aloca e copia a string
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

    // Inicializa vetores de opções e escolhas com valores padrão
    for (int i = 0; i < MAXCHOICE; ++i) {
        c->opcoesTexto[i] = NULL;
        c->escolhas[i] = -1;
    }

    return c;
}

// Função: criaNo
// Cria um nó de grafo que envolve uma Cena e reserva espaço para vizinhos
// cena: ponteiro para Cena já criada
// nVizinhos: número de conexões que esse nó terá (igual a nOpcoes da cena)
// Retorna: ponteiro para No alocado ou NULL em caso de erro
No *criaNo(Cena *cena, int nVizinhos) {
    // Aloca estrutura No
    No *no = malloc(sizeof(No));
    if (!no) return NULL;

    no->cena = cena;
    no->nVizinhos = nVizinhos;

    // Se houver vizinhos, aloca array de ponteiros para nós
    if (nVizinhos > 0) {
        no->vizinhos = malloc(nVizinhos * sizeof(No *));
        if (!no->vizinhos) {
            free(no);
            return NULL;
        }
        // Inicializa cada ponteiro de vizinho como NULL
        for (int i = 0; i < nVizinhos; ++i) {
            no->vizinhos[i] = NULL;
        }
    } else {
        no->vizinhos = NULL;
    }

    return no;
}

// Função: criaHistoria
// Inicializa a estrutura Historia apontando início e ativo para um nó inicial
// noInicio: nó que representa a cena inicial da história
// Retorna: ponteiro para Historia ou NULL em caso de erro
Historia *criaHistoria(No *noInicio) {
    Historia *h = malloc(sizeof(Historia));
    if (!h) return NULL;
    h->inicio = noInicio;
    h->ativo  = noInicio;
    return h;
}

// Função: libertaCena
// Libera a memória de uma Cena, inclusive descrição e textos de opções
// c: ponteiro para Cena a ser liberada
void libertaCena(Cena *c) {
    if (!c) return;
    free(c->descricao);
    for (int i = 0; i < c->nOpcoes; ++i) {
        free(c->opcoesTexto[i]);
    }
    free(c);
}

// Mostra as cenas escolhidas

void mostrarCena(Cena *c) {
    if (!c) return;

    // Imprime a descrição da cena
    if (c->descricao) {
        printf("%s\n", c->descricao);
    }

    // Se for uma cena do tipo NORMAL, imprime as opções no formato pedido
    if (c->tipo == NORMAL) {
        for (int i = 0; i < c->nOpcoes; ++i) {
            int destino = c->escolhas[i];
            char *texto = c->opcoesTexto[i];
            if (texto) {
                printf("+ %d. %s\n", destino, texto);
            }
        }
    }
}

// Função: libertaNo
// Libera um No do grafo, chamando libertaCena e liberando o array de vizinhos
// no: ponteiro para No a ser liberado
void libertaNo(No *no) {
    if (!no) return;
    libertaCena(no->cena);
    free(no->vizinhos);
    free(no);
}

// Função: juntaVizinhoNo
// Associa um nó vizinho em uma posição específica do array de vizinhos
// no: nó de origem
// pos: índice onde inserir o vizinho (0 <= pos < nVizinhos)
// v: nó de destino a ser associado
void juntaVizinhoNo(No *no, int pos, No *v) {
    if (!no || pos < 0 || pos >= no->nVizinhos) return;
    no->vizinhos[pos] = v;
}

// Função: proximoNo
// A partir de um nó ativo e escolha do usuário, retorna o nó destino correspondente
// no_ativo: nó atual
// escolha: ID da cena escolhida
// Retorna: ponteiro para nó destino ou NULL se escolha inválida
No *proximoNo(No *no_ativo, int escolha) {
    if (!no_ativo) return NULL;
    Cena *c = no_ativo->cena;

    // Se não existem opções, não há próximo nó
    if (!c || c->nOpcoes <= 0) {
        return NULL;
    }

    // Busca na lista de escolhas um ID que corresponda à escolha do usuário
    for (int i = 0; i < c->nOpcoes; ++i) {
        if (c->escolhas[i] == escolha) {
            return no_ativo->vizinhos[i];
        }
    }

    // Se nenhuma escolha foi encontrada, retorna NULL
    return NULL;
}

// Função: mostraCenaNo
// Exibe no terminal a descrição da cena e as opções disponíveis (se houver)
// no: nó cuja cena será mostrada
void mostraCenaNo(No *no) {
    if (!no || !no->cena) return;

    // Exibe descrição da cena
    if (no->cena->descricao) {
        printf("%s\n", no->cena->descricao);
    }

    // Se for cena NORMAL, lista cada opção com seu ID e texto
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

// Função: estadoCenaNo
// Retorna o tipo de cena (NORMAL, WON, FAILED) de um nó
// no: nó a verificar
// Retorna: TipoCena correspondente
TipoCena estadoCenaNo(No *no) {
    if (!no || !no->cena) return NORMAL;
    return no->cena->tipo;
}

// Função: escolheCenaNo
// Lê do stdin a escolha (inteiro) feita pelo usuário
// no: nó atual (não utilizado diretamente aqui)
// Retorna: valor inteiro lido ou -1 em caso de erro
int escolheCenaNo(No *no) {
    (void) no;  // Parâmetro não utilizado
    int escolha;
    if (scanf("%d", &escolha) != 1) {
        return -1;
    }
    return escolha;
}

// Função: leCenas
// Lê do stdin todas as cenas definidas pelo usuário, parseando descrições e opções
// nCenas: número total de cenas a ler
// Retorna: array de ponteiros para No criados (tamanho nCenas) ou NULL em caso de erro
No **leCenas(int nCenas) {
    // Cria array de ponteiros para armazenar nós indexados por ID de cena
    No **nodes = calloc(nCenas, sizeof(No *));
    if (!nodes) return NULL;

    char line[MAXTXT + 16];

    // Loop principal para ler cada cena
    for (int i = 0; i < nCenas; ++i) {
        int sceneId = -1;

        // Lê até encontrar linha iniciada com '[' para obter o ID da cena
        while (fgets(line, sizeof(line), stdin)) {
            if (line[0] == '[') {
                // Extrai dígitos entre '[' e ']'
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
                break;
            }
        }

        // Se ID inválido, usa índice i como fallback
        if (sceneId < 0 || sceneId >= nCenas) {
            sceneId = i;
        }

        // Avança leitura até marca de início de descrição "<<<"
        while (fgets(line, sizeof(line), stdin)) {
            if (strncmp(line, "<<<", 3) == 0) break;
        }

        // Prepara buffer dinâmico para acumular descrição
        char *descBuf = malloc(1);
        if (!descBuf) {
            // Em caso de falha, libera nós já criados e aborta
            for (int k = 0; k <= i; ++k) {
                libertaNo(nodes[k]);
            }
            free(nodes);
            return NULL;
        }
        descBuf[0] = '\0';
        size_t descLen = 0;

        // Variáveis para tipo e número de opções, definidas após descrição
        TipoCena tipo = NORMAL;
        int nOp = 0;

        // Lê linhas da descrição até encontrar marca de fim ">>>"
        while (fgets(line, sizeof(line), stdin)) {
            if (strncmp(line, ">>>", 3) == 0) {
                // Após ">>>" vem <nOp> ou <WON> ou <FAILED>
                char *p = line + 3;
                char tag[16];
                int ti = 0;
                // Ignora caracteres não alfanuméricos
                while (*p && !isalnum((unsigned char)*p)) p++;
                while (*p && isalnum((unsigned char)*p) && ti < (int)sizeof(tag) - 1) {
                    tag[ti++] = *p++;
                }
                tag[ti] = '\0';

                // Verifica se tag é numérica (NORMAL) ou WON/FAILED
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
                break;  // fim da leitura da descrição
            }
            // Acumula linha no buffer de descrição, realocando conforme necessário
            size_t len = strlen(line);
            char *newBuf = realloc(descBuf, descLen + len + 1);
            if (!newBuf) {
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

        // Cria Cena e Nó correspondentes
        Cena *c = criaCena(sceneId, descBuf, tipo, nOp);
        free(descBuf);
        No *no = NULL;
        if (c) {
            no = criaNo(c, nOp);
        }
        nodes[sceneId] = no;

        // Lê exatamente nOp linhas de opções iniciadas por '+'
        int parsed = 0;
        while (parsed < nOp && fgets(line, sizeof(line), stdin)) {
            char *p = line;
            while (*p && isspace((unsigned char)*p)) p++;
            if (*p != '+') continue;
            p++;
            // Extrai número de destino após '+'
            while (*p && !isdigit((unsigned char)*p)) p++;
            char numbuf[16]; int bi=0;
            while (*p && isdigit((unsigned char)*p) && bi < (int)sizeof(numbuf)-1) {
                numbuf[bi++]=*p++;
            }
            numbuf[bi]='\0';
            int dest = atoi(numbuf);
            // Avança até texto após ponto '.'
            while (*p && *p!='.') p++;
            if (*p=='.') p++;
            while (*p && isspace((unsigned char)*p)) p++;
            // Remove '\n' final se existir
            size_t tlen = strlen(p);
            if (tlen>0 && p[tlen-1]=='\n') p[--tlen]='\0';
            // Aloca e copia texto da opção
            char *optTxt = malloc(tlen+1);
            if (optTxt) {
                strcpy(optTxt, p);
                c->escolhas[parsed]    = dest;
                c->opcoesTexto[parsed] = optTxt;
                parsed++;
            } else {
                // Em caso de falha, ajusta nOpcoes para liberação correta
                no->cena->nOpcoes = parsed;
                libertaNo(no);
                for (int k=0;k<nCenas;++k) if(k!=sceneId) libertaNo(nodes[k]);
                free(nodes);
                return NULL;
            }
        }
    }

    return nodes;
}

// Função: montaGrafo
// Conecta cada nó aos seus vizinhos conforme as escolhas armazenadas em Cena
// nodes: array de nós
// nCenas: tamanho do array
void montaGrafo(No **nodes, int nCenas) {
    for (int i = 0; i < nCenas; ++i) {
        No *no = nodes[i];
        if (!no) continue;
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

// Função: simulaHistoria
// Executa a história lendo escolhas do usuário e movendo-se entre nós
// Retorna: 1 se WON, 2 se FAILED, 3 se WAITING por escolhas inválidas ou fim de input
int simulaHistoria(Historia *hist) {
    if (!hist || !hist->ativo) {
        return 3; // WAITING
    }
    int outcome = 0;  // 0=em andamento, 1=WON, 2=FAILED, 3=WAITING
    // Verifica se cena inicial já é final
    if (estadoCenaNo(hist->ativo) == WON) outcome = 1;
    else if (estadoCenaNo(hist->ativo) == FAILED) outcome = 2;

    int escolha;

    // Mostra a cena inicial
    //mostraCenaNo(hist->ativo);
    //mostrarCena(hist->ativo->cena);
    //RETIRAR COMENTARIO APENAS PARA MOSTRAR A CENA

    // Processa escolhas enquanto não houver desfecho
    while (outcome == 0 && scanf("%d", &escolha) == 1) {
        if (estadoCenaNo(hist->ativo) != NORMAL) break;
        No *next = proximoNo(hist->ativo, escolha);
        if (!next) {
            outcome = 3; // WAITING se escolha inválida
            break;
        }
        hist->ativo = next;

        // Mostra a nova cena após mudar de nó
        //mostraCenaNo(hist->ativo);
        //mostrarCena(hist->ativo->cena);
        //RETIRAR COMENTARIO APENAS PARA MOSTRAR A CENA

        if (estadoCenaNo(hist->ativo) == WON) { outcome = 1; break; }
        if (estadoCenaNo(hist->ativo) == FAILED) { outcome = 2; break; }
    }
    if (outcome == 0) outcome = 3;
    return outcome;
}

// Função principal: lê número de cenas, constrói grafo e executa simulação
int main(void) {
    int nCenas;
    // Lê a quantidade de cenas do input
    if (scanf("%d", &nCenas) != 1) {
        return 0;
    }
    // Descarta caracteres até o fim da linha atual
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }

    // Lê todas as cenas e obtém array de nós
    No **nodes = leCenas(nCenas);
    if (!nodes) {
        return 1;
    }

    // Conecta os nós conforme as escolhas
    montaGrafo(nodes, nCenas);

    // Cria e inicializa a história a partir do nó 0
    Historia *hist = criaHistoria(nodes[0]);
    if (!hist) {
        for (int i = 0; i < nCenas; ++i) {
            libertaNo(nodes[i]);
        }
        free(nodes);
        return 1;
    }

    // Executa simulação e captura resultado
    int outcome = simulaHistoria(hist);

    // Imprime resultado final: WON, FAILED ou WAITING
    if (outcome == 1) printf("WON\n");
    else if (outcome == 2) printf("FAILED\n");
    else printf("WAITING\n");

    // Libera toda a memória alocada
    for (int i = 0; i < nCenas; ++i) {
        libertaNo(nodes[i]);
    }
    free(nodes);
    free(hist);
    return 0;
}
