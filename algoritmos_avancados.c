#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 10
#define MAX_PISTAS 20

// Estrutura para um cômodo da mansão
typedef struct Sala {
    char nome[30];
    char pista[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para uma pista na BST
typedef struct PistaBST {
    char pista[50];
    struct PistaBST *esq;
    struct PistaBST *dir;
} PistaBST;

// Estrutura para entrada na tabela hash
typedef struct EntradaHash {
    char pista[50];
    char suspeito[30];
    struct EntradaHash *prox;
} EntradaHash;

// Estrutura para a tabela hash
typedef struct {
    EntradaHash *tabela[TAM_HASH];
} TabelaHash;

// Protótipos das funções
Sala* criarSala(char nome[], char pista[]);
void construirMansao(Sala **raiz);
void explorarSalas(Sala *raiz, PistaBST **arvorePistas, TabelaHash *hash);
PistaBST* inserirPista(PistaBST *raiz, char pista[]);
void adicionarPista(PistaBST **arvorePistas, char pista[]);
void inicializarHash(TabelaHash *hash);
int funcaoHash(char *chave);
void inserirNaHash(TabelaHash *hash, char pista[], char suspeito[]);
char* encontrarSuspeito(TabelaHash *hash, char pista[]);
void listarPistas(PistaBST *raiz);
void verificarSuspeitoFinal(PistaBST *arvorePistas, TabelaHash *hash);
int contarPistasPorSuspeito(PistaBST *raiz, TabelaHash *hash, char suspeito[]);
void liberarArvorePistas(PistaBST *raiz);
void liberarHash(TabelaHash *hash);
void liberarMansao(Sala *raiz);
void menuExploracao();

// Variável global para contar pistas coletadas
int pistasColetadas = 0;

int main() {
    Sala *mansao = NULL;
    PistaBST *arvorePistas = NULL;
    TabelaHash hash;
    
    printf("🕵️  === DETECTIVE QUEST === 🕵️\n");
    printf("Uma morte misteriosa ocorreu na Mansão Blackwood.\n");
    printf("Explore os cômodos, colete pistas e descubra o culpado!\n\n");
    
    // Inicializar estruturas
    construirMansao(&mansao);
    inicializarHash(&hash);
    
    // Popular tabela hash com associações pista-suspeito
    inserirNaHash(&hash, "Carta de amor rasgada", "Ana");
    inserirNaHash(&hash, "Manchas de tinta azul", "Carlos");
    inserirNaHash(&hash, "Recibo de joalheria caro", "Beatriz");
    inserirNaHash(&hash, "Fio de cabelo loiro", "Ana");
    inserirNaHash(&hash, "Cheiro de perfume caro", "Beatriz");
    inserirNaHash(&hash, "Livro sobre venenos", "Carlos");
    inserirNaHash(&hash, "Relógio quebrado", "David");
    inserirNaHash(&hash, "Pó branco misterioso", "Carlos");
    inserirNaHash(&hash, "Foto antiga rasgada", "Ana");
    inserirNaHash(&hash, "Chave de cofre", "David");
    inserirNaHash(&hash, "Nota de ameaça", "Beatriz");
    inserirNaHash(&hash, "Luvas manchadas", "David");
    
    // Iniciar exploração
    explorarSalas(mansao, &arvorePistas, &hash);
    
    // Liberar memória
    liberarMansao(mansao);
    liberarArvorePistas(arvorePistas);
    liberarHash(&hash);
    
    return 0;
}

/**
 * Cria dinamicamente um cômodo da mansão
 * @param nome Nome do cômodo
 * @param pista Pista associada ao cômodo
 * @return Ponteiro para a sala criada
 */
Sala* criarSala(char nome[], char pista[]) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * Constrói a mansão como uma árvore binária fixa
 * @param raiz Ponteiro para a raiz da árvore
 */
void construirMansao(Sala **raiz) {
    *raiz = criarSala("Entrada", "Porta arrombada - invasão forçada");
    
    // Salas à esquerda (ala oeste)
    (*raiz)->esquerda = criarSala("Biblioteca", "Livro sobre venenos");
    (*raiz)->esquerda->esquerda = criarSala("Escritório", "Carta de amor rasgada");
    (*raiz)->esquerda->direita = criarSala("Sala de Estar", "Manchas de tinta azul");
    
    // Salas à direita (ala leste)
    (*raiz)->direita = criarSala("Quarto Principal", "Cheiro de perfume caro");
    (*raiz)->direita->esquerda = criarSala("Banheiro", "Pó branco misterioso");
    (*raiz)->direita->direita = criarSala("Jardim de Inverno", "Fio de cabelo loiro");
    
    printf("Mansão Blackwood carregada com 7 cômodos misteriosos...\n");
}

/**
 * Navega pela árvore e coleta pistas de forma interativa
 * @param raiz Raiz da árvore da mansão
 * @param arvorePistas Ponteiro para a árvore BST de pistas
 * @param hash Ponteiro para a tabela hash de suspeitos
 */
void explorarSalas(Sala *raiz, PistaBST **arvorePistas, TabelaHash *hash) {
    Sala *atual = raiz;
    char movimento;
    
    printf("\n=== EXPLORAÇÃO DA MANSÃO ===\n");
    
    while (atual != NULL) {
        printf("\n📍 Você está na: %s\n", atual->nome);
        
        // Verificar se há pista neste cômodo
        if (strlen(atual->pista) > 0) {
            printf("🔍 Pista encontrada: %s\n", atual->pista);
            
            // Adicionar pista à BST
            adicionarPista(arvorePistas, atual->pista);
            pistasColetadas++;
            
            // Marcar pista como coletada (evitar repetição)
            strcpy(atual->pista, "");
            
            printf("✅ Pista adicionada ao seu caderno!\n");
        } else {
            printf("ℹ️  Nenhuma pista nova aqui...\n");
        }
        
        // Menu de navegação
        menuExploracao();
        printf("Para onde deseja ir? ");
        scanf(" %c", &movimento);
        getchar(); // Limpar buffer
        
        movimento = tolower(movimento);
        
        switch(movimento) {
            case 'e': // Esquerda
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("❌ Não há cômodos à esquerda!\n");
                }
                break;
                
            case 'd': // Direita
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("❌ Não há cômodos à direita!\n");
                }
                break;
                
            case 's': // Sair
                printf("\n🚪 Saindo da mansão...\n");
                atual = NULL;
                break;
                
            default:
                printf("❌ Movimento inválido! Use 'e', 'd' ou 's'.\n");
        }
    }
    
    // Fase final - acusação
    if (pistasColetadas > 0) {
        verificarSuspeitoFinal(*arvorePistas, hash);
    } else {
        printf("\n💀 VOCÊ FALHOU! Nenhuma pista foi coletada.\n");
        printf("O assassino escapou e você foi demitido!\n");
    }
}

