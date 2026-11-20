#include <stdio.h>
#include "../src/heap/heap.h"

/* compilação
mkdir -p build

clang -std=c99 -Wall -Wextra -Isrc/heap \
tests/test_heap.c src/heap/heap.c \
-o build/teste_heap

execução
./build/teste_heap
*/


int main() {
    MinHeap carne, ovo, bacon;
    int venc;

    printf("\n---- INICIO DOS TESTES - MIN-HEAP ----\n\n");

    // inicialização independente (validades: carne=4, ovo=3, bacon=5)
    inicializarHeap(&carne, 4);
    inicializarHeap(&ovo, 3);
    inicializarHeap(&bacon, 5);

    if(estaVazioHeap(&carne) && estaVazioHeap(&ovo) && estaVazioHeap(&bacon)){
        printf("OK: os 3 heaps iniciaram vazios\n");
    }

    // bloqueio de consumo quando vazio
    if(removerHeap(&carne, &venc) == 0){
        printf("OK: impediu consumo de carne com estoque vazio\n");
    }

    // preparo (inserção) em turnos diferentes e prioridade na raiz
    // turno 1: carne vence no turno 5, ovo vence no turno 4
    inserirHeap(&carne, 1);
    inserirHeap(&ovo, 1);

    // turno 3: carne vence em 7, bacon vence em 8
    inserirHeap(&carne, 3);
    inserirHeap(&bacon, 3);

    // turno 4: carne vence em 8
    inserirHeap(&carne, 4);

    printf("\nEstado apos preparos:\n");
    imprimirHeap(&carne, "Carne");
    imprimirHeap(&ovo, "Ovo");
    imprimirHeap(&bacon, "Bacon");

    if(proximoVencimentoHeap(&carne, &venc) && venc == 5){
        printf("\nOK: raiz da carne aponta pro menor vencimento (turno %d)\n", venc);
    }

    // descarte automático no início do turno 4 (ovo vence em 4 <= 4)
    int descOvo = descartarVencidosHeap(&ovo, 4);
    if(descOvo == 1 && estaVazioHeap(&ovo)){
        printf("OK: descartou 1 ovo vencido no turno 4 (estoque de ovo zerado)\n");
    }

    // descarte no início do turno 5 (carne de venc. 5 <= 5 sai; raiz vira 7)
    int descCarne = descartarVencidosHeap(&carne, 5);
    proximoVencimentoHeap(&carne, &venc);
    if(descCarne == 1 && venc == 7){
        printf("OK: descartou 1 carne vencida no turno 5. Nova raiz vence em %d\n", venc);
    }

    // consumo retira sempre o topo (menor vencimento)
    if(removerHeap(&carne, &venc) && venc == 7){
        proximoVencimentoHeap(&carne, &venc);
        if(venc == 8){
            printf("OK: consumiu carne de vencimento 7. Proxima raiz vence em %d\n", venc);
        }
    }

    // descarte simultâneo no turno 8 (carne restante e bacon vencem em 8)
    int descT8 = descartarVencidosHeap(&carne, 8) + descartarVencidosHeap(&bacon, 8);
    if(descT8 == 2 && estaVazioHeap(&carne) && estaVazioHeap(&bacon)){
        printf("OK: descartou %d itens vencidos no turno 8. Todos os estoques vazios\n", descT8);
    }

    // liberação de memória
    destruirHeap(&carne);
    destruirHeap(&ovo);
    destruirHeap(&bacon);

    printf("\n---- FIM DOS TESTES ----\n");
    return 0;
}