#include <stdlib.h>

#include <raylib.h>

#include "componentes.h"
#include "telas.h"
#include "../src/jogo/aplicacao.h"

enum { LARGURA = 1280, ALTURA = 720 };

static int executarGrafico(ContextoJogo *contexto)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(LARGURA, ALTURA, "Dado Burger");
    if (!IsWindowReady()) return EXIT_FAILURE;

    SetWindowMinSize(640, 360);
    SetTargetFPS(60);

    RenderTexture2D tela = LoadRenderTexture(LARGURA, ALTURA);
    if (!IsRenderTextureValid(tela)) {
        TraceLog(LOG_ERROR, "Falha ao criar a superfície da interface.");
        UnloadRenderTexture(tela);
        CloseWindow();
        return EXIT_FAILURE;
    }
    SetTextureFilter(tela.texture, TEXTURE_FILTER_BILINEAR);

    Texture2D salao = LoadTexture("assets/gui/cenario/salao.png");
    Texture2D balcao = LoadTexture("assets/gui/cenario/balcao.png");
    if (!IsTextureValid(salao) || !IsTextureValid(balcao)) {
        TraceLog(LOG_ERROR, "Falha ao carregar o salão ou o balcão em assets/gui/cenario/.");
        if (IsTextureValid(balcao)) UnloadTexture(balcao);
        if (IsTextureValid(salao)) UnloadTexture(salao);
        UnloadRenderTexture(tela);
        CloseWindow();
        return EXIT_FAILURE;
    }
    SetTextureFilter(salao, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(balcao, TEXTURE_FILTER_BILINEAR);

    ElencoClientes elenco = {0};
    if (!carregarElencoClientes(&elenco)) {
        UnloadTexture(balcao);
        UnloadTexture(salao);
        UnloadRenderTexture(tela);
        CloseWindow();
        return EXIT_FAILURE;
    }

    IngredientesGraficos ingredientes = {0};
    carregarIngredientesGraficos(&ingredientes);
    iniciarComponentes();
    Navegacao nav = {.tela = TELA_MENU, .contexto = contexto};

    while (nav.tela != TELA_SAIR) {
        if (WindowShouldClose()) {
            if (!nav.confirmarSaida) solicitarSaida(&nav, TELA_SAIR);
            if (nav.tela == TELA_SAIR) break;
        }
        const int largura = GetScreenWidth();
        const int altura = GetScreenHeight();
        if (largura <= 0 || altura <= 0) {
            WaitTime(0.05);
            continue;
        }

        const float escalaX = (float)largura / LARGURA;
        const float escalaY = (float)altura / ALTURA;
        const float escala = escalaX < escalaY ? escalaX : escalaY;
        const int margemX = (int)((largura - LARGURA * escala) / 2);
        const int margemY = (int)((altura - ALTURA * escala) / 2);
        const Rectangle destino = {margemX, margemY, LARGURA * escala, ALTURA * escala};

        SetMouseOffset(-margemX, -margemY);
        SetMouseScale(1.0f / escala, 1.0f / escala);

        BeginTextureMode(tela);
        ClearBackground(CREME);
        desenharTela(&nav, salao, balcao, &elenco, &ingredientes);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(tela.texture, (Rectangle){0, 0, LARGURA, -ALTURA},
                       destino, (Vector2){0, 0}, 0, WHITE);
        EndDrawing();
    }

    encerrarPartidaGrafica(&nav);
    liberarComponentes();
    liberarIngredientesGraficos(&ingredientes);
    liberarElencoClientes(&elenco);
    UnloadTexture(balcao);
    UnloadTexture(salao);
    UnloadRenderTexture(tela);
    CloseWindow();
    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Uso correto: %s <pedidos.txt> <historico.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    AplicacaoJogo app;
    if (!iniciarAplicacao(&app, argv[1], argv[2])) return EXIT_FAILURE;
    int resultado = executarGrafico(&app.contexto);
    finalizarAplicacao(&app);
    return resultado;
}
