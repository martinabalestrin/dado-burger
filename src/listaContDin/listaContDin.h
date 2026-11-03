#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../partida.h"

typedef struct {
    int tamanho;
    int capacidade;
    Partida *dados;
} ListaContDin;

void inicializarLCD(ListaContDin *l);
void destruirLCD(ListaContDin *l);
Partida acessarLCD(const ListaContDin *l, int pos);
void inserirLCD(ListaContDin *l, Partida novo);
void removerLCD(ListaContDin *l, int pos);
void carregarHistorico(ListaContDin *lista, char nomeArquivo[]);
void salvarHistorico(ListaContDin *lista, Partida nova, char nomeArquivo[]);
void depurarHistorico(ListaContDin *l);