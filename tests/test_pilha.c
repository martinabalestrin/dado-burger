#include <stdio.h>
#include "../src/pilha/pilha.h"
#include "../src/listaEnc/listaEnc.h"
#include "../src/receita.h"

/* compilação dos testes:
mkdir -p build

clang -Wall -Wextra -std=c99 \
  tests/test_pilha.c \
  src/pilha/pilha.c \
  src/listaEnc/listaEnc.c \
  -o build/test_pilha

execução dos testes:
./build/test_pilha
*/


int main() {
    PilhaEnc hamburguer;
    char removido;
    char topoAtual;

    printf("\n||| INICIO DOS TESTES (PILHA) |||\n\n");

    // inicialização: pão inferior já existe no prato
    printf("Teste 1: pao inferior inicial no prato\n");
    iniciarMontagemHamburguer(&hamburguer);
    topoPilha(&hamburguer, &topoAtual);

    if(tamanhoPilha(&hamburguer) == 1 && topoAtual == PAO_INFERIOR){
        printf("OK: pao inferior ('%c') inicializado no prato (tamanho = 1)\n", topoAtual);
    } else{
        printf("FALHA: erro na inicializacao do prato\n");
    }

    // proteção do pão inferior contra remoção
    printf("\nTeste 2: protecao do pao inferior ao desempilhar\n");
    if(removerIngredienteHamburguer(&hamburguer, &removido) == 0 && tamanhoPilha(&hamburguer) == 1){
        printf("OK: nao permitiu remover o pao inferior\n");
    } else{
        printf("FALHA: pao inferior foi removido\n");
    }

    // montagem do exemplo "nqz": carne ('n'), queijo ('q'), alface ('a')
    printf("\nTeste 3: empilhar e desempilhar ingredientes\n");
    adicionarIngredienteHamburguer(&hamburguer, 'n'); // carne
    adicionarIngredienteHamburguer(&hamburguer, 'q'); // queijo
    adicionarIngredienteHamburguer(&hamburguer, 'a'); // alface

    topoPilha(&hamburguer, &topoAtual);
    if(tamanhoPilha(&hamburguer) == 4 && topoAtual == 'a'){
        printf("OK: ingredientes empilhados | Topo atual: '%c'\n", topoAtual);
    }

    // remove um ingrediente (alface)
    if(removerIngredienteHamburguer(&hamburguer, &removido) == 1 && removido == 'a'){
        topoPilha(&hamburguer, &topoAtual);
        if(topoAtual == 'q' && tamanhoPilha(&hamburguer) == 3){
            printf("OK: desempilhou '%c' | Novo topo: '%c'\n", removido, topoAtual);
        }
    }

    // colocar pão superior ('z') e testar bloqueio de novas adições
    printf("\nTeste 4: pao superior ('z') e bloqueio de adicoes posteriores\n");
    int colocouPaoSup = adicionarIngredienteHamburguer(&hamburguer, 'z');
    int tentouAdicionarDepois = adicionarIngredienteHamburguer(&hamburguer, 't'); // tomate

    if(colocouPaoSup == 1 && tentouAdicionarDepois == 0 && tamanhoPilha(&hamburguer) == 4){
        printf("OK: pao superior ('z') adicionado e adicao posterior ('t') bloqueada\n");
    } else{
        printf("FALHA: bloqueio apos pao superior falhou\n");
    }

    imprimirPilhaHamburguer(&hamburguer);

    // comparação pilha x receita ("nqz") preservando a pilha original
    printf("\nTeste 5: comparacao pilha x receita ('nqz') sem destruir a pilha\n");
    Receita receitaExemplo = {1, "nqz"};
    ListaEnc *pedido = criarPedido(receitaExemplo);

    int tamAntes = tamanhoPilha(&hamburguer);
    int valido = verificarPedido(&hamburguer, pedido);
    int tamDepois = tamanhoPilha(&hamburguer);

    if(valido == 1 && tamAntes == tamDepois && tamDepois == 4){
        printf("OK: receita 'nqz' validada e pilha intacta (tamanho: %d)\n", tamDepois);
    } else{
        printf("FALHA: nao validado ou pilha modificada\n");
    }

    // casos especiais de erro na comparação
    printf("\nTeste 6: casos especiais de receita incorreta\n");
    Receita rOrdemInvertida = {2, "qnz"}; // queijo antes da carne
    ListaEnc *pedidoInvertido = criarPedido(rOrdemInvertida);

    Receita rSemPaoSuperior = {3, "nq"};  // faltou 'z' na receita
    ListaEnc *pedidoSemPao = criarPedido(rSemPaoSuperior);

    if(verificarPedido(&hamburguer, pedidoInvertido) == 0){
        printf("OK: rejeitou receita com ordem trocada ('qnz')\n");
    }
    if(verificarPedido(&hamburguer, pedidoSemPao) == 0){
        printf("OK: rejeitou receita com tamanho diferente ('nq')\n");
    }

    // liberação de memória
    destruirListaEnc(pedido);
    free(pedido);
    destruirListaEnc(pedidoInvertido);
    free(pedidoInvertido);
    destruirListaEnc(pedidoSemPao);
    free(pedidoSemPao);
    destruirPilha(&hamburguer);

    printf("\n||| FIM DOS TESTES |||\n");
    return 0;
}