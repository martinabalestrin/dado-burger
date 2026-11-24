#include "sessao.h"

#include <stdlib.h>

struct SessaoJogo {
    EstadoSessao estado;
    ContextoJogo *contextoJogo;
    Fila *filaClientes;
    int *pontuacao;
    ReceberEventoSessao receberEvento;
    void *dadosEvento;
    PilhaEnc *hamburguer;
    ListaEnc *hamburguerGabarito;
    MinHeap carne, ovo, bacon;
    FilaCozimento cozimento;
    int estoqueNaoPerecivel[4];
    int ultimoTurnoVerificado;
    int aguardandoProsseguir;
};

static void emitir(SessaoJogo *sessao, EventoSessao evento)
{
    evento.turno = sessao->estado.turno;
    if (sessao->receberEvento != NULL) {
        sessao->receberEvento(&evento, sessao->dadosEvento);
    }
}

// ================= FUNÇÕES AUXILIARES DE COZIMENTO =================

// Retorna o tempo de cozimento em turnos de cada ingrediente perecível
static int tempoCozimentoIngrediente(char simbolo) {
    switch (simbolo) {
        case CARNE:     return 2;
        case OVO_FRITO: return 1;
        case BACON:     return 2;
        default:        return 0;
    }
}

static int inicializarCozimento(FilaCozimento *fc) {
    fc->capacidade = 8;
    fc->quantidade = 0;
    fc->itens = (ItemCozinhando *) malloc(fc->capacidade * sizeof(ItemCozinhando));
    return fc->itens != NULL;
}

static void destruirCozimento(FilaCozimento *fc) {
    if (fc != NULL) {
        free(fc->itens);
        fc->itens = NULL;
        fc->quantidade = 0;
        fc->capacidade = 0;
    }
}

// Coloca um perecível na chapa; ele ficará pronto em (turnoAtual + tempoCozimento)
static int iniciarCozimento(FilaCozimento *fc, char simbolo, int turnoAtual) {
    int tempo = tempoCozimentoIngrediente(simbolo);
    if (tempo <= 0) return 0;

    if (fc->quantidade == fc->capacidade) {
        int novaCap = fc->capacidade * 2;
        ItemCozinhando *temp = (ItemCozinhando *) realloc(fc->itens, novaCap * sizeof(ItemCozinhando));
        if (temp == NULL) return 0;
        fc->itens = temp;
        fc->capacidade = novaCap;
    }

    fc->itens[fc->quantidade].simbolo = simbolo;
    fc->itens[fc->quantidade].turnoPronto = turnoAtual + tempo;
    fc->quantidade++;
    return 1;
}

// Verifica no início de cada turno quais itens terminaram de cozinhar e os transfere para o estoque (MinHeap).
// Como inserirHeap recebe 'turnoAtual', o prazo de validade começa a contar somente a partir deste momento.
// Retorna 1 se todas as transferências ocorreram com sucesso, ou 0 em caso de falha de memória.
static int atualizarCozimento(SessaoJogo *sessao, FilaCozimento *fc, MinHeap *carne, MinHeap *ovo, MinHeap *bacon, int turnoAtual) {
    int i = 0;
    while (i < fc->quantidade) {
        if (turnoAtual >= fc->itens[i].turnoPronto) {
            char simb = fc->itens[i].simbolo;
            int inseriu = 0;

            if (simb == CARNE)          inseriu = inserirHeap(carne, turnoAtual);
            else if (simb == OVO_FRITO) inseriu = inserirHeap(ovo, turnoAtual);
            else if (simb == BACON)     inseriu = inserirHeap(bacon, turnoAtual);

            if (!inseriu) {
                emitir(sessao, (EventoSessao){.tipo = SESSAO_FALHA_ATENDIMENTO,
                                         .erro = SESSAO_FALHA_TRANSFERIR_COZIMENTO, .ingrediente = simb});
                return 0; // Interrompe imediatamente sem descartar silenciosamente e sinaliza erro crítico
            }

            emitir(sessao, (EventoSessao){.tipo = SESSAO_INGREDIENTE_PRONTO,
                                     .ingrediente = simb, .quantidade = 1});

            // Só remove da lista de cozimento após a confirmação de inserção no MinHeap
            for (int j = i; j < fc->quantidade - 1; j++) {
                fc->itens[j] = fc->itens[j + 1];
            }
            fc->quantidade--;
        } else {
            i++;
        }
    }

    return 1;
}

