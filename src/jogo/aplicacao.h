#ifndef APLICACAO_H
#define APLICACAO_H

#include "sessao.h"

typedef struct {
    ContextoJogo contexto;
    Medicoes medicoes;
    MetricasAvl metricasAvl;
    MetricasOrdenacao metricasOrdenacao;
    TabelaHash pedidosCache;
    ListaContDin historico;
} AplicacaoJogo;

/* O contexto referencia esta instância e o caminho de histórico emprestado.
   Mantenha ambos válidos até finalizar a aplicação. */
int iniciarAplicacao(AplicacaoJogo *app, char *arquivoPedidos, char *arquivoHistorico);
void finalizarAplicacao(AplicacaoJogo *app);

/* A fila é inicializada pela função. Se falhar, retorna com a fila vazia. */
int montarFilaAtendimento(Fila *fila, const ContextoJogo *contexto, Dificuldade dificuldade);
void esvaziarFilaAtendimento(Fila *fila);

typedef enum {
    HISTORICO_ORDENADO,
    HISTORICO_VAZIO,
    HISTORICO_FALHA_COPIA,
    HISTORICO_FALHA_ORDENACAO
} SituacaoOrdenacaoHistorico;

typedef struct {
    SituacaoOrdenacaoHistorico situacao;
    Partida *partidas;
    int total;
} ConsultaHistorico;

/* Em caso de sucesso, o chamador é dono da cópia e deve liberá-la com free.
   Nos demais casos, partidas é NULL. */
ConsultaHistorico consultarHistoricoOrdenado(const ContextoJogo *contexto, CriterioOrdenacao criterio);

#endif
