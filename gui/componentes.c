#include "componentes.h"

#include <math.h>

#define RAYGUI_IMPLEMENTATION
#include "terceiros/raygui.h"

const Color CREME = {255, 240, 214, 255};
const Color VERMELHO = {168, 62, 52, 255};
const Color COR_MOSTARDA = {243, 190, 88, 255};
const Color MADEIRA = {104, 64, 46, 255};
const Color MENTA = {114, 163, 121, 255};
const Color TINTA = {62, 38, 31, 255};

static Font fonteCorpo, fonteTitulos;

static Font carregarFonte(const char *caminho)
{
    int caracteres[194];
    int total = 0;
    for (int i = 32; i <= 126; i++) caracteres[total++] = i;
    for (int i = 160; i <= 255; i++) caracteres[total++] = i;
    caracteres[total++] = 0x2013;
    caracteres[total++] = 0x2014;
    caracteres[total++] = 0x2026;

    Font padrao = GetFontDefault();
    Font fonte = LoadFontEx(caminho, 48, caracteres, total);
    if (!IsFontValid(fonte) || fonte.texture.id == padrao.texture.id) {
        TraceLog(LOG_WARNING, "Fonte indisponível (%s): usando a fonte padrão.", caminho);
        return padrao;
    }
    SetTextureFilter(fonte.texture, TEXTURE_FILTER_BILINEAR);
    return fonte;
}

void iniciarComponentes(void)
{
    fonteCorpo = carregarFonte("assets/gui/fontes/NunitoSans-SemiBold.ttf");
    fonteTitulos = carregarFonte("assets/gui/fontes/Fredoka-SemiBold.ttf");

    GuiLoadStyleDefault();
    GuiSetFont(fonteCorpo);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BLANK));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(BLANK));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(BLANK));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 23);
}

void liberarComponentes(void)
{
    Font padrao = GetFontDefault();
    GuiSetFont(padrao);
    if (fonteCorpo.texture.id != padrao.texture.id) UnloadFont(fonteCorpo);
    if (fonteTitulos.texture.id != padrao.texture.id) UnloadFont(fonteTitulos);
}

static void desenharTexto(Font fonte, const char *valor, float x, float y,
                          float tamanho, float limite, bool centrar, Color cor)
{
    float largura = MeasureTextEx(fonte, valor, tamanho, 0).x;
    if (limite > 0 && largura > limite) {
        tamanho *= limite / largura;
        largura = MeasureTextEx(fonte, valor, tamanho, 0).x;
    }
    DrawTextEx(fonte, valor, (Vector2){centrar ? x - largura / 2 : x, y}, tamanho, 0, cor);
}

void texto(const char *valor, float x, float y, int tamanho, Color cor)
{
    desenharTexto(fonteCorpo, valor, x, y, tamanho * 1.06f, 0, false, cor);
}

void textoCentrado(const char *valor, float x, float y, int tamanho, Color cor)
{
    desenharTexto(fonteCorpo, valor, x, y, tamanho * 1.06f, 0, true, cor);
}

void textoLimitado(const char *valor, float x, float y, int tamanho,
                   float largura, bool centrar, Color cor)
{
    desenharTexto(fonteCorpo, valor, x, y, tamanho * 1.06f, largura, centrar, cor);
}

void textoTitulo(const char *valor, float x, float y, int tamanho, bool centrar, Color cor)
{
    desenharTexto(fonteTitulos, valor, x, y, tamanho, 0, centrar, cor);
}

void painel(Rectangle area, Color cor)
{
    Rectangle sombra = area;
    sombra.y += 5;
    DrawRectangleRounded(sombra, .10f, 10, Fade(TINTA, .35f));
    DrawRectangleRounded(area, .10f, 10, cor);
    DrawRectangleRoundedLinesEx(area, .10f, 10, 2, Fade(MADEIRA, .45f));
}