// ================= FUNÇÕES AUXILIARES DE ESTOQUE E SELEÇÃO =================

// mapeia os 4 ingredientes não-perecíveis que exigem preparo ('s', 'q', 'c', 't')
static int indiceNaoPerecivel(char simbolo){
    switch(simbolo){
        case QUEIJO_SUICO:   
            return 0;
        case QUEIJO_CHEDDAR: 
            return 1;
        case CEBOLA:         
            return 2;
        case TOMATE:         
            return 3;
        default:             
            return -1;
    }
}

// Paciência ajustada para comportar o tempo de cozimento de receitas com múltiplos perecíveis
ConfiguracaoDificuldade configuracaoDaDificuldade(Dificuldade dificuldade){
    switch(dificuldade){
        case DIFICULDADE_FACIL:
            return (ConfiguracaoDificuldade){3, 18, 5, 5, 5};
        case DIFICULDADE_DIFICIL:
            return (ConfiguracaoDificuldade){7, 13, 2, 2, 2};
        case DIFICULDADE_MEDIO:
        default:
            return (ConfiguracaoDificuldade){5, 15, 3, 3, 3};
    }
}

static void avancarTurno(int *turno, Cliente *cliente){
    cliente->paciencia--;
    (*turno)++;
}

static void destruirEstoques(MinHeap *carne, MinHeap *ovo, MinHeap *bacon, FilaCozimento *cozimento){
    destruirHeap(carne);
    destruirHeap(ovo);
    destruirHeap(bacon);
    destruirCozimento(cozimento);
}

static void destruirMontagem(PilhaEnc *hamburguer, ListaEnc *gabarito){
    if(hamburguer != NULL){
        destruirPilha(hamburguer);
        free(hamburguer);
    }
    if(gabarito != NULL){
        destruirListaEnc(gabarito);
        free(gabarito);
    }
}

SessaoJogo *criarSessao(void)
{
    SessaoJogo *sessao = calloc(1, sizeof *sessao);
    if (sessao != NULL) {
        sessao->estado = (EstadoSessao){.situacao = SESSAO_NAO_INICIADA};
    }
    return sessao;
}

const EstadoSessao *consultarSessao(const SessaoJogo *sessao)
{
    if (sessao == NULL) return NULL;
    return &sessao->estado;
}

static void limparPedido(SessaoJogo *sessao)
{
    destruirMontagem(sessao->hamburguer, sessao->hamburguerGabarito);
    sessao->hamburguer = NULL;
    sessao->hamburguerGabarito = NULL;
    sessao->estado.montagem = NULL;
    sessao->estado.receita = NULL;
}

static SituacaoSessao encerrarAtendimento(SessaoJogo *sessao, SituacaoSessao situacao)
{
    limparPedido(sessao);
    destruirEstoques(&sessao->carne, &sessao->ovo, &sessao->bacon, &sessao->cozimento);
    sessao->estado.carne = NULL;
    sessao->estado.ovo = NULL;
    sessao->estado.bacon = NULL;
    sessao->estado.estoquePreparado = NULL;
    sessao->estado.chapa = NULL;
    sessao->estado.aguardando = NULL;
    sessao->estado.situacao = situacao;
    return situacao;
}

void liberarSessao(SessaoJogo *sessao)
{
    if (sessao != NULL && sessao->estado.situacao == SESSAO_EM_ATENDIMENTO) {
        encerrarAtendimento(sessao, SESSAO_SAIDA_VOLUNTARIA);
    }
    free(sessao);
}

