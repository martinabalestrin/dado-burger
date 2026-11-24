#include "menu.h"
#include "../jogo/aplicacao.h"

void visualizarEstoque(const MinHeap *carne, const MinHeap *ovo, const MinHeap *bacon, const int estoquePreparados[4], const FilaCozimento *cozimento, int turnoAtual){
    int proxVenc;

    printf("\n==================== ESTOQUE DA COZINHA ====================\n");
    printf("[COZINHANDO NA CHAPA]\n");
    if (cozimento == NULL || cozimento->quantidade == 0) {
        printf("  (Nenhum ingrediente cozinhando no momento)\n");
    } else {
        for (int i = 0; i < cozimento->quantidade; i++) {
            int faltam = cozimento->itens[i].turnoPronto - turnoAtual;
            if(faltam > 0){
                printf("  - (%c) %-15s: pronto no turno %d (falta %d turno%s)\n",
                    cozimento->itens[i].simbolo,
                    nomeDoIngrediente(cozimento->itens[i].simbolo),
                    cozimento->itens[i].turnoPronto,
                    faltam,
                    faltam == 1 ? "" : "s");
            } else {
                printf("  - (%c) %-15s: PRONTO NA CHAPA (aguardando transferência ao estoque)\n",
                       cozimento->itens[i].simbolo,
                       nomeDoIngrediente(cozimento->itens[i].simbolo));
            }
        }
    }

    printf("\n[PERECIVEIS PRONTOS NO ESTOQUE]\n");

    printf("  - (%c) %-15s: %d unid.", CARNE, nomeDoIngrediente(CARNE), carne->tamanho);
    if(proximoVencimentoHeap(carne, &proxVenc)){
        printf(" (prox. vence no turno %d)", proxVenc);
    }
    printf("\n");

    printf("  - (%c) %-15s: %d unid.", OVO_FRITO, nomeDoIngrediente(OVO_FRITO), ovo->tamanho);
    if(proximoVencimentoHeap(ovo, &proxVenc)){
        printf(" (prox. vence no turno %d)", proxVenc);
    }
    printf("\n");

    printf("  - (%c) %-15s: %d unid.", BACON, nomeDoIngrediente(BACON), bacon->tamanho);
    if(proximoVencimentoHeap(bacon, &proxVenc)){
        printf(" (prox. vence no turno %d)", proxVenc);
    }
    printf("\n");

    printf("\n[NAO-PERECIVEIS PREPARADOS]\n");
    printf("  - (%c) %-15s: %d unid.\n", QUEIJO_SUICO, nomeDoIngrediente(QUEIJO_SUICO), estoquePreparados[0]);
    printf("  - (%c) %-15s: %d unid.\n", QUEIJO_CHEDDAR, nomeDoIngrediente(QUEIJO_CHEDDAR), estoquePreparados[1]);
    printf("  - (%c) %-15s: %d unid.\n", CEBOLA, nomeDoIngrediente(CEBOLA), estoquePreparados[2]);
    printf("  - (%c) %-15s: %d unid.\n", TOMATE, nomeDoIngrediente(TOMATE), estoquePreparados[3]);

    printf("\n[PRONTOS PARA USO (BANCADA ILIMITADA)]\n");
    printf("  - (%c) %s | (%c) %s | (%c) %s | (%c) %s | (%c) %s\n",
           PAO_SUPERIOR, nomeDoIngrediente(PAO_SUPERIOR),
           KETCHUP, nomeDoIngrediente(KETCHUP),
           MOSTARDA, nomeDoIngrediente(MOSTARDA),
           PICLES, nomeDoIngrediente(PICLES),
           ALFACE, nomeDoIngrediente(ALFACE));
    printf("============================================================\n");
}

