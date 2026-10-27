#include <stdio.h>

int main(int argc, char *argv[]) {
    // argv[0] = nome do programa
    // argv[1] = caminho relativo do arquivo com pedidos
    // argv[2] = caminho relativo do arquivo com histórico

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

    // ...

    // no fim do código
    fclose(fpedidos);
    fclose(fhistorico);

    return 0;
}