/**
 * Insere uma pista na BST de forma recursiva
 * @param raiz Raiz da árvore BST
 * @param pista Pista a ser inserida
 * @return Nova raiz da árvore
 */
PistaBST* inserirPista(PistaBST *raiz, char pista[]) {
    if (raiz == NULL) {
        PistaBST *novaPista = (PistaBST*)malloc(sizeof(PistaBST));
        strcpy(novaPista->pista, pista);
        novaPista->esq = NULL;
        novaPista->dir = NULL;
        return novaPista;
    }
    
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    } else if (comparacao > 0) {
        raiz->dir = inserirPista(raiz->dir, pista);
    }
    // Se igual, não insere duplicata
    
    return raiz;
}

/**
 * Wrapper function para adicionar pista à BST
 * @param arvorePistas Ponteiro para a árvore de pistas
 * @param pista Pista a ser adicionada
 */
void adicionarPista(PistaBST **arvorePistas, char pista[]) {
    *arvorePistas = inserirPista(*arvorePistas, pista);
}

/**
 * Inicializa a tabela hash
 * @param hash Ponteiro para a tabela hash
 */
void inicializarHash(TabelaHash *hash) {
    for (int i = 0; i < TAM_HASH; i++) {
        hash->tabela[i] = NULL;
    }
}

/**
 * Função hash simples baseada no primeiro caractere
 * @param chave Chave para cálculo do hash
 * @return Índice na tabela hash
 */
int funcaoHash(char *chave) {
    return (tolower(chave[0]) - 'a') % TAM_HASH;
}

/**
 * Insere associação pista-suspeito na tabela hash
 * @param hash Ponteiro para a tabela hash
 * @param pista Pista a ser inserida
 * @param suspeito Suspeito associado à pista
 */
void inserirNaHash(TabelaHash *hash, char pista[], char suspeito[]) {
    int indice = funcaoHash(pista);
    
    EntradaHash *novaEntrada = (EntradaHash*)malloc(sizeof(EntradaHash));
    strcpy(novaEntrada->pista, pista);
    strcpy(novaEntrada->suspeito, suspeito);
    novaEntrada->prox = NULL;
    
    // Inserir no início da lista encadeada
    if (hash->tabela[indice] == NULL) {
        hash->tabela[indice] = novaEntrada;
    } else {
        novaEntrada->prox = hash->tabela[indice];
        hash->tabela[indice] = novaEntrada;
    }
}

/**
 * Consulta o suspeito correspondente a uma pista
 * @param hash Ponteiro para a tabela hash
 * @param pista Pista a ser consultada
 * @return Nome do suspeito ou "Desconhecido"
 */
