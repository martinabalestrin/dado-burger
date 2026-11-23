#include <stdio.h>
#include <string.h>

#include "partida.h"

Partida criarPartida() {
    Partida p;
    p.pontuacao = 0;

    printf("Insira seu nome: ");
    if (fgets(p.nome, sizeof(p.nome), stdin) == NULL) strcpy(p.nome, "Jogador");
    else {
        int tam = strlen(p.nome);
        if (tam > 0 && p.nome[tam-1] == '\n') {
            p.nome [tam - 1] = '\0';
        } 
        else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        if (p.nome[0] == '\0') strcpy(p.nome, "Jogador");
    }

    return p;
}