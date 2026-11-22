#include "ordenacaoHistorico.h"

// cria uma cópia independente dos dados armazenados na listaContDin
// a função é chamada antes do cronômetro iniciar
Partida *criarCopiaPartidas(const ListaContDin *lista) {
    if (lista == NULL || lista->tamanho <= 0) {
        return NULL;
    }

    // aloca memória com o tamanho exato de registros da lista
    Partida *copia = (Partida *) malloc(lista->tamanho * sizeof(Partida));
    if (copia == NULL) {
        printf("Falha na alocação de memória em *copia\n");
        return NULL;
    }

    // copia cada struct Partida para o vetor copia
    for (int i = 0; i < lista->tamanho; i++) {
        copia[i] = lista->dados[i];
    }

    return copia;
}

// função auxiliar de comparação: retorna 1 se 'a' deve vir antes ou empatar com 'b'
static int compararPartidas(Partida a, Partida b, CriterioOrdenacao criterio) {
    if (criterio == ORDEM_ALFABETICA) {
        // strcmp retorna <= 0 quando 'a.nome' vem antes ou é igual a 'b.nome'
        return strcmp(a.nome, b.nome) <= 0;
    } else {
        // se o criterio for pontuação decrescente: >= garante que maiores pontuações fiquem no início
        return a.pontuacao >= b.pontuacao;
    }
}

// recombinação/merge: intercala copia[ini..meio] e copia[meio+1..fim] já ordenados
static void merge(Partida *copia, Partida *aux, int ini, int meio, int fim, CriterioOrdenacao criterio) {
    // copia o trecho atual para o vetor auxiliar
    for (int t = ini; t <= fim; t++) {
        aux[t] = copia[t];
    }

    int i = ini;      // índice que percorre a metade esquerda (ini .. meio)
    int j = meio + 1; // índice que percorre a metade direita (meio+1 .. fim)
    int k = ini;      // índice que escreve de volta no vetor principal 'copia'

    // compara os elementos das duas metades e insere o menor/maior no vetor principal
    while (i <= meio && j <= fim) {
        if (compararPartidas(aux[i], aux[j], criterio)){
            copia[k++] = aux[i++];
        } else {
            copia[k++] = aux[j++];
        }
    }

    // se sobrarem elementos na metade esquerda -> copia o restante
    while (i <= meio) {
        copia[k++] = aux[i++];
    }

    // se sobrarem elementos na metade direita -> copia o restante
    while (j <= fim) {
        copia[k++] = aux[j++];
    }
}

// divisão e conquista: divide o vetor ao meio sucessivamente até restarem subvetores de tamanho 1
// depois chama merge() pra recombinar ordenadamente
static void mergeSortRec(Partida *copia, Partida *aux, int ini, int fim, CriterioOrdenacao criterio) {
    // caso base: subvetor com 0 ou 1 elemento -> já tá ordenado
    if (ini >= fim) {
        return;
    }

    // calcula o ponto médio
    int meio = ini + (fim - ini) / 2;

    // ordena recursivamente a metade esquerda e a metade direita
    mergeSortRec(copia, aux, ini, meio, criterio);
    mergeSortRec(copia, aux, meio + 1, fim, criterio);

    // recombinação: intercala as duas metades ordenadas
    merge(copia, aux, ini, meio, fim, criterio);
}

// aloca a memória auxiliar, cronometra só a execução do merge sort e libera a memória auxiliar no final
double ordenarPartidas(Partida *copia, int n, CriterioOrdenacao criterio) {
    Timer timer;
    
    // se há menos de 2 elementos -> não há o que ordenar (tempo = 0)
    if (copia == NULL || n < 2) {
        return 0.0;
    }

    // aloca o vetor auxiliar necessário para o merge sort
    Partida *aux = (Partida *) malloc(n * sizeof(Partida));
    if (aux == NULL) {
        printf("Falha na alocação de memória em *aux\n");
        return -1.0;
    }

    // inicia o cronômetro antes da ordenação
    timer_iniciar(&timer);

    mergeSortRec(copia, aux, 0, n - 1, criterio);

    // para o cronômetro depois do fim da ordenação
    double tempoExecucao = timer_parar(&timer);

    // libera o vetor auxiliar
    // obs: o vetor 'copia' continua válido para exibição
    free(aux);

    return tempoExecucao;
}

void exibirRanking(const Partida *vetor, int n, const char *titulo, double tempo) {
    printf("================================================\n");
    printf("  %s\n", titulo);
    printf("================================================\n");

    if (vetor == NULL || n <= 0) {
        printf("  Nenhuma partida registrada.\n");
        printf("================================================\n\n");
        return;
    }

    printf("  Total de partidas: %d\n\n", n);
    // printf("  Tempo de ordenação: %.6f s\n\n", tempo);

    printf(" %6s | %-20s | %s\n", "#", "Nome do Jogador", "Pontuação");
    printf("------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf(" %6d | %-20s | %d\n", i + 1, vetor[i].nome, vetor[i].pontuacao);
    }

    printf("================================================\n\n");
}