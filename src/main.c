#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "receita.h"
#include "medicoes/medicoes.h"
#include "nodoAvl/nodoAvl.h"
#include "listaContDin/listaContDin.h"
#include "listaEnc/listaEnc.h"
#include "cliente/cliente.h"
#include "fila/fila.h"
#include "tabelaHash/tabelaHash.h"
#include "menu/menu.h"

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Erro: argumentos insuficientes\n");
        fprintf(stderr, "Uso correto: %s <pedidos.txt> <historico.txt>\n", argv[0]);
        
        return 1;
    }
    
    // === Carregar os pedidos na AVL ===
    NodoAvl *pedidos = NULL;
    long qntPedidos = 0;
    pedidos = carregarPedidos(pedidos, argv[1], &qntPedidos);

    if (pedidos == NULL) {
        printf("Erro: Carregamento de pedidos.\n");
        return 1; 
    }

    // === LINHA DE DEBUG: AVL ===
    // Função de debug para a AVL
    // avlDebugImprimir(pedidos);

    // Ex.: Buscar receita primeiro em cache na Tabela Hash e depois na AVL (está implementado já)
    // 
    TabelaHash pedidosCache;
    if (inicializaTabelaHash(&pedidosCache) != 0) {
        avlDestruir(pedidos);
        return -1;
    }    

    // === Carregar histórico de partidas ===
    ListaContDin historico;
    inicializarLCD(&historico);
    carregarHistorico(&historico, argv[2]);

    // ORDENAÇÃO: MERGE SORT -> integrar depois quando tivermos funcionalidades de menu

    // === LINHA DE DEBUG: LISTA CONTÍGUA DINÂMICA ===
    // depurarHistorico(&historico); 

    // === LINHA DE DEBUG: LISTA ENCADEADA ===
    // ListaEnc *meuPedido = criarPedido(buscarPedido(pedidos, 13));
    // imprimirListaEnc(meuPedido);

    Medicoes medicoes;

    if (inicializarMedicoes(&medicoes) != 0) {
        fprintf(stderr, "Erro ao inicializar arquivo de medições\n");
        return 1;
    }

    // Inicializa o rand
    srand(time(NULL));

    // ^^^^^^^ Inicialização dos recursos

    // ==== CHAMAR MENU ====
    ContextoJogo contextoJogo = {
        &medicoes,
        pedidos,
        &historico,
        &pedidosCache,
        argv[2],            // arquivo de histórico
        qntPedidos
    };

    executarMenu(&contextoJogo);

    // vvvvvvvvv Liberação dos resursos

    if (fecharMedicoes(&medicoes) != 0) {
        fprintf(stderr, "Erro ao fechar o arquivo de medições\n");
        return 1;
    }

    avlDestruir(pedidos);
    destruirTabelaHash(&pedidosCache);
    destruirLCD(&historico);

    return 0;
}