#ifndef GUI_COMPONENTES_H
#define GUI_COMPONENTES_H

#include <raylib.h>
#include "../src/pilha/pilha.h"

extern const Color CREME, VERMELHO, COR_MOSTARDA, MADEIRA, MENTA, TINTA;

typedef enum {
    EXPRESSAO_FELIZ, EXPRESSAO_IMPACIENTE, EXPRESSAO_IRRITADA, TOTAL_EXPRESSOES
} ExpressaoCliente;

typedef struct {
    Texture2D corpoInteiro;
    Texture2D meioCorpo[TOTAL_EXPRESSOES];
} SpritesCliente;

enum { TOTAL_PERSONAGENS = 8 };

typedef struct {
    SpritesCliente personagens[TOTAL_PERSONAGENS];
} ElencoClientes;

typedef struct {
    Texture2D texturas[TOTAL_INGREDIENTES];
    float alturas[TOTAL_INGREDIENTES];
} IngredientesGraficos;

void iniciarComponentes(void);
void liberarComponentes(void);
void textoLimitado(const char *valor, float x, float y, int tamanho,
                   float largura, bool centrar, Color cor);
void textoTitulo(const char *valor, float x, float y, int tamanho, bool centrar, Color cor);
void texto(const char *valor, float x, float y, int tamanho, Color cor);
void textoCentrado(const char *valor, float x, float y, int tamanho, Color cor);
void painel(Rectangle area, Color cor);
bool botao(Rectangle area, const char *rotulo, Color cor);
void botaoIndisponivel(Rectangle area, const char *rotulo);
bool campoTexto(Rectangle area, char *valor, int capacidade, bool editando);
void desenharSalao(Texture2D textura);
void desenharBalcao(Texture2D textura);
bool carregarElencoClientes(ElencoClientes *elenco);
void liberarElencoClientes(ElencoClientes *elenco);
const SpritesCliente *obterSpritesCliente(const ElencoClientes *elenco, int indice);
void desenharClienteInteiro(Texture2D textura, Vector2 pes, float escala, bool espelhar);
void desenharClienteMeioCorpo(Texture2D textura, Vector2 base);
void desenharHamburguer(float x, float y, float escala, bool fechado);
void carregarIngredientesGraficos(IngredientesGraficos *ingredientes);
void liberarIngredientesGraficos(IngredientesGraficos *ingredientes);
void desenharMontagem(Vector2 base, const PilhaEnc *montagem,
                      const IngredientesGraficos *ingredientes);
/* Os símbolos devem estar na ordem de montagem, da base ao topo. */
void desenharCamadasMontagem(Vector2 base, const char *simbolos, size_t total,
                             float ampliacao, const IngredientesGraficos *kit);
void desenharChapa(Rectangle area, bool ilustracao);

#endif
