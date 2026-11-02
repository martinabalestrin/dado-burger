#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "receita.h"
#include "medicoes/medicoes.h"
#include "nodoAvl/nodoAvl.h"

NodoAvl* carregarPedidos(NodoAvl *raiz, char nomeArquivo[]);

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Erro: argumentos insuficientes\n");
        fprintf(stderr, "Uso correto: %s <pedidos.txt> <historico.txt>\n", argv[0]);
        
        return 1;
    }
    
    // === Carregar os pedidos na AVL ===
    NodoAvl *pedidos = NULL;
    pedidos = carregarPedidos(pedidos, argv[1]);

    if (pedidos == NULL) {
        printf("Erro: Carregamento de pedidos.\n");
        return 1; 
    }

    FILE *fhistorico = fopen(argv[2], "a+");
    if (fhistorico == NULL) {
        perror(argv[2]);
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

NodoAvl* carregarPedidos(NodoAvl *raiz, char nomeArquivo[]) {

    FILE *fpedidos = fopen(nomeArquivo, "r");
    if (fpedidos == NULL) {
        perror(nomeArquivo);
        return NULL;
    }
    
    int codigo;
    char buffer[256];

    while (fscanf(fpedidos, "%d %255s", &codigo, buffer) == 2) {
        char *ordem = malloc(strlen(buffer) + 1);
        if (ordem == NULL) {
            printf("Erro: Memória insuficiente!\n");
            return NULL;
        }
        else strcpy(ordem, buffer);

        Receita receita = {codigo, ordem};
        raiz = avlInserir(raiz, receita);
    }

    fclose(fpedidos);
    return raiz;
}