char selecionarIngrediente(int apenasPreparaveis){
    char simbolo;
    int c;

    printf("\nIngredientes disponiveis:\n");
    if(apenasPreparaveis){
        printf("  Pereciveis (cozimento): (n) carne [2t] | (o) ovo frito [1t] | (b) bacon [2t]\n");
        printf("  Nao-pereciveis: (s) queijo suico | (q) queijo cheddar | (c) cebola | (t) tomate\n");
    } else {
        printf("  Pereciveis: (n) carne | (o) ovo frito | (b) bacon\n");
        printf("  Preparados: (s) queijo suico | (q) queijo cheddar | (c) cebola | (t) tomate\n");
        printf("  Bancada: (z) pao superior | (k) ketchup | (m) mostarda | (p) picles | (a) alface\n");
    }
    printf("  (0) Cancelar\n");
    printf("Digite o simbolo do ingrediente: ");

    if(scanf(" %c", &simbolo) != 1){
        while((c = getchar()) != '\n' && c != EOF);
        return '\0';
    }
    while((c = getchar()) != '\n' && c != EOF);

    if(simbolo == '0'){
        return '0';
    }
    return simbolo;
}

// ================= MENU PRINCIPAL E FLUXO DE PARTIDA =================

void executarMenu(ContextoJogo *contextoJogo) {
    int escolha = 0;
    int caractere;

    while (1) {
        limparTela();

        printf("===== DADO BURGUER =====\n\n");
        printf("1. Novo Jogo\n");
        printf("2. Como jogar\n");
        printf("3. Histórico\n");
        printf("0. Sair\n");

        printf("Escolha: ");
        
        if (scanf("%d", &escolha) != 1) {
            while ((caractere = getchar()) != '\n' && caractere != EOF);
            printf("Entrada inválida! Pressione ENTER para continuar.");
            getchar();
            continue;
        }

        // Limpa o restante da entrada
        while ((caractere = getchar()) != '\n' && caractere != EOF);

        switch(escolha) {
            case 1:
                limparTela();
                novoJogo(contextoJogo);

                printf("Aperte ENTER para voltar ao menu!\n\n");
                getchar();
                break;

            case 2:
                limparTela();
                comoJogar();

                printf("Aperte ENTER para voltar ao menu!\n\n");
                getchar();
                break;

            case 3:
                limparTela();
                abrirHistorico(contextoJogo->historico, contextoJogo->metricasOrdenacao);
                break;

            case 0:
                return;

            default:
                printf("Opção inválida! Pressione ENTER para tentar novamente.");
                getchar();
                break;
        }
    }
}

void limparTela() {
    // Verifica se é Windows
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        // Mac e Linux
        printf("\033[2J\033[3J\033[H");         // desse jeito limpa o histórico também e fica bonitinho :3
        fflush(stdout);
        // system("clear");
    #endif
}

static Dificuldade selecionarDificuldade(){
    int opcao = 0;
    int caractere;

    while (1) {
        limparTela();
        printf("===== SELECIONE A DIFICULDADE =====\n\n");
        printf("1. Fácil (3 clientes | Paciência: 18 | Validade: 5 turnos)\n");
        printf("2. Médio (5 clientes | Paciência: 15 | Validade: 3 turnos)\n");
        printf("3. Difícil (7 clientes | Paciência: 13  | Validade: 2 turnos)\n\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) == 1 && opcao >= 1 && opcao <= 3) {
            while ((caractere = getchar()) != '\n' && caractere != EOF);
            return (Dificuldade)opcao;
        }

        while ((caractere = getchar()) != '\n' && caractere != EOF);
        printf("Opção inválida! Pressione ENTER para tentar novamente");
        getchar();
    }
}

void novoJogo(ContextoJogo *contextoJogo) {
    Dificuldade dificuldade = selecionarDificuldade();

    limparTela();
    Partida partida = criarPartida();

    Fila clientes;
    if (!montarFilaAtendimento(&clientes, contextoJogo, dificuldade)) {
        printf("Erro de memória ao montar a fila de clientes. A partida não foi iniciada.\n");
        return;
    }

    int resultado = atenderClientes(
        contextoJogo, 
        &clientes, 
        &partida.pontuacao,
        dificuldade
    );
    
    // -> se a partida acabar com derrota: dequeue pra remover todos clientes da fila e liberar memória   
    esvaziarFilaAtendimento(&clientes);
    
    limparTela();

    if (resultado < 0) {
        printf("Ocorreu um erro durante a partida e ela foi encerrada.\n");
        return;
    }

    if (!resultado) printf("Sentimos muito pela derrota, na próxima você consegue ;)\n");
    else printf("Parabéns pela vitória!\n");
    printf("Pontuação: %d\n\n", partida.pontuacao);
    
    // registra nome e pontuação no histórico
    salvarHistorico(contextoJogo->historico, partida, contextoJogo->arquivoHistorico);
}