char* encontrarSuspeito(TabelaHash *hash, char pista[]) {
    int indice = funcaoHash(pista);
    EntradaHash *atual = hash->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->prox;
    }
    
    return "Desconhecido";
}

/**
 * Lista todas as pistas coletadas em ordem (in-order traversal)
 * @param raiz Raiz da árvore BST de pistas
 */
void listarPistas(PistaBST *raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esq);
        printf("   - %s\n", raiz->pista);
        listarPistas(raiz->dir);
    }
}

/**
 * Conduz à fase de julgamento final e verifica a acusação
 * @param arvorePistas Árvore BST com todas as pistas coletadas
 * @param hash Tabela hash com associações pista-suspeito
 */
void verificarSuspeitoFinal(PistaBST *arvorePistas, TabelaHash *hash) {
    char suspeitoAcusado[30];
    int pistasDoSuspeito;
    
    printf("\n🎭 === FASE FINAL - ACUSAÇÃO === 🎭\n");
    printf("Suspeitos: Ana, Carlos, Beatriz, David\n\n");
    
    printf("📖 Pistas coletadas (%d no total):\n", pistasColetadas);
    listarPistas(arvorePistas);
    
    printf("\n🔎 Análise das pistas:\n");
    
    // Função auxiliar para analisar cada pista
    void analisarPista(PistaBST *no) {
        if (no != NULL) {
            analisarPista(no->esq);
            char *suspeito = encontrarSuspeito(hash, no->pista);
            printf("   - '%s' → aponta para %s\n", no->pista, suspeito);
            analisarPista(no->dir);
        }
    }
    
    analisarPista(arvorePistas);
    
    printf("\n🗣️  Quem é o culpado? Digite o nome: ");
    fgets(suspeitoAcusado, 30, stdin);
    suspeitoAcusado[strcspn(suspeitoAcusado, "\n")] = 0; // Remover quebra de linha
    
    // Contar quantas pistas apontam para o suspeito acusado
    pistasDoSuspeito = contarPistasPorSuspeito(arvorePistas, hash, suspeitoAcusado);
    
    printf("\n=== VEREDITO ===\n");
    printf("Acusação: %s\n", suspeitoAcusado);
    printf("Pistas que sustentam a acusação: %d\n", pistasDoSuspeito);
    
    if (pistasDoSuspeito >= 2) {
        printf("🎉 PARABÉNS! A acusação é sustentada por múltiplas pistas!\n");
        printf("🔒 %s foi preso(a)! Caso resolvido!\n", suspeitoAcusado);
    } else if (pistasDoSuspeito == 1) {
        printf("⚠️  A acusação é fraca! Apenas uma pista aponta para %s.\n", suspeitoAcusado);
        printf("💼 O caso foi arquivado por falta de provas.\n");
    } else {
        printf("💀 FRACASSO TOTAL! Nenhuma pista aponta para %s.\n", suspeitoAcusado);
        printf("🚓 Você foi demitido por acusação infundada!\n");
    }
}

/**
 * Conta quantas pistas apontam para um determinado suspeito
 * @param raiz Raiz da árvore BST de pistas
 * @param hash Tabela hash com associações
 * @param suspeito Suspeito a ser verificado
 * @return Número de pistas que apontam para o suspeito
 */
int contarPistasPorSuspeito(PistaBST *raiz, TabelaHash *hash, char suspeito[]) {
    if (raiz == NULL) return 0;
    
    int count = 0;
    
    // Função recursiva para percorrer a BST e contar pistas
    void contarRecursivo(PistaBST *no) {
        if (no != NULL) {
            contarRecursivo(no->esq);
            char *suspeitoDaPista = encontrarSuspeito(hash, no->pista);
            if (strcmp(suspeitoDaPista, suspeito) == 0) {
                count++;
            }
            contarRecursivo(no->dir);
        }
    }
    
    contarRecursivo(raiz);
    return count;
}

/**
 * Exibe o menu de opções de exploração
 */
void menuExploracao() {
    printf("\nOpções:\n");
    printf("  'e' - Ir para cômodo à esquerda\n");
    printf("  'd' - Ir para cômodo à direita\n");
    printf("  's' - Sair da mansão\n");
}

// Funções para liberar memória
void liberarArvorePistas(PistaBST *raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esq);
        liberarArvorePistas(raiz->dir);
        free(raiz);
    }
}

void liberarHash(TabelaHash *hash) {
    for (int i = 0; i < TAM_HASH; i++) {
        EntradaHash *atual = hash->tabela[i];
        while (atual != NULL) {
            EntradaHash *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

void liberarMansao(Sala *raiz) {
    if (raiz != NULL) {
        liberarMansao(raiz->esquerda);
        liberarMansao(raiz->direita);
        free(raiz);
    }
}
