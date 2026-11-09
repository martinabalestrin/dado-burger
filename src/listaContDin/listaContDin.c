#include "listaContDin.h"

void inicializarLCD(ListaContDin *l) {
    l->tamanho = 0;
    l->capacidade = 1;
    l->dados = (Partida*) malloc(sizeof(Partida));

    if (l->dados == NULL) {
        printf("Erro: memória insuficiente.\n");
        exit(1);
    }
}

void destruirLCD(ListaContDin *l) {
    free(l->dados);
    l->dados = NULL;
    l->tamanho = 0;
}

Partida acessarLCD(const ListaContDin *l, int pos) {
    if (pos >= l->tamanho || pos < 0) return (Partida){-1, ""};
    return l->dados[pos];
}

void inserirLCD(ListaContDin *l, Partida novo) {
    if (l->tamanho == l->capacidade) {
        l->capacidade++;
        Partida *temp = realloc(l->dados, l->capacidade * sizeof(Partida));

        if (temp == NULL) {
            printf("Erro: Falha ao realocar memória.\n");
            exit(1);
        }

        l->dados = temp;
    }

    l->dados[l->tamanho] = novo;
    l->tamanho++;
}

void removerLCD(ListaContDin *l, int pos) {
    if (pos >= l->tamanho || pos < 0) return;

    for (int i = pos; i < l->tamanho - 1; i++) {
        l->dados[i] = l->dados[i + 1];
    }

    l->tamanho--;
}

void carregarHistorico(ListaContDin *lista, char nomeArquivo[]) {
    FILE *fhistorico = fopen(nomeArquivo, "r");
    if (fhistorico == NULL) {
        return;
    }

    int pontuacao;
    char nome[21];

    while (fscanf(fhistorico, " %20[^\n]", nome) == 1) {
        if (fscanf(fhistorico, "%d", &pontuacao) == 1) {
            Partida partida;
            partida.pontuacao = pontuacao;
            strcpy(partida.nome, nome);
            inserirLCD(lista, partida);
        }
    }

    fclose(fhistorico);
}

void salvarHistorico(ListaContDin *lista, Partida nova, char nomeArquivo[]) {
    FILE *fhistorico = fopen(nomeArquivo, "a");
    if (fhistorico == NULL) {
        perror(nomeArquivo);
        return;
    }

    fprintf(fhistorico, "%20s\n", nova.nome);
    fprintf(fhistorico, "%d\n", nova.pontuacao);
    fclose(fhistorico);

    inserirLCD(lista, nova);
}

// USAR PARA TESTAR O CARREGAMENTO DE HISTÓRICO AGORA MAS PODE APAGAR DEPOIS !!!
void depurarHistorico(ListaContDin *l) {
    printf("\n========= DEBUG: HISTORICO DE PARTIDAS =========\n");
    
    // CASO 1: Se o ponteiro da lista for nulo
    if (l == NULL) {
        printf("[ERRO] A lista passada para o debug eh NULL!\n");
        printf("================================================\n\n");
        return;
    }

    // CASO 2: Se a lista estiver vazia (Usando l->tamanho)
    if (l->tamanho == 0) {
        printf("[AVISO] A lista existe, mas esta VAZIA (0 partidas).\n");
        printf("================================================\n\n");
        return;
    }

    // CASO 3: Imprimir os itens (Usando l->tamanho e l->dados)
    printf("Total de partidas carregadas: %d\n\n", l->tamanho);
    printf("  # | %-20s | %s\n", "Nome do Jogador", "Pontuacao");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < l->tamanho; i++) {
        Partida p = l->dados[i]; 
        printf(" %2d | %-20s | %d\n", i + 1, p.nome, p.pontuacao);
    }
    
    printf("================================================\n\n");
}