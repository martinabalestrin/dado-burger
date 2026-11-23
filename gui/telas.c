#include "telas.h"
#include "componentes.h"
#include "terceiros/raygui.h"
#include <stddef.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>

static const double DURACAO_ENTREGA = 1.8;

static const char *const dificuldades[] = {
    [DIFICULDADE_FACIL] = "Fácil", [DIFICULDADE_MEDIO] = "Médio",
    [DIFICULDADE_DIFICIL] = "Difícil"
};

static void abrir(Navegacao *nav, Tela tela)
{
    if (nav->tela == TELA_HISTORICO) {
        free(nav->historicoOrdenado.partidas);
        nav->historicoOrdenado = (ConsultaHistorico){0};
    }
    nav->tela = tela;
    nav->aviso[0] = '\0';
    nav->pagina = 0;
    nav->paginaPedido = 0;
    nav->paginaEstoque = 0;
    nav->paginaChapa = 0;
    nav->selecao = SELECAO_FECHADA;
}

void encerrarPartidaGrafica(Navegacao *nav)
{
    if (nav->sessao == NULL) return;

    SituacaoSessao situacao = consultarSessao(nav->sessao)->situacao;
    if (situacao == SESSAO_EM_ATENDIMENTO) {
        situacao = aplicarAcaoSessao(nav->sessao, (AcaoSessao){.tipo = SESSAO_SAIR}).situacao;
    }
    liberarSessao(nav->sessao);
    nav->sessao = NULL;
    esvaziarFilaAtendimento(&nav->fila);
    if (situacao == SESSAO_VITORIA || situacao == SESSAO_DERROTA ||
        situacao == SESSAO_SAIDA_VOLUNTARIA) {
        salvarHistorico(nav->contexto->historico, nav->partida, nav->contexto->arquivoHistorico);
    }
}

void solicitarSaida(Navegacao *nav, Tela destino)
{
    nav->entrega = (ComemoracaoEntrega){0};
    nav->consumirSolturaEntrega = false;
    if (nav->sessao != NULL) {
        nav->confirmarSaida = true;
        nav->destinoSaida = destino;
    } else {
        abrir(nav, destino);
    }
}

static void confirmarSaida(Navegacao *nav)
{
    painel((Rectangle){350, 302, 580, 187}, CREME);
    textoTitulo("Encerrar a partida?", 640, 330, 32, true, TINTA);
    textoCentrado("A pontuação atual será enviada ao histórico.", 640, 377, 18, MADEIRA);
    if (botao((Rectangle){396, 426, 220, 42}, "Continuar", MENTA)) {
        nav->confirmarSaida = false;
    }
    if (botao((Rectangle){664, 426, 220, 42}, "Encerrar partida", VERMELHO)) {
        encerrarPartidaGrafica(nav);
        nav->confirmarSaida = false;
        abrir(nav, nav->destinoSaida);
    }
}

static void cabecalho(Navegacao *nav, const char *titulo)
{
    DrawRectangleGradientV(0, 0, 1280, 60, VERMELHO, ColorBrightness(VERMELHO, -.20f));
    DrawRectangle(0, 57, 1280, 3, COR_MOSTARDA);
    if (botao((Rectangle){24, 8, 112, 38}, "Menu", COR_MOSTARDA)) {
        solicitarSaida(nav, TELA_MENU);
    }
    textoTitulo(titulo, 158, 10, 32, false, CREME);

}

static void menu(Navegacao *nav)
{
    painel((Rectangle){430, 327, 420, 335}, CREME);
    textoTitulo("Dado Burger", 640, 344, 30, true, MADEIRA);

    const char *opcoes[] = {"Novo Jogo", "Como Jogar", "Histórico", "Sair"};
    const Tela destinos[] = {TELA_DIFICULDADE, TELA_TUTORIAL, TELA_HISTORICO, TELA_SAIR};
    for (int i = 0; i < 4; i++) {
        if (botao((Rectangle){482, 390 + i * 61, 316, 46}, opcoes[i], i == 0 ? COR_MOSTARDA : i == 3 ? VERMELHO : CREME)) {
            abrir(nav, destinos[i]);
        }
    }
}

static void dificuldade(Navegacao *nav)
{
    cabecalho(nav, "Novo Jogo");
    painel((Rectangle){164, 302, 952, 355}, CREME);
    textoTitulo("Escolha uma dificuldade", 640, 315, 34, true, TINTA);
    const Color cores[] = {MENTA, COR_MOSTARDA, VERMELHO};
    for (int i = 0; i < 3; i++) {
        float x = 194 + i * 302;
        painel((Rectangle){x, 370, 288, 255}, ColorBrightness(CREME, -.035f));
        desenharHamburguer(x + 144, 454, .53f, true);
        Dificuldade nivel = (Dificuldade)(DIFICULDADE_FACIL + i);
        ConfiguracaoDificuldade config = configuracaoDaDificuldade(nivel);
        textoCentrado(TextFormat("%d clientes", config.numClientes), x + 144, 490, 20, TINTA);
        textoCentrado(TextFormat("%d de paciência", config.paciencia), x + 144, 516, 20, TINTA);
        textoCentrado(TextFormat("Validade de %d turnos", config.validadeCarne), x + 144, 542, 20, TINTA);
        if (botao((Rectangle){x + 34, 570, 220, 42}, dificuldades[nivel], cores[i])) {
            nav->dificuldade = nivel;
            nav->partida = (Partida){0};
            nav->editandoNome = false;
            abrir(nav, TELA_NOME);
        }
    }
}

