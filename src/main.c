#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "receita.h"
#include "medicoes/medicoes.h"
#include "nodoAvl/nodoAvl.h"

void carregarPedidos(FILE *pedidos, NodoAvl *raiz);

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Erro: argumentos insuficientes\n");
        fprintf(stderr, "Uso correto: %s <pedidos.txt> <historico.txt>\n", argv[0]);
        
        return 1;
    }
    
    // === Carregar os pedidos na AVL ===
    FILE *fpedidos = fopen(argv[1], "r");
    NodoAvl *pedidos = NULL;

    if (fpedidos == NULL) {
        perror(argv[1]);
        return 1;
    }
    else {
        carregarPedidos(fpedidos, pedidos);
        fclose(fpedidos);
    }

    FILE *fhistorico = fopen(argv[2], "a+");
    if (fhistorico == NULL) {
        perror(argv[2]);
        fclose(fpedidos);
        return 1;
    }

    Medicoes medicoes;

    if (inicializarMedicoes(&medicoes) != 0) {
        fprintf(stderr, "Erro ao inicializar arquivo de medições\n");
        return 1;
    }

    // ...

    // no fim do código
    if (fecharMedicoes(&medicoes) != 0) {
        fprintf(stderr, "Erro ao fechar o arquivo de medições\n");
        return 1;
    }

    fclose(fhistorico);

    return 0;
}

void carregarPedidos(FILE *pedidos, NodoAvl *raiz) {
    
    int codigo;
    char buffer[256];

    while (fscanf(pedidos, "%d %255s", &codigo, buffer) == 2) {
        char *ordem = malloc(strlen(buffer) + 1);
        if (ordem == NULL) printf("Erro: Memória insuficiente!\n");

        Receita receita = {codigo, ordem};
        raiz = avlInserir(raiz, receita);
    }
}