static int lerOpcaoAtendimento(void) {
    printf("\nQual ação você deseja tomar?\n");
    printf("1. Preparar / colocar ingredientes para cozinhar (consome turno)\n");
    printf("2. Adicionar ingrediente pronto ao hambúrguer (consome turno)\n");
    printf("3. Remover ingrediente do topo (consome turno)\n");
    printf("4. Aguardar 1 turno (consome turno)\n");
    printf("5. Ver estoque e chapa\n");
    printf("6. Ver pedido\n");
    printf("0. Sair da partida\n");
    printf("Opção: ");

    int opcao;
    if (scanf("%d", &opcao) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        opcao = -1;
    }
    return opcao;
}

static void apresentarFalhaAtendimento(const EventoSessao *evento)
{
    switch (evento->erro) {
        case SESSAO_FALHA_INICIALIZAR_ESTOQUES:
            fprintf(stderr, "Erro: falha ao inicializar o estoque de perecíveis.\n");
            break;
        case SESSAO_FALHA_BUSCAR_PEDIDO:
            fprintf(stderr, "Erro: não foi possível recuperar o pedido %d.\n", evento->codigoPedido);
            break;
        case SESSAO_FALHA_ALOCAR_MONTAGEM:
            fprintf(stderr, "Erro de memória ao iniciar a montagem do hambúrguer.\n");
            break;
        case SESSAO_FALHA_INICIAR_MONTAGEM:
            fprintf(stderr, "Erro de memória ao colocar o pão inferior no prato.\n");
            break;
        case SESSAO_FALHA_CRIAR_GABARITO:
            fprintf(stderr, "Erro de memória ao criar o gabarito do pedido.\n");
            break;
        case SESSAO_FALHA_INICIAR_COZIMENTO:
            printf("Erro de memória ao iniciar cozimento de %s!\n", nomeDoIngrediente(evento->ingrediente));
            break;
        case SESSAO_FALHA_TRANSFERIR_COZIMENTO:
            fprintf(stderr, "Erro de memória ao transferir %s cozido para o estoque.\n",
                    nomeDoIngrediente(evento->ingrediente));
            break;
        case SESSAO_SEM_ERRO:
            break;
    }
}

static void apresentarEventoAtendimento(const EventoSessao *evento, void *dados)
{
    (void)dados;
    switch (evento->tipo) {
        case SESSAO_COZIMENTO_INICIADO: {
            int tempo = evento->turnoPronto - evento->turno;
            printf("1x %s colocado(a) para cozinhar! Ficará pronto no turno %d (%d turno%s).\n",
                   nomeDoIngrediente(evento->ingrediente), evento->turnoPronto,
                   tempo, tempo == 1 ? "" : "s");
            break;
        }
        case SESSAO_PREPARO_IMEDIATO:
            printf("1x %s preparado e adicionado ao estoque!\n", nomeDoIngrediente(evento->ingrediente));
            break;
        case SESSAO_INGREDIENTE_PRONTO:
            printf("[PRONTO] 1x %s terminou de cozinhar e foi para o estoque!\n",
                   nomeDoIngrediente(evento->ingrediente));
            break;
        case SESSAO_INGREDIENTE_VENCIDO:
            if (evento->ingrediente == CARNE) {
                printf("%d carne(s) descartada(s) pois venceram nesse turno!\n", evento->quantidade);
            } else {
                printf("%d %s(s) descartado(s) pois venceram nesse turno!\n", evento->quantidade,
                       evento->ingrediente == OVO_FRITO ? "ovo" : "bacon");
            }
            break;
        case SESSAO_INGREDIENTE_ADICIONADO:
            if (evento->temVencimento) {
                printf("1x %s (vencimento turno %d) adicionado ao hambúrguer!\n",
                       nomeDoIngrediente(evento->ingrediente), evento->turnoVencimento);
            } else {
                printf("1x %s adicionado(a) ao hambúrguer!\n", nomeDoIngrediente(evento->ingrediente));
            }
            break;
        case SESSAO_INGREDIENTE_REMOVIDO:
            printf("\n%s removido(a) e descartado no lixo!\n", nomeDoIngrediente(evento->ingrediente));
            break;
        case SESSAO_PEDIDO_ENTREGUE:
            printf("\nPedido do Cliente #%d entregue com sucesso! (+1 ponto)\n", evento->numeroCliente);
            break;
        case SESSAO_FALHA_ATENDIMENTO:
            apresentarFalhaAtendimento(evento);
            break;
    }
}

