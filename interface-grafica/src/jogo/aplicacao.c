#include "aplicacao.h"

#include <stdlib.h>
#include <time.h>

#include "../timer/timer.h"

int iniciarAplicacao(AplicacaoJogo *app, char *arquivoPedidos, char *arquivoHistorico)
{
    *app = (AplicacaoJogo){0};
    if (inicializarMedicoes(&app->medicoes) == 0) {
        fprintf(stderr, "Erro ao inicializar arquivo de medições\n");
        return 0;
    }

    Timer t;
    long nosInseridos = 0;
    long rotacoesInsercao = 0;
    timer_iniciar(&t);
    NodoAvl *pedidos = carregarPedidos(NULL, arquivoPedidos, &nosInseridos, &rotacoesInsercao);
    double tempoExecucao = timer_parar(&t);
    if (pedidos == NULL) {
        printf("Erro: Carregamento de pedidos.\n");
        fecharMedicoes(&app->medicoes);
        return 0;
    }
    inserirAvlCarregamento(&app->medicoes, nosInseridos, pedidos->altura, rotacoesInsercao, tempoExecucao);

    if (inicializaTabelaHash(&app->pedidosCache) != 0) {
        avlDestruir(pedidos);
        printf("Erro: Inicialização do cache.\n");
        fecharMedicoes(&app->medicoes);
        return 0;
    }

    inicializarLCD(&app->historico);
    carregarHistorico(&app->historico, arquivoHistorico);
    inserirHistoricoCarregamento(&app->medicoes, app->historico.tamanho);
    srand(time(NULL));

    app->contexto = (ContextoJogo){
        .medicoes = &app->medicoes,
        .pedidos = pedidos,
        .historico = &app->historico,
        .pedidosCache = &app->pedidosCache,
        .arquivoHistorico = arquivoHistorico,
        .qntPedidos = nosInseridos,
        .metricasAvl = &app->metricasAvl,
        .metricasOrdenacao = &app->metricasOrdenacao
    };
    return 1;
}

void finalizarAplicacao(AplicacaoJogo *app)
{
    inserirAvlDurantePartida(&app->medicoes, &app->metricasAvl);
    inserirHashDurantePartida(&app->medicoes, &app->pedidosCache);
    inserirOrdenacaoHistorico(&app->medicoes, &app->metricasOrdenacao);

    avlDestruir(app->contexto.pedidos);
    destruirTabelaHash(&app->pedidosCache);
    destruirLCD(&app->historico);
    fecharMedicoes(&app->medicoes);
}

void esvaziarFilaAtendimento(Fila *fila)
{
    Cliente lixo;
    while (!estaVaziaFila(fila)) {
        dequeueFila(fila, &lixo);
    }
}

int montarFilaAtendimento(Fila *fila, const ContextoJogo *contexto, Dificuldade dificuldade)
{
    inicializaFila(fila);
    ConfiguracaoDificuldade config = configuracaoDaDificuldade(dificuldade);
    for (int i = 0; i < config.numClientes; i++) {
        Cliente cliente = inicializarCliente(config.paciencia, contexto->qntPedidos);
        if (!enqueueFila(fila, cliente)) {
            esvaziarFilaAtendimento(fila);
            return 0;
        }
    }
    return 1;
}

ConsultaHistorico consultarHistoricoOrdenado(const ContextoJogo *contexto, CriterioOrdenacao criterio)
{
    const ListaContDin *historico = contexto->historico;
    if (historico->tamanho <= 0) return (ConsultaHistorico){.situacao = HISTORICO_VAZIO};

    Partida *copia = criarCopiaPartidas(historico);
    if (copia == NULL) return (ConsultaHistorico){.situacao = HISTORICO_FALHA_COPIA};

    int total = historico->tamanho;
    double tempo = ordenarPartidas(copia, total, criterio);
    contexto->metricasOrdenacao->tamanho = total;
    contexto->metricasOrdenacao->tempo = tempo;
    if (tempo < 0) {
        free(copia);
        return (ConsultaHistorico){.situacao = HISTORICO_FALHA_ORDENACAO};
    }
    return (ConsultaHistorico){.situacao = HISTORICO_ORDENADO, .partidas = copia, .total = total};
}