static void notificar(Navegacao *nav, const char *mensagem)
{
    if (nav->totalNotificacoes == TOTAL_NOTIFICACOES) {
        memmove(nav->notificacoes, nav->notificacoes + 1,
                (TOTAL_NOTIFICACOES - 1) * sizeof nav->notificacoes[0]);
        nav->totalNotificacoes--;
    }
    snprintf(nav->notificacoes[nav->totalNotificacoes++], sizeof nav->notificacoes[0], "%s", mensagem);
}

static void receberEvento(const EventoSessao *evento, void *dados)
{
    Navegacao *nav = dados;
    const char *nome = evento->ingrediente ? nomeDoIngrediente(evento->ingrediente) : "";
    switch (evento->tipo) {
        case SESSAO_COZIMENTO_INICIADO:
            notificar(nav, TextFormat("%s na chapa · pronto no turno %d", nome, evento->turnoPronto));
            break;
        case SESSAO_PREPARO_IMEDIATO:
            notificar(nav, TextFormat("%s preparado · disponível no estoque", nome));
            break;
        case SESSAO_INGREDIENTE_PRONTO:
            notificar(nav, TextFormat("%s pronto · transferido ao estoque", nome));
            break;
        case SESSAO_INGREDIENTE_VENCIDO:
            notificar(nav, TextFormat("%d × %s vencido · descartado", evento->quantidade, nome));
            break;
        case SESSAO_INGREDIENTE_ADICIONADO:
            if (evento->temVencimento) {
                notificar(nav, TextFormat("%s adicionado · vencia no turno %d", nome, evento->turnoVencimento));
            } else {
                notificar(nav, TextFormat("%s adicionado à montagem", nome));
            }
            break;
        case SESSAO_INGREDIENTE_REMOVIDO:
            notificar(nav, TextFormat("%s removido e descartado", nome));
            break;
        case SESSAO_PEDIDO_ENTREGUE:
            if (nav->entrega.totalCamadas > 0 && !nav->entrega.ativa) {
                nav->entrega.inicio = GetTime();
                nav->entrega.ativa = true;
            }
            notificar(nav, TextFormat("Pedido entregue · cliente %d · +1 ponto", evento->numeroCliente));
            break;
        case SESSAO_FALHA_ATENDIMENTO:
            if (evento->erro == SESSAO_FALHA_BUSCAR_PEDIDO) {
                snprintf(nav->aviso, sizeof nav->aviso, "Pedido %d não encontrado.", evento->codigoPedido);
            } else if (evento->erro == SESSAO_FALHA_INICIAR_COZIMENTO) {
                snprintf(nav->aviso, sizeof nav->aviso, "Não foi possível colocar %s na chapa.", nome);
            } else {
                snprintf(nav->aviso, sizeof nav->aviso, "Não foi possível preparar o atendimento.");
            }
            notificar(nav, nav->aviso);
            break;
    }
}

static void iniciarPartidaGrafica(Navegacao *nav)
{
    const char *nome = nav->partida.nome;
    size_t inicio = strspn(nome, " ");
    if (nome[inicio] == '\0') {
        strcpy(nav->partida.nome, "Jogador");
    } else if (inicio > 0) {
        memmove(nav->partida.nome, nome + inicio, strlen(nome + inicio) + 1);
    }

    nav->aviso[0] = '\0';
    nav->totalNotificacoes = 0;
    nav->sessao = criarSessao();
    if (nav->sessao == NULL) {
        snprintf(nav->aviso, sizeof nav->aviso, "Memória insuficiente para iniciar a partida.");
        return;
    }
    if (!montarFilaAtendimento(&nav->fila, nav->contexto, nav->dificuldade)) {
        liberarSessao(nav->sessao);
        nav->sessao = NULL;
        snprintf(nav->aviso, sizeof nav->aviso, "Não foi possível montar a fila de clientes.");
        return;
    }
    SituacaoSessao situacao = iniciarAtendimento(nav->sessao, nav->contexto, &nav->fila,
        &nav->partida.pontuacao, nav->dificuldade, receberEvento, nav);

    if (situacao == SESSAO_EM_ATENDIMENTO) {
        nav->inicioElenco = nav->proximoInicioElenco;
        nav->proximoInicioElenco = (nav->inicioElenco + 1) % TOTAL_PERSONAGENS;
        abrir(nav, TELA_PARTIDA);
    } else {
        encerrarPartidaGrafica(nav);
        if (!nav->aviso[0]) {
            snprintf(nav->aviso, sizeof nav->aviso, "Nenhum atendimento foi iniciado.");
        }
    }
}