static void apresentarRecusa(MotivoBloqueioSessao motivo, char ingrediente)
{
    switch (motivo) {
        case SESSAO_INGREDIENTE_INVALIDO:
            printf("Ingrediente inválido!\n");
            break;
        case SESSAO_PREPARO_DESNECESSARIO:
            printf("'%s' já vem pronto na bancada e não precisa de preparo!\n", nomeDoIngrediente(ingrediente));
            break;
        case SESSAO_ESTOQUE_VAZIO:
            printf("Sem estoque de %s! Prepare antes de adicionar\n", nomeDoIngrediente(ingrediente));
            break;
        case SESSAO_ESTOQUE_PREPARADO_VAZIO:
            printf("Sem estoque preparado de %s! Prepare antes de adicionar\n", nomeDoIngrediente(ingrediente));
            break;
        case SESSAO_MONTAGEM_FECHADA:
            printf("Não é possível adicionar mais ingredientes após o pão superior!\n");
            break;
        case SESSAO_PAO_INFERIOR_FIXO:
            printf("\nNenhum ingrediente a remover! Há apenas o pão inferior no prato!\n");
            break;
        case SESSAO_ACAO_INVALIDA:
            printf("Opção inválida\n");
            break;
        case SESSAO_SEM_ATENDIMENTO:
        case SESSAO_SEM_BLOQUEIO:
            break;
    }
}

static void apresentarAtendimento(const EstadoSessao *estado)
{
    imprimirPilhaHamburguer(estado->montagem);
    if (estado->chapa->quantidade > 0) {
        printf("\n[Chapa] ");
        for (int i = 0; i < estado->chapa->quantidade; i++) {
            const ItemCozinhando *item = &estado->chapa->itens[i];
            const char *separador = i < estado->chapa->quantidade - 1 ? ", " : "";
            if (item->turnoPronto > estado->turno) {
                printf("%s (pronto T%d)%s", nomeDoIngrediente(item->simbolo), item->turnoPronto, separador);
            } else {
                printf("%s (pronto na chapa)%s", nomeDoIngrediente(item->simbolo), separador);
            }
        }
        printf("\n");
    }

    printf("\nCliente #%d | Paciência restante: %d | Turno atual: %d\n",
           estado->pontuacao + 1, estado->clienteAtual.paciencia, estado->turno);
}

static void visualizarEstoqueAtendimento(const EstadoSessao *estado)
{
    visualizarEstoque(estado->carne, estado->ovo, estado->bacon,
                      estado->estoquePreparado, estado->chapa, estado->turno);
}

