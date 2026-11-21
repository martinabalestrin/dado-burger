#include "menu.h"

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

void visualizarEstoque(const MinHeap *carne, const MinHeap *ovo, const MinHeap *bacon, const int estoquePreparados[4]){
    int proxVenc;

    printf("\n==================== ESTOQUE DA COZINHA ====================\n");
    printf("[PERECIVEIS PREPARADOS]\n");

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
        printf("  Pereciveis: (n) carne | (o) ovo frito | (b) bacon\n");
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

void executarMenu(ContextoJogo *contextoJogo) {
    int escolha = 0;
    int caractere;

    while (1) {
        limparTela();

        printf("===== DADO BURGUER =====\n\n");
        printf("1. Novo Jogo\n");
        printf("2. Como jogar\n");
        printf("3. Histórico\n");

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
                verHistorico(contextoJogo->historico);

                printf("Aperte ENTER para voltar ao menu!\n\n");
                getchar();
                break;

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
        printf("1. Fácil (3 clientes | Paciência: 15 | Validade maior)\n");
        printf("2. Médio (5 clientes | Paciência: 10 | Validade padrão)\n");
        printf("3. Difícil (7 clientes | Paciência: 8  | Validade curta)\n\n");
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
    inicializaFila(&clientes);
    
    int numClientes;  
    int paciencia;

    switch(dificuldade){
        case DIFICULDADE_FACIL:
            numClientes = 3;
            paciencia = 15;
            break;
        case DIFICULDADE_DIFICIL:
            numClientes = 7;
            paciencia = 8;
            break;
        case DIFICULDADE_MEDIO:
        default:
            numClientes = 5;
            paciencia = 10;
            break;
    }

    for (int i = 0; i < numClientes; i++) {
        Cliente c = inicializarCliente(paciencia, contextoJogo->qntPedidos);
        enqueueFila(&clientes, c);
    }

    int resultado = atenderClientes(
        contextoJogo, 
        &clientes, 
        &partida.pontuacao,
        dificuldade
    );
    
    // -> se a partida acabar com derrota: dequeue pra remover todos clientes da fila e liberar memória   
    Cliente lixo;
    while(!estaVaziaFila(&clientes)){
        dequeueFila(&clientes, &lixo);
    }
    
    limparTela();

    if (!resultado) printf("Sentimos muito pela derrota, na próxima você consegue ;)\n");
    else printf("Parabéns pela vitória!\n");
    printf("Pontuação: %d\n\n", partida.pontuacao);
    
    // registra nome e pontuação no histórico
    salvarHistorico(contextoJogo->historico, partida, contextoJogo->arquivoHistorico);
}

int atenderClientes(ContextoJogo *contextoJogo, Fila *filaClientes, int *pontuacao, Dificuldade dificuldade) {    
   // Define validade dos itens perecíveis conforme a dificuldade escolhida
    int valCarne, valOvo, valBacon;
    switch(dificuldade){
        case DIFICULDADE_FACIL:
            valCarne = 6;
            valOvo = 5;
            valBacon = 7;
            break;
        case DIFICULDADE_DIFICIL:
            valCarne = 3;
            valOvo = 2;
            valBacon = 4;
            break;
        case DIFICULDADE_MEDIO:
        default:
            valCarne = 4;
            valOvo = 3;
            valBacon = 5;
            break;
    }
   
    // Criar estoques
    MinHeap carne, ovo, bacon;          // Ajustar validade dos itens perecíveis
    inicializarHeap(&carne, valCarne);
    inicializarHeap(&ovo, valOvo);
    inicializarHeap(&bacon, valBacon);
    
    // estoque de ingredientes não-perecíveis preparados: [0]=s, [1]=q, [2]=c, [3]=t
    int estoqueNaoPerecivel[4] = {0, 0, 0, 0};
    int turno = 1;
    int ultimoTurnoVerificado = 0; // garante que o descarte ocorra apenas 1x no início de cada turno

    // Iteração por cliente
    while (!estaVaziaFila(filaClientes)) {
        // Enquanto houver clientes, mostrar o pedido do cliente da frente e ler uma ação
        Receita receita;
        Cliente cliente = filaClientes->frente->dados;

        // Cliente guarda apenas o código do pedido do cliente, que vai ser carregado apenas na hora que for a vez dele
        buscarPedido(
            contextoJogo->pedidosCache,
            contextoJogo->pedidos,
            cliente.codigoDoPedido,
            &receita
        );

        // Hamburguer montado pelo jogador
        PilhaEnc *hamburguer = (PilhaEnc*)malloc(sizeof(PilhaEnc));
        iniciarMontagemHamburguer(hamburguer);

        // Gabarito do hamburguer que o cliente pediu
        ListaEnc *hamburguerGabarito = criarPedido(receita);

        // Iteração por turno
        while (
            !verificarPedido(hamburguer, hamburguerGabarito)
            && cliente.paciencia > 0
        ) { 
            // início de cada turno: descarta perecíveis vencidos (executa apenas uma vez por turno)
            if(turno != ultimoTurnoVerificado){

                int descartados = descartarVencidosHeap(&carne, turno);
                if (descartados > 0) printf("%d carne(s) descartada(s) pois venceram nesse turno!\n", descartados);
                descartados = descartarVencidosHeap(&ovo, turno);
                if (descartados > 0) printf("%d ovo(s) descartado(s) pois venceram nesse turno!\n", descartados);
                descartados = descartarVencidosHeap(&bacon, turno);
                if (descartados > 0) printf("%d bacon(s) descartado(s) pois venceram nesse turno!\n", descartados);
                
                ultimoTurnoVerificado = turno;
            }

            imprimirPilhaHamburguer(hamburguer);

            printf("\nCliente #%d | Paciência restante: %d | Turno atual: %d\n", (*pontuacao) + 1, cliente.paciencia, turno); // (*pontuacao) + 1 :: atua como um cliente.id
            printf("\nQual ação você deseja tomar?\n");
            printf("1. Preparar ingrediente (consome turno)\n");
            printf("2. Adicionar ingrediente ao hambúrguer (consome turno)\n");
            printf("3. Remover ingrediente do topo (consome turno)\n");
            printf("4. Ver estoque\n");
            printf("5. Ver pedido\n");
            printf("0. Sair da partida\n");
            printf("Opção: ");

            int opcao;
            if (scanf("%d", &opcao) != 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                opcao = -1;
            }

            int acaoConsumiuTurno = 0;
            char ing;

            switch (opcao) {
                case 1: // preparar ingrediente (consome 1 turno se válido)
                    visualizarEstoque(&carne, &ovo, &bacon, estoqueNaoPerecivel);
                    ing = selecionarIngrediente(1);

                    if(ing == '0'){
                        printf("Preparo cancelado\n");
                        break;
                    }

                    if(!ingredienteValido(ing) || ing == PAO_INFERIOR){
                        printf("Ingrediente inválido!\n");
                        break;
                    }

                    if(!ingredienteRequerPreparo(ing)){
                        printf("'%s' já vem pronto na bancada e não precisa de preparo!\n", nomeDoIngrediente(ing));
                        break;
                    }

                    if(ingredientePerecivel(ing)){
                        int inseriu = 0;
                        if(ing == CARNE){
                            inseriu = inserirHeap(&carne, turno);
                        }
                        else if(ing == OVO_FRITO){
                            inseriu = inserirHeap(&ovo, turno);
                        }
                        else if(ing == BACON){
                            inseriu = inserirHeap(&bacon, turno);
                        }

                        if(inseriu){
                            printf("1x %s preparado e adicionado ao estoque de perecíveis!\n", nomeDoIngrediente(ing));
                            acaoConsumiuTurno = 1;
                        } else{
                            printf("Erro de memória ao armazenar %s!\n", nomeDoIngrediente(ing));
                        }
                    } else{
                        int idx = indiceNaoPerecivel(ing);
                        if(idx != -1){
                            estoqueNaoPerecivel[idx]++;
                            printf("1x %s preparado e adicionado ao estoque!\n", nomeDoIngrediente(ing));
                            acaoConsumiuTurno = 1;
                        }
                    }
                    break;

                case 2: // adicionar ingrediente ao hambúrguer (consome 1 turno se válido)
                    visualizarEstoque(&carne, &ovo, &bacon, estoqueNaoPerecivel);
                    ing = selecionarIngrediente(0);

                    if(ing == '0'){
                        printf("Adição cancelada\n");
                        break;
                    }

                    if(!ingredienteValido(ing) || ing == PAO_INFERIOR){
                        printf("Ingrediente inválido!\n");
                        break;
                    }

                    // verifica disponibilidade em estoque caso exija preparo
                    if(ingredienteRequerPreparo(ing)){
                        if(ingredientePerecivel(ing)){
                            MinHeap *heapAlvo = NULL;
                            if(ing == CARNE){
                                heapAlvo = &carne;
                            }
                            else if(ing == OVO_FRITO){
                                heapAlvo = &ovo;
                            }
                            else if(ing == BACON){
                                heapAlvo = &bacon;
                            }

                            if(heapAlvo == NULL || estaVazioHeap(heapAlvo)){
                                printf("Sem estoque de %s! Prepare antes de adicionar\n", nomeDoIngrediente(ing));
                                break;
                            }

                            // tenta empilhar primeiro (caso o pão de cima já esteja no topo, não gasta estoque)
                            if(adicionarIngredienteHamburguer(hamburguer, ing)){
                                int vencConsumido;
                                removerHeap(heapAlvo, &vencConsumido);
                                printf("1x %s (vencimento turno %d) adicionado ao hambúrguer!\n", nomeDoIngrediente(ing), vencConsumido);
                                acaoConsumiuTurno = 1;
                            } else{
                                printf("Não é possível adicionar mais ingredientes após o pão superior!\n");
                            }
                        } else{
                            int idx = indiceNaoPerecivel(ing);
                            if(idx == -1 || estoqueNaoPerecivel[idx] <= 0){
                                printf("Sem estoque preparado de %s! Prepare antes de adicionar\n", nomeDoIngrediente(ing));
                                break;
                            }

                            if(adicionarIngredienteHamburguer(hamburguer, ing)){
                                estoqueNaoPerecivel[idx]--;
                                printf("1x %s adicionado(a) ao hambúrguer!\n", nomeDoIngrediente(ing));
                                acaoConsumiuTurno = 1;
                            } else{
                                printf("Não é possível adicionar mais ingredientes após o pão superior!\n");
                            }
                        }
                    } else{
                        // ingredientes prontos da bancada (z, k, m, p, a)
                        if(adicionarIngredienteHamburguer(hamburguer, ing)){
                            printf("1x %s adicionado(a) ao hambúrguer!\n", nomeDoIngrediente(ing));
                            acaoConsumiuTurno = 1;
                        } else{
                            printf("Não é possível adicionar mais ingredientes após o pão superior!\n");
                        }
                    }
                    break;

                case 3: // remover ingrediente do topo: descarta no lixo, consome 1 turno se havia algo removível
                    if(removerIngredienteHamburguer(hamburguer)){
                        printf("\n");
                        acaoConsumiuTurno = 1;
                    } else{
                        printf("\n");
                    }
                    break;

                case 4: // ver estoque (não consome turno)
                    visualizarEstoque(&carne, &ovo, &bacon, estoqueNaoPerecivel);
                    break;

                case 5: // ver pedido (não consome turno)
                    printf("\n--- PEDIDO DO CLIENTE #%d ---\n", (*pontuacao) + 1);
                    imprimirListaEnc(hamburguerGabarito);
                    printf("----------------------------\n");
                    break;
                    
                case 0: // encerrar partida
                    destruirPilha(hamburguer);
                    free(hamburguer);
                    destruirListaEnc(hamburguerGabarito);
                    free(hamburguerGabarito);
                    destruirHeap(&carne);
                    destruirHeap(&ovo);
                    destruirHeap(&bacon);
                    return 0;
                    
                default:
                    printf("Opção inválida\n");
                    break;
            }

            // só avança turno e reduz paciência se uma ação válida de jogo foi executada
            if(acaoConsumiuTurno){
                cliente.paciencia--;
                turno++;
            }

        }

        // libera estruturas da montagem do pedido concluído
        destruirPilha(hamburguer);
        free(hamburguer);
        destruirListaEnc(hamburguerGabarito);
        free(hamburguerGabarito);

        // se foi derrotado por falta de paciência
        if(cliente.paciencia <= 0){
            destruirHeap(&carne);
            destruirHeap(&ovo);
            destruirHeap(&bacon);
            return 0;
        }

        // Se entregou o pedido certo então remove cliente da fila e pontuacao++
        printf("\nPedido do Cliente #%d entregue com sucesso! (+1 ponto)\n", (*pontuacao) + 1);
        dequeueFila(filaClientes, &cliente);
        (*pontuacao)++;
    }

    destruirHeap(&carne);
    destruirHeap(&ovo);
    destruirHeap(&bacon);
    return 1;
}

// Texto gerado por IA a partir do pdf do trabalho como "filler". Após finalização das mecânicas, revisar e fazer as alterações necessárias.
void comoJogar() {
    printf("\n");
    printf("============================================================\n");
    printf("                       COMO JOGAR                            \n");
    printf("============================================================\n");

    printf("\n");
    printf(" OBJETIVO\n");
    printf(" ------------------------------------------------------------\n");
    printf(" Prepare os hamburgueres dos clientes seguindo exatamente\n");
    printf(" a ordem de ingredientes indicada em cada pedido.\n");
    printf(" Atenda todos os clientes antes que percam a paciencia!\n");

    printf("\n");
    printf(" INGREDIENTES\n");
    printf(" ------------------------------------------------------------\n");
    printf("  z - Pao de cima       k - Ketchup\n");
    printf("  m - Mostarda          p - Picles\n");
    printf("  a - Alface            s - Queijo suico\n");
    printf("  q - Queijo cheddar    c - Cebola\n");
    printf("  t - Tomate            n - Carne\n");
    printf("  o - Ovo frito         b - Bacon\n");

    printf("\n");
    printf(" COMO MONTAR SEU HAMBURGUER\n");
    printf(" ------------------------------------------------------------\n");
    printf(" 1. O pao inferior ja comeca automaticamente no prato.\n");
    printf(" 2. Consulte o pedido do cliente atual.\n");
    printf(" 3. Prepare os ingredientes que necessitam de preparo.\n");
    printf(" 4. Adicione os ingredientes na ordem indicada,\n");
    printf("    de baixo para cima.\n");
    printf(" 5. Finalize com o pao de cima para verificar o pedido.\n");
    printf(" 6. Se necessario, remova ingredientes do topo.\n");

    printf("\n");
    printf(" ACOES E TURNOS\n");
    printf(" ------------------------------------------------------------\n");
    printf(" Cada uma das seguintes acoes consome 1 turno:\n");
    printf("  - Preparar um ingrediente.\n");
    printf("  - Adicionar um ingrediente ao hamburguer.\n");
    printf("  - Remover o ingrediente do topo.\n");
    printf("\n");
    printf(" Consultar o pedido e verificar o estoque NAO consome turno.\n");

    printf("\n");
    printf(" ATENCAO!\n");
    printf(" ------------------------------------------------------------\n");
    printf("  - Os clientes sao atendidos por ordem de chegada.\n");
    printf("  - Carne, ovo frito e bacon sao pereciveis!\n");
    printf("  - Ingredientes vencidos sao descartados automaticamente.\n");
    printf("  - A paciencia diminui apenas para o cliente atual,\n");
    printf("    a cada acao realizada.\n");

    printf("\n");
    printf(" VITORIA E DERROTA\n");
    printf(" ------------------------------------------------------------\n");
    printf("  [VITORIA] Atenda todos os clientes sem perder nenhum!\n");
    printf("  [DERROTA] A paciencia do cliente atual chegou a zero\n");
    printf("            antes da conclusao do pedido.\n");
    printf("\n");
    printf(" Sua pontuacao e igual ao numero de clientes atendidos.\n");

    printf("\n============================================================\n");
}