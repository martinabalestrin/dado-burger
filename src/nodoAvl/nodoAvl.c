#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodoAvl.h"

// Retorna 0 se for vazia
int avlAltura(const NodoAvl *no) {
    if (no == NULL) return 0;
    return no->altura;
}

// FB = h(esq) - h(dir)
// Positivo = pesado à esq
// Negativo = pesado à dir
int avlFb(const NodoAvl *no) {
    if (no == NULL) return 0;
    return avlAltura(no->esq) - avlAltura(no->dir);
}

// Recalcula e armazena a altura de 'no' com base nas alturas dos filhos
// Chamado sempre que um filho muda (inserção, remoção ou rotação)
void avlAtualizarAltura(NodoAvl *no) {
    int ae = avlAltura(no->esq);
    int ad = avlAltura(no->dir);
    if (ae > ad)
        no->altura = 1 + ae;
    else
        no->altura = 1 + ad;
}

// Rotação simples à DIREITA (caso LL)
// x sobe; B (filho dir de x) passa para filho esq de y
// Atualiza y primeiro (ficou abaixo), depois x
NodoAvl* avlRotDir(NodoAvl *y) {
    NodoAvl *x = y->esq;
    NodoAvl *B = x->dir;
    x->dir = y;
    y->esq = B;
    avlAtualizarAltura(y);
    avlAtualizarAltura(x);
    return x;
}

// Rotação simples à ESQUERDA (caso RR). Simétrica à anterior
// y sobe; B (filho esq de y) passa para filho dir de x
NodoAvl* avlRotEsq(NodoAvl *x) {
    NodoAvl *y = x->dir;
    NodoAvl *B = y->esq;
    y->esq = x;
    x->dir = B;
    avlAtualizarAltura(x);
    avlAtualizarAltura(y);
    return y;
}

// Verifica o FB e aplica a rotação correta
// Usada tanto na inserção quanto na remoção
NodoAvl* avlReequilibrar(NodoAvl *raiz) {
    if (raiz == NULL) return NULL;
    
    avlAtualizarAltura(raiz);
    int fb = avlFb(raiz);

    // LL: pesado à esq, filho esq também pesado à esq ou equilibrado
    if (fb == +2 && avlFb(raiz->esq) >= 0)
        return avlRotDir(raiz);

    // RR: pesado à dir, filho dir também pesado à dir ou equilibrado
    if (fb == -2 && avlFb(raiz->dir) <= 0)
        return avlRotEsq(raiz);

    // LR: pesado à esq, filho esq pesado à dir -> transforma LR em LL
    if (fb == +2 && avlFb(raiz->esq) < 0) {
        raiz->esq = avlRotEsq(raiz->esq);
        return avlRotDir(raiz);
    }

    // RL: pesado à dir, filho dir pesado à esq -> transforma RL em RR
    if (fb == -2 && avlFb(raiz->dir) > 0) {
        raiz->dir = avlRotDir(raiz->dir);
        return avlRotEsq(raiz);
    }

    return raiz;   // sem desequilíbrio
}

// Busca: idêntica à Abp - a altura garantida torna-a O(log n)
NodoAvl* avlBuscar(NodoAvl *raiz, int cod) {
    if (raiz == NULL)           return NULL;
    if (cod == raiz->dado.cod) return raiz;
    if (cod < raiz->dado.cod)  return avlBuscar(raiz->esq, cod);
    return avlBuscar(raiz->dir, cod);
}

// Inserção: descida igual à Abp; na subida, reequilibra cada ancestral.
// No máximo uma rotação por inserção.
// Uso: raiz = avlInserir(raiz, receita);
NodoAvl* avlInserir(NodoAvl *raiz, Receita valor, long *qntPedidos) {
    if (raiz == NULL) {
        NodoAvl *novo = (NodoAvl*) malloc(sizeof(NodoAvl));
        novo->dado   = valor;
        novo->esq    = NULL;
        novo->dir    = NULL;
        novo->altura = 1;
        (*qntPedidos)++;
        return novo;
    }
    if (valor.cod < raiz->dado.cod)
        raiz->esq = avlInserir(raiz->esq, valor, qntPedidos);
    else if (valor.cod > raiz->dado.cod)
        raiz->dir = avlInserir(raiz->dir, valor, qntPedidos);
    else
        return raiz;   /* duplicado: ignora */
    return avlReequilibrar(raiz);
}

// Auxiliar: menor nó da subárvore (sucessor em-ordem).
NodoAvl* avlMinimo(NodoAvl *raiz) {
    while (raiz->esq != NULL) raiz = raiz->esq;
    return raiz;
}

// Remoção: mesmos três casos da Abp; na subida, reequilibra cada ancestral.
// Podem ser necessárias múltiplas rotações (uma por nível desequilibrado).
// Uso: raiz = avlRemover(raiz, cod);
NodoAvl* avlRemover(NodoAvl *raiz, int cod) {
    if (raiz == NULL) return NULL;

    if (cod < raiz->dado.cod) {
        raiz->esq = avlRemover(raiz->esq, cod);
    } else if (cod > raiz->dado.cod) {
        raiz->dir = avlRemover(raiz->dir, cod);
    } else {
        if (raiz->esq == NULL) {
            NodoAvl *filho = raiz->dir;
            free(raiz);
            return filho;
        }
        if (raiz->dir == NULL) {
            NodoAvl *filho = raiz->esq;
            free(raiz);
            return filho;
        }
        /* dois filhos: substitui pelo sucessor em-ordem */
        NodoAvl *sucessor = avlMinimo(raiz->dir);
        raiz->dado = sucessor->dado;
        raiz->dir  = avlRemover(raiz->dir, sucessor->dado.cod);
    }
    return avlReequilibrar(raiz);
}

// Destroi toda a árvore em pós-ordem.
// Uso: raiz = avlDestruir(raiz);
NodoAvl* avlDestruir(NodoAvl *raiz) {
    if (raiz == NULL) return NULL;
    raiz->esq = avlDestruir(raiz->esq);
    raiz->dir = avlDestruir(raiz->dir);
    free(raiz);
    return NULL;
}

static void avlDebugImprimirRecursivo(const NodoAvl *raiz, size_t *total) {
    if (raiz == NULL) return;

    avlDebugImprimirRecursivo(raiz->esq, total);
    printf("%d %s\n", raiz->dado.cod, raiz->dado.ordem);
    (*total)++;
    avlDebugImprimirRecursivo(raiz->dir, total);
}

void avlDebugImprimir(const NodoAvl *raiz) {
    size_t total = 0;
    printf("Pedidos carregados na AVL (em ordem de código):\n");
    avlDebugImprimirRecursivo(raiz, &total);
    printf("Total de pedidos na AVL: %zu\n", total);
}

NodoAvl* carregarPedidos(NodoAvl *raiz, char nomeArquivo[], long *qntPedidos) {

    FILE *fpedidos = fopen(nomeArquivo, "r");
    if (fpedidos == NULL) {
        perror(nomeArquivo);
        return NULL;
    }
    
    int codigo;
    char ordem[8];

    while (fscanf(fpedidos, "%d %7s", &codigo, ordem) == 2) {
        Receita receita;
        receita.cod = codigo;
        strcpy(receita.ordem, ordem);

        raiz = avlInserir(raiz, receita, qntPedidos);
    }

    fclose(fpedidos);
    return raiz;
}

NodoAvl* buscarPedidoAvl(NodoAvl *raiz, int codigo) {
    NodoAvl *pedidoNodo = avlBuscar(raiz, codigo);
    return pedidoNodo;
}
