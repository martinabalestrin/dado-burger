#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "medicoes/medicoes.h"
#include "nodoAvl/nodoAvl.h"
#include "listaContDin/listaContDin.h"
#include "cliente/cliente.h"
#include "fila/fila.h"
#include "tabelaHash/tabelaHash.h"
#include "menu/menu.h"
#include "timer/timer.h"

int main(int argc, char *argv[]) {

    // Se há menos arquivos do que o necessário
    if (argc < 3) {
        fprintf(stderr, "Erro: argumentos insuficientes\n");
        fprintf(stderr, "Uso correto: %s <pedidos.txt> <historico.txt>\n", argv[0]);
        return -1;
    }

    // === Inicializar medições ===
    Medicoes medicoes;
    if (inicializarMedicoes(&medicoes) == 0) {
        fprintf(stderr, "Erro ao inicializar arquivo de medições\n");
        return -1;
    }

    MetricasAvl metricasAvl = {0, 0, 0.0};

    // === Criar Timer
    Timer t;
    double tempoExecucao;
    
    // === Carregar os pedidos na AVL ===
    NodoAvl *pedidos = NULL;
    long nosInseridos = 0;
    long rotacoesInsercao = 0;
    
    timer_iniciar(&t);
    pedidos = carregarPedidos(pedidos, argv[1], &nosInseridos, &rotacoesInsercao);
    tempoExecucao = timer_parar(&t);

    if (pedidos == NULL) {
        printf("Erro: Carregamento de pedidos.\n");
        return -1; 
    }

    // Anotar medições de carregamento da AVL
    inserirAvlCarregamento(&medicoes, nosInseridos, pedidos->altura, rotacoesInsercao, tempoExecucao);

    // === Inicializar a Tabela Hash para cache ===
    TabelaHash pedidosCache;
    if (inicializaTabelaHash(&pedidosCache) != 0) {
        avlDestruir(pedidos);
        printf("Erro: Inicialização do cache.\n");
        return -1;
    }    

    // === Carregar histórico de partidas ===
    ListaContDin historico;
    inicializarLCD(&historico);
    carregarHistorico(&historico, argv[2]);

    // Anotar medições de carregamento do histórico
    inserirHistoricoCarregamento(&medicoes, historico.tamanho);

    // === Inicializa o random ===
    srand(time(NULL));

    // ==== Chamar menu ====
    ContextoJogo contextoJogo = {
        &medicoes,
        pedidos,
        &historico,
        &pedidosCache,
        argv[2],            // arquivo de histórico
        nosInseridos,       // quantidade de pedidos
        &metricasAvl
    };

    executarMenu(&contextoJogo);

    inserirAvlDurantePartida(&medicoes, &metricasAvl);
    inserirHashDurantePartida(&medicoes, &pedidosCache);


    // Liberação de memória das estruturas criadas na main
    avlDestruir(pedidos);
    destruirTabelaHash(&pedidosCache);
    destruirLCD(&historico);
    fecharMedicoes(&medicoes);

    return 0;
}