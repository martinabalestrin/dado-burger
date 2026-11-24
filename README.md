# Dado Burger

O projeto oferece duas versões do jogo: uma interface de terminal em `src/` e
uma interface gráfica em `interface-grafica/`.

```
dado-burguer/
├── src/                    # código-fonte da versão de terminal
├── interface-grafica/
│   ├── gui/                # telas e componentes gráficos
│   ├── src/                # módulos usados pela versão gráfica
│   └── assets/gui/         # cenários, personagens, ingredientes e fontes
├── build/                  # executáveis gerados pelos scripts
├── run.sh                  # compila e executa a versão de terminal
├── run-gui.sh              # compila e executa a versão gráfica
├── pedidos.txt             # pedidos usados como entrada
├── historico.txt           # histórico de partidas
├── relatorio.pdf           # relatório do projeto
└── README.md
```

## Compilação e execução

Os exemplos abaixo partem da raiz do projeto. Os scripts usam `clang` por
padrão e aceitam outro compilador pela variável `CC`.

Para compilar a versão de terminal:

```bash
./run.sh --build-only
```

Para compilar a interface gráfica:

```bash
./run-gui.sh --build-only
```

Os executáveis são criados em `build/dado_burger` e
`build/dado_burger_gui`, respectivamente. Para compilar e executar, informe
o arquivo de pedidos e o arquivo de histórico:

```bash
./run.sh pedidos.txt historico.txt
./run-gui.sh pedidos.txt historico.txt
```

A versão de terminal compila os arquivos C de `src/` e depende das bibliotecas
padrão do sistema e da biblioteca matemática. A interface gráfica também
requer a raylib. No macOS com Homebrew, instale-a com `brew install raylib`;
o script localiza a instalação usando `brew --prefix raylib`. [Fórmula oficial
do Homebrew](https://formulae.brew.sh/formula/raylib)

Para usar outra instalação local da raylib, defina `RAYLIB_PREFIX` com o
prefixo que contém `include/raylib.h` e `lib/libraylib.dylib`:

```bash
RAYLIB_PREFIX="/caminho/para/raylib" ./run-gui.sh --build-only
```

O cabeçalho raygui e os arquivos de fontes estão incluídos no projeto.
