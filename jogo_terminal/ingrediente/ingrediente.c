#include "ingrediente.h"

static const IngredienteInfo ingredientes[TOTAL_INGREDIENTES] = {
    { PAO_SUPERIOR,   "pão superior",   0, 0 },
    { PAO_INFERIOR,   "pão inferior",   0, 0 },
    { KETCHUP,        "ketchup",        0, 0 },
    { MOSTARDA,       "mostarda",       0, 0 },
    { PICLES,         "picles",         0, 0 },
    { ALFACE,         "alface",         0, 0 },
    { QUEIJO_SUICO,   "queijo suíço",   1, 0 },
    { QUEIJO_CHEDDAR, "queijo cheddar", 1, 0 },
    { CEBOLA,         "cebola",         1, 0 },
    { TOMATE,         "tomate",         1, 0 },
    { CARNE,          "carne",          1, 1 },
    { OVO_FRITO,      "ovo frito",      1, 1 },
    { BACON,          "bacon",          1, 1 },
};

static const IngredienteInfo* buscarInfo(char simbolo) {
    for (int i = 0; i < TOTAL_INGREDIENTES; i++) {
        if (ingredientes[i].simbolo == simbolo) return &ingredientes[i];
    }

    return NULL;
}

int ingredienteValido(char simbolo) {
    return buscarInfo(simbolo) != NULL;
}

const char* nomeDoIngrediente(char simbolo) {
    const IngredienteInfo *info = buscarInfo(simbolo);
    
    if (info == NULL) return "Ingrediente inválido!";
    else return info->nome;
}

char ingredientePegarSimbolo(const char *nome) {
    if (nome == NULL) return '\0';

    for (int i = 0; i < TOTAL_INGREDIENTES; i++) {
        if (strcmp(ingredientes[i].nome, nome) == 0) return ingredientes[i].simbolo;
    }

    return '\0';
}

int ingredienteRequerPreparo(char simbolo) {
    const IngredienteInfo *info = buscarInfo(simbolo);
    if (info == NULL) return -1;

    return info->requerPreparo;
}

int ingredientePerecivel(char simbolo) {
    const IngredienteInfo *info = buscarInfo(simbolo);
    if (info == NULL) return -1;
    
    return info->perecivel;
}
