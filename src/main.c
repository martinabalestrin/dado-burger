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
    
    /*
    //=========== BUSCA DE PEDIDOS DE FORMA ALEATÓRIA ===========
    srand(time(NULL));
    int codigo;
    int status;
    Receita receita;

    // DEBUG DE QNTPEDIDOS
    printf("Pedidos: %ld\nLinhas em %s: %ld\n", qntPedidos, argv[1], qntPedidos*2);

    codigo = 1 + rand() % qntPedidos;
    status = buscarPedido(&pedidosCache, pedidos, codigo, &receita);
    if (status == 1) printf("%d: %s\n", receita.cod, receita.ordem);
    else printf("ERRO!!!!!");

    codigo = 1 + rand() %qntPedidos;
    status = buscarPedido(&pedidosCache, pedidos, codigo, &receita);
    if (status == 1) printf("%d: %s\n", receita.cod, receita.ordem);
    else printf("ERRO!!!!!");

    codigo = 1 + rand() %qntPedidos;
    status = buscarPedido(&pedidosCache, pedidos, codigo, &receita);
    if (status == 1) printf("%d: %s\n", receita.cod, receita.ordem);
    else printf("ERRO!!!!!");

    // === DEBUG TABELA HASH ===
    printf("Itens: %d\nColisoes: %d\n", pedidosCache.n, pedidosCache.colisao);
    */

    // === Carregar histórico de partidas ===
    ListaContDin historico;
    inicializarLCD(&historico);
    carregarHistorico(&historico, argv[2]);

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

    // ...

    // === Inicialização da fila de clientes ===
    // A inicialização deve acontecer no início de cada partida (não do jogo)
    // Todos os clientes entram na fila no início
    // Remoção após conclusão do pedido
    FilaEnc clientes;
    inicializaFila(&clientes);
    // a lógica do jogo vai definir numero de clientes -> laço para enqueueFila
    /* essa integração ainda depende do menu do jogo, do sorteio dos pedidos (sugiro sortear todos antes
    de enfileirar) e da paciência (setar o valor antes de enfileirar/criar clientes)
    */

    // no fim do código
    if (fecharMedicoes(&medicoes) != 0) {
        fprintf(stderr, "Erro ao fechar o arquivo de medições\n");
        return 1;
    }

    avlDestruir(pedidos);
    destruirTabelaHash(&pedidosCache);

    return 0;
}