static void nomeJogador(Navegacao *nav)
{
    cabecalho(nav, "Novo Jogo");
    if (nav->tela != TELA_NOME) return;

    painel((Rectangle){370, 324, 540, 292}, CREME);
    textoCentrado(TextFormat("Nome do jogador · %s", dificuldades[nav->dificuldade]), 640, 352, 28, TINTA);
    Rectangle campo = {440, 411, 400, 44};
    if (campoTexto(campo, nav->partida.nome, sizeof nav->partida.nome, nav->editandoNome)) {
        nav->editandoNome = !nav->editandoNome;
    }
    textoCentrado("Até 20 bytes · em branco usa Jogador", 640, 471, 17, MADEIRA);
    if (nav->aviso[0]) textoCentrado(nav->aviso, 640, 500, 18, VERMELHO);
    if (botao((Rectangle){440, 551, 174, 44}, "Voltar", CREME)) abrir(nav, TELA_DIFICULDADE);
    if (botao((Rectangle){630, 551, 210, 44}, "Iniciar partida", COR_MOSTARDA)) iniciarPartidaGrafica(nav);
}

static void tutorial(Navegacao *nav, Texture2D corpoInteiro)
{
    static const struct { const char *titulo, *linha1, *linha2, *linha3; } paginas[] = {
        {"O objetivo", "Atenda os clientes pela ordem da fila.", "Leia o pedido na prancheta", "antes de começar a montagem."},
        {"A montagem", "O pão inferior já está no prato.", "Monte de baixo para cima na ordem", "do pedido e termine com o pão superior."},
        {"O preparo", "Alguns ingredientes vêm da bancada.", "Outros precisam ser preparados", "antes de entrar no hambúrguer."},
        {"A chapa", "Carne e bacon levam 2 turnos.", "O ovo leva 1 turno. A validade", "começa quando o item fica pronto."},
        {"As ações", "Ações válidas e Aguardar gastam", "um turno de paciência. Consultar", "o pedido e cancelar não gastam turno."},
        {"O resultado", "O pedido correto é entregue na hora.", "Atenda todos os clientes antes que", "a paciência do cliente atual acabe."}
    };
    const int total = sizeof paginas / sizeof paginas[0];
    const int pagina = nav->pagina;
    cabecalho(nav, "Como Jogar");
    painel((Rectangle){150, 302, 980, 355}, MADEIRA);
    painel((Rectangle){166, 316, 948, 323}, CREME);
    DrawRectangleGradientH(631, 326, 18, 298, Fade(MADEIRA, .25f), Fade(MADEIRA, 0));
    textoTitulo(paginas[pagina].titulo, 402, 340, 32, true, VERMELHO);
    texto(paginas[pagina].linha1, 202, 410, 21, TINTA);
    texto(paginas[pagina].linha2, 202, 441, 21, TINTA);
    texto(paginas[pagina].linha3, 202, 472, 21, TINTA);
    if (pagina == 0 || pagina == 4) {
        desenharClienteInteiro(corpoInteiro, (Vector2){862, 568}, 1.36f, false);
    } else if (pagina == 2 || pagina == 3) {
        desenharChapa((Rectangle){723, 430, 278, 85}, true);
    } else {
        desenharHamburguer(865, 494, 1.05f, pagina != 1);
    }
    textoCentrado(TextFormat("%d / %d", pagina + 1, total), 640, 595, 20, TINTA);
    if (nav->tela != TELA_TUTORIAL || nav->confirmarSaida) return;
    if (nav->sessao && botao((Rectangle){1024, 8, 232, 38}, "Voltar à partida", MENTA)) {
        abrir(nav, TELA_PARTIDA);
    }
    if (pagina > 0 && botao((Rectangle){198, 576, 174, 46}, "Anterior", COR_MOSTARDA)) {
        nav->pagina--;
    }
    if (pagina < total - 1 && botao((Rectangle){912, 576, 174, 46}, "Próxima", COR_MOSTARDA)) {
        nav->pagina++;
    }
}

static void paginacao(int *pagina, int total, float centro, float y)
{
    if (*pagina >= total) *pagina = total > 0 ? total - 1 : 0;
    if (total <= 1) return;
    if (*pagina > 0 && botao((Rectangle){centro - 75, y, 34, 24}, "<", COR_MOSTARDA)) {
        (*pagina)--;
    }
    if (*pagina < total - 1 && botao((Rectangle){centro + 41, y, 34, 24}, ">", COR_MOSTARDA)) {
        (*pagina)++;
    }
    textoCentrado(TextFormat("%d / %d", *pagina + 1, total), centro, y + 4, 17, TINTA);
}

