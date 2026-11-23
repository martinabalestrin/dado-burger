# Preparação acadêmica do Dado Burger

Esta revisão descreve a implementação de `cef5b57` e propõe conteúdo para validação pela dupla. O PDF disponível no repositório é o enunciado. O relatório da dupla não estava disponível para consulta. [Enunciado](../trab_final.pdf)

## Auditoria do §5.2

O §5.2 exige que o relatório seja exibido automaticamente e gravado antes do retorno ao menu após vitória, derrota ou saída voluntária. Para o histórico, exige medição em uma cópia independente, com a cópia fora do intervalo cronometrado, e atualização do log em acessos posteriores.

| Exigência do enunciado | Comportamento atual | Situação | Arquivos e funções | Encaminhamento |
|---|---|---|---|---|
| Exibir o relatório de desempenho automaticamente ao terminar uma partida. | Terminal e GUI mostram resultado e pontuação. O terminal exibe o tempo de uma ordenação quando o usuário escolhe o critério no histórico. Nenhum modo exibe automaticamente o relatório completo ao terminar a partida. | Divergência | `src/menu/menu.c`, `novoJogo` e `abrirHistorico`. `gui/telas.c`, `resultado`. | A dupla deve revisar o fluxo de apresentação e confirmar a exigência com o professor antes de ajustar. |
| Gravar o relatório antes de voltar ao menu após vitória, derrota ou saída voluntária. | A partida acrescenta uma entrada ao histórico. As seções de AVL, hash e ordenação são gravadas em `medicoes.log` em `finalizarAplicacao`, quando o programa termina. A carga inicial é gravada na abertura. | Divergência | `src/jogo/aplicacao.c`, `iniciarAplicacao` e `finalizarAplicacao`. `src/medicoes/medicoes.c`, funções de gravação. | Encaminhar à dupla o ponto de gravação por partida e preservar as rotinas de medição avaliadas. |
| Informar total de comparações, média por busca e tempo total de busca na AVL. | Os três campos constam no log final, junto com o total de buscas. As medições são acumuladas ao longo da execução. | Conteúdo confirmado. Divergência no momento de gravação e no recorte por partida. | `src/jogo/aplicacao.c`, `iniciarAplicacao` e `finalizarAplicacao`. `src/medicoes/medicoes.c`, `inserirAvlDurantePartida`. | Validar com o professor como reportar partidas consecutivas com a mesma aplicação aberta. |
| Informar hits, misses, taxa de acerto, fator de carga e total de colisões nas inserções da hash. | Todos os campos constam no log final, junto com o tamanho da tabela e o número de entradas. O cache e os contadores são mantidos durante a execução. | Conteúdo confirmado. Divergência no momento de gravação e no recorte por partida. | `src/jogo/aplicacao.c`, `iniciarAplicacao` e `finalizarAplicacao`. `src/medicoes/medicoes.c`, `inserirHashDurantePartida`. | A dupla deve confirmar o recorte de medição sem alterar a política de cache por conta própria. |
| Ordenar uma cópia independente, excluindo a cópia do tempo medido. | Ambos os modos criam a cópia antes de chamar `ordenarPartidas`. A cronometragem ocorre nessa função. | Conformidade confirmada neste ponto | `src/menu/menu.c`, `abrirHistorico`. `src/jogo/aplicacao.c`, `consultarHistoricoOrdenado`. `src/ordenacao/ordenacaoHistorico.c`, `ordenarPartidas`. | Manter a separação entre cópia e cronometragem nos ensaios. |
| Informar o tempo de execução do algoritmo escolhido. | O Merge Sort é cronometrado ao escolher um critério. O log final registra o tamanho e o tempo da última ordenação. Os valores iniciais ficam zerados quando não há ordenação. | Conteúdo confirmado após ordenação. Divergência na atualização do log durante a execução. | `ordenarPartidas`, `finalizarAplicacao` e `src/medicoes/medicoes.c`, `inserirOrdenacaoHistorico`. | Incluir uma ordenação com os dados oficiais de desenvolvimento no roteiro de medições. |
| Repetir a medição e atualizar o log em acessos subsequentes ao Histórico. | A medição é repetida ao selecionar um critério novamente. Entrar no Histórico não dispara a ordenação por si só. O tamanho e o tempo anteriores são substituídos em memória e só o último resultado é gravado no encerramento. | Divergência na atualização do log. Confirmar com o professor se o gatilho deve ser a entrada na tela ou a escolha do critério. | `abrirHistorico`, `gui/telas.c`, `historico` e `ordenarHistoricoGrafico`, `consultarHistoricoOrdenado` e `inserirOrdenacaoHistorico`. | Confirmar a interpretação de acesso e o formato esperado para medições sucessivas. |

