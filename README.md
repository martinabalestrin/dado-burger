# Dado Burger

Trabalho prático final de **INF01203 – Estruturas de Dados** (UFRGS, 2026/2).

---

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
./run.sh pedidos.txt historico.txt
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

```bash
CC=gcc ./run.sh --build-only
```

---