static void desenharBotao(Rectangle area, const char *rotulo, Color cor, bool ativo)
{
    const Color contorno = {76, 41, 29, 255};
    bool sobre = ativo && CheckCollisionPointRec(GetMousePosition(), area);
    bool pressionado = sobre && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    Rectangle face = area;
    if (pressionado) face.y += 3;

    if (ColorIsEqual(cor, COR_MOSTARDA)) cor = (Color){245, 191, 88, 255};
    else if (ColorIsEqual(cor, CREME)) cor = (Color){255, 240, 211, 255};
    else if (ColorIsEqual(cor, MENTA)) cor = (Color){130, 170, 131, 255};
    else if (ColorIsEqual(cor, VERMELHO)) cor = (Color){201, 91, 78, 255};
    Color base = ativo ? cor : (Color){200, 180, 155, 255};
    Color preenchimento = pressionado ? ColorBrightness(base, -.12f)
        : sobre ? ColorBrightness(base, .13f) : base;
    DrawRectangleRounded((Rectangle){area.x, area.y + 4, area.width, area.height},
                         .42f, 10, Fade(contorno, ativo ? .8f : .5f));
    DrawRectangleRounded(face, .42f, 10, preenchimento);
    DrawRectangleRoundedLinesEx(face, .42f, 10, sobre ? 2.5f : 1.7f, contorno);
    DrawLineEx((Vector2){face.x + 12, face.y + 5},
               (Vector2){face.x + face.width - 12, face.y + 5}, 1.5f,
               Fade(WHITE, pressionado ? .25f : .55f));
    float tamanho = area.height <= 27 ? 18 : 24.5f;
    Color corTexto = ColorIsEqual(cor, (Color){201, 91, 78, 255})
        ? (Color){33, 15, 11, 255} : contorno;
    desenharTexto(fonteTitulos, rotulo, face.x + face.width / 2,
                  face.y + (face.height - tamanho) / 2, tamanho, face.width - 12, true, corTexto);
}

bool botao(Rectangle area, const char *rotulo, Color cor)
{
    desenharBotao(area, rotulo, cor, true);
    return GuiButton(area, NULL) != 0;
}

void botaoIndisponivel(Rectangle area, const char *rotulo)
{
    desenharBotao(area, rotulo, CREME, false);
}

bool campoTexto(Rectangle area, char *valor, int capacidade, bool editando)
{
    return GuiTextBox(area, valor, capacidade, editando) != 0;
}

void desenharSalao(Texture2D textura)
{
    DrawTexturePro(textura, (Rectangle){0, 0, textura.width, textura.height},
                   (Rectangle){0, 0, 1280, 720}, (Vector2){0, 0}, 0, WHITE);
}

void desenharBalcao(Texture2D textura)
{
    DrawTexturePro(textura, (Rectangle){0, 0, textura.width, textura.height},
                   (Rectangle){0, 494, 1280, 226}, (Vector2){0, 0}, 0, WHITE);
}

static Texture2D carregarSprite(const char *caminho, int largura, int altura, Rectangle *limites)
{
    Image imagem = LoadImage(caminho);
    if (!IsImageValid(imagem)) return (Texture2D){0};

    Texture2D textura = {0};
    if (imagem.width == largura && imagem.height == altura &&
        imagem.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 &&
        GetImageColor(imagem, 0, 0).a == 0) {
        if (limites) *limites = GetImageAlphaBorder(imagem, .08f);
        if (!limites || limites->height > 0) textura = LoadTextureFromImage(imagem);
    }
    UnloadImage(imagem);
    if (!IsTextureValid(textura)) {
        TraceLog(LOG_ERROR, "Sprite inválido: %s (esperado %dx%d RGBA com margem transparente)",
                 caminho, largura, altura);
        return (Texture2D){0};
    }
    SetTextureFilter(textura, TEXTURE_FILTER_BILINEAR);
    return textura;
}

static void liberarSpritesCliente(SpritesCliente *cliente)
{
    if (IsTextureValid(cliente->corpoInteiro)) UnloadTexture(cliente->corpoInteiro);
    for (int i = 0; i < TOTAL_EXPRESSOES; i++) {
        if (IsTextureValid(cliente->meioCorpo[i])) UnloadTexture(cliente->meioCorpo[i]);
    }
    *cliente = (SpritesCliente){0};
}