A verificação de conteúdo confirma a presença dos campos, sem atestar a exatidão experimental dos contadores ou dos tempos para os arquivos oficiais de desenvolvimento. Os dados de ensaio em `tests/dados/` servem à validação funcional.

## Testes isolados e integrados

| Teste | Estado | Evidência ou encaminhamento |
|---|---|---|
| `tests/test_heap.c` e `tests/test_pilha.c` | Passaram na auditoria. | Executados em cópia isolada com os módulos correspondentes. |
| `tests/test_fila.c` | Bloqueado na compilação. | Usa o tipo `FilaEnc`, campos como `Cliente.id` e inicializadores com mais campos do que a estrutura atual. Encaminhar à dupla a atualização do teste para a API existente. |
| `tests/test_ordenacao.c` | Bloqueado na compilação. | Chama `depurarHistorico`, função sem declaração na API atual. Encaminhar à dupla a revisão dessa chamada no teste. |
| Atendimento e histórico integrados | Passaram nos ensaios da Fase 2. | O terminal completou três entregas com preparo e cozimento. A GUI exercitou a fila, o histórico, a ordenação, a paginação e a entrega. Esses ensaios cobrem o fluxo de uso e deixam pendentes os dois testes isolados. |

## Estrutura proposta para o relatório

Usar o template da disciplina e a ordem do §9. O limite é de oito páginas,
incluindo a capa. A distribuição abaixo é uma sugestão de organização.

| Seção obrigatória | Conteúdo a ajustar ou completar | Espaço sugerido |
|---|---|---:|
| Capa | Identificação da dupla, trabalho e disciplina. | 1 página |
| 1. Introdução | Objetivo, requisitos atendidos e lacunas verificadas, com justificativa. | ½ página |
| 2. O jogo | Turnos, ações, parâmetros e suas justificativas, fluxo terminal e gráfico. Incluir o diagrama dos dois pontos de entrada chamando a mesma sessão e indicar onde ficam as partes de apresentação. | 1 página |
| 3. Estruturas de dados | Papel e complexidade de AVL, hash, lista contígua, lista encadeada, pilha, fila, heap e Merge Sort. Explicar e justificar as adaptações do código de aula, função hash, tamanho da tabela e política de colisão. | 2 páginas |
| 4. Medições e análise | Método, ambiente, dados oficiais, tabelas com resultados coletados pela dupla e comparação com a teoria. Resumir testes, pendências e limitações, inclusive o §5.2. | 2½ páginas |
| 5. Conclusão | Dificuldades, divisão de tarefas e aprendizados. | ½ página |
| 6. Declaração de uso de IA | Ferramentas, finalidades e artefatos, com a revisão de escopo indicada abaixo. | ½ página |

## Proposta de declaração de uso de IA

> Usamos ChatGPT para orientar o design e elaborar instruções de produção
> visual, e ChatGPT/ImageGen para criar, revisar e preparar artes da interface,
> incluindo cenário, personagens e ingredientes. Usamos Codex para implementar
> a interface gráfica, preparar roteiros de validação e documentação e auxiliar
> a refatoração da coordenação do jogo. Essa refatoração concentrou a
> inicialização e o encerramento em um módulo de aplicação e o atendimento
> em um módulo de sessão. As duas interfaces usam ambos. Os integrantes da
> dupla revisaram o escopo, aprovaram decisões de design, avaliaram os artefatos produzidos e
> validaram os resultados por etapas.
>
> As implementações avaliadas de AVL, hash, listas, pilha, fila, heap mínimo,
> Merge Sort e os módulos de métricas são distintas do código da GUI. A
> coordenação assistida inclui chamadas às operações existentes dessas
> estruturas e às rotinas de medição. A comparação de versões confirma que os
> arquivos das implementações avaliadas ficaram intactos nos commits da
> interface e da sessão. A procedência do trabalho anterior a essa comparação
> deve ser confirmada pela dupla.

O §10 exige a declaração de todo uso de IA e veda seu uso para gerar,
modificar, sugerir código ou depurar implementações das estruturas, operações,
algoritmos e contadores avaliados. A extração assistida da coordenação moveu
sequências de chamadas a operações avaliadas de `src/menu/menu.c` para
`src/jogo/sessao.c` e `src/jogo/aplicacao.c`. Isso deixa uma questão de
enquadramento acadêmico mesmo com as implementações avaliadas intactas.
Recomenda-se apresentar ao professor os diffs e o escopo exato da assistência
para obter uma avaliação explícita antes da entrega. A dupla deve complementar
a declaração com a verificação de autoria e assistência no restante do projeto.