static void pedido(Navegacao *nav, const EstadoSessao *estado)
{
    enum { POR_PAGINA = 5 };
    int total = 1; // A receita exclui o pão inferior, incluído na prancheta.
    for (const NodoListaEnc *item = estado->receita->ini; item; item = item->prox) total++;

    painel((Rectangle){22, 460, 300, 184}, CREME);
    DrawRectangleRounded((Rectangle){118, 452, 108, 17}, .65f, 10, COR_MOSTARDA);
    textoTitulo(TextFormat("Pedido #%d", estado->clienteAtual.codigoDoPedido), 42, 476, 24, false, TINTA);
    paginacao(&nav->paginaPedido, (total + POR_PAGINA - 1) / POR_PAGINA, 172, 609);

    int inicio = nav->paginaPedido * POR_PAGINA;
    const NodoListaEnc *item = estado->receita->ini;
    for (int i = 0; i < total && i < inicio + POR_PAGINA; i++) {
        char ingrediente = i == 0 ? PAO_INFERIOR : item->ingrediente;
        if (i >= inicio) {
            texto(TextFormat("%d.  %s", i + 1, nomeDoIngrediente(ingrediente)),
                  43, 503 + (i - inicio) * 20, 20, TINTA);
        }
        if (i > 0) item = item->prox;
    }
}

static const char ingredientes[] = {CARNE, OVO_FRITO, BACON, QUEIJO_SUICO, QUEIJO_CHEDDAR,
    CEBOLA, TOMATE, ALFACE, PICLES, MOSTARDA, KETCHUP, PAO_SUPERIOR};
enum { TOTAL_ESTOQUE_GUI = sizeof ingredientes / sizeof ingredientes[0] };

static int quantidadeDisponivel(const EstadoSessao *estado, int indice)
{
    const MinHeap *heaps[] = {estado->carne, estado->ovo, estado->bacon};
    return indice < 3 ? heaps[indice]->tamanho : estado->estoquePreparado[indice - 3];
}

static void estoque(Navegacao *nav, const EstadoSessao *estado)
{
    enum { LINHAS = 3, LINHAS_CHAPA = 2 };
    const MinHeap *heaps[] = {estado->carne, estado->ovo, estado->bacon};

    painel((Rectangle){816, 502, 442, 142}, CREME);
    textoTitulo("Estoque", 830, 511, 23, false, TINTA);
    textoTitulo("Chapa", 1040, 511, 23, false, TINTA);
    DrawLine(1025, 518, 1025, 628, Fade(MADEIRA, .35f));
    desenharChapa((Rectangle){1040, 539, 204, 40}, false);
    if (nav->selecao == SELECAO_FECHADA) {
        paginacao(&nav->paginaEstoque, (TOTAL_ESTOQUE_GUI + LINHAS - 1) / LINHAS, 916, 609);
    }

    int inicio = nav->paginaEstoque * LINHAS;
    for (int i = inicio; i < TOTAL_ESTOQUE_GUI && i < inicio + LINHAS; i++) {
        const char *nome = nomeDoIngrediente(ingredientes[i]);
        const char *linha;
        if (i < 3) {
            int vence;
            if (proximoVencimentoHeap(heaps[i], &vence)) {
                linha = TextFormat("%s ×%d · vence T%d", nome, quantidadeDisponivel(estado, i), vence);
            } else {
                linha = TextFormat("%s ×%d", nome, quantidadeDisponivel(estado, i));
            }
        } else if (i < 7) {
            linha = TextFormat("%s ×%d", nome, quantidadeDisponivel(estado, i));
        } else {
            linha = TextFormat("%s · ilimitado", nome);
        }
        textoLimitado(linha, 830, 541 + (i - inicio) * 21, 19, 187, false, TINTA);
    }

    const FilaCozimento *chapa = estado->chapa;
    if (nav->selecao == SELECAO_FECHADA) {
        paginacao(&nav->paginaChapa, (chapa->quantidade + LINHAS_CHAPA - 1) / LINHAS_CHAPA, 1140, 613);
    }
    if (chapa->quantidade == 0) {
        texto("Chapa vazia", 1040, 585, 17, MADEIRA);
    }
    inicio = nav->paginaChapa * LINHAS_CHAPA;
    for (int i = inicio; i < chapa->quantidade && i < inicio + LINHAS_CHAPA; i++) {
        const ItemCozinhando *item = &chapa->itens[i];
        const char *nome = nomeDoIngrediente(item->simbolo);
        int faltam = item->turnoPronto - estado->turno;
        texto(TextFormat("%s · %d %s", nome, faltam, faltam == 1 ? "turno" : "turnos"),
              1040, 581 + (i - inicio) * 16, 16, TINTA);
    }
}

static int quantidadeNaChapa(const EstadoSessao *estado, char ingrediente)
{
    int total = 0;
    for (int i = 0; i < estado->chapa->quantidade; i++) {
        if (estado->chapa->itens[i].simbolo == ingrediente) total++;
    }
    return total;
}

static const char *mensagemRecusa(MotivoBloqueioSessao motivo, int naChapa)
{
    switch (motivo) {
        case SESSAO_INGREDIENTE_INVALIDO: return "Ingrediente inválido.";
        case SESSAO_PAO_INFERIOR_FIXO: return "O pão inferior é fixo.";
        case SESSAO_PREPARO_DESNECESSARIO: return "Este ingrediente já está na bancada.";
        case SESSAO_ESTOQUE_VAZIO:
        case SESSAO_ESTOQUE_PREPARADO_VAZIO:
            return naChapa ? "Ainda em cozimento · sem unidades prontas." : "Sem estoque preparado.";
        case SESSAO_MONTAGEM_FECHADA: return "Montagem fechada · remova o pão superior.";
        case SESSAO_ACAO_INVALIDA: return "Ação inválida.";
        case SESSAO_SEM_ATENDIMENTO: return "Nenhum atendimento disponível.";
        case SESSAO_SEM_BLOQUEIO: return "A sessão recusou a ação.";
    }
    return "A sessão recusou a ação.";
}

