#include "heap.h"

static void siftUp(int dados[], int i){

    while(i > 0){
        int pai = (i - 1) / 2;
        
        if(dados[i] < dados[pai]){
            int tmp = dados[i];
            dados[i] = dados[pai];
            dados[pai] = tmp;
            i = pai;
        } else{
            break;
        }
    }
}

static void heapify(int dados[], int n, int i){
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && dados[esq] < dados[menor]) menor = esq;
    if (dir < n && dados[dir] < dados[menor]) menor = dir;

    if(menor != i){
        int tmp = dados[i];
        dados[i] = dados[menor];
        dados[menor] = tmp;
        heapify(dados, n, menor);
    }
}

int inicializarHeap(MinHeap *h, int validade){
    if (h == NULL || validade <= 0) return 0;
    
    h->capacidade = 8;
    h->tamanho = 0;
    h->validade = validade;
    h->dados = (int *) malloc(h->capacidade * sizeof(int));
    
    if (h->dados == NULL) {
        h->capacidade = 0;
        return 0;
    }
    return 1;
}

void destruirHeap(MinHeap *h){
    if (h == NULL) return;

    free(h->dados);
    h->dados = NULL;
    h->tamanho = 0;
    h->capacidade = 0;
}

int estaVazioHeap(const MinHeap *h){
    return h == NULL || h->dados == NULL || h->tamanho == 0;
}

int inserirHeap(MinHeap *h, int turnoAtual){
    if(h == NULL || h->dados == NULL || h->capacidade <= 0){
        return 0;
    }

    if(h->tamanho == h->capacidade){
        int novaCapacidade = 2 * h->capacidade;
        int *temp = (int *) realloc(h->dados, novaCapacidade * sizeof(int));

        if(temp == NULL){
            return 0;
        }
        
        h->dados = temp;
        h->capacidade = novaCapacidade;
    }

    h->dados[h->tamanho] = turnoAtual + h->validade;
    siftUp(h->dados, h->tamanho);
    h->tamanho++;
    return 1;
}

int proximoVencimentoHeap(const MinHeap *h, int *venc){

    if(estaVazioHeap(h) || venc == NULL){
        return 0;
    }

    *venc = h->dados[0];
    return 1;
}

int removerHeap(MinHeap *h, int *vencRemovido){

    if(estaVazioHeap(h)){
        return 0;
    }

    if(vencRemovido != NULL){
        *vencRemovido = h->dados[0];
    }

    h->tamanho--;

    if(h->tamanho > 0){
        h->dados[0] = h->dados[h->tamanho];
        heapify(h->dados, h->tamanho, 0);
    }

    return 1;
}

int descartarVencidosHeap(MinHeap *h, int turnoAtual){
    if (estaVazioHeap(h)) return 0;

    int descartados = 0;

    while(!estaVazioHeap(h) && h->dados[0] <= turnoAtual){
        removerHeap(h, NULL);
        descartados++;
    }

    return descartados;
}

void imprimirHeap(const MinHeap *h, const char *nome){
    if(h == NULL || h->dados == NULL) return;

    printf("%s (qtd: %d): [ ", nome, h->tamanho);

    for(int i = 0; i < h->tamanho; i++){
        printf("%d ", h->dados[i]);
    }

    printf("]\n");
}