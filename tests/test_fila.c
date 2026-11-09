#include <stdio.h>
#include "../src/fila/fila.h"

int main(){

    /*
    compilação e exec:
    1)
    mkdir -p build

    clang -std=c99 -Wall -Wextra -Isrc/fila \
    tests/test_fila.c src/fila/fila.c \
    -o build/teste_fila
    2)
    ./build/teste_fila
    */

    // dados de teste
    int id1 = 1, id2 = 2, id3 = 3, id4 = 40;
    FilaEnc filaTeste;

    // inicialização correta
    inicializa(&filaTeste);
    if(estaVazia(&filaTeste)){
        printf("Fila criada\n");
    }
    if(tamanho(&filaTeste) == 0){
        printf("Fila vazia\n");
    }

    // enfileirar
    int elementoDaFrente = 0;
    enqueue(&filaTeste, id1);
    frente(&filaTeste, &elementoDaFrente);
    printf("Frente da fila: %d\n", elementoDaFrente);
    printf("Tamanho da fila: %d\n\n", tamanho(&filaTeste));

    enqueue(&filaTeste, id2);
    frente(&filaTeste, &elementoDaFrente);
    printf("Frente da fila: %d\n", elementoDaFrente);
    printf("Tamanho da fila: %d\n\n", tamanho(&filaTeste));

    enqueue(&filaTeste, id3);
    frente(&filaTeste, &elementoDaFrente);
    printf("Frente da fila: %d\n", elementoDaFrente);
    printf("Tamanho da fila: %d\n\n", tamanho(&filaTeste));

    // desenfileirar
    int elementoRemovido = 0;
    dequeue(&filaTeste, &elementoRemovido);
    frente(&filaTeste, &elementoDaFrente);
    printf("Frente da fila: %d\n", elementoDaFrente);
    printf("Tamanho da fila: %d\n", tamanho(&filaTeste));
    printf("Elemento removido: %d\n\n", elementoRemovido);

    dequeue(&filaTeste, &elementoRemovido);
    frente(&filaTeste, &elementoDaFrente);
    printf("Frente da fila: %d\n", elementoDaFrente);
    printf("Tamanho da fila: %d\n", tamanho(&filaTeste));
    printf("Elemento removido: %d\n\n", elementoRemovido);

    dequeue(&filaTeste, &elementoRemovido);
    if((frente(&filaTeste, &elementoDaFrente)) == 0){
        printf("Sem elementos na frente\n");
    }
    else{
        printf("Frente da fila: %d\n", elementoDaFrente);
    }
    printf("Tamanho da fila: %d\n", tamanho(&filaTeste));
    printf("Elemento removido: %d\n\n", elementoRemovido);
    if(estaVazia(&filaTeste)){
        printf("Fila esvaziada completamente\n");
    }

    enqueue(&filaTeste, id4);
    frente(&filaTeste, &elementoDaFrente);
    printf("Frente da fila: %d\n", elementoDaFrente);
    printf("Tamanho da fila: %d\n\n", tamanho(&filaTeste));


    return 0;
}