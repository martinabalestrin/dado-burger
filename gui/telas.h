#ifndef GUI_TELAS_H
#define GUI_TELAS_H

#include "componentes.h"
#include "../src/jogo/aplicacao.h"

typedef enum {
    TELA_MENU, TELA_DIFICULDADE, TELA_NOME, TELA_TUTORIAL, TELA_PARTIDA,
    TELA_HISTORICO, TELA_RESULTADO, TELA_SAIR
} Tela;

typedef enum {
    SELECAO_FECHADA, SELECAO_PREPARAR, SELECAO_ADICIONAR
} SelecaoIngrediente;

enum { TOTAL_NOTIFICACOES = 8 };

typedef struct {
    char camadas[1 + sizeof ((Receita *)0)->ordem];
    size_t totalCamadas;
    double inicio;
    bool ativa;
    int indiceCliente;
    int codigoPedido;
    int esperando;
} ComemoracaoEntrega;

typedef struct {
    Tela tela;
    Dificuldade dificuldade;
    int inicioElenco;
    int proximoInicioElenco;
    int pagina;
    int paginaPedido;
    int paginaEstoque;
    int paginaChapa;
    ContextoJogo *contexto;
    SessaoJogo *sessao;
    Fila fila;
    Partida partida;
    bool editandoNome;
    bool confirmarSaida;
    Tela destinoSaida;
    char aviso[160];
    SelecaoIngrediente selecao;
    SituacaoSessao resultado;
    ComemoracaoEntrega entrega;
    bool consumirSolturaEntrega;
    int totalNotificacoes;
    char notificacoes[TOTAL_NOTIFICACOES][96];
    ConsultaHistorico historicoOrdenado;
    CriterioOrdenacao criterioHistorico;
} Navegacao;

void desenharTela(Navegacao *nav, Texture2D salao, Texture2D balcao,
                  const ElencoClientes *elenco, const IngredientesGraficos *ingredientes);
void solicitarSaida(Navegacao *nav, Tela destino);
void encerrarPartidaGrafica(Navegacao *nav);

#endif
