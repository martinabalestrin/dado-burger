#include "jogo/aplicacao.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Erro: argumentos insuficientes\n");
        fprintf(stderr, "Uso correto: %s <pedidos.txt> <historico.txt>\n", argv[0]);
        return -1;
    }

    AplicacaoJogo app;
    if (!iniciarAplicacao(&app, argv[1], argv[2])) return -1;
    executarMenu(&app.contexto);
    finalizarAplicacao(&app);
    return 0;
}
