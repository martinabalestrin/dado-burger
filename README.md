# Dado Burger

Trabalho prático final de **INF01203 – Estruturas de Dados** (UFRGS, 2026/2).
Jogo de terminal em que o jogador atende pedidos de clientes montando hambúrgueres, integrando e medindo as estruturas de dados estudadas na disciplina.

A especificação completa está em [`trab_final.pdf`](trab_final.pdf) — **fonte única de verdade** para qualquer decisão de escopo.

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

Na raiz do projeto, use o script `run.sh` (requer Bash e Clang):

```bash
# Apenas compilar, sem iniciar o programa
./run.sh --build-only

# Compilar e executar
./run.sh pedidos.txt historico.txt
```

O script cria `build/` se necessário, encontra todos os arquivos `.c` em `src/`
(inclusive subpastas) e recompila o executável `build/dado_burger` a cada chamada,
com `-Wall -std=c99` e `-lm`. Não é necessário listar os módulos manualmente.
Se a compilação falhar, o programa não é executado.

Por padrão, o compilador é o Clang. Para usar GCC, por exemplo:

```bash
CC=gcc ./run.sh --build-only
```

Se aparecer `permission denied`, use `bash run.sh` (ou `bash run.sh --build-only`),
ou conceda permissão uma vez com `chmod +x run.sh`.

Os argumentos são repassados ao programa sem alteração. Conforme o **Apêndice C**,
os arquivos de pedidos e histórico devem ser recebidos via `argv[1]` e `argv[2]`,
sem nomes fixos; esse uso depende da implementação da entrada por linha de comando
(issue #1). O script não lê nem valida esses arquivos.

**VS Code:** `tasks.json` já compila o projeto inteiro e `launch.json` roda/depura com os
dois argumentos — basta `F5`. (A pasta `.vscode/` é local, não versionada.)

---

## Fluxo de trabalho (dupla)

Todas as issues seguem o mesmo fluxo:

1. **Uma issue = uma branch** a partir de `main`, nomeada como indicado na issue (`feat/...`).
2. Desenvolver e commitar na branch.
3. Abrir uma **Pull Request** para `main`.
4. A PR é **revisada pelo outro membro da dupla** antes do merge — ninguém dá merge na própria PR.
5. Após aprovação, merge e a branch pode ser apagada.

As issues estão organizadas em **ondas** (ver abaixo e no GitHub via *labels* e *milestones*).

---

## Ordem recomendada (ondas)

| Onda | Issues | Tema | Observação |
|------|--------|------|------------|
| **0 — Infra** | #1–#3 | argc/argv, timer, log de medições | Fazer **primeiro, juntos**; quase tudo depende disso. |
| **1 — Estruturas** | #4–#11 | AVL, lista contígua, hash, lista encadeada, pilha, fila, heap, ordenação | Independentes entre si → ideais para **dividir em paralelo**. |
| **2 — Mecânicas** | #12–#17 | menu, turnos, sorteio, paciência, parâmetros, interface | Consomem as estruturas da Onda 1. |
| **3 — Medições** | #18–#20 | métricas de carregamento, de busca, relatório | Só fazem sentido com estruturas e partida prontas. |

**Dependências dentro da Onda 1** (quem depende espera a entrega do pré-requisito):

- #6 (hash) depende de #4 (AVL) — lógica hit/miss sobre a AVL.
- #8 (pilha) depende de #7 (lista encadeada) — verificação da receita compara as duas.
- As demais (#5, #9, #10, #11) não têm pré-requisito.

Cada issue lista explicitamente suas dependências no campo **Depende de:**.

---

## Expectativa de tempo por issue

Estimativas **por pessoa**, assumindo o código das aulas como ponto de partida (adaptar, não
reescrever do zero) e incluindo abrir a PR. A revisão da PR pelo par soma ~30 min–1h por issue.

| Tamanho (label) | Significado | Tempo estimado |
|-----------------|-------------|----------------|
| 🟢 `size: pequena` | Adaptação direta de código de aula ou lógica isolada | **1–3 h** |
| 🟡 `size: media-pequena` | Estrutura com adaptação de interface + integração com outra parte | **3–6 h** |

Se uma issue 🟡 passar de ~6h na prática, é sinal de que vale quebrá-la em partes menores.

---

## Regras de implementação (da especificação)

- **Linguagem:** C ou C++.
- **Proibido** usar estruturas prontas de bibliotecas (ex.: `std::map`, `std::priority_queue`,
  `std::stack`, `std::queue`, `std::unordered_map`). Todas as estruturas devem ser as da
  disciplina, podendo ser adaptadas. *(§2)*
- O código das estruturas foi dado em aula e **deve ser o ponto de partida** — o objetivo é
  adaptar e organizar, não reimplementar. Decisões de projeto são critério de avaliação e devem
  ser justificadas no relatório. *(§2, §4)*
- **Medições obrigatórias:** todas gravadas em `medicoes.log` (criado/sobrescrito a cada
  execução). A ausência do arquivo ou de qualquer métrica gera desconto. *(§5)*

---

## Entregáveis (Moodle)

Um único `.zip` (`tp-grupoX.zip`) contendo:

- **Código-fonte** organizado por módulo, que compile e rode conforme o Apêndice C.
- **`medicoes.log`** gerado com as versões de desenvolvimento (`pedidos.txt`, `historico.txt`).
- **Relatório em PDF** (máx. 8 páginas) seguindo o template da disciplina, com as seções:
  Introdução, O jogo, Estruturas de dados, Medições e análise, Conclusão e Declaração de uso de IA.

Datas (definição de grupo, entrega/apresentação) estão no Moodle. No dia da apresentação são
divulgados `pedidos_final.txt` e `historico_final.txt`, executados ao vivo **sem alterar o código**.
