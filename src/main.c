#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "receita.h"
#include "medicoes/medicoes.h"
#include "nodoAvl/nodoAvl.h"

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

    // Função de debug para a AVL
    // avlDebugImprimir(pedidos);

    // Ex.: Buscar receita na AVL
    // Receita receita = buscarPedido(pedidos, 0);
    // debug:
    // printf("%d %s\n", receita.cod, receita.ordem);


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