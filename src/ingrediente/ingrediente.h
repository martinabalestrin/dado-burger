#ifndef INGREDIENTE_H
#define INGREDIENTE_H

#include <stdio.h>
#include <string.h>

#define TOTAL_INGREDIENTES 13

typedef enum {
    PAO_SUPERIOR = 'z',
    PAO_INFERIOR = 'i',
    KETCHUP = 'k',
    MOSTARDA = 'm',
    PICLES = 'p',
    ALFACE = 'a',
    QUEIJO_SUICO = 's',
    QUEIJO_CHEDDAR = 'q',
    CEBOLA = 'c',
    TOMATE = 't',
    CARNE = 'n',
    OVO_FRITO = 'o',
    BACON = 'b'
} IngredienteID;

typedef struct {
    char simbolo;
    const char *nome;
    int requerPreparo;
    int perecivel; 
} IngredienteInfo;

const char* nomeDoIngrediente(char simbolo);        // nome por extenso, apenas para impressão
char ingredientePegarSimbolo(const char *nome);
int ingredienteValido(char simbolo);
int ingredienteRequerPreparo(char simbolo);
int ingredientePerecivel(char simbolo);

#endif