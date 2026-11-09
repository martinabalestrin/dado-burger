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
    Cliente cliente1 = {1, 10, 1100};
    Cliente cliente2 = {2, 10, 2200};
    Cliente cliente3 = {3, 10, 3300};
    Cliente cliente4 = {4, 10, 4400};
    FilaEnc filaTeste;

    printf("\n||| INÍCIO DOS TESTES |||\n\n");

    // inicialização correta
    printf("- Testes de inicialização - \n");
    inicializa(&filaTeste);
    if(estaVazia(&filaTeste)){
        printf("Teste de inicialização de fila: OK\n");
    }
    if(tamanho(&filaTeste) == 0){
        printf("Inicialização de fila vazia: OK\n\n");
    }

    // enfileirar
    printf("- Testes de enfileiramento - \n");
    printf("Enfileiramento do cliente id #1\n");
    Cliente clienteDaFrente = {0, 0, 0};
    enqueue(&filaTeste, cliente1);
    frente(&filaTeste, &clienteDaFrente);
    printf("Teste da função frente\nValor esperado: 1\nValor recebido: %d\n", clienteDaFrente.id);
    if(clienteDaFrente.id == 1){
        printf("Teste da função frente: OK\n");
    }
    printf("Teste da função tamanho\nValor esperado: 1\nValor recebido: %d\n", tamanho(&filaTeste));
    if(tamanho(&filaTeste) == 1){
        printf("Teste da função tamanho: OK\n");
    }

    printf("\nEnfileiramento do cliente id #2\n");
    enqueue(&filaTeste, cliente2);
    frente(&filaTeste, &clienteDaFrente);
    printf("Teste da função frente\nValor esperado: 1\nValor recebido: %d\n", clienteDaFrente.id);
    if(clienteDaFrente.id == 1){
        printf("Teste da função frente: OK\n");
    }
    printf("Teste da função tamanho\nValor esperado: 2\nValor recebido: %d\n", tamanho(&filaTeste));
    if(tamanho(&filaTeste) == 2){
        printf("Teste da função tamanho: OK\n");
    }

    printf("\nEnfileiramento do cliente id #3\n");
    enqueue(&filaTeste, cliente3);
    frente(&filaTeste, &clienteDaFrente);
    printf("Teste da função frente\nValor esperado: 1\nValor recebido: %d\n", clienteDaFrente.id);
    if(clienteDaFrente.id == 1){
        printf("Teste da função frente: OK\n");
    }
    printf("Teste da função tamanho\nValor esperado: 3\nValor recebido: %d\n", tamanho(&filaTeste));
    if(tamanho(&filaTeste) == 3){
        printf("Teste da função tamanho: OK\n");
    }

    // desenfileirar
    printf("\n- Testes de desenfileiramento - \n");
    printf("Desenfileiramento do cliente id #1\n");
    Cliente clienteRemovido = {0, 0, 0};
    dequeue(&filaTeste, &clienteRemovido);
    frente(&filaTeste, &clienteDaFrente);
    printf("Teste da função dequeue\nValor esperado: 2\nValor recebido: %d\n", clienteDaFrente.id);
    if(clienteDaFrente.id == 2){
        printf("Teste da função dequeue: OK\n");
    }
    printf("Tamanho da fila\nValor esperado: 2\nValor recebido: %d\n", tamanho(&filaTeste));
    if(tamanho(&filaTeste) == 2){
        printf("Atualização do tamanho da fila: OK\n");
    }
    printf("Elemento removido: id #%d\n\n", clienteRemovido.id);

    printf("Desenfileiramento do cliente id #2\n");
    dequeue(&filaTeste, &clienteRemovido);
    frente(&filaTeste, &clienteDaFrente);
    printf("Teste da função dequeue\nValor esperado: 3\nValor recebido: %d\n", clienteDaFrente.id);
    if(clienteDaFrente.id == 3){
        printf("Teste da função dequeue: OK\n");
    }
    printf("Tamanho da fila\nValor esperado: 1\nValor recebido: %d\n", tamanho(&filaTeste));
    if(tamanho(&filaTeste) == 1){
        printf("Atualização do tamanho da fila: OK\n");
    }
    printf("Elemento removido: id #%d\n\n", clienteRemovido.id);

    printf("Desenfileiramento do último cliente id #3\n");
    dequeue(&filaTeste, &clienteRemovido);
    if((frente(&filaTeste, &clienteDaFrente)) == 0){
        printf("Teste da função dequeue: OK -> Sem elementos na frente\n");
    }
    else{
        printf("Teste da função dequeue: FALHO -> Frente da fila: %d\n", clienteDaFrente.id);
    }
    printf("Tamanho da fila\nValor esperado: 0\nValor recebido: %d\n", tamanho(&filaTeste));
    if(tamanho(&filaTeste) == 0){
        printf("Atualização do tamanho da fila: OK\n");
    }
    printf("Elemento removido: id #%d\n\n", clienteRemovido.id);
    if(estaVazia(&filaTeste)){
        printf("Fila esvaziada completamente\n");
    }

    // Teste extra de enfileiramento pós-esvaziamento
    printf("\n- Testes de enfileiramento pós-esvaziamento -\n");
    printf("Enfileiramento do cliente id #4\n");
    enqueue(&filaTeste, cliente4);
    frente(&filaTeste, &clienteDaFrente);
    printf("Teste da função enqueue/frente\nValor esperado: 4\nValor recebido: %d\n", clienteDaFrente.id);
    if(clienteDaFrente.id == 4){
        printf("Teste da função enqueue: OK\n");
    }
    printf("Teste da função tamanho\nValor esperado: 1\nValor recebido: %d\n", tamanho(&filaTeste));
    if(tamanho(&filaTeste) == 1){
        printf("Atualização do tamanho da fila: OK\n");
    }

    printf("\n||| FIM DOS TESTES |||\n");


    return 0;
}