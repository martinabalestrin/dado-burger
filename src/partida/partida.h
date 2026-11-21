#ifndef PARTIDA_H
#define PARTIDA_H

typedef struct {
    int pontuacao;
    char nome[21];
} Partida;

Partida criarPartida();

#endif // PARTIDA_H