static SituacaoSessao falharAtendimento(SessaoJogo *sessao, ErroSessao erro)
{
    emitir(sessao, (EventoSessao){.tipo = SESSAO_FALHA_ATENDIMENTO, .erro = erro,
                                 .codigoPedido = sessao->estado.clienteAtual.codigoDoPedido});
    return encerrarAtendimento(sessao, SESSAO_ERRO);
}

static ErroSessao abrirCliente(SessaoJogo *sessao)
{
    EstadoSessao *estado = &sessao->estado;
    ContextoJogo *contextoJogo = sessao->contextoJogo;
    const NodoFila *frente = sessao->filaClientes->frente;
    Receita receita;

    estado->clienteAtual = frente->dados;
    estado->aguardando = frente->prox;
    if (buscarPedido(contextoJogo->pedidosCache, contextoJogo->pedidos,
                     estado->clienteAtual.codigoDoPedido, &receita,
                     contextoJogo->metricasAvl) != 1) {
        return SESSAO_FALHA_BUSCAR_PEDIDO;
    }

    sessao->hamburguer = malloc(sizeof *sessao->hamburguer);
    if (sessao->hamburguer == NULL) return SESSAO_FALHA_ALOCAR_MONTAGEM;

    if (!iniciarMontagemHamburguer(sessao->hamburguer)) {
        return SESSAO_FALHA_INICIAR_MONTAGEM;
    }

    sessao->hamburguerGabarito = criarPedido(receita);
    if (sessao->hamburguerGabarito == NULL) return SESSAO_FALHA_CRIAR_GABARITO;

    estado->montagem = sessao->hamburguer;
    estado->receita = sessao->hamburguerGabarito;
    return SESSAO_SEM_ERRO;
}

static void descartarVencidos(SessaoJogo *sessao, MinHeap *heap, char ingrediente)
{
    int descartados = descartarVencidosHeap(heap, sessao->estado.turno);
    if (descartados > 0) {
        emitir(sessao, (EventoSessao){.tipo = SESSAO_INGREDIENTE_VENCIDO,
                                     .ingrediente = ingrediente, .quantidade = descartados});
    }
}

SituacaoSessao prosseguirAtendimento(SessaoJogo *sessao)
{
    if (sessao == NULL) return SESSAO_ERRO;
    EstadoSessao *estado = &sessao->estado;
    while (estado->situacao == SESSAO_EM_ATENDIMENTO) {
        if (sessao->hamburguerGabarito == NULL) {
            if (estaVaziaFila(sessao->filaClientes)) {
                return encerrarAtendimento(sessao, SESSAO_VITORIA);
            }
            ErroSessao erro = abrirCliente(sessao);
            if (erro != SESSAO_SEM_ERRO) return falharAtendimento(sessao, erro);
        }

        // Reavalia a condição do laço original antes de oferecer outra opção.
        if (!verificarPedido(sessao->hamburguer, sessao->hamburguerGabarito)
            && estado->clienteAtual.paciencia > 0) {
            if (estado->turno != sessao->ultimoTurnoVerificado) {
                if (!atualizarCozimento(sessao, &sessao->cozimento, &sessao->carne,
                                       &sessao->ovo, &sessao->bacon, estado->turno)) {
                    return encerrarAtendimento(sessao, SESSAO_ERRO);
                }
                descartarVencidos(sessao, &sessao->carne, CARNE);
                descartarVencidos(sessao, &sessao->ovo, OVO_FRITO);
                descartarVencidos(sessao, &sessao->bacon, BACON);
                sessao->ultimoTurnoVerificado = estado->turno;
            }
            sessao->aguardandoProsseguir = 0;
            return estado->situacao;
        }

        // O laço original confirma a entrega mesmo se a paciência chegou a zero.
        int pedidoConcluido = verificarPedido(sessao->hamburguer, sessao->hamburguerGabarito);
        limparPedido(sessao);
        if (!pedidoConcluido) return encerrarAtendimento(sessao, SESSAO_DERROTA);

        emitir(sessao, (EventoSessao){.tipo = SESSAO_PEDIDO_ENTREGUE,
                                     .numeroCliente = *sessao->pontuacao + 1});
        dequeueFila(sessao->filaClientes, &estado->clienteAtual);
        (*sessao->pontuacao)++;
        estado->pontuacao = *sessao->pontuacao;
    }
    return estado->situacao;
}

