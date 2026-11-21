#include "menu.h"

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

void novoJogo(ContextoJogo *contextoJogo) {
    limparTela();
    Partida partida = criarPartida();

    Fila clientes;
    inicializaFila(&clientes);
    // SOBRE FILA:
    // definir número de clientes + paciência de acordo com as dificuldades + codPedido:
    // usar um for com inicializaClientes com inserirFila no fim da função?

    // enquanto houver clientes, mostrar o pedido do cliente da frente e ler uma ação
    Receita receita;
    Cliente cliente = clientes.frente->dados;

    // Cliente guarda apenas o código do pedido do cliente, que vai ser carregado apenas na hora que for a vez dele
    buscarPedido(
        contextoJogo->pedidosCache,
        contextoJogo->pedidos,
        cliente.codigoDoPedido,
        &receita
    );

    // Cria a pilha de receita


    // === TURNOS ===
    // apenas depois da criação da pilha de receita que começa o turno
    
    // se a ação for válida, consumir o turno, reduzir paciência
    // ao concluir o pedido, remove o cliente da fila e incrementa a pontuação com partida.pontuacao += 1

    // encerra quando a fila acaba (vitória) ou quando a paciência do cliente atual chega a zero (derrota)
    // -> se a partida acabar com derrota tem que usar dequeue pra remover todos clientes da fila e liberar memória    

    // registra nome e pontuação no histórico
    salvarHistorico(contextoJogo->historico, partida, contextoJogo->arquivoHistorico);
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
    printf(" 1. Consulte o pedido do cliente atual.\n");
    printf(" 2. Prepare os ingredientes que necessitam de preparo.\n");
    printf(" 3. Adicione os ingredientes na ordem indicada,\n");
    printf("    de baixo para cima.\n");
    printf(" 4. Finalize com o pao de cima para verificar o pedido.\n");
    printf(" 5. Se necessario, remova ingredientes do topo.\n");

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