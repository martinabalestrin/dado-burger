# Dado Burger

Trabalho prático final de **INF01203 – Estruturas de Dados** (UFRGS, 2026/2).
Dado Burger é um jogo por turnos em que o jogador prepara ingredientes e monta
hambúrgueres para atender os clientes na ordem de chegada. O projeto oferece
dois executáveis que compartilham a sessão de jogo.

- **Terminal** — interface textual em C, com opções numéricas e símbolos de ingredientes.
- **Gráfico** — janela redimensionável em C, com raylib para desenho e raygui para controles.

## Compilação

## Estrutura do projeto

```
dado-burguer/
├── src/              # código-fonte (.c e .h) — um módulo por estrutura
│   └── main.c
├── build/            # artefatos de compilação (ignorado no git)
├── run.sh            # compila todos os módulos e executa o projeto
├── .vscode/          # tasks.json (build) e launch.json (run/debug) — config local
├── ISSUES.md         # lista de issues (espelho das issues do GitHub)
├── trab_final.pdf    # especificação
└── README.md
```

Os módulos (`avl.c/.h`, `hash.c/.h`, `heap.c/.h`, `pilha.c/.h`, `fila.c/.h`, `lista.c/.h`,
`timer.c/.h`, etc.) ficam em `src/`, conforme forem sendo criados pelas issues.

---

## Compilação e execução

Para o jogo no terminal:
Na raiz do projeto, use o script `run.sh` (requer Bash e Clang):
Na raiz do projeto, use Bash e um compilador C99. Os scripts usam `clang` por
padrão e aceitam outro compilador por meio de `CC`. O terminal compila os
arquivos C de `src/` e depende apenas das bibliotecas padrão do sistema e da
biblioteca matemática. A instalação gráfica é opcional.

```bash
./run.sh --build-only       # build/dado_burger
./run-gui.sh --build-only   # build/dado_burger_gui
## Estrutura do projeto

```
dado-burguer/
├── src/              # código-fonte (.c e .h) — um módulo por estrutura
│   └── main.c
├── build/            # artefatos de compilação (ignorado no git)
├── run.sh            # compila todos os módulos e executa o projeto
├── .vscode/          # tasks.json (build) e launch.json (run/debug) — config local
├── ISSUES.md         # lista de issues (espelho das issues do GitHub)
├── trab_final.pdf    # especificação
└── README.md
```

Os módulos (`avl.c/.h`, `hash.c/.h`, `heap.c/.h`, `pilha.c/.h`, `fila.c/.h`, `lista.c/.h`,
`timer.c/.h`, etc.) ficam em `src/`, conforme forem sendo criados pelas issues.

---

## Compilação e execução

Para o jogo no terminal:
Na raiz do projeto, use o script `run.sh` (requer Bash e Clang):

```bash
# Apenas compilar, sem iniciar o programa
./run.sh --build-only

# Compilar e executar
./run.sh pedidos_final.txt historico_final.txt
```

Para o jogo na interface gráfica:
Na raiz do projeto, use o script `run-gui.sh` (requer Bash, Clang e raylib):

```bash
# Apenas compilar, sem iniciar o programa
./run-gui.sh --build-only

# Compilar e executar
./run-gui.sh pedidos_final.txt historico_final.txt
```

Para o jogo na interface gráfica:
Na raiz do projeto, use o script `run-gui.sh` (requer Bash, Clang e raylib):

```bash
# Apenas compilar, sem iniciar o programa
./run-gui.sh --build-only

