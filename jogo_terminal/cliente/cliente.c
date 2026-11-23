#include "cliente.h"

Cliente inicializarCliente(int paciencia, long qntPedidos) {
    Cliente cliente;

    cliente.paciencia = paciencia;
    cliente.codigoDoPedido = 1 + rand() % qntPedidos;
    
    return cliente;
}