SituacaoSessao iniciarAtendimento(SessaoJogo *sessao, ContextoJogo *contextoJogo,
                                 Fila *filaClientes, int *pontuacao, Dificuldade dificuldade,
                                 ReceberEventoSessao receberEvento, void *dadosEvento)
{
    if (sessao == NULL || contextoJogo == NULL || filaClientes == NULL || pontuacao == NULL
        || sessao->estado.situacao != SESSAO_NAO_INICIADA) return SESSAO_ERRO;

    ConfiguracaoDificuldade config = configuracaoDaDificuldade(dificuldade);
    sessao->contextoJogo = contextoJogo;
    sessao->filaClientes = filaClientes;
    sessao->pontuacao = pontuacao;
    sessao->receberEvento = receberEvento;
    sessao->dadosEvento = dadosEvento;
    sessao->estado = (EstadoSessao){
        .situacao = SESSAO_EM_ATENDIMENTO, .dificuldade = dificuldade,
        .turno = 1, .pontuacao = *pontuacao,
        .totalClientes = *pontuacao, .pacienciaInicial = config.paciencia,
        .carne = &sessao->carne, .ovo = &sessao->ovo, .bacon = &sessao->bacon,
        .estoquePreparado = sessao->estoqueNaoPerecivel, .chapa = &sessao->cozimento
    };
    for (const NodoFila *cliente = filaClientes->frente; cliente != NULL; cliente = cliente->prox) {
        sessao->estado.totalClientes++;
    }

    if (!inicializarHeap(&sessao->carne, config.validadeCarne)
        || !inicializarHeap(&sessao->ovo, config.validadeOvo)
        || !inicializarHeap(&sessao->bacon, config.validadeBacon)
        || !inicializarCozimento(&sessao->cozimento)) {
        return falharAtendimento(sessao, SESSAO_FALHA_INICIALIZAR_ESTOQUES);
    }
    return prosseguirAtendimento(sessao);
}

static void prepararIngrediente(SessaoJogo *sessao, char ing, ResultadoAcaoSessao *resultado)
{
    if (!ingredienteRequerPreparo(ing)) {
        resultado->motivo = SESSAO_PREPARO_DESNECESSARIO;
        return;
    }

    if (ingredientePerecivel(ing)) {
        if (!iniciarCozimento(&sessao->cozimento, ing, sessao->estado.turno)) {
            emitir(sessao, (EventoSessao){.tipo = SESSAO_FALHA_ATENDIMENTO,
                                         .erro = SESSAO_FALHA_INICIAR_COZIMENTO, .ingrediente = ing});
            resultado->retorno = SESSAO_ACAO_FALHOU;
            return;
        }
        int tempo = tempoCozimentoIngrediente(ing);
        emitir(sessao, (EventoSessao){.tipo = SESSAO_COZIMENTO_INICIADO, .ingrediente = ing,
                                     .turnoPronto = sessao->estado.turno + tempo, .quantidade = 1});
    } else {
        int idx = indiceNaoPerecivel(ing);
        if (idx == -1) {
            resultado->motivo = SESSAO_INGREDIENTE_INVALIDO;
            return;
        }
        sessao->estoqueNaoPerecivel[idx]++;
        emitir(sessao, (EventoSessao){.tipo = SESSAO_PREPARO_IMEDIATO, .ingrediente = ing, .quantidade = 1});
    }
    resultado->consumiuTurno = 1;
}

