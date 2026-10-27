#include <stdio.h>

#include "medicoes/medicoes.c"

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Erro: argumentos insuficientes\n");
        fprintf(stderr, "Uso correto: %s <pedidos.txt> <historico.txt>\n", argv[0]);
        
        return 1;
    }
    
    FILE *fpedidos = fopen(argv[1], "r");
    if (fpedidos == NULL) {
        perror(argv[1]);
        return 1;
    }

    FILE *fhistorico = fopen(argv[2], "a+");
    if (fhistorico == NULL) {
        perror(argv[2]);
        fclose(fpedidos);
        return 1;
    }

    Medicoes medicoes;

    if (inicializarMedicoes(&medicoes) != 0) {
        fprintf(stderr, "Erro ao inicializar arquivo de medições\n");
        return 1;
    }

    // ...

    // no fim do código
    if (fecharMedicoes(&medicoes) != 0) {
        fprintf(stderr, "Erro ao fechar o arquivo de medições\n");
        return 1;
    }

    fclose(fpedidos);
    fclose(fhistorico);

    return 0;
}
