#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int paciencia;
    int codigoDoPedido;
} Cliente;

Cliente inicializarCliente(int paciencia, long qntPedidos);

#endif