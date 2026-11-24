#ifndef SESSAO_H
#define SESSAO_H

#include <stddef.h>

#include "../menu/menu.h"

typedef struct SessaoJogo SessaoJogo;

typedef struct {
    int numClientes;
    int paciencia;
    int validadeCarne;
    int validadeOvo;
    int validadeBacon;
} ConfiguracaoDificuldade;

ConfiguracaoDificuldade configuracaoDaDificuldade(Dificuldade dificuldade);

typedef enum {
    SESSAO_NAO_INICIADA,
    SESSAO_EM_ATENDIMENTO,
    SESSAO_VITORIA,
    SESSAO_DERROTA,
    SESSAO_SAIDA_VOLUNTARIA,
    SESSAO_ERRO
} SituacaoSessao;

typedef enum {
    SESSAO_PREPARAR,
    SESSAO_ADICIONAR,
    SESSAO_REMOVER,
    SESSAO_AGUARDAR,
    SESSAO_SAIR
} TipoAcaoSessao;

typedef struct {
    TipoAcaoSessao tipo;
    char ingrediente; /* Usado apenas para preparar e adicionar. */
} AcaoSessao;

typedef enum {
    SESSAO_ACAO_APLICADA,
    SESSAO_ACAO_RECUSADA,
    SESSAO_ACAO_FALHOU
} RetornoAcaoSessao;

typedef enum {
    SESSAO_SEM_BLOQUEIO,
    SESSAO_SEM_ATENDIMENTO,
    SESSAO_ACAO_INVALIDA,
    SESSAO_INGREDIENTE_INVALIDO,
    SESSAO_PAO_INFERIOR_FIXO,
    SESSAO_PREPARO_DESNECESSARIO,
    SESSAO_ESTOQUE_VAZIO,
    SESSAO_ESTOQUE_PREPARADO_VAZIO,
    SESSAO_MONTAGEM_FECHADA
} MotivoBloqueioSessao;

typedef enum {
    SESSAO_COZIMENTO_INICIADO,
    SESSAO_PREPARO_IMEDIATO,
    SESSAO_INGREDIENTE_PRONTO,
    SESSAO_INGREDIENTE_VENCIDO,
    SESSAO_INGREDIENTE_ADICIONADO,
    SESSAO_INGREDIENTE_REMOVIDO,
    SESSAO_PEDIDO_ENTREGUE,
    SESSAO_FALHA_ATENDIMENTO
} TipoEventoSessao;

typedef enum {
    SESSAO_SEM_ERRO,
    SESSAO_FALHA_INICIALIZAR_ESTOQUES,
    SESSAO_FALHA_BUSCAR_PEDIDO,
    SESSAO_FALHA_ALOCAR_MONTAGEM,
    SESSAO_FALHA_INICIAR_MONTAGEM,
    SESSAO_FALHA_CRIAR_GABARITO,
    SESSAO_FALHA_INICIAR_COZIMENTO, /* A partida permanece ativa. */
    SESSAO_FALHA_TRANSFERIR_COZIMENTO
} ErroSessao;

typedef struct {
    TipoEventoSessao tipo;
    int turno; /* Turno em que ocorreu o evento. */
    char ingrediente;
    int quantidade;
    int turnoPronto; /* Usado em SESSAO_COZIMENTO_INICIADO. */
    int temVencimento;
    int turnoVencimento; /* Válido se temVencimento, inclusive após consumo. */
    int numeroCliente;   /* Número exibido na CLI, capturado antes de avançar a fila. */
    int codigoPedido;    /* Usado em SESSAO_FALHA_BUSCAR_PEDIDO. */
    ErroSessao erro;     /* Usado em SESSAO_FALHA_ATENDIMENTO. */
} EventoSessao;

/* O evento é válido durante a chamada. O receptor pode copiar seus valores.
   O receptor deve usar apenas o evento, sem reentrar na sessão. */
typedef void (*ReceberEventoSessao)(const EventoSessao *evento, void *dados);

typedef struct {
    RetornoAcaoSessao retorno;
    MotivoBloqueioSessao motivo;
    SituacaoSessao situacao;
    int consumiuTurno;
} ResultadoAcaoSessao;

typedef struct {
    SituacaoSessao situacao;
    Dificuldade dificuldade;
    int turno;
    int pontuacao;
    int totalClientes;
    int pacienciaInicial;
    Cliente clienteAtual; /* Válido enquanto a sessão estiver em atendimento. */

    const NodoFila *aguardando; /* Ordem FIFO. Exclui o cliente atual. */
    const ListaEnc *receita;
    const PilhaEnc *montagem;
    const MinHeap *carne;
    const MinHeap *ovo;
    const MinHeap *bacon;
    const int *estoquePreparado; /* suíço, cheddar, cebola, tomate */
    const FilaCozimento *chapa;
} EstadoSessao;

/* Aloca uma sessão ainda não iniciada. Retorna NULL em caso de falha. */
SessaoJogo *criarSessao(void);

/* Visão pertencente à sessão. Retorna NULL se a sessão for NULL.
   As estruturas e seus nós são emprestados apenas para leitura e ficam
   válidos até a próxima ação, retomada do atendimento ou liberação.
   A consulta não executa buscas, verificações, operações medidas ou avanço. */
const EstadoSessao *consultarSessao(const SessaoJogo *sessao);

/* Libera a sessão e os recursos que ela possuir. Aceita NULL. */
void liberarSessao(SessaoJogo *sessao);

/* Inicia uma sessão recém-criada. O receptor de eventos é opcional.
   As operações emitem eventos em ordem, antes da próxima transição. */
SituacaoSessao iniciarAtendimento(SessaoJogo *sessao, ContextoJogo *contextoJogo,
                                 Fila *filaClientes, int *pontuacao, Dificuldade dificuldade,
                                 ReceberEventoSessao receberEvento, void *dadosEvento);

/* Aplica a ação e consome o turno, se cabível. Depois de apresentar seu
   retorno, chamar prosseguirAtendimento para chegar à próxima decisão ou
   ao resultado. Sair encerra imediatamente. */
ResultadoAcaoSessao aplicarAcaoSessao(SessaoJogo *sessao, AcaoSessao acao);

/* Retoma o laço antigo, inclusive após consulta, cancelamento ou opção
   inválida. Não chamar como parte do desenho ou da leitura do estado. */
SituacaoSessao prosseguirAtendimento(SessaoJogo *sessao);

/*
 * Contrato de propriedade do atendimento:
 * - O chamador é dono de ContextoJogo e da fila. Ambos devem permanecer
 *   válidos até a liberação da sessão. A sessão pode retirar clientes
 *   atendidos. O chamador limpa os clientes restantes após liberá-la.
 * - O chamador também é dono da pontuação. O endereço emprestado deve
 *   permanecer válido até a liberação e é atualizado pela sessão.
 * - A sessão possui a montagem e o gabarito do cliente atual, assim como
 *   os heaps e a chapa da partida. Os primeiros são liberados ao concluir
 *   cada atendimento. Os demais duram até o encerramento da partida.
 * - Na inicialização parcial, saída ou erro, liberar cada recurso próprio
 *   adquirido uma única vez e invalidar suas referências. A liberação
 *   final solta somente os recursos próprios ainda presentes. A visão de
 *   leitura não transfere propriedade e não deve ser guardada após mutação.
 *
 * Na CLI, consulta, cancelamento e opção inválida conservam a passagem de
 * volta à condição do laço. Sair encerra sem essa passagem.
 */

#endif