static void adicionarIngrediente(SessaoJogo *sessao, char ing, ResultadoAcaoSessao *resultado)
{
    MinHeap *heapAlvo = NULL;
    int idx = -1;
    if (ingredienteRequerPreparo(ing)) {
        if (ingredientePerecivel(ing)) {
            if (ing == CARNE)          heapAlvo = &sessao->carne;
            else if (ing == OVO_FRITO) heapAlvo = &sessao->ovo;
            else if (ing == BACON)     heapAlvo = &sessao->bacon;

            if (heapAlvo == NULL || estaVazioHeap(heapAlvo)) {
                resultado->motivo = SESSAO_ESTOQUE_VAZIO;
                return;
            }
        } else {
            idx = indiceNaoPerecivel(ing);
            if (idx == -1 || sessao->estoqueNaoPerecivel[idx] <= 0) {
                resultado->motivo = SESSAO_ESTOQUE_PREPARADO_VAZIO;
                return;
            }
        }
    }

    // Só consome o estoque depois de empilhar com sucesso.
    if (!adicionarIngredienteHamburguer(sessao->hamburguer, ing)) {
        resultado->motivo = SESSAO_MONTAGEM_FECHADA;
        return;
    }

    EventoSessao evento = {.tipo = SESSAO_INGREDIENTE_ADICIONADO, .ingrediente = ing, .quantidade = 1};
    if (heapAlvo != NULL) {
        removerHeap(heapAlvo, &evento.turnoVencimento);
        evento.temVencimento = 1;
    } else if (idx != -1) {
        sessao->estoqueNaoPerecivel[idx]--;
    }
    emitir(sessao, evento);
    resultado->consumiuTurno = 1;
}

ResultadoAcaoSessao aplicarAcaoSessao(SessaoJogo *sessao, AcaoSessao acao)
{
    ResultadoAcaoSessao resultado = {.retorno = SESSAO_ACAO_RECUSADA,
        .situacao = sessao == NULL ? SESSAO_ERRO : sessao->estado.situacao};

    if (resultado.situacao != SESSAO_EM_ATENDIMENTO || sessao->aguardandoProsseguir) {
        resultado.motivo = SESSAO_SEM_ATENDIMENTO;
        return resultado;
    }
    sessao->aguardandoProsseguir = 1;

    char ing = acao.ingrediente;
    switch (acao.tipo) {
        case SESSAO_PREPARAR:
        case SESSAO_ADICIONAR:
            if (!ingredienteValido(ing) || ing == PAO_INFERIOR) {
                resultado.motivo = SESSAO_INGREDIENTE_INVALIDO;
            } else if (acao.tipo == SESSAO_PREPARAR) {
                prepararIngrediente(sessao, ing, &resultado);
            } else {
                adicionarIngrediente(sessao, ing, &resultado);
            }
            break;
        case SESSAO_REMOVER: {
            char removido;
            if (removerIngredienteHamburguer(sessao->hamburguer, &removido)) {
                emitir(sessao, (EventoSessao){.tipo = SESSAO_INGREDIENTE_REMOVIDO,
                                             .ingrediente = removido, .quantidade = 1});
                resultado.consumiuTurno = 1;
            } else {
                resultado.motivo = SESSAO_PAO_INFERIOR_FIXO;
            }
            break;
        }
        case SESSAO_AGUARDAR:
            resultado.consumiuTurno = 1;
            break;
        case SESSAO_SAIR:
            resultado.situacao = encerrarAtendimento(sessao, SESSAO_SAIDA_VOLUNTARIA);
            resultado.retorno = SESSAO_ACAO_APLICADA;
            return resultado;
        default:
            resultado.motivo = SESSAO_ACAO_INVALIDA;
            break;
    }

    if (resultado.consumiuTurno) {
        avancarTurno(&sessao->estado.turno, &sessao->estado.clienteAtual);
        resultado.retorno = SESSAO_ACAO_APLICADA;
    }
    return resultado;
}