static void executarOpcaoAtendimento(SessaoJogo *sessao, int opcao)
{
    const EstadoSessao *estado = consultarSessao(sessao);
    AcaoSessao acao = {0};
    switch (opcao) {
        case 1:
        case 2:
            visualizarEstoqueAtendimento(estado);
            acao.ingrediente = selecionarIngrediente(opcao == 1);
            if (acao.ingrediente == '0') {
                printf("%s cancelad%s\n", opcao == 1 ? "Preparo" : "Adição", opcao == 1 ? "o" : "a");
                return;
            }
            acao.tipo = opcao == 1 ? SESSAO_PREPARAR : SESSAO_ADICIONAR;
            break;
        case 3:
            acao.tipo = SESSAO_REMOVER;
            break;
        case 4:
            printf("Você aguardou 1 turno...\n");
            acao.tipo = SESSAO_AGUARDAR;
            break;
        case 5:
            visualizarEstoqueAtendimento(estado);
            return;
        case 6:
            printf("\n--- PEDIDO DO CLIENTE #%d ---\n", estado->pontuacao + 1);
            imprimirListaEnc(estado->receita);
            printf("----------------------------\n");
            return;
        case 0:
            acao.tipo = SESSAO_SAIR;
            break;
        default:
            printf("Opção inválida\n");
            return;
    }

    ResultadoAcaoSessao resultado = aplicarAcaoSessao(sessao, acao);
    if (resultado.retorno == SESSAO_ACAO_RECUSADA) {
        apresentarRecusa(resultado.motivo, acao.ingrediente);
    }
}

// Retorna 1 em vitória, 0 em derrota/saída e -1 em erro.
int atenderClientes(ContextoJogo *contextoJogo, Fila *filaClientes, int *pontuacao, Dificuldade dificuldade)
{
    SessaoJogo *sessao = criarSessao();
    if (sessao == NULL) {
        fprintf(stderr, "Erro: falha ao alocar a sessão de atendimento.\n");
        return -1;
    }
    SituacaoSessao situacao = iniciarAtendimento(sessao, contextoJogo, filaClientes, pontuacao,
                                                 dificuldade, apresentarEventoAtendimento, NULL);
    while (situacao == SESSAO_EM_ATENDIMENTO) {
        apresentarAtendimento(consultarSessao(sessao));
        executarOpcaoAtendimento(sessao, lerOpcaoAtendimento());
        situacao = prosseguirAtendimento(sessao);
    }
    liberarSessao(sessao);

    if (situacao == SESSAO_VITORIA) return 1;
    if (situacao == SESSAO_DERROTA || situacao == SESSAO_SAIDA_VOLUNTARIA) return 0;
    return -1;
}

void comoJogar() {
    printf("\n");
    printf("============================================================\n");
    printf("                       COMO JOGAR                            \n");
    printf("============================================================\n");

    printf("\n");
    printf(" OBJETIVO\n");
    printf(" ------------------------------------------------------------\n");
    printf(" Prepare os hambúrgueres dos clientes seguindo exatamente\n");
    printf(" a ordem de ingredientes indicada em cada pedido.\n");
    printf(" Atenda todos os clientes antes que percam a paciência!\n");

    printf("\n");
    printf(" INGREDIENTES\n");
    printf(" ------------------------------------------------------------\n");
    printf("  z - Pão superior          k - Ketchup\n");
    printf("  m - Mostarda              p - Picles\n");
    printf("  a - Alface                s - Queijo suíço\n");
    printf("  q - Queijo cheddar        c - Cebola\n");
    printf("  t - Tomate                n - Carne\n");
    printf("  o - Ovo frito             b - Bacon\n");

    printf("\n");
    printf(" COMO MONTAR SEU HAMBÚRGUER\n");
    printf(" ------------------------------------------------------------\n");
    printf(" 1. O pão inferior já começa automaticamente no prato.\n");
    printf(" 2. Consulte o pedido do cliente atual.\n");
    printf(" 3. Prepare os ingredientes necessários (carne, ovo e bacon\n");
    printf("    vão para a chapa e levam turnos para ficar prontos).\n");
    printf(" 4. Enquanto cozinham, você pode preparar/adicionar outros\n");
    printf("    ingredientes ou escolher 'Aguardar 1 turno'.\n");
    printf(" 5. Adicione os ingredientes prontos de baixo para cima.\n");
    printf(" 6. Se necessário, remova ingredientes do topo.\n");
    printf(" 7. Finalize com o pão superior ('z') para validar o pedido.\n");

    printf("\n");
    printf(" AÇÕES E TURNOS\n");
    printf(" ------------------------------------------------------------\n");
    printf(" Cada uma das seguintes ações consome 1 turno:\n");
    printf("  - Preparar / colocar ingrediente para cozinhar.\n");
    printf("  - Adicionar um ingrediente pronto ao hambúrguer.\n");
    printf("  - Remover o ingrediente do topo (vai para o lixo).\n");
    printf("  - Aguardar 1 turno.\n");
    printf("\n");
    printf(" Consultar o pedido e verificar o estoque/chapa NÃO consome turno.\n");

    printf("\n");
    printf(" ATENÇÃO!\n");
    printf(" ------------------------------------------------------------\n");
    printf("  - Os clientes são atendidos por ordem de chegada.\n");
    printf("  - Vários ingredientes podem cozinhar ao mesmo tempo!\n");
    printf("  - Carne, ovo frito e bacon sao perecíveis!\n");
    printf("  - O prazo de validade dos perecíveis só começa a contar\n");
    printf("    depois que terminam de cozinhar.\n");
    printf("  - Ingredientes vencidos são descartados automaticamente.\n");
    printf("  - A paciência diminui apenas para o cliente atual,\n");
    printf("    a cada ação realizada.\n");

    printf("\n");
    printf(" VITÓRIA E DERROTA\n");
    printf(" ------------------------------------------------------------\n");
    printf("  [VITÓRIA] Atenda todos os clientes sem perder nenhum!\n");
    printf("  [DERROTA] A paciência do cliente atual chegou a zero\n");
    printf("            antes da conclusão do pedido.\n");
    printf("\n");
    printf(" Sua pontuação é igual ao número de clientes atendidos.\n");

    printf("\n============================================================\n");
}

