#include "../receita.h"

typedef struct str_NodoListaEnc NodoListaEnc;

struct str_NodoListaEnc {
    NodoListaEnc *prox;
    char ingrediente;
};

typedef struct {
    NodoListaEnc *ini;
} ListaEnc;

void    inicializarListaEnc(ListaEnc *lista);
void    imprimirListaEnc(const ListaEnc *lista);
int     buscarListaEnc(const ListaEnc *lista, char ingrediente);
int     inserirInicioListaEnc(ListaEnc *lista, char ingrediente);
int     inserirFimListaEnc(ListaEnc *lista, char ingrediente);
int     removerIngrediente(ListaEnc *lista, char ingrediente);
void    destruirListaEnc(ListaEnc *lista);
int     tamanhoListaEnc(const ListaEnc *lista);

ListaEnc*   criarPedido(Receita pedido);