static void guardarMontagemEntregavel(Navegacao *nav, const EstadoSessao *estado)
{
    ComemoracaoEntrega *entrega = &nav->entrega;
    size_t total = 0;
    for (const NodoPilha *nodo = estado->montagem->topo; nodo; nodo = nodo->prox) {
        if (total == sizeof entrega->camadas) return;
        entrega->camadas[total++] = nodo->ingrediente;
    }
    for (size_t i = 0; i < total / 2; i++) {
        char topo = entrega->camadas[i];
        entrega->camadas[i] = entrega->camadas[total - 1 - i];
        entrega->camadas[total - 1 - i] = topo;
    }
    entrega->totalCamadas = total;
    entrega->codigoPedido = estado->clienteAtual.codigoDoPedido;
    entrega->indiceCliente = nav->inicioElenco + estado->pontuacao;
    for (const NodoFila *nodo = estado->aguardando; nodo; nodo = nodo->prox) entrega->esperando++;
}

static void executarAcaoGrafica(Navegacao *nav, AcaoSessao acao)
{
    if (nav->entrega.ativa) return;
    const EstadoSessao *antes = consultarSessao(nav->sessao);
    int pontuacaoAnterior = antes->pontuacao;
    int naChapa = acao.tipo == SESSAO_ADICIONAR ? quantidadeNaChapa(antes, acao.ingrediente) : 0;

    nav->totalNotificacoes = 0;
    ResultadoAcaoSessao retorno = aplicarAcaoSessao(nav->sessao, acao);
    if (retorno.retorno == SESSAO_ACAO_RECUSADA) {
        notificar(nav, mensagemRecusa(retorno.motivo, naChapa));
    } else if (retorno.retorno == SESSAO_ACAO_APLICADA) {
        nav->selecao = SELECAO_FECHADA;
        if (acao.tipo == SESSAO_AGUARDAR && retorno.consumiuTurno) {
            notificar(nav, "Você aguardou 1 turno.");
        }
    }

    // A ação já atualizou a pilha. A próxima chamada pode liberar a montagem.
    if (retorno.retorno == SESSAO_ACAO_APLICADA && acao.tipo == SESSAO_ADICIONAR) {
        guardarMontagemEntregavel(nav, consultarSessao(nav->sessao));
    }
    SituacaoSessao situacao = prosseguirAtendimento(nav->sessao);
    if (!nav->entrega.ativa) nav->entrega = (ComemoracaoEntrega){0};
    const EstadoSessao *atual = consultarSessao(nav->sessao);
    if (situacao != SESSAO_EM_ATENDIMENTO) {
        nav->resultado = situacao;
        encerrarPartidaGrafica(nav);
        abrir(nav, TELA_RESULTADO);
    } else if (atual->pontuacao > pontuacaoAnterior) {
        nav->paginaPedido = 0;
        notificar(nav, TextFormat("Cliente %d · pedido #%d",
                                   atual->pontuacao + 1, atual->clienteAtual.codigoDoPedido));
    }
}

static void exibirNotificacoes(const Navegacao *nav)
{
    if (nav->totalNotificacoes == 0) return;
    painel((Rectangle){24, 69, 470, 14 + 20 * nav->totalNotificacoes}, CREME);
    for (int i = 0; i < nav->totalNotificacoes; i++) {
        textoLimitado(nav->notificacoes[i], 35, 77 + i * 20, 17, 448, false, TINTA);
    }
}

static void bandejaIngredientes(Navegacao *nav, const EstadoSessao *estado)
{
    bool preparar = nav->selecao == SELECAO_PREPARAR;
    char topo;
    bool fechado = topoPilha(estado->montagem, &topo) && topo == PAO_SUPERIOR;
    painel((Rectangle){816, 326, 442, 316}, CREME);
    textoTitulo(preparar ? "Preparar ingrediente" : "Adicionar ingrediente", 831, 338, 23, false, TINTA);
    if (botao((Rectangle){1107, 334, 136, 34}, "Cancelar", CREME)) {
        nav->selecao = SELECAO_FECHADA;
        return;
    }

    int cartao = 0;
    for (int i = 0; i < TOTAL_ESTOQUE_GUI; i++) {
        char ingrediente = ingredientes[i];
        bool exigePreparo = ingredienteRequerPreparo(ingrediente);
        if (preparar && !exigePreparo) continue;

        int pronto = exigePreparo ? quantidadeDisponivel(estado, i) : 0;
        int cozinhando = quantidadeNaChapa(estado, ingrediente);
        bool disponivel = preparar || (!fechado && (!exigePreparo || pronto > 0));
        const char *status;
        if (preparar) {
            status = ingredientePerecivel(ingrediente) ? "Na chapa" : "Preparo imediato";
        } else if (exigePreparo && pronto == 0) {
            status = cozinhando ? "Em cozimento" : "Sem estoque";
        } else if (fechado) {
            status = "Montagem fechada";
        } else {
            status = exigePreparo ? TextFormat("Pronto ×%d", pronto) : "Ilimitado";
        }

        Rectangle area = {830 + 140 * (cartao % 3), 381 + 61 * (cartao / 3), 133, 49};
        bool clicar = botao(area, "", disponivel ? COR_MOSTARDA : (Color){200, 180, 155, 255});
        textoLimitado(nomeDoIngrediente(ingrediente), area.x + area.width / 2, area.y + 7, 18, area.width - 10, true, TINTA);
        textoLimitado(status, area.x + area.width / 2, area.y + 27, 15, area.width - 10, true, MADEIRA);
        if (clicar) {
            executarAcaoGrafica(nav, (AcaoSessao){
                .tipo = preparar ? SESSAO_PREPARAR : SESSAO_ADICIONAR, .ingrediente = ingrediente
            });
            return;
        }
        cartao++;
    }
}