static bool carregarSpritesCliente(SpritesCliente *cliente, const char *prefixo)
{
    static const char *sufixos[TOTAL_EXPRESSOES] = {
        [EXPRESSAO_FELIZ] = "meio_corpo_feliz",
        [EXPRESSAO_IMPACIENTE] = "meio_corpo_impaciente",
        [EXPRESSAO_IRRITADA] = "meio_corpo_irritado"
    };
    Rectangle limites;
    cliente->corpoInteiro = carregarSprite(
        TextFormat("assets/gui/clientes/%s_corpo_inteiro.png", prefixo), 144, 312, &limites);
    if (!IsTextureValid(cliente->corpoInteiro)) return false;

    for (int i = 0; i < TOTAL_EXPRESSOES; i++) {
        cliente->meioCorpo[i] = carregarSprite(
            TextFormat("assets/gui/clientes/%s_%s.png", prefixo, sufixos[i]), 288, 352, &limites);
        if (!IsTextureValid(cliente->meioCorpo[i])) {
            liberarSpritesCliente(cliente);
            return false;
        }
    }
    return true;
}

bool carregarElencoClientes(ElencoClientes *elenco)
{
    if (!carregarSpritesCliente(&elenco->personagens[0], "piloto")) return false;

    for (int i = 1; i < TOTAL_PERSONAGENS; i++) {
        char prefixo[16];
        snprintf(prefixo, sizeof prefixo, "cliente_%02d", i + 1);
        if (!carregarSpritesCliente(&elenco->personagens[i], prefixo)) {
            TraceLog(LOG_WARNING, "Conjunto incompleto (%s): usando o piloto na fila e no atendimento.", prefixo);
        }
    }
    return true;
}

void liberarElencoClientes(ElencoClientes *elenco)
{
    for (int i = 0; i < TOTAL_PERSONAGENS; i++) {
        liberarSpritesCliente(&elenco->personagens[i]);
    }
}

const SpritesCliente *obterSpritesCliente(const ElencoClientes *elenco, int indice)
{
    const SpritesCliente *cliente = &elenco->personagens[indice % TOTAL_PERSONAGENS];
    return IsTextureValid(cliente->corpoInteiro) ? cliente : &elenco->personagens[0];
}

void desenharClienteInteiro(Texture2D textura, Vector2 pes, float escala, bool espelhar)
{
    float larguraOrigem = espelhar ? -textura.width : textura.width;
    DrawTexturePro(textura, (Rectangle){0, 0, larguraOrigem, textura.height},
                   (Rectangle){pes.x, pes.y, 72 * escala, 156 * escala},
                   (Vector2){36 * escala, 150 * escala}, 0, WHITE);
}

void desenharClienteMeioCorpo(Texture2D textura, Vector2 base)
{
    const float altura = 208;
    const float largura = altura * textura.width / textura.height;
    DrawTexturePro(textura, (Rectangle){0, 0, textura.width, textura.height},
                   (Rectangle){base.x, base.y, largura, altura}, (Vector2){largura / 2, altura}, 0, WHITE);
}

static void camada(float x, float y, float largura, float altura, Color cor)
{
    DrawEllipse(x, y + 3, largura, altura, ColorBrightness(cor, -.28f));
    DrawEllipse(x, y, largura, altura, cor);
    DrawEllipse(x - largura * .15f, y - altura * .32f, largura * .72f,
                altura * .35f, ColorBrightness(cor, .17f));
}

static void desenharPrato(float x, float y, float e)
{
    DrawEllipse(x, y + 28 * e, 159 * e, 34 * e, Fade(TINTA, .28f));
    DrawEllipse(x, y + 20 * e, 151 * e, 31 * e, CREME);
    DrawEllipseLines(x, y + 19 * e, 130 * e, 23 * e, MENTA);
}

