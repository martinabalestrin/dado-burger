#include <stdio.h>
#include <stdlib.h>

// PROTÓTIPOS TEMPORÁRIOS
void novo_jogo(){};
void como_jogar(){};
void ver_historico(){};

void limpar_tela() {
    // Verifica se é Windows
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        // Mac e Linux
        system("clear");
    #endif
}

int iniciar() {
    // TELA DE INÍCIO
    int escolha;

    do {
        limpar_tela();
        printf("===== DADO BURGUER =====\n\n");
        printf("1. Novo Jogo\n");
        printf("2. Como jogar\n");
        printf("3. Histórico\n\n");

        printf("Escolha: ");
        scanf("%d", &escolha);

        switch(escolha) {
            case 1:
                novo_jogo();
                break;
            case 2:
                como_jogar();
                break;
            case 3:
                ver_historico();
                break;
            default:
                printf("Opção inválida! Pressione Enter para tentar novamente.");
                while(getchar() != '\n');
                getchar();
                break;
        }
    } while (escolha < 1 || escolha > 3);

    return 0;
}