# Compilar e executar
./run-gui.sh pedidos.txt historico.txt
```

O script cria `build/` se necessário, encontra todos os arquivos `.c` em `src/`
(inclusive subpastas) e recompila o executável `build/dado_burger` a cada chamada,
com `-Wall -std=c99` e `-lm`. Não é necessário listar os módulos manualmente.
Se a compilação falhar, o programa não é executado.

Por padrão, o compilador é o Clang. Para usar GCC, utilize nesse formato:
Os dois modos foram validados em **macOS arm64 com Apple Clang 21**. A GUI
foi validada com **raylib 6.0.0**. Linux e Windows ainda não foram validados.
O script gráfico procura a biblioteca dinâmica do macOS, `libraylib.dylib`.

No macOS com Homebrew, a fórmula `raylib` pode ser instalada com
`brew install raylib`. O comando instala a versão oferecida pelo Homebrew, que
pode diferir da versão validada. O script encontra o prefixo com
`brew --prefix raylib`. [Fórmula oficial do Homebrew](https://formulae.brew.sh/formula/raylib)

Para usar uma instalação local específica, aponte `RAYLIB_PREFIX` para o
prefixo que contém `include/raylib.h` e `lib/libraylib.dylib` da arquitetura
do compilador. Exemplo com o prefixo de uma instalação Homebrew:

```bash
RAYLIB_PREFIX="$(brew --prefix raylib)" ./run-gui.sh --build-only
```

---
A raygui 5.0 e os arquivos de fontes já acompanham o projeto.

## Execução com os dados de ensaio

A partir da raiz, copie o histórico de exemplo para gravar a partida em um
arquivo temporário. Execute os dois modos em sequência e encerre o primeiro
antes de iniciar o segundo.

```bash
historico_ensaio="$(mktemp)"
cp tests/dados/historico_teste.txt "$historico_ensaio"
./run.sh tests/dados/pedidos_teste.txt "$historico_ensaio"
./run-gui.sh tests/dados/pedidos_teste.txt "$historico_ensaio"
```

Os scripts recompilam e mudam para a raiz do projeto antes de executar. Os
caminhos relativos dos argumentos são interpretados a partir dessa raiz,
inclusive quando o script é chamado de outro diretório. Use caminhos absolutos
para arquivos fora do projeto. A GUI resolve o cenário, os sprites e as fontes
a partir da raiz.

O primeiro argumento é o arquivo de pedidos, aberto para leitura. O segundo é
o histórico, lido na inicialização e acrescido ao salvar uma partida. Um
histórico inexistente começa vazio e pode ser criado ao salvar, desde que o
diretório de destino exista e seja gravável. A ordenação para exibição usa uma
cópia em memória e preserva a ordem dos registros no arquivo.

Os executáveis gravam `medicoes.log` na raiz quando iniciados pelos scripts. O arquivo é sobrescrito a cada
execução. A carga inicial é registrada na abertura e as métricas acumuladas de
buscas, cache e última ordenação são gravadas ao sair do programa. Execute uma
instância por vez quando houver histórico ou log compartilhado.

## Como jogar

1. Escolha **Novo Jogo**, a dificuldade e o nome do jogador. Consulte o pedido
   do cliente atual.
2. Prepare os ingredientes necessários. Carne, ovo e bacon passam pela chapa e
   têm validade após ficarem prontos.
3. Use **Adicionar** para montar o hambúrguer da base ao topo. **Remover**
   descarta o ingrediente do topo e **Aguardar** avança um turno.
4. O pedido correto é entregue automaticamente. A partida é vencida ao atender
   toda a fila e termina em derrota se a paciência do cliente atual se esgotar
   antes da entrega.

No terminal, o menu de partida usa `1` para preparar, `2` para adicionar,
`3` para remover, `4` para aguardar, `5` para consultar estoque e chapa,
`6` para ver o pedido e `0` para sair da partida. A seleção de ingredientes
usa os símbolos apresentados no menu. Na GUI, use o mouse nos botões e cartões.
A comemoração de 1,8 segundo usa a montagem entregue e pode ser pulada com um
clique. O efeito é visual e não consome turnos.

## Organização do projeto

| Caminho | Responsabilidade |
|---|---|
| `src/main.c` e `src/menu/` | Entrada por argumentos, menus e controle por texto do terminal. |
| `src/jogo/aplicacao.c` e `.h` | Inicialização e finalização de pedidos, cache, histórico e métricas, além da preparação da fila de atendimento e da consulta ordenada usada pela GUI. |
| `src/jogo/sessao.c` e `.h` | Estado, ações e eventos do atendimento. Coordena turnos, paciência, preparo, estoque, montagem e entrega com os módulos de estruturas existentes. |
| `gui/` | Entrada e apresentação gráfica. Converte cliques em ações da sessão e desenha o estado consultado. |
| `assets/gui/` | Cenário, clientes, ingredientes e fontes da GUI. |
| `tests/` | Testes isolados e dados de ensaio. |

A GUI e o terminal chamam a mesma API para iniciar o atendimento, aplicar
ações, consultar estado e prosseguir. A pontuação é definida pela sessão. A GUI
usa os eventos para apresentar efeitos como a comemoração de entrega.

## Recursos gráficos e licenças

O cenário V2 contém salão e balcão. Há oito personagens, cada um com um sprite
de corpo inteiro e três expressões em meio corpo, e treze sprites de
ingredientes. As artes do projeto foram criadas ou revisadas com assistência
de IA. As licenças abaixo se referem aos respectivos componentes de terceiros.

- **raylib 6.0.0** — dependência externa na plataforma validada, sob licença zlib. [Licença oficial da raylib 6.0](https://github.com/raysan5/raylib/blob/6.0/LICENSE)
- **raygui 5.0** — cabeçalho incorporado em `gui/terceiros/`, sob licença zlib. [Origem](https://github.com/raysan5/raygui/tree/5.0) · [Licença incluída](gui/terceiros/LICENSE-raygui.txt)
- **Fredoka SemiBold** — Google Fonts, SIL OFL 1.1. [Origem](https://github.com/google/fonts/tree/main/ofl/fredoka) · [Licença incluída](assets/gui/fontes/Fredoka-OFL.txt)
- **Nunito Sans SemiBold** — Google Fonts, SIL OFL 1.1. [Origem](https://github.com/google/fonts/tree/main/ofl/nunitosans) · [Licença incluída](assets/gui/fontes/NunitoSans-OFL.txt)

## Testes e entrega acadêmica

Os testes isolados de heap e pilha passaram. Os testes isolados de fila e
ordenação têm incompatibilidades de compilação conhecidas. Consulte a
preparação acadêmica para os resultados integrados, a auditoria do §5.2 e a
proposta de declaração de uso de IA. [Preparação acadêmica](docs/preparacao-academica.md)

O enunciado prevê um ZIP com código-fonte, relatório em PDF de até oito páginas
incluindo capa e `medicoes.log` gerado com os dados oficiais de desenvolvimento.
No dia da apresentação, use os arquivos finais fornecidos pela disciplina como
argumentos, sem alterações no código. [Enunciado](trab_final.pdf)

## Fluxo de trabalho (dupla)

Todas as issues seguem o mesmo fluxo:

1. **Uma issue = uma branch** a partir de `main`, nomeada como indicado na issue (`feat/...`).
2. Desenvolver e commitar na branch.
3. Abrir uma **Pull Request** para `main`.
4. A PR é **revisada pelo outro membro da dupla** antes do merge — ninguém dá merge na própria PR.
5. Após aprovação, merge e a branch pode ser apagada.

As issues estão organizadas em **ondas** no GitHub, por *labels* e *milestones*.