void desenharHamburguer(float x, float y, float e, bool fechado)
{
    desenharPrato(x, y, e);
    camada(x, y, 105 * e, 24 * e, (Color){221, 143, 64, 255});
    for (int i = -3; i <= 3; i++) {
        camada(x + 25 * e * i, y - (16 + i % 2 * 3) * e, 29 * e, 10 * e, MENTA);
    }
    camada(x, y - 28 * e, 101 * e, 18 * e, MADEIRA);
    camada(x, y - 45 * e, 96 * e, 10 * e, COR_MOSTARDA);
    DrawTriangle((Vector2){x - 45 * e, y - 45 * e}, (Vector2){x - 6 * e, y - 26 * e},
                 (Vector2){x + 28 * e, y - 45 * e}, COR_MOSTARDA);
    if (fechado) {
        camada(x, y - 72 * e, 106 * e, 44 * e, (Color){230, 155, 73, 255});
        for (int i = -3; i <= 3; i++) {
            DrawEllipse(x + i * 21 * e, y - (85 + i % 3 * 8) * e, 4 * e, 2 * e, CREME);
        }
    }
}

enum {
    LARGURA_SPRITE = 512, ALTURA_SPRITE = 192,
    ANCORA_X = 256, ANCORA_Y = 160
};

// Arquivos e avanços da seção "arquivos" do especificacoes.json do kit.
static const struct {
    char simbolo;
    const char *arquivo;
    float avanco;
    float raio;
    Color cor;
} spritesIngredientes[TOTAL_INGREDIENTES] = {
    {PAO_INFERIOR, "pao_inferior", 28, 23, {221, 143, 64, 255}},
    {PAO_SUPERIOR, "pao_superior", 0, 30, {230, 155, 73, 255}},
    {CARNE, "carne", 23, 12, {104, 64, 46, 255}},
    {QUEIJO_CHEDDAR, "queijo_cheddar", 13, 12, {243, 190, 88, 255}},
    {QUEIJO_SUICO, "queijo_suico", 13, 12, {245, 216, 135, 255}},
    {ALFACE, "alface", 17, 12, {114, 163, 121, 255}},
    {BACON, "bacon", 22, 12, {166, 81, 57, 255}},
    {OVO_FRITO, "ovo_frito", 21, 12, {255, 240, 214, 255}},
    {TOMATE, "tomate", 17, 12, {194, 63, 46, 255}},
    {CEBOLA, "cebola", 13, 12, {216, 171, 185, 255}},
    {PICLES, "picles", 15, 12, {104, 128, 62, 255}},
    {KETCHUP, "ketchup", 8, 12, {142, 47, 36, 255}},
    {MOSTARDA, "mostarda", 8, 12, {223, 164, 46, 255}}
};

void carregarIngredientesGraficos(IngredientesGraficos *kit)
{
    for (int i = 0; i < TOTAL_INGREDIENTES; i++) {
        const char *caminho = TextFormat("assets/gui/ingredientes/%s.png", spritesIngredientes[i].arquivo);
        Rectangle limites = {0};
        kit->texturas[i] = carregarSprite(caminho, LARGURA_SPRITE, ALTURA_SPRITE, &limites);
        if (IsTextureValid(kit->texturas[i])) {
            kit->alturas[i] = (ANCORA_Y - limites.y) * .5f;
        } else {
            kit->alturas[i] = 2 * spritesIngredientes[i].raio + 3;
            TraceLog(LOG_WARNING, "Usando desenho geométrico para o ingrediente: %s", caminho);
        }
    }
}

void liberarIngredientesGraficos(IngredientesGraficos *kit)
{
    for (int i = 0; i < TOTAL_INGREDIENTES; i++) {
        if (IsTextureValid(kit->texturas[i])) UnloadTexture(kit->texturas[i]);
    }
    *kit = (IngredientesGraficos){0};
}

static int indiceSprite(char simbolo)
{
    for (int i = 0; i < TOTAL_INGREDIENTES; i++) {
        if (spritesIngredientes[i].simbolo == simbolo) return i;
    }
    return -1;
}

