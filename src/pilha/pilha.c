#include "pilha.h"

void inicializaPilha(PilhaEnc *p){
    p->topo = NULL;
}

int estaVaziaPilha(const PilhaEnc *p){
    return p->topo == NULL;
}

int pushPilha(PilhaEnc *p, char ingrediente){
    NodoPilha *novo = (NodoPilha*) malloc(sizeof(NodoPilha));
    if(novo == NULL){
        return 0;
    }

    novo->ingrediente = ingrediente;
    novo->prox = p->topo;
    p->topo = novo;
    return 1;
}

int popPilha(PilhaEnc *p, char *ingredienteRemovido){
    if(estaVaziaPilha(p)){
        return 0;
    }

    NodoPilha *removido = p->topo;
    if(ingredienteRemovido != NULL){
        *ingredienteRemovido = removido->ingrediente;
    }
    p->topo = removido->prox;
    free(removido);
    return 1;
}

int topoPilha(const PilhaEnc *p, char *ingrediente){
    if(estaVaziaPilha(p)){
        return 0;
    }

    *ingrediente = p->topo->ingrediente;
    return 1;
}

int tamanhoPilha(const PilhaEnc *p){
    NodoPilha *aux = p->topo;
    int contador = 0;

    while(aux != NULL){
        contador++;
        aux = aux->prox;
    }

    return contador;
}

void destruirPilha(PilhaEnc *p){
    char lixo;
    while(!estaVaziaPilha(p)){
        popPilha(p, &lixo);
    }
}

// inicia o prato pra um novo hambúrguer: inicializa a pilha e empilha automaticamente o PAO_INFERIOR
int iniciarMontagemHamburguer(PilhaEnc *p){
    inicializaPilha(p);
    return pushPilha(p, PAO_INFERIOR);
}

// adiciona um ingrediente no topo, desde que seguindo as regras
int adicionarIngredienteHamburguer(PilhaEnc *p, char ingrediente){
    char topoAtual;

    // garante que a montagem já possui o pão inferior no prato
    if(estaVaziaPilha(p)){
        return 0;
    }

    // bloqueia adição se o topo já for o pão superior 'z'
    if(topoPilha(p, &topoAtual) && topoAtual == PAO_SUPERIOR){
        return 0;
    }

    // impede empilhar outro pão inferior no meio da montagem
    if(ingrediente == PAO_INFERIOR){
        return 0;
    }

    return pushPilha(p, ingrediente);
}

// remove o ingrediente do topo / desempilha
// obs: o pão inferior não pode ser removido do prato
int removerIngredienteHamburguer(PilhaEnc *p){
    // se tamanho <= 1, apenas o pão inferior tá no prato
    if(tamanhoPilha(p) <= 1){
        printf("\nNenhum ingrediente a remover! Há apenas o pão inferior no prato!");
        return 0;
    }

    char ingredienteRemovido;
    popPilha(p, &ingredienteRemovido);
    printf("%s removido(a) e descartado no lixo!", nomeDoIngrediente(ingredienteRemovido));
    return 1;
}

// compara a pilha com a ListaEnc da receita: lida de baixo para cima, ignorando o pão inferior
// preservando a pilha original intacta
int verificarPedido(const PilhaEnc *p, const ListaEnc *receita) {
    if(p == NULL || receita == NULL || estaVaziaPilha(p)){
        return 0;
    }

    // pilha auxiliar temporária pra inverter a leitura (topo->base pra base->topo)
    PilhaEnc auxInversa;
    inicializaPilha(&auxInversa);

    NodoPilha *atualPilha = p->topo;
    while(atualPilha != NULL){
        if(!pushPilha(&auxInversa, atualPilha->ingrediente)){
            destruirPilha(&auxInversa);
            return 0;
        }
        atualPilha = atualPilha->prox;
    }

    // remove o pão inferior da base da pilha auxiliar (está fora da receita)
    char base;
    if(!popPilha(&auxInversa, &base) || base != PAO_INFERIOR){
        destruirPilha(&auxInversa);
        return 0;
    }

    // compara cada ingrediente com a lista encadeada da receita
    NodoListaEnc *atualReceita = receita->ini;
    char ingPilha;
    int saoIguais = 1;

    while(!estaVaziaPilha(&auxInversa) && atualReceita != NULL){
        popPilha(&auxInversa, &ingPilha);
        if(ingPilha != atualReceita->ingrediente){
            saoIguais = 0;
            break;
        }
        atualReceita = atualReceita->prox;
    }

    // verifica se sobrou ingrediente em alguma das estruturas (tamanhos diferentes)
    if(!estaVaziaPilha(&auxInversa) || atualReceita != NULL){
        saoIguais = 0;
    }

    destruirPilha(&auxInversa);
    return saoIguais;
}

// imprime a montagem atual do topo à base
void imprimirPilhaHamburguer(const PilhaEnc *p){
    printf("\nHambúrguer atual: \n");
    if(estaVaziaPilha(p)){
        printf("[Prato vazio]\n");
        return;
    }

    NodoPilha *aux = p->topo;
    while(aux != NULL){
        printf("%s\n", nomeDoIngrediente(aux->ingrediente));
        aux = aux->prox;
    }
}