static const char *const acoes[] = {"Preparar", "Adicionar", "Remover", "Aguardar"};

static void desenharAcoesBloqueadas(void)
{
    for (int i = 0; i < 4; i++) {
        botaoIndisponivel((Rectangle){24 + i * 166, 663, 150, 44}, acoes[i]);
    }
}

static void desenharFilaClientes(const ElencoClientes *elenco, int indiceAtual, int esperando)
{
    const Vector2 pes[] = {{793, 482}, {867, 478}, {941, 474}};
    for (int i = 0; i < 3 && i < esperando; i++) {
        const SpritesCliente *aguardando = obterSpritesCliente(elenco, indiceAtual + i + 1);
        // Os PNGs dos clientes 06 e 07 já olham para a esquerda.
        bool espelhar = aguardando != &elenco->personagens[5] && aguardando != &elenco->personagens[6];
        desenharClienteInteiro(aguardando->corpoInteiro, pes[i], 1.30f * (1.0f - i * .04f), espelhar);
    }
}

static void desenharComemoracao(const ComemoracaoEntrega *entrega, double decorrido, Texture2D balcao,
                               const ElencoClientes *elenco, const IngredientesGraficos *ingredientes)
{
    desenharFilaClientes(elenco, entrega->indiceCliente, entrega->esperando);
    const SpritesCliente *cliente = obterSpritesCliente(elenco, entrega->indiceCliente);
    desenharClienteMeioCorpo(cliente->meioCorpo[EXPRESSAO_FELIZ], (Vector2){411, 518});
    desenharBalcao(balcao);

    const float entrada = fminf(1, (float)(decorrido / .36));
    const float saida = fminf(1, (float)((DURACAO_ENTREGA - decorrido) / .40));
    const float opacidade = fminf(entrada, saida);
    const float opacidadeBrilhos = fminf(saida, fmaxf(0, (float)((decorrido - .20) / .20)));
    const float ampliacao = 1 + .08f * sinf(entrada * 1.5707963f) * saida;
    desenharCamadasMontagem((Vector2){600, 537 - 4 * sinf(entrada * 1.5707963f) * saida},
                            entrega->camadas, entrega->totalCamadas, ampliacao, ingredientes);

    const Vector2 brilhos[] = {{466, 394}, {730, 368}, {744, 475}, {459, 451}, {699, 331}};
    for (size_t i = 0; i < sizeof brilhos / sizeof *brilhos; i++) {
        Vector2 p = brilhos[i];
        float raio = 7 + 3 * sinf((float)decorrido * 8 + i);
        Color dourado = Fade(COR_MOSTARDA, opacidadeBrilhos);
        DrawLineEx((Vector2){p.x - raio, p.y}, (Vector2){p.x + raio, p.y}, 3, dourado);
        DrawLineEx((Vector2){p.x, p.y - raio}, (Vector2){p.x, p.y + raio}, 3, dourado);
    }

    DrawRectangleRounded((Rectangle){446, 575, 308, 67}, .30f, 10, Fade(CREME, .93f * opacidade));
    textoTitulo("Pedido entregue!", 600, 578, 28, true, Fade(TINTA, opacidade));
    textoTitulo("+1 ponto!", 600, 610, 22, true, Fade((Color){54, 96, 56, 255}, opacidadeBrilhos));
    DrawRectangleGradientV(0, 650, 1280, 70, TINTA, MADEIRA);
    desenharAcoesBloqueadas();
    textoCentrado("Clique para pular", 1128, 671, 18, CREME);

    painel((Rectangle){22, 460, 300, 184}, CREME);
    textoTitulo(TextFormat("Pedido #%d", entrega->codigoPedido), 42, 476, 24, false, TINTA);
    for (size_t i = 0; i < entrega->totalCamadas; i++) {
        textoLimitado(nomeDoIngrediente(entrega->camadas[i]), 43, 505 + i * 16, 16, 257, false, TINTA);
    }

    painel((Rectangle){816, 502, 442, 142}, CREME);
    textoTitulo("Estoque", 830, 511, 23, false, TINTA);
    textoTitulo("Chapa", 1040, 511, 23, false, TINTA);
    DrawLine(1025, 518, 1025, 628, Fade(MADEIRA, .35f));
    desenharChapa((Rectangle){1040, 539, 204, 40}, false);
}