void desenharMontagem(Vector2 base, const PilhaEnc *montagem, const IngredientesGraficos *kit)
{
    size_t total = 0;
    for (const NodoPilha *nodo = montagem->topo; nodo; nodo = nodo->prox) total++;
    if (total == 0) {
        desenharPrato(base.x, base.y - 19, .95f);
        return;
    }

    char simbolos[total];
    size_t posicao = total;
    for (const NodoPilha *nodo = montagem->topo; nodo; nodo = nodo->prox) {
        simbolos[--posicao] = nodo->ingrediente;
    }
    desenharCamadasMontagem(base, simbolos, total, 1, kit);
}

void desenharCamadasMontagem(Vector2 base, const char *simbolos, size_t total,
                             float ampliacao, const IngredientesGraficos *kit)
{
    desenharPrato(base.x, base.y - 19, .95f);
    if (total == 0) return;

    struct {
        int sprite;
        float deslocamento;
        float altura;
    } camadas[total];

    const float limite = (base.y - 320) / ampliacao; // Mantém a montagem abaixo do letreiro.
    float avanco = 0;
    float compressao = 1;
    for (size_t i = 0; i < total; i++) {
        int sprite = indiceSprite(simbolos[i]);
        camadas[i].sprite = sprite;
        camadas[i].deslocamento = avanco;
        camadas[i].altura = sprite >= 0 ? kit->alturas[sprite] : 27;
        if (avanco > 0) compressao = fminf(compressao, (limite - camadas[i].altura) / avanco);
        avanco += sprite >= 0 ? spritesIngredientes[sprite].avanco : 18;
    }

    // Preserva espaço entre camadas quando comprimir somente os avanços as deixaria ilegíveis.
    compressao = fmaxf(.65f, compressao);
    float altura = 0;
    for (size_t i = 0; i < total; i++) {
        altura = fmaxf(altura, camadas[i].deslocamento * compressao + camadas[i].altura);
    }
    const float escala = ampliacao * fminf(1, limite / altura);
    for (size_t i = 0; i < total; i++) {
        int sprite = camadas[i].sprite;
        float y = base.y - camadas[i].deslocamento * compressao * escala;
        if (sprite >= 0 && IsTextureValid(kit->texturas[sprite])) {
            DrawTexturePro(kit->texturas[sprite], (Rectangle){0, 0, LARGURA_SPRITE, ALTURA_SPRITE},
                           (Rectangle){base.x, y, 256 * escala, 96 * escala},
                           (Vector2){ANCORA_X * .5f * escala, ANCORA_Y * .5f * escala}, 0, WHITE);
        } else {
            float raio = sprite >= 0 ? spritesIngredientes[sprite].raio : 12;
            Color cor = sprite >= 0 ? spritesIngredientes[sprite].cor : MADEIRA;
            camada(base.x, y - raio * escala - 3, 99 * escala, raio * escala, cor);
        }
    }
}

void desenharChapa(Rectangle area, bool ilustracao)
{
    DrawRectangleRounded(area, .20f, 10, (Color){159, 151, 132, 255});
    Rectangle grelha = {area.x + 8, area.y + 5, area.width - 16, area.height - 12};
    DrawRectangleRounded(grelha, .20f, 10, TINTA);
    for (float y = grelha.y + 11; y < grelha.y + grelha.height - 4; y += 11)
        DrawLineEx((Vector2){grelha.x + 9, y}, (Vector2){grelha.x + grelha.width - 9, y},
                   2, Fade(CREME, .24f));
    if (!ilustracao) return;
    const float escala = area.height < 60 ? area.height / 60 : 1;
    const float centroY = grelha.y + grelha.height / 2;
    camada(area.x + area.width * .32f, centroY, 35 * escala, 13 * escala, MADEIRA);
    camada(area.x + area.width * .70f, centroY, 29 * escala, 14 * escala, CREME);
    DrawEllipse(area.x + area.width * .70f, centroY - 3 * escala, 11 * escala, 8 * escala, COR_MOSTARDA);
}
