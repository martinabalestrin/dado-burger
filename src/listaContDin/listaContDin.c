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
    FILE *fhistorico = fopen(nomeArquivo, "a+");
    if (fhistorico == NULL) {
        perror(nomeArquivo);
        return;
    }

    int pontuacao;
    char nome[21];

    while (fscanf(fhistorico, "%20s %d", nome, &pontuacao) == 2) {
        Partida partida;
        partida.pontuacao = pontuacao;
        strcpy(partida.nome, nome);
        inserirLCD(lista, partida);
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