static void partida(Navegacao *nav, Texture2D balcao, const ElencoClientes *elenco,
                    const IngredientesGraficos *ingredientes)
{
    const EstadoSessao *estado = consultarSessao(nav->sessao);
    int esperando = 0;
    for (const NodoFila *item = estado->aguardando; item; item = item->prox) esperando++;

    const int indiceAtual = nav->inicioElenco + estado->pontuacao;
    desenharFilaClientes(elenco, indiceAtual, esperando);
    int paciencia = estado->clienteAtual.paciencia;
    float proporcao = (float)paciencia / estado->pacienciaInicial;
    ExpressaoCliente expressao = proporcao <= .30f ? EXPRESSAO_IRRITADA :
                                proporcao <= .60f ? EXPRESSAO_IMPACIENTE : EXPRESSAO_FELIZ;
    const SpritesCliente *cliente = obterSpritesCliente(elenco, indiceAtual);
    desenharClienteMeioCorpo(cliente->meioCorpo[expressao], (Vector2){411, 518});
    desenharBalcao(balcao);

    pedido(nav, estado);
    estoque(nav, estado);
    desenharMontagem((Vector2){600, 537}, estado->montagem, ingredientes);
    char topo;
    painel((Rectangle){446, 579, 308, 35}, CREME);
    textoCentrado(topoPilha(estado->montagem, &topo)
                  ? TextFormat("Topo: %s", nomeDoIngrediente(topo)) : "Montagem vazia",
                  600, 587, 19, TINTA);

    painel((Rectangle){327, 258, 160, 46}, CREME);
    texto("Paciência", 337, 264, 19, TINTA);
    texto(TextFormat("%d / %d", paciencia, estado->pacienciaInicial), 421, 264, 19, TINTA);
    DrawRectangleRounded((Rectangle){337, 290, 140, 7}, .8f, 8, Fade(MADEIRA, .25f));
    DrawRectangleRounded((Rectangle){337, 290, 140 * proporcao, 7}, .8f, 8, MENTA);
    painel((Rectangle){785, 248, 187, 36}, CREME);
    textoCentrado(TextFormat("Aguardando: %d", esperando), 878, 256, 19, TINTA);
    if (esperando > 3) {
        painel((Rectangle){976, 407, 42, 30}, CREME);
        textoCentrado(TextFormat("+%d", esperando - 3), 997, 413, 18, TINTA);
    }

    cabecalho(nav, "Dado Burger");
    textoLimitado(TextFormat("Turno %d   ·   Pontuação %d   ·   %s   ·   Cliente %d/%d",
                            estado->turno, estado->pontuacao, dificuldades[estado->dificuldade],
                            estado->pontuacao + 1, estado->totalClientes),
                 365, 15, 23, 622, false, CREME);
    textoLimitado(nav->partida.nome, 1132, 19, 19, 235, true, CREME);

    DrawRectangleGradientV(0, 650, 1280, 70, TINTA, MADEIRA);
    exibirNotificacoes(nav);
    if (nav->confirmarSaida) return;
    if (nav->selecao != SELECAO_FECHADA) {
        desenharAcoesBloqueadas();
        bandejaIngredientes(nav, estado);
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (!botao((Rectangle){24 + i * 166, 663, 150, 44}, acoes[i], COR_MOSTARDA)) continue;
        switch (i) {
            case 0: nav->selecao = SELECAO_PREPARAR; break;
            case 1: nav->selecao = SELECAO_ADICIONAR; break;
            case 2: executarAcaoGrafica(nav, (AcaoSessao){.tipo = SESSAO_REMOVER}); break;
            case 3: executarAcaoGrafica(nav, (AcaoSessao){.tipo = SESSAO_AGUARDAR}); break;
        }
        return;
    }
    if (botao((Rectangle){1099, 663, 157, 44}, "Ajuda", CREME)) {
        abrir(nav, TELA_TUTORIAL);
    }
}

static void resultado(Navegacao *nav)
{
    cabecalho(nav, "Resultado");
    painel((Rectangle){323, 325, 634, 290}, CREME);

    bool vitoria = nav->resultado == SESSAO_VITORIA;
    const char *titulo = vitoria ? "Vitória!" :
                         nav->resultado == SESSAO_DERROTA ? "Derrota" : "Atendimento interrompido";
    textoTitulo(titulo, 640, 357, 36, true, vitoria ? ColorBrightness(MENTA, -.34f) : VERMELHO);
    textoCentrado(TextFormat("%s · Pontuação: %d", nav->partida.nome, nav->partida.pontuacao),
                  640, 429, 25, TINTA);
    if (nav->totalNotificacoes > 0) {
        textoCentrado(nav->notificacoes[nav->totalNotificacoes - 1], 640, 477, 18, MADEIRA);
    }
    if (botao((Rectangle){480, 535, 320, 52}, "Voltar ao menu", COR_MOSTARDA)) {
        abrir(nav, TELA_MENU);
    }
}