void abrirHistorico(ListaContDin *historico, MetricasOrdenacao *metricasOrdenacao) {

    int opcao = -1;
    int caractere;

    while (1) {
        limparTela();

        printf("Como você deseja ordenar o histórico?\n\n");
        printf("1. Ordem alfabética\n");
        printf("2. Maior Pontuação\n");
        printf("0. Voltar ao menu\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            while ((caractere = getchar()) != '\n' && caractere != EOF);
            opcao = -1;
            printf("Entrada inválida! Pressione ENTER para continuar.");
            getchar();
            continue;
        }

        // Limpa o restante da entrada
        while ((caractere = getchar()) != '\n' && caractere != EOF);

        switch (opcao) {
            case 1:             // Ordem alfabética
                limparTela();

                Partida *ordemAlfabetica = criarCopiaPartidas(historico);
                if (ordemAlfabetica != NULL) {
                    double tempoAlfa = ordenarPartidas(ordemAlfabetica, historico->tamanho, ORDEM_ALFABETICA);
                    metricasOrdenacao->tamanho = historico->tamanho;
                    metricasOrdenacao->tempo = tempoAlfa;
                    exibirRanking(ordemAlfabetica, historico->tamanho, "Histórico (A-Z)", tempoAlfa);
                    free(ordemAlfabetica);          // libera a cópia depois de exibir
                }
                else printf("Nenhuma partida registrada.\n");

                printf("\nAperte ENTER para voltar à página de histórico!\n");
                getchar();
                break;

            case 2:             // Pontuação decrescente
                limparTela();

                Partida *pontuacaoDecrescente = criarCopiaPartidas(historico);
                if (pontuacaoDecrescente != NULL) {
                    double tempoPontos = ordenarPartidas(pontuacaoDecrescente, historico->tamanho, ORDEM_PONTUACAO_DECRESCENTE);
                    metricasOrdenacao->tamanho = historico->tamanho;
                    metricasOrdenacao->tempo = tempoPontos;
                    exibirRanking(pontuacaoDecrescente, historico->tamanho, "Histórico (Maior Pontuação)", tempoPontos);
                    free(pontuacaoDecrescente);     // libera a cópia depois de exibir
                }
                else printf("Nenhuma partida registrada.\n");

                printf("\nAperte ENTER para voltar à página de histórico!\n\n");
                getchar();
                break;
                
            case 0:
                return;

            default:
                printf("Opção inválida! Pressione ENTER para tentar novamente.");
                getchar();
                break;
            }
    }

    return;
}