static void ordenarHistoricoGrafico(Navegacao *nav, CriterioOrdenacao criterio)
{
    free(nav->historicoOrdenado.partidas);
    nav->historicoOrdenado = consultarHistoricoOrdenado(nav->contexto, criterio);
    nav->criterioHistorico = criterio;
    nav->pagina = 0;
}

static void historico(Navegacao *nav)
{
    cabecalho(nav, "Histórico");
    if (nav->tela != TELA_HISTORICO) return;

    painel((Rectangle){228, 302, 824, 360}, CREME);
    textoTitulo("Histórico de partidas", 640, 313, 35, true, VERMELHO);
    const ListaContDin *lista = nav->contexto->historico;
    if (lista->tamanho == 0) {
        textoCentrado("Nenhuma partida registrada.", 640, 425, 22, MADEIRA);
        return;
    }
    bool ordenado = nav->historicoOrdenado.partidas != NULL;
    Color corAlfa = ordenado && nav->criterioHistorico == ORDEM_ALFABETICA ? MENTA : COR_MOSTARDA;
    Color corPontos = ordenado && nav->criterioHistorico == ORDEM_PONTUACAO_DECRESCENTE ? MENTA : COR_MOSTARDA;
    if (botao((Rectangle){365, 359, 260, 39}, "Ordem alfabética", corAlfa)) {
        ordenarHistoricoGrafico(nav, ORDEM_ALFABETICA);
    } else if (botao((Rectangle){655, 359, 260, 39}, "Maior pontuação", corPontos)) {
        ordenarHistoricoGrafico(nav, ORDEM_PONTUACAO_DECRESCENTE);
    }

    const ConsultaHistorico *consulta = &nav->historicoOrdenado;
    ordenado = consulta->partidas != NULL;
    const char *ordem = "Ordem do arquivo";
    Color corOrdem = MADEIRA;
    if (ordenado) {
        ordem = nav->criterioHistorico == ORDEM_ALFABETICA ? "Ordem alfabética" : "Pontuação decrescente";
    } else if (consulta->situacao == HISTORICO_FALHA_COPIA) {
        ordem = "Falha ao copiar · ordem do arquivo";
        corOrdem = VERMELHO;
    } else if (consulta->situacao == HISTORICO_FALHA_ORDENACAO) {
        ordem = "Falha ao ordenar · ordem do arquivo";
        corOrdem = VERMELHO;
    }
    texto("Jogador", 288, 407, 20, MADEIRA);
    textoCentrado(ordem, 640, 409, 16, corOrdem);
    texto("Pontuação", 858, 407, 20, MADEIRA);
    enum { POR_PAGINA = 4 };
    int total = ordenado ? consulta->total : lista->tamanho;
    paginacao(&nav->pagina, (total - 1) / POR_PAGINA + 1, 640, 625);
    int inicio = nav->pagina * POR_PAGINA;
    for (int i = inicio; i < total && i - inicio < POR_PAGINA; i++) {
        Partida registro = ordenado ? consulta->partidas[i] : acessarLCD(lista, i);
        float y = 436 + 45 * (i - inicio);
        DrawRectangleRounded((Rectangle){275, y, 730, 38}, .35f, 10, Fade(COR_MOSTARDA, .28f));
        textoLimitado(registro.nome, 291, y + 6, 23, 535, false, TINTA);
        textoCentrado(TextFormat("%d", registro.pontuacao), 914, y + 6, 23, TINTA);
    }
}

void desenharTela(Navegacao *nav, Texture2D salao, Texture2D balcao,
                  const ElencoClientes *elenco, const IngredientesGraficos *ingredientes)
{
    desenharSalao(salao);
    if (nav->entrega.ativa) {
        double decorrido = GetTime() - nav->entrega.inicio;
        if (decorrido < DURACAO_ENTREGA && !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            desenharComemoracao(&nav->entrega, decorrido, balcao, elenco, ingredientes);
            return;
        }
        nav->entrega = (ComemoracaoEntrega){0};
        nav->consumirSolturaEntrega = true;
    }
    if (nav->confirmarSaida) {
        confirmarSaida(nav);
        return;
    }

    // Bloqueia a interface no quadro de término e até consumir uma soltura pendente.
    const bool bloquear = nav->consumirSolturaEntrega;
    if (bloquear) GuiLock();
    switch (nav->tela) {
        case TELA_MENU: menu(nav); break;
        case TELA_DIFICULDADE: dificuldade(nav); break;
        case TELA_NOME: nomeJogador(nav); break;
        case TELA_TUTORIAL: tutorial(nav, elenco->personagens[0].corpoInteiro); break;
        case TELA_PARTIDA: partida(nav, balcao, elenco, ingredientes); break;
        case TELA_HISTORICO: historico(nav); break;
        case TELA_RESULTADO: resultado(nav); break;
        case TELA_SAIR: break;
    }
    if (bloquear) {
        GuiUnlock();
        nav->consumirSolturaEntrega = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    }
}
