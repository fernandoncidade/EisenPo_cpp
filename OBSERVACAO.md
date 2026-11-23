<!-- Multilanguage OBSERVACAO.md for EisenPo_cpp -->

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> |
  <a href="#enus">🇺🇸 English (US)</a> |
  <a href="#tree">🌳 Árvore de Arquivos / File Tree</a>
</p>

---

<h2 id="ptbr">🇧🇷 Português (BR)</h2>

# OBSERVAÇÃO TÉCNICA - Estado atual do projeto

Versão documentada: `2026.11.22.0`
Data do snapshot técnico: `22 de novembro de 2026`
Repositório: `https://github.com/fernandoncidade/EisenPo_cpp` (`EisenPo_cpp`)
Autor identificado no projeto: `Fernando Nillsson Cidade` (`linceu_lighthouse@outlook.com`)
Licença do repositório: `GNU AGPLv3 (GNU Affero General Public License v3 — LICENSE)`
Licença da Edição Microsoft Store: `EULA Oficial + CLC (Distribuição Oficial 9P289X0185C3)`
Documentação em `source/assets/`: `Autorizada liberdade total para alteração, exclusão e adaptação por desenvolvedores e forks`

---

## 1. Escopo deste documento e Arquitetura de Duplo Licenciamento

Este documento registra o estado técnico real do projeto **EisenPo** após as implementações, unificação nativa dos módulos, modernização de toolchains e estruturação documental até a versão **`2026.11.22.0`**.

O objetivo é documentar com fidelidade factual:

- a arquitetura modular nativa integrada ao executável `EisenPo.exe` em C++ (configurado para `C++23` em `CMakeLists.txt`/`CMakePresets.json`) e **Qt 6.12.0**;
- os fluxos oficiais de build dual-toolchain (**MSVC `14.52.37025` / Windows SDK `10.0.28000.0`** e **MinGW-w64 `GCC 15.1.0`**) e distribuição externa limpa sem `vc_redist.x64.exe` via CMake e `CMakePresets.json`;
- o modelo completo de **Duplo Licenciamento** (`GNU AGPLv3` no código-fonte público, liberdade irrestrita aos desenvolvedores nos documentos em `source/assets/` e **EULA Oficial** para o pacote assinado na Microsoft Store) e a suíte documental bilíngue em `docs/` e `.github/`;
- a persistência local 100% offline em `%LOCALAPPDATA%\EisenPo` (`tarefas.json`, `listas.db`, `eisenpo_gantt.db`), o sistema de logs `file_eisenpo` e a validação integral de compilação em **C++23** (`CMAKE_CXX_SCAN_FOR_MODULES OFF` e refatoração do identificador `concept` para `concept_text`/`conceito` nos 5 arquivos de `source/modules/feynman/Feynman/`).

---

## 2. Linha técnica da versão 2026.11.22.0

A versão 2026.11.22.0 consolida a padronização cromática unificada entre as visões de planejamento (WBS e Gantt) e a simulação 4D BIM, implementa tooltips informativos ricos e dinâmicos retraduzidos em tempo de execução na linha do tempo do Gantt, e expande o utilitário Version Editor para cobrir toda a base contratual e legal do módulo Agenda:

| Componente | Tipo | Estado operacional |
| --- | --- | --- |
| EisenPo (Eisenhower) | Core integrado | Sempre disponível e permanente no núcleo do executável. |
| Pomodoro | Módulo nativo | Compilado e integrado nativamente no executável EisenPo. |
| Feynman | Módulo nativo | Compilado e integrado nativamente no executável EisenPo. |
| Registro de Atividades | Módulo nativo | Compilado e integrado nativamente no executável EisenPo. |
| Gantt & BIM 4D | Módulo nativo | Compilado e integrado nativamente no executável EisenPo. |

Principais mudanças consolidadas na versão 2026.11.22.0:
- **Padronização Cromática Unificada de Status na Tabela WBS e no Diagrama de Gantt (`GanttWbsTreeWidget.cpp`, `GanttTimelineWidget.cpp`)**:
  - Unificação completa das cores de fonte da "Tabela WBS" e das caixas/nós na régua temporal do Gantt com base nos badges de status do quadro "Simulação Temporal - 4D BIM" vinculado à tabela "Simulação 4D":
    - `✅ Executado:` Verde (`#4caf50`) para atividades 100% concluídas, aplicando uma tonalidade suave e contraste calibrado nas caixas do Gantt para diferenciar sutilmente as atividades concluídas das demais;
    - `🔄 Em Andamento:` Azul (`#2196f3`) para atividades atualmente em execução dentro do prazo;
    - `⚠️ Atrasado:` Vermelho (`#f44336`), cobrindo rigorosamente tanto o início atrasado (data atual superior ao início previsto com 0% de execução) quanto o término atrasado (data atual superior ao término planejado com tarefa não concluída);
    - `⏳ Futuro:` Cinza neutro (`#9e9e9e`) para atividades agendadas que ainda não atingiram sua data de início;
  - Cálculo de status temporal dinâmico e consistente, eliminando divergências visuais entre as tabelas de engenharia e o diagrama gráfico.
- **Tooltips Informativos Dinâmicos, Ricos e Retraduzidos em Tempo de Execução (`GanttTimelineWidget.cpp`, `GanttTimelineWidget.hpp`)**:
  - Exibição de tooltip completo ao passar o mouse sobre as caixas de atividade e nós na régua do Gantt: código WBS, badge estilizado de status, datas de início e término formatadas conforme localidade (`dd/MM/yyyy HH:mm` vs `MM/dd/yyyy HH:mm`), duração em dias inteiros, percentual de progresso físico, caminho crítico colorido (`SIM`/`Não`, `YES`/`No`) e folga total em horas;
  - Resposta a `QEvent::LanguageChange` reconstruindo imediatamente os textos informativos com tradução dinâmica e dismiss limpo do tooltip ativo.
- **Ampliação do Editor de Versões para Documentos Contratuais e Legais do Módulo Agenda (`mocks/find_VersionEditor.hpp`, `mocks/find_VersionEditor.cpp`)**:
  - Automação da atualização de versão e data em todos os 12 arquivos de termos legais, licenças, avisos de direitos autorais e políticas de privacidade do `plugin_agenda` nos idiomas pt_BR e en_US (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `NOTICES`, `PRIVACY_POLICY`), além dos arquivos em `library/`, todos os `.md` da raiz e `docs/*.md` (incluindo renomeação/título dinâmico de `docs/release-v2026.11.22.0.md`).
- **Reorganização Estrutural, MSVC Puro, Distribuições Planas e Manuais Atualizados (`source/`, `library/`, `images/interfaces/`, `MANUAL.md`, `incore_12/16/17`)**:
  - Realocação de `windows_app_icon.rc.in` para `source/windows_app_icon.rc.in`, centralização dos scripts e manifestos de empacotamento em `library/` e movimentação das capturas de tela oficiais para `images/interfaces/{pt_BR,en_US}`;
  - Compilação em **MSVC puro** (`Visual Studio 18 2026` / MSBuild `18.12.3`) em `build/build_EisenPo_msvc` e `build/build_mocks_msvc`, remoção dos presets portáteis e distribuições externas planas em `D:/MISCELANEAS/VSCode/EisenPo/{EisenPo_cpp_msvc,EisenPo_cpp_mingw,mocks_msvc,mocks_mingw}`;
  - Ampliação do manual do usuário (`MANUAL.md`, `incore_12_Manual.*`, `incore_16_manual_pt-BR.*`, `incore_17_manual_en-US.*`) e notas de lançamento bilíngues (`source/assets/eisenhower/RELEASE/RELEASE NOTES_*.txt`) para a versão `2026.11.22.0`.

---

## 3. Linha técnica da versão 2026.9.27.0

A versão 2026.9.27.0 consolida a unificação arquitetural definitiva dos módulos de produtividade dentro do executável principal do EisenPo, centraliza todos os catálogos de tradução no repositório canônico e otimiza a concorrência e persistência de dados:

| Componente | Tipo | Estado operacional |
| --- | --- | --- |
| EisenPo (Eisenhower) | Core integrado | Sempre disponível e permanente no núcleo do executável. |
| Pomodoro | Módulo nativo | Compilado e integrado nativamente no executável EisenPo. |
| Feynman | Módulo nativo | Compilado e integrado nativamente no executável EisenPo. |
| Registro de Atividades | Módulo nativo | Compilado e integrado nativamente no executável EisenPo. |
| Gantt & BIM 4D | Módulo nativo | Compilado e integrado nativamente no executável EisenPo. |

Principais mudanças consolidadas na versão 2026.9.27.0:
- **Unificação Arquitetural e Compilação Nativa dos Módulos (`source/modules/`, `CMakeLists.txt`)**:
  - Integração definitiva dos quatro módulos de produtividade (`pomodoro`, `feynman`, `agenda` e `gantt`) na árvore `source/modules/`;
  - Transformação dos módulos em alvos diretos de compilação do executável `EisenPo`, eliminando o overhead de carregamento dinâmico via `QPluginLoader` e a necessidade de distribuição de arquivos DLL separados (`PomodoroPlugin.dll`, `FeynmanPlugin.dll`, `AgendaPlugin.dll`, `GanttPlugin.dll`) na pasta `plugins/`;
  - Limpeza profunda no `CMakeLists.txt`, removendo alvos órfãos (mapamental, modelo, audiolivro), scripts redundantes de deploy e cópia de artefatos de plugins;
  - Despacho imediato de chamadas e sinais em memória entre o host e os módulos, garantindo inicialização e resposta instantâneas.
- **Centralização e Unificação Completa dos Catálogos de Tradução (`source/language/translations/`)**:
  - Consolidação de todos os arquivos de tradução (`.ts` e `.qm`) dos módulos (`eisenhower_organizer`, `pomodoro`, `feynman`, `agenda`, `gantt`) sob `source/language/translations/`;
  - Implementação de comando único de deploy no `CMakeLists.txt` copiando todos os arquivos `.qm` diretamente para o diretório `translations/` do executável;
  - Atualização dos utilitários `CompileTranslationsTerminal` e `CompileTranslationsGUI` em `mocks/` para suportar compilação por módulo específico (`--module <id>`) ou em lote global (`--all`), com resolução robusta de ferramentas Qt Linguist (`lrelease.exe`).
- **Persistência Assíncrona e Performance Concorrente (`UnifiedTaskStore.cpp`, `app_09_add_task.cpp`)**:
  - Gravação atômica não-bloqueante no armazenamento unificado de tarefas (`tarefas.json`), eliminando micro-congelamentos na thread principal de interface durante o salvamento de tarefas e sincronização com o cronograma.

---

## 4. Linha técnica da versão 2026.9.26.0

A versão 2026.9.26.0 implementa o sistema completo de registro persistente e carregamento automático das disposições de layout de tabelas do Cronograma Gantt & BIM 4D no arquivo `layout.json`, otimiza drasticamente o tempo de abertura de todas as ferramentas do menu Ferramentas por meio de extração de tarefas em memória $O(1)$ sem I/O síncrono em disco e restaura a resolução nativa de dependências dinâmicas no Windows (`qsqlite.dll` e DLLs do sistema):

| Componente | Tipo | Estado operacional |
| --- | --- | --- |
| EisenPo (Eisenhower) | Host integrado | Sempre disponível e permanente. |
| Pomodoro | Plugin opcional | Carregado dinamicamente de `plugins/pomodoro`. |
| Feynman | Plugin opcional | Carregado dinamicamente de `plugins/feynman`. |
| Registro de Atividades | Plugin opcional | Carregado dinamicamente de `plugins/agenda`. |
| Gantt & BIM 4D | Plugin opcional | Carregado dinamicamente de `plugins/gantt` (`GanttPlugin.dll`). |

Principais mudanças consolidadas na versão 2026.9.26.0:
- **Persistência Completa de Layout de Tabelas do Gantt em `layout.json` (`plugin/gantt/Gantt/ui/GanttMainWindow.hpp`, `GanttMainWindow.cpp`, `source/app/app_07_set_task_layout.cpp`)**:
  - Implementação dos métodos dedicados `load_table_layout_from_json()` e `save_table_layout_to_json(TableLayoutMode mode)` na classe `GanttMainWindow`;
  - Localização canônica do arquivo via `QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/layout.json"`;
  - Mapeamento estrito bidirecional do enum `TableLayoutMode` para identificadores JSON:
    - `TableLayoutMode::Original` $\leftrightarrow$ `"original"`;
    - `TableLayoutMode::VerticalTop` $\leftrightarrow$ `"vertical_top"`;
    - `TableLayoutMode::VerticalBottom` $\leftrightarrow$ `"vertical_bottom"`;
    - `TableLayoutMode::HorizontalLeft` $\leftrightarrow$ `"horizontal_left"`;
    - `TableLayoutMode::HorizontalRight` $\leftrightarrow$ `"horizontal_right"`;
  - Gravação atômica segura via `QSaveFile` com preservação e mesclagem de chaves JSON pré-existentes, garantindo total compatibilidade com as opções salvas pela janela principal (`_save_task_layout_preference` em `app_07_set_task_layout.cpp`);
  - Carregamento automático na montagem da interface (`setup_ui`), salvamento imediato a cada clique no menu `Configurações -> 📐 Layout -> 📊 Tabelas` e sincronização de checkmarks em tempo de execução (`_retranslate_actions`).
- **Abertura Instantânea e Extração em Memória $O(1)$ de Ferramentas de Produtividade (`recpro_01_utilitarios_compartilhados.hpp`, `recpro_03_recorrencias.cpp`, `recpro_04_lembretes_locais.cpp`, `recpro_06_revisoes_plugins.cpp`)**:
  - Implementação do método `obter_tarefas_unificadas_diretas(...)` em `recpro_01_utilitarios_compartilhados.hpp`, extraindo a coleção de tarefas diretamente das estruturas de dados e caches em memória do host e dos plugins;
  - Eliminação completa de chamadas prévias síncronas a `salvar_tudo_com_backup()` ao acionar itens do menu Ferramentas, eliminando travamentos de I/O em disco;
  - Proteção por flag `is_initializing` contra emissão excessiva de sinais de widgets durante a construção dos diálogos (*Gerar próximas recorrências* e *Ativar lembretes locais*);
  - Renderização de prévia e cálculo de itens em lote em passagem única ($O(N)$ direto em memória).
- **Cache de Tarefas e Sincronização Sob Demanda no Gantt (`EisenhowerGanttHostAdapter.hpp`, `EisenhowerGanttHostAdapter.cpp`, `GanttWbsTreeWidget.cpp`)**:
  - Implementação de tabela hash `task_cache_` (`QHash<QString, HostTaskDto>`) indexada por identificador único no adaptador do Gantt, permitindo buscas de tarefas em tempo $O(1)$;
  - Sincronização condicional quando a janela do Gantt estiver minimizada ou oculta, adiando a atualização pesada de nós da árvore e linhas do tempo até que a janela seja exibida (`showEvent`);
  - Otimização do método `populate` em `GanttWbsTreeWidget.cpp`, substituindo varreduras lineares aninhadas por mapeamento em tabela hash para resolução de hierarquias WBS e dependências CPM em tempo linear $O(N)$.
- **Otimização de Inicialização do Registro de Atividades (`ginter_09_change_language.cpp`, `ginter_14_custom_agendas.cpp`)**:
  - Persistência idempotente e sob demanda de `agenda_config.json`, eliminando acessos redundantes em disco e chamadas repetitivas de re-tradução na abertura da janela de atividades.
- **Restauração da Resolução Nativa de DLLs no Windows (`source/app/app_02_get_text.cpp`)**:
  - Remoção da diretiva restritiva `SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS)` preservando o registro de diretórios adicionais via `AddDllDirectory`;
  - Restabelecimento do encadeamento padrão do `%PATH%` no Windows, garantindo o carregamento imediato e sem falhas de drivers de banco de dados (`qsqlite.dll`) e dependências nativas.

Histórico das versões consolidadas:

- **Versão 2026.9.25.0**:
  - **Cálculo Físico Discreto de Progresso no Gantt por Ciclos Pomodoro (`plugin/gantt/gantt_plugin.cpp`)**:
    - Cálculo de avanço físico proporcional: `(pomodoros_completados / total_target) * 100.0`;
    - Degraus discretos: 0%, 25%, 50%, 75%, 100% para meta de 4 pomodoros; 0%, 20%, 40%, 60%, 80%, 100% para meta de 5;
    - Eliminação do percentual arbitrário inicial de 10%;
  - **Sincronização Instantânea em Tempo Real entre Host e Cronograma (`gantt_plugin.cpp`, `app_01_eisenhower_matrix_app.cpp`, `agregador_01_ui.cpp`)**;
  - **Tradução Dinâmica em Tempo de Execução das Janelas de Ferramentas (`recpro_01_utilitarios_compartilhados.hpp`, `recpro_03_recorrencias.cpp`, `recpro_04_lembretes_locais.cpp`, `recpro_05_relatorios_fluxos.cpp`, `recpro_06_revisoes_plugins.cpp`)**;
- **Versão 2026.9.23.0**:
  - **Eliminação de Lentidão e Inicialização Instantânea Sem Congelamentos (`pomodoro_support.cpp`, `pomo_01_setup_ui.cpp`, `gtemp_01_setup_ui.cpp`)**:
    - Resolução da perda expressiva de desempenho e do travamento/congelamento de vários segundos que ocorria ao inicializar o aplicativo;
    - Substituição da instanciação prévia forçada e aquecimento de motores multimídia por inicialização sob demanda (*lazy initialization*), impedindo bloqueios síncronos no loop de eventos do Qt durante a montagem da interface principal;
    - Liberação instantânea da GUI para interação manual logo na inicialização.
  - **Layout Integrado PoCa (Pomodoro + Calendário Duplo) (`app_07_set_task_layout.cpp`, `incore_01_initUI.cpp`, `incore_03_criar_menu_configuracoes.cpp`, `app_10_atualizar_textos.cpp`, `app_02_get_text.cpp`)**:
    - Implementação do layout `PoCa` acessível via `Configurações -> Layout de Tarefas -> Interface do Usuário -> PoCa -> Ativar / Desativar`;
    - Agrupamento vertical unificado na coluna esquerda da aplicação dos módulos de foco e tempo: 1. Temporizador Pomodoro, 2. Configurações, 3. Calendário 1 e 4. Calendário 2;
    - Sincronização e transições fluidas ao alternar layouts de tarefas, preservando preferências em `QSettings`.
  - **Alinhamento Simétrico das Molduras dos 4 Quadros (`ui_14_CalendarPanel.cpp`, `ui_09_Calendar.cpp`, `pomo_01_setup_ui.cpp`, `gtemp_01_setup_ui.cpp`)**:
    - Padronização rigorosa das margens laterais externas (esquerda e direita) dos quadros "Temporizador", "Configurações", "Calendário 1" e "Calendário 2";
    - Alinhamento milimétrico simétrico entre todas as molduras da coluna lateral esquerda, garantindo uma linha de contorno perfeitamente coesa em toda a extensão vertical.
  - **Espaçamento Rente Vertical Uniforme (6px) entre Calendários (`ui_14_CalendarPanel.cpp`)**:
    - Compactação da distância vertical entre o Calendário 1 e o Calendário 2, reduzindo o espaçamento excessivo para 6px, idêntico ao espaçamento existente abaixo do quadro "Configurações", conferindo acabamento rente contínuo.
  - **Sequenciamento Dinâmico $2 \times K$ de Meses na Replicação Horizontal de Calendários (`ui_14_CalendarPanel.cpp`, `ui_09_Calendar.cpp`)**:
    - Lógica matricial sequencial contínua ao arrastar a barra divisória vertical (*splitter*) para a direita:
      - Linha 1 (calendários superiores): meses $M, M+1, \dots, M+K-1$;
      - Linha 2 (calendários inferiores): meses $M+K, M+K+1, \dots, M+2K-1$;
    - Exemplo com 2 colunas ($2 \times 2 = 4$ calendários):
      - Linha 1: Calendário 1 = Mês de Setembro; Calendário 3 = Mês de Outubro.
      - Linha 2: Calendário 2 = Mês de Novembro; Calendário 4 = Mês de Dezembro.
    - Sincronização contínua encadeada ao navegar meses por qualquer seletor.
  - **Proteção e Dimensionamento Preventivo da Barra de Rolagem (`app_03_initUI.cpp`, `app_01_eisenhower_matrix_app.cpp`, `ui_14_CalendarPanel.cpp`)**:
    - Prevenção ativa contra sobreposição da barra de rolagem vertical sobre a moldura direita dos quadros da coluna lateral ao inicializar a interface gráfica nas dimensões padronizadas;
    - Definição de largura mínima da área de rolagem considerando a largura da scrollbar e margens úteis (`setMinimumWidth(396)`), com ajuste proporcional inicial na distribuição do splitter em relação aos quadrantes da Matriz de Eisenhower.

- **Versão 2026.9.21.0**:
  - **Resposta Instantânea e Desacoplamento Assíncrono no Diálogo de Edição de Tarefas (`mmc_11_edit_task_dialog.cpp`)**:
    - Eliminação do congelamento de interface e atraso no clique de confirmação do botão "OK" da caixa de diálogo informativa `"✏️ Editar Tarefa. (¡) Alterações aplicadas com sucesso."`;
    - Desacoplamento da gravação síncrona em disco (`save_tasks_now()`) e da sincronização pesada do cronograma Gantt com recálculo de CPM que eram executadas previamente antes e durante o bloqueio modal `message_box.exec()`;
    - Aplicação imediata das alterações em memória e na interface gráfica, permitindo fechamento sub-milissegundo da janela de diálogo com persistência assíncrona diferida via `QTimer::singleShot(0, ...)` logo após a liberação da interação manual do usuário.
  - **Ciclo de Vida Completo e Exibição de Tarefas `[Em Progresso]` no Calendário (`ui_14_CalendarPanel.cpp`, `caldial_04_update_task_list.cpp`, `agregador_01_ui.hpp`/`.cpp`)**:
    - Correção na "Lista Global de Tarefas:" e nas dicas de datas (*tooltips*) do Calendário, que anteriormente mostravam apenas tarefas `[Concluída]` e `[Pendente]`;
    - Implementação das funções utilitárias `task_utils::is_task_in_progress` e `task_utils::task_status_text` em `agregador_01_ui.hpp`/`.cpp`;
    - Integração da etiqueta `[Em Progresso]` / `[In Progress]` com chave de tradução dinâmica para tarefas ativas na coluna *Doing* do Pomodoro ou em andamento na Matriz;
    - Exibição consistente da contagem de pomodoros executados e da meta planejada (`🍅 X pomodoros`);
    - Suporte completo à retradução dinâmica em tempo de execução ao alternar o idioma do aplicativo (`pt_BR` e `en_US`);
    - Resolução direta de tarefas em andamento em interações manuais (clique duplo para abertura de arquivos anexados e acionamento do menu de contexto com botão direito).

- **Versão 2026.9.20.0**:
  - **Metas Individuais de Ciclos Pomodoro (`ciclos_totais` em `UnifiedTaskStore` e `Tarefa`)**:
    - Incorporação do atributo `ciclos_totais` (inteiro de 1 a 999, valor padrão 4) nas estruturas fundamentais `Tarefa` (`tmp_01_Tarefa.hpp`, `tmp_01_Tarefa.cpp`), na serialização/desserialização JSON (`taref_01_to_dict.cpp`, `taref_02_from_dict.cpp`) e no armazenamento compartilhado `UnifiedTaskStore` (`UnifiedTaskStore.hpp`, `UnifiedTaskStore.cpp`);
    - Campo dedicado `Meta de Ciclos Pomodoro (🎯):` no diálogo `EditTaskDialog` (`incore_14_EditTaskDialog.hpp`, `incore_14_EditTaskDialog.cpp`) e integração com o menu de contexto (`mmc_11_edit_task_dialog.cpp`);
    - Cálculo dinâmico unificado do percentual de conclusão das tarefas no Pomodoro (`calcular_percentual_conclusao`): $(\text{ciclos\_completos} / \text{ciclos\_totais}) \times 100.0\%$, estritamente delimitado entre $[0.0, 100.0]$;
    - Transição de estado para concluída estritamente subordinada à meta individual da tarefa (somente se move da coluna `Em Andamento` para `Concluídas` quando atinge seus próprios `ciclos_totais`).
  - **Diálogo de Alerta de Divergência de Metas (`pomo_14_aviso_metas_ciclos.hpp`, `pomo_14_aviso_metas_ciclos.cpp`, `pomo_04_toggle_timer.cpp`)**:
    - Ao acionar `toggle_timer()` para iniciar o temporizador Pomodoro, verificação preventiva das tarefas ativas na coluna `Em Andamento`;
    - Se houver tarefas com meta diferente do limite de ciclos configurado no timer (`ciclo_atual_ / ciclos_pomodoro_`), exibe a janela modal `AvisoMetasCiclosDialog`;
    - Tabela com 5 colunas comparativas: Tarefa, Módulo / Origem, Meta (🎯), Ciclo Timer (⚙️) e Comportamento no Fluxo;
    - Três ações possíveis via enum `ResultadoAvisoMetas`:
      - `RedefinirTodasParaTimer`: atualiza em lote `ciclos_totais` de todas as tarefas ativas para o valor do timer via `gtar_28_redefinir_ciclos.cpp`, salva a base e inicia o cronômetro;
      - `IniciarMesmoAssim`: inicia a contagem regressiva preservando as metas individuais customizadas;
      - `Cancelar`: aborta a inicialização do timer sem alterar estados ou tarefas.
  - **Sincronização Bidirecional Multi-Mês no Painel de Calendário (`ui_14_CalendarPanel.cpp`)**:
    - Navegação coordenada entre calendários múltiplos: avançar ou retroceder páginas em qualquer visão mensal atualiza automaticamente os demais calendários de forma contínua e encadeada (mês atual, mês seguinte, etc.), sem saltos ou duplicidades.
  - **Otimização de Performance e Fluidez no Arrastar e Soltar (Drag-and-Drop) de Tarefas (`agregador_01_ui.cpp`)**:
    - Eliminação de gravações redundantes em disco e recálculos síncronos da agenda Gantt a cada item durante operações de arrastar e soltar entre quadrantes, abas e listas;
    - Resposta instantânea da interface com movimento limpo e sem travamentos.
  - **Abertura Instantânea de Ferramentas e Pré-aquecimento em Tempo Ocioso (Idle Pre-Warming) (`gantt_plugin.cpp`, `agenda_plugin.cpp`, `GanttDatabase.cpp`)**:
    - Eliminação definitiva da perda de desempenho e congelamento na primeira abertura de **Ferramentas > Cronograma de Gantt BIM 4D**, **Ferramentas > Registro de Atividades** e ao passar o cursor / acessar o submenu **Configurações > Registro de Atividades > Excluir item**;
    - Mecanismo de pré-aquecimento assíncrono escalonado durante o tempo ocioso (*idle time*) do loop de eventos principal do Qt (150ms para Agenda e 350ms para Gantt após a exibição da janela do EisenPo);
    - Primeira interação do usuário com qualquer uma dessas ferramentas passa a ser 100% imediata (0ms), idêntica às execuções posteriores;
    - *Fast-path* de banco de dados em `GanttDatabase::run_migrations`: checagem de existência prévia de tabelas (`schedule_items`, `schedules`, `bim_links`) eliminando a reexecução custosa de dezenas de instruções DDL a cada abertura, com suporte a transação única (`db.transaction()`);
    - Supressão de sincronização dupla redundante no Gantt (`needs_eisenhower_sync_ = false;` imediatamente após o construtor já ter sincronizado as tarefas);
    - Conexão e filtragem de ciclo de vida (`AgendaParentCloseFilter` e `ParentCloseEventFilter`) garantindo fechamento limpo caso a aplicação host seja encerrada.
  - **Retradução Dinâmica In-Place de Diálogos e Tooltips (`changeEvent` e `QEvent::LanguageChange`)**:
    - Propagação imediata de tradução para diálogos abertos (`EditTaskDialog`), botões, cabeçalhos de quadrantes e tooltips da coluna Doing do Pomodoro sem fechar janelas ou perder dados de edição.

- **Versão 2026.9.14.0**:
  - **Cinco Configurações de Layout das Tabelas e Diagrama (`GanttMainWindow`)**:
    - Submenu `Configurações > Layout > Tabelas` com 5 modos exclusivos e persistência automática em `QSettings` (`gantt/table_layout_mode`):
      - `Original`: WBS e Gantt dividindo 50%/50% o espaço superior e Simulação 4D na base;
      - `Vertical Topo`: WBS e Simulação 4D dispostas lado a lado (50%/50% horizontal) no topo, com o Diagrama de Gantt na base;
      - `Vertical Inferior`: Diagrama de Gantt no topo, com WBS e Simulação 4D lado a lado (50%/50% horizontal) na base;
      - `Horizontal Esquerda`: WBS e Simulação 4D empilhadas verticalmente (50%/50%) à esquerda, e Diagrama de Gantt à direita;
      - `Horizontal Direita`: Diagrama de Gantt à esquerda, e WBS e Simulação 4D empilhadas verticalmente (50%/50%) à direita;
    - Divisão proporcional 50%/50% estrita entre Tabela WBS e Diagrama de Gantt, garantindo espaço confortável para dados e régua temporal.
  - **Parentesco de Diálogos e Suporte Multi-Telas (`GanttMainWindow`, `IEisenhowerHost`)**:
    - Centralização de janelas de confirmação de exclusão (`QMessageBox`) diretamente sobre a janela do Gantt, mesmo quando posicionada em monitores secundários;
    - Eliminação da chamada de ativação que trazia a janela principal do EisenPo indevidamente para o primeiro plano;
    - Preservação integral do ciclo de confirmação em 3 etapas e exclusão segura sincronizada com a Matriz de Eisenhower e com o Pomodoro.
  - **Estabilidade da Linha Fantasma (`GanttTimelineWidget`)**:
    - Correção na projeção da Linha de Base no gráfico de Gantt (`show_baseline_ghost_`): a adição de novas tarefas no EisenPo ou de atividades no cronograma mantém a Linha Fantasma visível e sincronizada.
  - **Barra de Controle Adaptativa da Simulação 4D em 3 Modos (`Bim4DSimulationPanel`)**:
    - Reorganização dinâmica do container `ctrl_box_` ao movimentar divisores verticais (*splitters*):
      - `SingleLineLayout` (largura >= 1200px): botões, controles de data, scrubber e badges em uma linha;
      - `TwoLinesLayout` (largura entre 780px e 1200px): botões e Data de Referência na linha superior; badges ("Total:", "Executado:", "Em Andamento:", "Atrasado:", "Futuro:") na linha inferior;
      - `ThreeLinesLayout` (largura < 780px): linha 1 com botões de reprodução; linha 2 com grupo de Data de Referência e barra deslizante; linha 3 com todos os badges de status;
    - Ajuste dinâmico automático da altura do container (~58px, ~84px, ~114px) prevenindo qualquer corte ou sobreposição;
    - Bloqueio rígido de largura mínima dos botões de simulação com base no texto (`calculate_button_content_width`);
    - Agrupamento compacto (3px) entre o rótulo "Data de Referência:" e o seletor de calendário (`date_group_container_`).
  - **Localização Dinâmica Completa em Tempo de Execução**:
    - Atualização dinâmica instantânea em Português (`pt_BR`) e Inglês (`en_US`) para todos os novos menus, submenus, diálogos e botões.

- **Versão 2026.9.13.0**:
  - **Configuração de Dias Úteis e Jornada de Trabalho (`GanttWorkingDaysDialog`, `WorkCalendar`)**:
    - Nova janela de configuração acessível via menu `Configurações > Dias Úteis` no módulo Gantt & BIM 4D (`GanttMainWindow`);
    - Seleção independente e granular de todos os 7 dias da semana (Segunda a Domingo) para definição dos dias úteis do projeto;
    - Definição do turno de trabalho com horários de início e término via controles `QTimeEdit`, com cálculo e atualização instantânea da duração líquida da jornada diária;
    - Integração direta com a classe de domínio `WorkCalendar` e sincronização com a engine de caminho crítico (`CpmEngine`), garantindo que o avanço temporal das tarefas e cálculos de datas de término considerem estritamente a jornada útil configurada;
  - **Gerenciador de Feriados e Dias Não Úteis de Nações (`GanttHolidaysDialog`, `WorkCalendar`)**:
    - Nova janela especializada acessível via menu `Configurações > Feriados` no módulo Gantt & BIM 4D;
    - Calendário interativo visual (`QCalendarWidget`) integrado para seleção imediata de datas e visualização de feriados já cadastrados;
    - Inserção manual de feriados personalizados com descrição customizada e exclusão pontual de feriados cadastrados;
    - Carregamento automatizado de feriados nacionais para 18 nações e países via lista suspensa (`QComboBox`) e botão "Aplicar" com algoritmo astronômico de Páscoa e feriados móveis;
    - Tabela de feriados com estilo visual idêntico à EAP/WBS, incluindo cabeçalho com redimensionamento bidirecional (`BiDirectionalResizeHeaderView`), seleção flexível e realce adaptativo ao tema do Windows 11 (`GanttCellHoverDelegate`);
  - **Reestruturação Geométrica em 3 Linhas do Quadro "Nova Tarefa" (`app_07_set_task_layout.cpp`, `agregador_01_ui.cpp`)**:
    - Reorganização ergonômica em 3 linhas com auto-ajuste de largura de combos e proteção contra splitters;
  - **Integração Completa de Duração de Tarefas em Dias** e **Localização Dinâmica Completa**.

- **Versão 2026.9.8.0**:
  - **Barra de Menus Nativa no Módulo Gantt & BIM 4D (`GanttMainWindow`)**:
    - Implementação de barra de menus superior estruturada nos menus "Arquivo" e "Configurações":
      - Menu `Arquivo`: `➕ Nova Atividade`, `✏️ Editar`, `❌ Excluir`, `📥 Importar CSV`, `📤 Exportar CSV`;
      - Menu `Configurações`: `🔗 Vinculo/Predecessora`, `⚡ Recalcular CPM`, `📌 Salvar Baseline`, `📊 Comparar Baseline`, `👁️ Linha Fantasma`, `🔍 Zoom Dia`, `🔍 Zoom Semana`, `🔍 Zoom Mês`, `📋 Tabela WBS`, `🏗️ Simulação 4D`, `🔄 Sincronizar com EisenPo`, `📋 Importar do EisenPo`;
    - Suporte completo a tradução dinâmica e recarga instantânea de strings via `retranslate_ui()` em tempo de execução;
  - **Redimensionamento Bidirecional de Colunas e Linhas (`GanttWbsTreeWidget`, `Bim4DSimulationPanel`, `QHeaderView`)**:
    - Implementação de lógica bidirecional em cabeçalhos: redimensionamento dinâmico arrastando tanto a borda direita (direita expande, esquerda reduz) quanto a borda esquerda (esquerda expande, direita reduz) das colunas;
    - Extensão da capacidade bidirecional às linhas da tabela de Simulação 4D por meio das divisórias horizontais do cabeçalho vertical;
  - **Correção de Seleção Pontual e Suporte a Seleções Múltiplas / Colunas / Linhas**:
    - Resolução definitiva de desalinhamento de seleção em células: o clique simples seleciona pontualmente a célula exata clicada, eliminando deslocamentos espúrios para células vizinhas;
    - Suporte completo a seleção retangular e contígua (Shift+Clique ou arrasto) e seleção alternada descontínua (Ctrl+Clique);
    - Seleção de colunas completas ao clicar no cabeçalho horizontal superior (com suporte a múltiplas colunas) e seleção de linhas completas no cabeçalho vertical;
  - **Realce Visual Dinâmico Adaptativo ao Tema do Windows 11 (`GanttCellHoverDelegate`)**:
    - Delegado de renderização customizado monitorando eventos de passagem do cursor (*hover*) e seleção em células;
    - No tema escuro: realce em tom de cinza suave mais claro que a cor de fundo padrão da célula;
    - No tema claro: realce em tom de cinza elegante mais escuro que a cor de fundo padrão da célula;
    - Propagação do realce para células únicas, blocos múltiplos, linhas completas e colunas completas;
    - Preservação da legibilidade tipográfica e das cores dos indicadores de status (`✅ Executado:`, `🔄 Em Andamento:`, `⚠️ Atrasado:`, `⏳ Futuro:`);
  - **Linha de Base (Baseline), Linha Fantasma (Ghost Line) e Recálculo Forçado de CPM**:
    - Gravação e leitura de snapshots de Baseline em `gantt.db`;
    - Renderização da Linha Fantasma no `GanttTimelineWidget` para visualização comparativa imediata do planejado versus realizado;
    - Execução forçada e instantânea do motor de cálculo de caminho crítico `CpmEngine`;
  - **Documentação Legal e Conformidade do Plugin Gantt**:
    - Criação e integração dos 12 novos documentos legais e normativos (6 em Português e 6 em Inglês: ABOUT, CLC, COPYRIGHT, EULA, NOTICES e PRIVACY_POLICY);
    - Integração no diálogo "Sobre" com alternância dinâmica em tempo de execução;
    - Atualização do utilitário `find_VersionEditor` para gerenciamento automatizado das versões dos documentos.

- **Versão 2026.9.7.0**:
  - **Novo Módulo de Gestão de Cronograma, Gantt & BIM 4D (`plugins/gantt`)**:
    - Implementação sob Arquitetura Limpa (*Clean Architecture*) estrita com 4 camadas desacopladas (`domain`, `domain/rules`, `application/services`, `infrastructure/persistence`, `ui`);
    - Motor analítico `CpmEngine` com ordenação topológica em grafo direcionado acíclico (DAG), cálculo de passes *forward* e *backward*, folgas livres e totais, caminho crítico e detecção preventiva de ciclos;
    - Painel multimídia `Bim4DSimulationPanel`, vinculação temporal com elementos construtivos IFC (`IFC GUID / UniqueId`) e reprodutor temporal interativo;
    - Banco de dados SQLite transacional local `gantt.db`;
  - **Cache em Memória O(1) e Eliminação de Gargalo de I/O na Troca de Idioma**:
    - Tabela hash `QHash<QString, TaskLinkSummary>` em memória protegida por mutex, eliminando consultas pesadas a disco no SQLite durante a troca de idioma e reduzindo o tempo de atualização para menos de 5ms;
    - Worker multithread `GanttTranslationWorker` operando em `QThread` dedicada para carga assíncrona dos arquivos `.qm`;
  - **Localização Dinâmica Completa em Tempo Real**:
    - Tradução em tempo de execução dos meses e dias da semana na régua temporal do Gantt;
    - Atualização imediata dos indicadores de vínculo (`🔗 Vinculada` / `🔗 Linked`);
    - Tradução em tempo real dos cabeçalhos da tabela de simulação 4D (`Status 4D` e `GUID IFC / Id Único`).

- **Versão 2026.9.6.0**:
  - **Sistema de Redimensionamento Responsivo Inteligente do Diálogo de Edição (`EditTaskDialog`)**:
    - Implementação de arquitetura adaptativa dinâmica com modos `LayoutMode::Wide` e `LayoutMode::Compact`, orquestrados por `_update_layout_mode()`, `_apply_wide_layout()` e `_apply_compact_layout()`;
    - Habilitação dos botões nativos de controle de janela `Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint` na barra de títulos;
    - No modo amplo (`width >= 760px` ou janela maximizada), a área de notas `desc_container_` expande-se ocupando todo o lado esquerdo da janela, enquanto os controles com dimensões congeladas (`top_fields_container_` e `bottom_fields_container_`) ancoram-se no canto inferior direito (`Qt::AlignRight | Qt::AlignBottom`);
    - Faixa de histerese inteligente de 80px (ativação do modo Wide em 760px e retorno para Compact em 680px) eliminando oscilações, tremores ou efeitos de chaveamento repetido ao arrastar as bordas laterais;
    - Desacoplamento da altura na transição de modos, imposição de limites mínimos rígidos (`compact_min_h` $\approx 508\text{px}$ e `wide_min_h` $\approx 416\text{px}$) e altura mínima garantida no `desc_container_`, prevenindo sobreposições ou esmagamento de widgets ao estreitar a janela verticalmente;
  - **Personalização de Rótulos do Método Feynman (`FeynmanLabelsConfig` e `EditarRotulosDialog`)**:
    - Criação de `source/utils/FeynmanLabelsConfig.hpp/cpp` para persistência em JSON (`feynman_rotulos_config.json`) dos títulos das 5 etapas da técnica Feynman com suporte a restauração e obtenção de rótulo efetivo ativo (`get_effective_feynman_label()`);
    - Criação do diálogo `EditarRotulosDialog` (`plugin/feynman/Feynman/fey_08_editar_rotulos_dialog.hpp/cpp`) estruturado com widgets nativos do Qt6 (`QGroupBox`, `QGridLayout`, `QLabel`, `QLineEdit`) em matriz de 3 colunas (Padrão, Atual, Novo), sem uso de tabelas;
    - Padronização visual e dimensional dos botões de ação (`padronizar_botoes()`) em harmonia com os botões da janela `MetodoFeynman`;
    - Adição de barra de menus na janela Feynman com a ação `Configurações -> Editar Rótulos` e adição de botão dedicado `Fechar` no rodapé da janela;
    - Retrocompatibilidade total nos parsers de descrição (`description_section_key()`), reconhecendo rótulos customizados e formatos históricos;
  - **Reorganização Canônica do Menu de Contexto (`mmc_13_mostrar_menu_contexto.cpp`)**:
    - Nova sequência estratégica das 7 ações do menu contextual acionado pelo botão direito: 1. Editar Tarefa, 2. Descrição Feynman, 3. Recorrência, 4. Colorir tarefas, 5. Configurações de Fonte, 6. Mover para outro quadrante, 7. Remover Tarefa;
  - **Alinhamento Simétrico de Bordas e Divisores Verticais**:
    - Alinhamento lateral milimétrico das bordas esquerda e direita do quadro "Nova Tarefa" com as extremidades dos quadrantes abaixo em todos os modos visuais;
    - Sincronização uniforme da espessura de todos os divisores verticais (`QSplitter`) entre Calendário, Pomodoro e Quadrantes.

- **Versão 2026.8.22.0**:
  - Quebra automática de linha adaptativa (`wordWrap`) e recálculo dinâmico síncrono (`doItemsLayout`) em todas as listas de tarefas;
  - Refatoração da renderização delegada (`LinkedTitleUnderlineDelegate`) com `QTextLayout` para suporte a sublinhado e tachado em múltiplas linhas;
  - Política otimizada de barras de rolagem sob demanda (`ScrollBarAsNeeded`).

- **Versão 2026.8.17.0**:
  - Expansão dos modos visuais da Matriz de Eisenhower em `Configurações -> Layout de Tarefas -> Interface do Usuário -> EisenPo`: `Layout em Abas > Horizontal` e `Layout em Abas > Vertical`;
  - Cálculo dinâmico e responsivo da largura mínima dos layouts de tarefas (`_calcular_largura_minima_layout_tarefas`), garantindo distribuição proporcional e evitando colapsos de colunas;
  - Reestruturação da área de entrada "Nova Tarefa" (`_update_input_group_layout_for_width`), garantindo uma organização harmoniosa e estável em duas linhas de controles;
  - Alinhamento lateral simétrico e milimétrico das bordas esquerda e direita do contorno do quadro "Nova Tarefa" com as extremidades dos quadrantes em todos os layouts visuais;
  - Aprimoramento robusto do subsistema de alarme sonoro do Pomodoro (`SoundManager`): resolução multi-diretório de áudios `.wav` (`resolve_sound_path`), recarregamento dinâmico (`load_config`) e volume calibrado (`1.0f`) no `QAudioOutput`;
  - Tradução dinâmica imediata em tempo de execução das abas de status ("Pendentes", "Em Progresso", "Concluídas") ao alternar idiomas sem necessidade de reiniciar.

- **Versão 2026.8.15.0**:
  - Lançamento oficial da suíte **EisenPo** integrando os 4 pilares: Matriz de Eisenhower, Método Pomodoro, Técnica Feynman e Registro de Atividades/Agendas;
  - Unificação do diretório de persistência sob `%LOCALAPPDATA%/EisenPo`, com rotina de migração transparente de bases legadas e marcador `.eisenpo_persistence_migrated`;
  - Centralização dos registros de diagnóstico no padrão `logs/file_eisenpo_*.log`;
  - Implementação de purga síncrona e exclusão segura entre módulos com proteção contra cliques acidentais;
  - Otimização completa dos presets CMake para compilação MinGW e MSVC/Ninja x64;
  - Alinhamento dos scripts de empacotamento Inno Setup (`EisenPo.iss`, `EisenPo_MinGW.iss`, `EisenPo_MSVC.iss`).

- **Versão 2026.8.14.0**:
  - Inclusão do **Layout Matriz 2x2** na Matriz de Eisenhower em `Configurações -> Layout de Tarefas -> Interface do Usuário -> Matriz de Eisenhower`;
  - Exibição simultânea dos 4 quadrantes dispostos em grade 2x2 com abas internas individuais `Pendente` e `Concluídas`;
  - Conclusão e reabertura de tarefas por clique na caixa de seleção ou por arraste e solte direto sobre as abas;
  - Suporte a listas aninhadas em árvore hierárquica nas agendas personalizadas (ex.: Curso > Semestre > Disciplina);
  - Filtragem dinâmica automática da lista filha conforme o item pai selecionado e integridade referencial automática na exclusão de itens pai;
  - Inclusão do **Gerenciador de logs** em `Ferramentas -> Gerenciador de logs` com leitura cronológica em tempo real;
  - Confirmação de remoção de tarefas no Eisenhower com tradução dinâmica dos botões e foco padrão em `Não`;
  - Ações `Excluir Tarefa` e `Excluir Tarefas Selecionadas` no menu de contexto do Feynman;
  - Coloração manual de tarefas selecionadas em `Configurações -> Configurações de Fonte -> Colorir tarefas`;
  - Criação do menu **Configurações de Fonte** com configuração independente para `Tarefas em Geral` e `Arquivo Vinculado`;
  - Controle independente do modo de cores (`Preto` e `Coloridas`) para cada ambiente (`EisenPo`, `Pomodoro` e `Feynman`) e opção unificada `Geral`.

---

## 3. Arquitetura de alto nível

O projeto é desenvolvido em `C++17` utilizando `Qt 6.11.1 Widgets`.

| Módulo / Camada | Responsabilidade Técnica |
| --- | --- |
| **Host Principal (EisenPo)** | Janela principal, Matriz de Eisenhower, menus globais, calendário, persistência central, tradução, manuais e carregador de plugins. |
| **Interfaces de Plugin** | Interfaces virtuais puras e bridges para descoberta, inicialização, sincronização de tarefas e tradução dos módulos. |
| **Plugin Pomodoro** | Temporizador de ciclos de foco/pausa, alarmes multimídia, modo automático e quadro de tarefas executadas. |
| **Plugin Feynman** | Metodologia de estudos em 4 etapas (Conceito, Explicação, Lacunas, Síntese) com visualização em Abas, Vertical ou Horizontal. |
| **Plugin Agenda** | Registro avaliativo detalhado, banco SQLite `listas.db`, criação de tarefas e gerenciamento de agendas personalizadas. |
| **Plugin Gantt & BIM 4D** | Gestão de cronogramas, EAP/WBS hierárquica, motor CPM analítico, gráfico interativo de Gantt, simulação construtiva 4D vinculada a elementos IFC e banco SQLite local `gantt.db`. |
| **Recursos de Produtividade** | Busca global, painel hoje, recorrências em lote, lembretes locais, exportação ICS, histórico, templates e revisões espaçadas. |
| **Mocks e Ferramentas** | Utilitários de compilação de traduções (`.ts` para `.qm`), editor de versão (`find_VersionEditor`) e scripts de manutenção. |

---

## 4. Host principal

Arquivos centrais em `source/`:

- `main.cpp`: Ponto de entrada, inicialização do `QApplication`, instalação do logger e exibição da janela principal.
- `source/app/app_01_eisenhower_matrix_app.hpp` / `cpp`: Definição central do estado do host, quadrantes, widgets, conexões de sinais e slots.
- `source/app/EisenhowerGanttHostAdapter.hpp` / `cpp`: Adaptador de integração do host EisenPo com o plugin Gantt, gerenciamento de vinculação de tarefas e cache de desempenho.
- `source/services/incore_01_initUI.cpp`: Montagem dos layouts da interface, splitters, calendário e listas de tarefas.
- `source/services/incore_03_criar_menu_configuracoes.cpp`: Construção de todos os menus superiores (`Arquivo`, `Configurações`, `Ferramentas`, `Opções`), atalhos de teclado e submenus dinâmicos.
- `source/utils/AppConfig.hpp`: Configurações de identidade (`EisenPo`), caminhos de persistência e prefixos de log.
- `source/utils/CaminhoPersistenteUtils.cpp`: Resolução de diretórios em `%LOCALAPPDATA%/EisenPo` e migração automática.
- `source/utils/LogManager.cpp`: Logger multithread em tempo real gravando em arquivos e notificando a UI.

---

## 5. Sistema de abas de módulos

As abas superiores suportam a seguinte composição:

1. `Eisenhower` (Host nativo)
2. `Pomodoro` (Plugin opcional)
3. `Feynman` (Plugin opcional)

Os plugins `agenda` e `gantt` operam como ferramentas ricas com janelas próprias dedicadas abertas sob demanda via menu ou atalhos (`Ctrl+Shift+B` para o Gantt), não ocupando aba fixa na janela principal do EisenPo. O Gantt conta ainda com integração contextual bidirecional através do clique direito em qualquer tarefa da Matriz.

---

## 6. Descoberta e carregamento de plugins

O host procura as bibliotecas dinâmicas nos seguintes diretórios de saída e runtime:

- `plugins/pomodoro/PomodoroPlugin.dll`
- `plugins/feynman/FeynmanPlugin.dll`
- `plugins/agenda/AgendaPlugin.dll`
- `plugins/gantt/GanttPlugin.dll`
- `build/build_pomodoro_mingw/plugins/pomodoro/PomodoroPlugin.dll`
- `build/build_pomodoro_msvc/plugins/pomodoro/PomodoroPlugin.dll`
- `build/build_feynman_mingw/plugins/feynman/FeynmanPlugin.dll`
- `build/build_feynman_msvc/plugins/feynman/FeynmanPlugin.dll`
- `build/build_agenda_mingw/plugins/agenda/AgendaPlugin.dll`
- `build/build_gantt_mingw/plugins/gantt/GanttPlugin.dll`
- `build/build_gantt_msvc/plugins/gantt/GanttPlugin.dll`
- `build/build_agenda_msvc/plugins/agenda/AgendaPlugin.dll`

---

## 7. Menu de configurações e plugins

A hierarquia sob `Configurações` segue a seguinte organização:

- **Layout de Tarefas**:
  - `Interface do Usuário`: Eisenhower (`Layout em Abas`, `Layout em Quadrantes`, `Layout Matriz 2x2`, `Título`, `Marca d'Água`), Pomodoro (`Título`, `Marca d'Água`), Feynman (`Abas`, `Vertical`, `Horizontal`).
  - `Mostrar Painéis`: Calendário (`Ctrl+M`), Pomodoro (`Ctrl+Shift+E`).
  - `Organizar por Data`: Crescente / Decrescente.
  - `Organizar por Horário`: Crescente / Decrescente.
  - `Organizar por Ordem Alfabética`: Crescente / Decrescente.
- **Configurações de Fonte**:
  - `Tarefas em Geral`: Configuração tipográfica de tarefas comuns.
  - `Arquivo Vinculado`: Configuração tipográfica de tarefas com anexos externos.
  - `Colorir tarefas`: Seletor de cores para itens selecionados.
- **Configurações Específicas**:
  - `Pomodoro`: Alarme, Modo automático, Reportar Pomodoros (`EisenPo`, `Pomodoro`, `Calendário`).
- **Registro de Atividades**:
  - `Excluir item`: Remoção de categorias da agenda ativa.
  - `Cores`: Submenus `Preto` e `Coloridas` com seleção para `Geral`, `EisenPo`, `Pomodoro` e `Feynman`.
- **Idioma**:
  - `Português (Brasil)`
  - `English (United States)`

---

## 8. Plugin Registro de Atividades

Diretório: `plugin/agenda/`

Recursos:
- Janela `Cadastro de Atividades` compacta e responsiva;
- Vínculo de curso, ementa, semestre, disciplina, turma, tipo de atividade, sequência, prioridade, nível de domínio, data, horário e arquivos vinculados;
- Conversão direta de cadastros em tarefas unificadas em `tarefas.json`;
- Banco SQLite local `listas.db` para armazenamento de opções de listas;
- Armazenamento de agendas personalizadas em arquivos JSON no subdiretório `agendas/`;
- Listas aninhadas em árvore hierárquica com integridade referencial nas exclusões;
- Modos visuais `Preto` (com contraste automático para tema claro/escuro) e `Coloridas`.

---

## 9. Plugins Feynman e Pomodoro

### Feynman (`plugin/feynman/`):
- Quadro de estudos em 4 etapas estruturadas;
- Visualização em Abas, Vertical ou Horizontal;
- Sistema de personalização de rótulos via diálogo nativo `EditarRotulosDialog` (`fey_08_editar_rotulos_dialog.hpp/cpp`);
- Barra de menus superior integrada à janela com acesso à ação `Configurações -> Editar Rótulos`;
- Botão dedicado `Fechar` no rodapé da janela Feynman;
- Persistência das definições de etapas em `feynman_rotulos_config.json`;
- Painel lateral `Minhas Tarefas` com exclusão individual e em lote;
- Sincronização bidirecional de conceitos e tarefas com o host.

### Pomodoro (`plugin/pomodoro/`):
- Temporizador com intervalos de foco, pausas curtas e longas;
- Transição automática inteligente com repetições configuráveis de alarme;
- Sincronização de pomodoros estimados vs. realizados;
- Painel lateral `Em Progresso`.

---

## 10. Recursos de produtividade do host

Localizados em `source/services/RecursosProdutividade/`:

- `Busca global` (`Ctrl+Shift+G`): Pesquisa em tempo real em todas as tarefas e notas.
- `Hoje / Próximos 7 dias` (`Ctrl+Shift+H`): Painel de curto prazo para entregas urgentes.
- `Filtros` (`Ctrl+Alt+F`): Filtragem detalhada por quadrante, status e prioridade.
- `Gerar próximas recorrências`: Geração em lote com prévia, limites por item e fallback.
- `Ativar lembretes locais`: Notificações locais na interface e na bandeja do sistema.
- `Exportar calendário .ics`: Exportação padrão iCalendar.
- `Histórico de produtividade`: Relatórios e contagens semanais.
- `Fluxo integrado de estudo`: Integração de textos e tarefas em conceitos Feynman.
- `Templates`: Modelos pré-configurados de rotinas de trabalho e estudo.
- `Criar revisões espaçadas`: Algoritmo de repetição espaçada por nível de domínio.
- `Gerenciador de plugins`: Diagnóstico de plugins carregados, caminhos e erros.
- `Gerenciador de logs`: Leitor cronológico em tempo real dos arquivos de log.

---

## 11. Persistência e armazenamento local

Base persistente do usuário:

```text
%LOCALAPPDATA%/EisenPo
```

*(Com rotina automática de migração transparente de `%LOCALAPPDATA%/EisenhowerOrganizer`)*

Estrutura de arquivos persistidos:

| Arquivo | Conteúdo |
| --- | --- |
| `tarefas.json` | Banco de dados unificado de tarefas compartilhadas. |
| `layout.json` | Preferências de layout, painéis visíveis e configurações de interface. |
| `language.json` | Código do idioma ativo (`pt_BR` ou `en_US`). |
| `recorrencias_config.json` | Preferências do gerador de recorrências. |
| `recorrencias_estado_original.json` | Snapshot para reversão de recorrências. |
| `lembretes_locais_config.json` | Preferências de agendamento de lembretes. |
| `tarefas_gerais_font_config.json` | Configuração tipográfica para tarefas gerais. |
| `arquivo_vinculado_font_config.json` | Configuração tipográfica para tarefas com arquivo vinculado. |
| `feynman_rotulos_config.json` | Configurações de rótulos personalizados das 5 etapas do Método Feynman. |
| `listas.db` | Banco SQLite contendo listas personalizadas da Agenda. |
| `agenda_config.json` | Configurações de cores e última agenda aberta. |
| `agendas/*.json` | Estruturas de agendas personalizadas criadas pelo usuário. |
| `eisenpo_gantt.db` | Banco SQLite local do módulo Gantt & BIM 4D (EAP/WBS, dependências CPM, baselines, calendários). |
| `logs/file_eisenpo_*.log` | Registros cronológicos de inicialização, ações e erros. |

---

## 12. Build CMake

Arquivo principal: `CMakeLists.txt` (`cmake_minimum_required(VERSION 3.28)`, `CMAKE_CXX_STANDARD 23`, Qt `6.12.0` módulos `Core`, `Gui`, `Widgets`, `Multimedia`, `Sql`, `Network`).

Opções de compilação disponíveis:
- `EISENHOWER_BUILD_HOST_APP`: Compila a aplicação `EisenPo` com todos os 4 módulos nativos (`pomodoro`, `feynman`, `agenda`, `gantt`) integrados no mesmo binário (padrão: `ON`).
- `EISENHOWER_BUILD_MOCKS`: Compila os utilitários de desenvolvimento em `mocks/` (padrão: `OFF`, ativado nos presets `mocks-*`).
- `EISENHOWER_ENABLE_IPO`: Otimização interprocedural / LTO (padrão: `ON`).
- `EISENHOWER_ENABLE_NATIVE_OPTIMIZATIONS`, `EISENHOWER_ENABLE_FAST_MATH`, `EISENHOWER_ENABLE_AGGRESSIVE_INLINING`, `EISENHOWER_PGO_MODE`: Perfis avançados e variantes portáteis.
- `EISENHOWER_STRIP_RELEASE_BINARIES`: Remoção de símbolos de debug em builds MinGW Release (padrão: `ON`).

---

## 13. Presets CMake

Configurados em `CMakePresets.json` (versão 6):

| Preset | Toolchain / Perfil | Pasta de saída | Distribuição Externa Limpa |
| --- | --- | --- | --- |
| `host-qt6-msvc-release` | MSVC `14.52.37025` puro / MSBuild / Qt `6.12.0` | `build/build_EisenPo_msvc` | `D:/MISCELANEAS/VSCode/EisenPo/EisenPo_cpp_msvc` |
| `host-qt6-mingw-release` | MinGW `15.1.0` / Ninja / Qt `6.12.0` | `build/build_EisenPo_mingw` | `D:/MISCELANEAS/VSCode/EisenPo/EisenPo_cpp_mingw` |
| `host-qt6-{msvc,mingw}-pgo-{generate,use}` | Perfis PGO | `build/build_EisenPo_{msvc_pgo,mingw_pgo}` | Mesmo destino da variante principal |
| `mocks-msvc-release` | MSVC `14.52.37025` puro Mocks / MSBuild | `build/build_mocks_msvc` | `D:/MISCELANEAS/VSCode/EisenPo/mocks_msvc` |
| `mocks-mingw-release` | MinGW `15.1.0` Mocks / Ninja | `build/build_mocks_mingw` | `D:/MISCELANEAS/VSCode/EisenPo/mocks_mingw` |

---

## 14. Targets de build e deploy

- `EisenPo` / `EisenPoDeploy` (aliases de compatibilidade: `EisenhowerOrganizer` / `EisenhowerOrganizerDeploy`)
- `CompileTranslationsGUI` / `CompileTranslationsGUIDeploy`
- `CompileTranslationsTerminal` / `CompileTranslationsTerminalDeploy`
- `find_VersionEditor` / `find_VersionEditorDeploy`

O deploy gera `qt.conf` (`Prefix = .`, `Plugins = plugins`, `Translations = translations`), copia estritamente as 5 DLLs `Microsoft.VC145.CRT` (MSVC) ou as 3 DLLs do MinGW 15.1.0, remove `*.pdb`, `vc_redist*.exe`, plugins não usados e banners de loja, e implanta os ativos de licenças/documentação e `.qm`.

---

## 15. VS Code e automação local

Configurado em `.vscode/{tasks.json,cmake-kits.json,c_cpp_properties.json,settings.json}`:
- Tarefas de compilação e execução do `EisenPo` e `mocks/` via **MinGW 15.1.0** e **MSVC 14.52** (incluindo variantes portáteis e PGO);
- Compilação centralizada de arquivos `.ts` para `.qm` via `mocks/CompileTranslationsTerminal`;
- Limpeza de artefatos de build regeneráveis.

---

## 16. Ferramentas auxiliares em mocks

Localizadas em `mocks/`:
- `CompileTranslationsGUI`: Interface gráfica para compilação em lote de traduções.
- `CompileTranslationsTerminal`: Utilitário de linha de comando para automação de `.qm`.
- `find_VersionEditor`: Utilitário para atualização síncrona de versões em código e documentação.

---

## 17. Sistema de traduções e logs

- Suporte completo a **Português do Brasil (`pt_BR`)** e **Inglês dos EUA (`en_US`)**;
- Tradução imediata em tempo de execução sem reinício através de `changeEvent`;
- Logging centralizado e multithread em arquivos rotativos com prefixo `file_eisenpo`.

---

## 18. Sobre, assets e notas condicionais

- Localizados em `source/assets/eisenhower/`;
- Exibição condicional de notas de versão e documentação de plugins com delimitadores:
  - `[[POMODORO_PLUGIN_BEGIN]]` ... `[[POMODORO_PLUGIN_END]]`
  - `[[FEYNMAN_PLUGIN_BEGIN]]` ... `[[FEYNMAN_PLUGIN_END]]`
  - `[[AGENDA_PLUGIN_BEGIN]]` ... `[[AGENDA_PLUGIN_END]]`

---

## 19. Importação e exportação

- **JSON**: Backup e restauração completa de tarefas e metadados.
- **CSV**: Tabela estruturada por quadrante e atributos.
- **XLSX**: Planilhas formatadas com abas por quadrante e tarefas concluídas.
- **PDF**: Relatórios estruturados para impressão e documentação.
- **ICS**: Arquivo iCalendar padrão para sincronização com calendários externos.

---

## 20. Scripts de manutenção e empacotamento

Localizados em `library/`:
- `AppxManifest.xml` e `EisenPo_cpp.txt`: Manifesto de identidade MSIX e escopo de lançamento.
- `EisenPo.iss`, `EisenPo_MinGW.iss`, `EisenPo_MSVC.iss`: Scripts Inno Setup para geração de instaladores Windows.
- `limpar_artefatos_regeneraveis_qt.ps1`: Limpeza de caches e saídas intermediárias de compilação.
- `MANUAL_TECNICO_CONFIGURACAO_AMBIENTE_CPP_QT6.txt`: Guia técnico de preparação do ambiente de desenvolvimento.

Localizados em `images/interfaces/`:
- `images/interfaces/pt_BR` e `images/interfaces/en_US`: Capturas de tela oficiais da interface em Português (Brasil) e Inglês (EUA).

---

## 21. Correções e melhorias funcionais consolidadas

- Consolidação oficial do **EisenPo v2026.8.22.0**;
- Quebra automática de linha (`wordWrap`) e ajuste adaptativo em todas as listas de tarefas;
- Recálculo dinâmico e imediato de altura de itens e layout (`doItemsLayout`) no redimensionamento de janelas e splitters;
- Refatoração da renderização delegada de títulos com `QTextLayout` para suporte multilinhas em arquivos vinculados e tarefas concluídas;
- Política sob demanda para barras de rolagem horizontal (`ScrollBarAsNeeded`);
- Persistência e logs unificados sob `%LOCALAPPDATA%/EisenPo`;
- Modos de layout em Abas (Horizontal e Vertical), Quadrantes e Matriz 2x2 com abas internas `Pendente` e `Concluídas`;
- Agendas personalizadas com listas aninhadas em árvore hierárquica;
- Controle independente de cores `Preto` e `Coloridas` por módulo e geral;
- Configurações avançadas de fonte para tarefas comuns e arquivos vinculados;
- Sistema de alarme sonoro do Pomodoro aperfeiçoado com prévia e calibração de volume;
- Confirmação segura de remoção de tarefas com foco inicial em `Não`;
- Gerenciador de logs e Gerenciador de plugins integrados em tempo real;
- Bilinguismo integral e tradução dinâmica em tempo de execução.

---

## 22. Estado técnico resumido

O projeto `EisenPo_cpp` representa:
- Uma aplicação desktop nativa em **C++** (`CMAKE_CXX_STANDARD 23`) e **Qt 6.12.0**;
- Host principal (`EisenPo.exe`) com a Matriz de Eisenhower e os 4 módulos nativos compilados diretamente (`Pomodoro`, `Feynman`, `Agenda` e `Gantt & BIM 4D`);
- Arquitetura transparente de **Duplo Licenciamento** (`GNU AGPLv3` no código-fonte público, liberdade total aos desenvolvedores sobre `source/assets/`, e **EULA Oficial + CLC** na Microsoft Store `9P289X0185C3`);
- Persistência 100% local e offline em `%LOCALAPPDATA%\EisenPo`;
- Suporte completo e validado a builds limpos em **C++23** (`CMAKE_CXX_SCAN_FOR_MODULES OFF`, identificador `concept` refatorado em 5 arquivos de `source/modules/feynman/Feynman/`) nas toolchains **MSVC `14.52.37025` x64** (`114,81 MB` / `176` arquivos; instalador `49,33 MB`) e **MinGW-w64 `GCC 15.1.0` x64** (`129,46 MB` / `174` arquivos; instalador `53,70 MB`) sem `vc_redist.x64.exe` no Inno Setup.

---

## 23. Comandos recomendados de verificação

### Compilar Host MinGW / MSVC (C++23 / Qt 6.12.0):
```powershell
cmake --preset host-qt6-mingw-release && cmake --build --preset host-qt6-mingw-release
cmake --preset host-qt6-msvc-release && cmake --build --preset host-qt6-msvc-release
```

### Compilar Mocks e Verificar Consistência Documental:
```powershell
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release
.\build\build_mocks_msvc\Release\find_VersionEditor.exe --check --root .
.\build\build_mocks_msvc\Release\CompileTranslationsTerminal.exe --all
```

### Gerar Instaladores Inno Setup Limpos (Sem `vc_redist.x64.exe`):
```powershell
& "C:\Program Files\Inno Setup 7\ISCC.exe" .\library\EisenPo.iss
```

---

## 24. Observação final

O ecossistema **`EisenPo`** na linha **`2026.11.22.0`** consolida a unificação nativa de seus módulos em um binário único, com documentação técnica completa em [`docs/`](docs/architecture.md), governança comunitária (`CODE_OF_CONDUCT.md`, `CONTRIBUTING.md`, `SECURITY.md`, `.github/`) e duplo licenciamento explícito em [`LICENSE`](LICENSE) e [`source/assets/`](source/assets/LICENSES/THIRD_PARTY_COMPONENT_NOTICES.txt). Toda a persistência permanece estritamente no perfil local do usuário (`%LOCALAPPDATA%\EisenPo`), preservando soberania e privacidade integral.

---

<h2 id="enus">🇺🇸 English (US) — Technical Snapshot Summary</h2>

<details>
<summary>Click to expand / collapse Technical Observation summary in English</summary>

# EISENPO TECHNICAL SNAPSHOT (`2026.11.22.0`)

- **Documented Version:** `2026.11.22.0` (`November 22, 2026`)
- **Repository:** `https://github.com/fernandoncidade/EisenPo_cpp`
- **Author & Copyright Holder:** `Fernando Nillsson Cidade` (`linceu_lighthouse@outlook.com`)
- **Dual Licensing Model:** Public source repository licensed under **GNU AGPLv3** ([`LICENSE`](LICENSE)); complete freedom expressly granted to developers and forks to alter/remove/adapt institutional docs in [`source/assets/`](source/assets/eisenhower/COPYRIGHT/COPYRIGHT%20AND%20TRADEMARK%20NOTICE_en_US.txt); official Microsoft Store package (`9P289X0185C3`) governed by the author's **[Official EULA](source/assets/eisenhower/EULA/EULA_en_US.txt)** + **[CLC](source/assets/eisenhower/CLC/CLC_en_US.txt)**; supplied **"AS IS"** as an auxiliary tool with zero Microsoft support or privacy coverage.
- **Native Architecture:** Single native Windows x64 executable (`EisenPo.exe`) built with **Qt 6.12.0** uniting Eisenhower Matrix, Pomodoro, Feynman, Agenda (SQLite `listas.db`), and Gantt & 4D BIM (4-layer Clean Architecture, DAG `CpmEngine`, 18-country Holiday Manager via Meeus/Jones/Butcher, SQLite `eisenpo_gantt.db`), 100% offline under `%LOCALAPPDATA%\EisenPo`.
- **Dual Toolchain & Clean Packaging:** Supports both pure **MSVC `14.52.37025` (`v145`) / Windows SDK `10.0.28000.0`** via `Visual Studio 18 2026` / MSBuild `18.12.3` (`114.97 MB` / `176` files; installer `49.34 MB`) and **MinGW-w64 `GCC 15.1.0`** via Ninja `1.13.2` (`129.48 MB` / `174` files; installer `53.70 MB`) through `CMakePresets.json`, deploying strict app-local DLLs with `qt.conf`, zero store banners, zero `.pdb`, and **zero `vc_redist.x64.exe` (~18 MB)** inside Inno Setup installers (`library/EisenPo*.iss`).
- **Verified C++23 Migration:** Full codebase compiles cleanly under strict `CMAKE_CXX_STANDARD 23` (`CMAKE_CXX_SCAN_FOR_MODULES OFF`) following renaming of `concept` (`concept_text` / `conceito`) in 5 files of `source/modules/feynman/Feynman/`, audited in [`docs/technical-audit-and-roadmap.md`](docs/technical-audit-and-roadmap.md#enus).

</details>

---

<a id="tree"></a>
## Estrutura de arquivos do projeto

**Root Path:** `c:\Users\ferna\APLICATIVOS\CPP\EisenPo_cpp\source`

```
├── 📁 app
│   ├── ⚡ EisenhowerGanttHostAdapter.cpp
│   ├── ⚡ EisenhowerGanttHostAdapter.hpp
│   ├── ⚡ app_01_eisenhower_matrix_app.cpp
│   ├── ⚡ app_01_eisenhower_matrix_app.hpp
│   ├── ⚡ app_02_get_text.cpp
│   ├── ⚡ app_02_get_text.hpp
│   ├── ⚡ app_03_initUI.cpp
│   ├── ⚡ app_03_initUI.hpp
│   ├── ⚡ app_04_refresh_module_tabs.cpp
│   ├── ⚡ app_04_refresh_module_tabs.hpp
│   ├── ⚡ app_05_load_feynman_plugin.cpp
│   ├── ⚡ app_05_load_feynman_plugin.hpp
│   ├── ⚡ app_06_send_task_to_feynman.cpp
│   ├── ⚡ app_06_send_task_to_feynman.hpp
│   ├── ⚡ app_07_set_task_layout.cpp
│   ├── ⚡ app_07_set_task_layout.hpp
│   ├── ⚡ app_08_apply_theme_palette.cpp
│   ├── ⚡ app_08_apply_theme_palette.hpp
│   ├── ⚡ app_09_add_task.cpp
│   ├── ⚡ app_09_add_task.hpp
│   ├── ⚡ app_10_atualizar_textos.cpp
│   ├── ⚡ app_10_atualizar_textos.hpp
│   └── ⚡ plugin_mocks.hpp
├── 📁 assets
│   ├── 📁 LICENSES
│   │   ├── 📄 Apache License Version 2.0.txt
│   │   ├── 📄 BSD License Clause-2.txt
│   │   ├── 📄 BSD License Clause-3.txt
│   │   ├── 📄 DirectXShaderCompiler License.txt
│   │   ├── 📄 FFmpeg_LGPL_BUILD_CONFIGURATION.txt
│   │   ├── 📄 GCC Runtime Library Exception 3.1.txt
│   │   ├── 📄 GNU AFFERO GENERAL PUBLIC LICENSE Version3.txt
│   │   ├── 📄 GNU GENERAL PUBLIC LICENSE Version 3.txt
│   │   ├── 📄 GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.txt
│   │   ├── 📄 GNU LESSER GENERAL PUBLIC LICENSE Version 3.txt
│   │   ├── 📄 GPL-3.0.txt
│   │   ├── 📄 ISC LICENSE.txt
│   │   ├── 📄 LICENSE
│   │   ├── 📄 LLVM OpenMP License.txt
│   │   ├── 📄 MIT License.txt
│   │   ├── 📕 MS-Store-ADA-v8.9-FINAL-EN.pdf
│   │   ├── 📄 Microsoft Redistributable Runtime Notice.txt
│   │   ├── 📄 MinGW-w64 Runtime License.txt
│   │   ├── 📄 OPEN_SOURCE_SOURCE_OFFER_en_US.txt
│   │   ├── 📄 OPEN_SOURCE_SOURCE_OFFER_pt_BR.txt
│   │   ├── 📄 PYTHON SOFTWARE FOUNDATION LICENSE.txt
│   │   ├── 📄 SQLite Blessing.txt
│   │   ├── 📄 THIRD_PARTY_COMPONENT_NOTICES.txt
│   │   ├── 📄 Zlib License.txt
│   │   └── 📄 llama.cpp MIT License.txt
│   └── 📁 eisenhower
│       ├── 📁 ABOUT
│       │   ├── 📄 ABOUT_EisenPo_en_US.txt
│       │   ├── 📄 ABOUT_EisenPo_pt_BR.txt
│       │   ├── 📄 ABOUT_en_US.txt
│       │   ├── 📄 ABOUT_plugin_agenda_en_US.txt
│       │   ├── 📄 ABOUT_plugin_agenda_pt_BR.txt
│       │   ├── 📄 ABOUT_plugin_feynman_en_US.txt
│       │   ├── 📄 ABOUT_plugin_feynman_pt_BR.txt
│       │   ├── 📄 ABOUT_plugin_gantt_en_US.txt
│       │   ├── 📄 ABOUT_plugin_gantt_pt_BR.txt
│       │   ├── 📄 ABOUT_plugin_pomodoro_en_US.txt
│       │   ├── 📄 ABOUT_plugin_pomodoro_pt_BR.txt
│       │   ├── 📄 ABOUT_pt_BR.txt
│       │   ├── 📄 History_APP_en_US.txt
│       │   └── 📄 History_APP_pt_BR.txt
│       ├── 📁 CLC
│       │   ├── 📄 CLC_EisenPo_en_US.txt
│       │   ├── 📄 CLC_EisenPo_pt_BR.txt
│       │   ├── 📄 CLC_en_US.txt
│       │   ├── 📄 CLC_plugin_agenda_en_US.txt
│       │   ├── 📄 CLC_plugin_agenda_pt_BR.txt
│       │   ├── 📄 CLC_plugin_feynman_en_US.txt
│       │   ├── 📄 CLC_plugin_feynman_pt_BR.txt
│       │   ├── 📄 CLC_plugin_gantt_en_US.txt
│       │   ├── 📄 CLC_plugin_gantt_pt_BR.txt
│       │   ├── 📄 CLC_plugin_pomodoro_en_US.txt
│       │   ├── 📄 CLC_plugin_pomodoro_pt_BR.txt
│       │   └── 📄 CLC_pt_BR.txt
│       ├── 📁 COPYRIGHT
│       │   ├── 📄 AVISO DE COPYRIGHT E MARCA REGISTRA_pt_BR.txt
│       │   ├── 📄 COPYRIGHT AND TRADEMARK NOTICE_en_US.txt
│       │   ├── 📄 Copyright_Notice_EisenPo_en_US.txt
│       │   ├── 📄 Copyright_Notice_EisenPo_pt_BR.txt
│       │   ├── 📄 Copyright_Notice_plugin_agenda_en_US.txt
│       │   ├── 📄 Copyright_Notice_plugin_agenda_pt_BR.txt
│       │   ├── 📄 Copyright_Notice_plugin_feynman_en_US.txt
│       │   ├── 📄 Copyright_Notice_plugin_feynman_pt_BR.txt
│       │   ├── 📄 Copyright_Notice_plugin_gantt_en_US.txt
│       │   ├── 📄 Copyright_Notice_plugin_gantt_pt_BR.txt
│       │   ├── 📄 Copyright_Notice_plugin_pomodoro_en_US.txt
│       │   └── 📄 Copyright_Notice_plugin_pomodoro_pt_BR.txt
│       ├── 📁 EULA
│       │   ├── 📄 EULA_EisenPo_en_US.txt
│       │   ├── 📄 EULA_EisenPo_pt_BR.txt
│       │   ├── 📄 EULA_en_US.txt
│       │   ├── 📄 EULA_plugin_agenda_en_US.txt
│       │   ├── 📄 EULA_plugin_agenda_pt_BR.txt
│       │   ├── 📄 EULA_plugin_feynman_en_US.txt
│       │   ├── 📄 EULA_plugin_feynman_pt_BR.txt
│       │   ├── 📄 EULA_plugin_gantt_en_US.txt
│       │   ├── 📄 EULA_plugin_gantt_pt_BR.txt
│       │   ├── 📄 EULA_plugin_pomodoro_en_US.txt
│       │   ├── 📄 EULA_plugin_pomodoro_pt_BR.txt
│       │   └── 📄 EULA_pt_BR.txt
│       ├── 📁 NOTICES
│       │   ├── 📄 NOTICE_EisenPo_en_US.txt
│       │   ├── 📄 NOTICE_EisenPo_pt_BR.txt
│       │   ├── 📄 NOTICE_en_US.txt
│       │   ├── 📄 NOTICE_plugin_agenda_en_US.txt
│       │   ├── 📄 NOTICE_plugin_agenda_pt_BR.txt
│       │   ├── 📄 NOTICE_plugin_feynman_en_US.txt
│       │   ├── 📄 NOTICE_plugin_feynman_pt_BR.txt
│       │   ├── 📄 NOTICE_plugin_gantt_en_US.txt
│       │   ├── 📄 NOTICE_plugin_gantt_pt_BR.txt
│       │   ├── 📄 NOTICE_plugin_pomodoro_en_US.txt
│       │   ├── 📄 NOTICE_plugin_pomodoro_pt_BR.txt
│       │   └── 📄 NOTICE_pt_BR.txt
│       ├── 📁 PRIVACY_POLICY
│       │   ├── 📄 Privacy_Policy_EisenPo_en_US.txt
│       │   ├── 📄 Privacy_Policy_EisenPo_pt_BR.txt
│       │   ├── 📄 Privacy_Policy_en_US.txt
│       │   ├── 📄 Privacy_Policy_plugin_agenda_en_US.txt
│       │   ├── 📄 Privacy_Policy_plugin_agenda_pt_BR.txt
│       │   ├── 📄 Privacy_Policy_plugin_feynman_en_US.txt
│       │   ├── 📄 Privacy_Policy_plugin_feynman_pt_BR.txt
│       │   ├── 📄 Privacy_Policy_plugin_gantt_en_US.txt
│       │   ├── 📄 Privacy_Policy_plugin_gantt_pt_BR.txt
│       │   ├── 📄 Privacy_Policy_plugin_pomodoro_en_US.txt
│       │   ├── 📄 Privacy_Policy_plugin_pomodoro_pt_BR.txt
│       │   └── 📄 Privacy_Policy_pt_BR.txt
│       ├── 📁 RELEASE
│       │   ├── 📄 RELEASE NOTES_en_US.txt
│       │   └── 📄 RELEASE NOTES_pt_BR.txt
│       └── 📁 icones
│           ├── 📄 calendar.ico
│           ├── 🖼️ calendar.png
│           ├── 📄 eisenpo.ico
│           ├── 🖼️ eisenpo.png
│           ├── 📄 eisenpo1.ico
│           ├── 🖼️ eisenpo1.png
│           ├── 🖼️ eisenpo2.png
│           ├── 🖼️ eisenpo_1080-1080.png
│           ├── 🖼️ eisenpo_1440-2160.png
│           ├── 🖼️ eisenpo_150-150.png
│           ├── 🖼️ eisenpo_1920-1080.png
│           ├── 🖼️ eisenpo_2160-1440.png
│           ├── 🖼️ eisenpo_2160-2160.png
│           ├── 🖼️ eisenpo_300-300.png
│           ├── 🖼️ eisenpo_3840-2160.png
│           ├── 🖼️ eisenpo_584-800.png
│           ├── 🖼️ eisenpo_71-71.png
│           ├── 🖼️ eisenpo_en-US.png
│           └── 🖼️ eisenpo_pt-BR.png
├── 📁 language
│   ├── 📁 translations
│   │   ├── 📄 agenda_en_US.qm
│   │   ├── 📄 agenda_en_US.ts
│   │   ├── 📄 agenda_pt_BR.qm
│   │   ├── 📄 agenda_pt_BR.ts
│   │   ├── 📄 eisenhower_organizer_en_US.qm
│   │   ├── 📄 eisenhower_organizer_en_US.ts
│   │   ├── 📄 eisenhower_organizer_pt_BR.qm
│   │   ├── 📄 eisenhower_organizer_pt_BR.ts
│   │   ├── 📄 feynman_en_US.qm
│   │   ├── 📄 feynman_en_US.ts
│   │   ├── 📄 feynman_pt_BR.qm
│   │   ├── 📄 feynman_pt_BR.ts
│   │   ├── 📄 languagegantt_en_US.qm
│   │   ├── 📄 languagegantt_en_US.ts
│   │   ├── 📄 languagegantt_pt_BR.qm
│   │   ├── 📄 languagegantt_pt_BR.ts
│   │   ├── 📄 pomodoro_en_US.qm
│   │   ├── 📄 pomodoro_en_US.ts
│   │   ├── 📄 pomodoro_pt_BR.qm
│   │   └── 📄 pomodoro_pt_BR.ts
│   ├── ⚡ tr_01_gerenciadorTraducao.cpp
│   └── ⚡ tr_01_gerenciadorTraducao.hpp
├── 📁 models
│   ├── ⚡ model_01_TaskRecordListModel.cpp
│   └── ⚡ model_01_TaskRecordListModel.hpp
├── 📁 modules
│   ├── 📁 agenda
│   │   ├── 📁 AgendaAvaliacoes
│   │   │   ├── 📁 BancoDeDados
│   │   │   │   ├── 📁 DicionariosListasBancoDeDados
│   │   │   │   │   ├── ⚡ CURSOS_EN.cpp
│   │   │   │   │   ├── ⚡ CURSOS_EN.hpp
│   │   │   │   │   ├── ⚡ CURSOS_PT.cpp
│   │   │   │   │   ├── ⚡ CURSOS_PT.hpp
│   │   │   │   │   ├── ⚡ EstruturasCursos.hpp
│   │   │   │   │   ├── ⚡ dicionario_de_cores.cpp
│   │   │   │   │   ├── ⚡ dicionario_de_cores.hpp
│   │   │   │   │   ├── ⚡ dicionario_disciplinas.cpp
│   │   │   │   │   ├── ⚡ dicionario_disciplinas.hpp
│   │   │   │   │   ├── ⚡ lista_avaliacoes_en_US.cpp
│   │   │   │   │   ├── ⚡ lista_avaliacoes_en_US.hpp
│   │   │   │   │   ├── ⚡ lista_avaliacoes_pt_BR.cpp
│   │   │   │   │   ├── ⚡ lista_avaliacoes_pt_BR.hpp
│   │   │   │   │   ├── ⚡ lista_cursos_en_US.cpp
│   │   │   │   │   ├── ⚡ lista_cursos_en_US.hpp
│   │   │   │   │   ├── ⚡ lista_cursos_pt_BR.cpp
│   │   │   │   │   ├── ⚡ lista_cursos_pt_BR.hpp
│   │   │   │   │   ├── ⚡ lista_turmas_en_US.cpp
│   │   │   │   │   ├── ⚡ lista_turmas_en_US.hpp
│   │   │   │   │   ├── ⚡ lista_turmas_pt_BR.cpp
│   │   │   │   │   ├── ⚡ lista_turmas_pt_BR.hpp
│   │   │   │   │   ├── ⚡ mapeamento_cursos.cpp
│   │   │   │   │   ├── ⚡ mapeamento_cursos.hpp
│   │   │   │   │   ├── ⚡ mapeamento_ementas.cpp
│   │   │   │   │   ├── ⚡ mapeamento_ementas.hpp
│   │   │   │   │   ├── ⚡ mapeamento_semestres.cpp
│   │   │   │   │   └── ⚡ mapeamento_semestres.hpp
│   │   │   │   ├── ⚡ Banco_Dados.cpp
│   │   │   │   ├── ⚡ Banco_Dados.hpp
│   │   │   │   ├── ⚡ Listas_Personalizadas.cpp
│   │   │   │   └── ⚡ Listas_Personalizadas.hpp
│   │   │   ├── 📁 GerenciamentoAtividades
│   │   │   │   ├── 📁 GerenciadorAtividades
│   │   │   │   │   ├── ⚡ gativ_01_criar_tabela.cpp
│   │   │   │   │   ├── ⚡ gativ_01_criar_tabela.hpp
│   │   │   │   │   ├── ⚡ gativ_02_adicionar_atividade.cpp
│   │   │   │   │   ├── ⚡ gativ_02_adicionar_atividade.hpp
│   │   │   │   │   ├── ⚡ gativ_03_remover_atividade.cpp
│   │   │   │   │   ├── ⚡ gativ_03_remover_atividade.hpp
│   │   │   │   │   ├── ⚡ gativ_04_listar_atividades.cpp
│   │   │   │   │   ├── ⚡ gativ_04_listar_atividades.hpp
│   │   │   │   │   ├── ⚡ gativ_05_buscar_atividade.cpp
│   │   │   │   │   ├── ⚡ gativ_05_buscar_atividade.hpp
│   │   │   │   │   ├── ⚡ gativ_06_atualizar_atividade.cpp
│   │   │   │   │   ├── ⚡ gativ_06_atualizar_atividade.hpp
│   │   │   │   │   ├── ⚡ gativ_07_submiter.cpp
│   │   │   │   │   ├── ⚡ gativ_07_submiter.hpp
│   │   │   │   │   ├── ⚡ gativ_08_limpar_entradas.cpp
│   │   │   │   │   ├── ⚡ gativ_08_limpar_entradas.hpp
│   │   │   │   │   ├── ⚡ gativ_09_limpar_ultima_entrada.cpp
│   │   │   │   │   ├── ⚡ gativ_09_limpar_ultima_entrada.hpp
│   │   │   │   │   ├── ⚡ gativ_10_exportar_para_pdf.cpp
│   │   │   │   │   ├── ⚡ gativ_10_exportar_para_pdf.hpp
│   │   │   │   │   ├── ⚡ gativ_11_update_textbox.cpp
│   │   │   │   │   ├── ⚡ gativ_11_update_textbox.hpp
│   │   │   │   │   ├── ⚡ gativ_12_excluir_item.cpp
│   │   │   │   │   ├── ⚡ gativ_12_excluir_item.hpp
│   │   │   │   │   ├── ⚡ gativ_13_editar_item.cpp
│   │   │   │   │   ├── ⚡ gativ_13_editar_item.hpp
│   │   │   │   │   ├── ⚡ gativ_14_editar_detalhes_atividade.cpp
│   │   │   │   │   ├── ⚡ gativ_14_editar_detalhes_atividade.hpp
│   │   │   │   │   ├── ⚡ gativ_15_update_ementa.cpp
│   │   │   │   │   ├── ⚡ gativ_15_update_ementa.hpp
│   │   │   │   │   ├── ⚡ gativ_16_update_semestre.cpp
│   │   │   │   │   ├── ⚡ gativ_16_update_semestre.hpp
│   │   │   │   │   ├── ⚡ gativ_17_update_disciplinas.cpp
│   │   │   │   │   ├── ⚡ gativ_17_update_disciplinas.hpp
│   │   │   │   │   ├── ⚡ gativ_18_carregar_dados.cpp
│   │   │   │   │   ├── ⚡ gativ_18_carregar_dados.hpp
│   │   │   │   │   ├── ⚡ gativ_19_definir_modo_cores.cpp
│   │   │   │   │   ├── ⚡ gativ_19_definir_modo_cores.hpp
│   │   │   │   │   ├── ⚡ gativ_20_obter_modo_cores.cpp
│   │   │   │   │   ├── ⚡ gativ_20_obter_modo_cores.hpp
│   │   │   │   │   ├── ⚡ gativ_21_cor_texto_do_sistema.cpp
│   │   │   │   │   └── ⚡ gativ_21_cor_texto_do_sistema.hpp
│   │   │   │   ├── ⚡ Gerenciador_Atividades.cpp
│   │   │   │   └── ⚡ Gerenciador_Atividades.hpp
│   │   │   ├── 📁 GerenciamentoInterfaceGrafica
│   │   │   │   ├── 📁 GerenciadorInterfaceGrafica
│   │   │   │   │   ├── ⚡ ginter_04_center_on_screen.cpp
│   │   │   │   │   ├── ⚡ ginter_04_center_on_screen.hpp
│   │   │   │   │   ├── ⚡ ginter_05_showEvent.cpp
│   │   │   │   │   ├── ⚡ ginter_05_showEvent.hpp
│   │   │   │   │   ├── ⚡ ginter_06_create_widgets.cpp
│   │   │   │   │   ├── ⚡ ginter_06_create_widgets.hpp
│   │   │   │   │   ├── ⚡ ginter_07_create_button.cpp
│   │   │   │   │   ├── ⚡ ginter_07_create_button.hpp
│   │   │   │   │   ├── ⚡ ginter_08_create_menu.cpp
│   │   │   │   │   ├── ⚡ ginter_08_create_menu.hpp
│   │   │   │   │   ├── ⚡ ginter_09_change_language.cpp
│   │   │   │   │   ├── ⚡ ginter_09_change_language.hpp
│   │   │   │   │   ├── ⚡ ginter_10_change_color_mode.cpp
│   │   │   │   │   ├── ⚡ ginter_10_change_color_mode.hpp
│   │   │   │   │   ├── ⚡ ginter_11_retranslate_ui.cpp
│   │   │   │   │   ├── ⚡ ginter_11_retranslate_ui.hpp
│   │   │   │   │   ├── ⚡ ginter_12_excluir_item_lista_personalizada.cpp
│   │   │   │   │   ├── ⚡ ginter_12_excluir_item_lista_personalizada.hpp
│   │   │   │   │   ├── ⚡ ginter_13_populate_host_menus.cpp
│   │   │   │   │   └── ⚡ ginter_14_custom_agendas.cpp
│   │   │   │   ├── ⚡ Interface_Grafica.cpp
│   │   │   │   └── ⚡ Interface_Grafica.hpp
│   │   │   ├── 📁 language
│   │   │   │   ├── ⚡ tr_01_gerenciadorTraducao.cpp
│   │   │   │   └── ⚡ tr_01_gerenciadorTraducao.hpp
│   │   │   └── 📁 utils
│   │   │       ├── ⚡ CaminhoPersistenteUtils.cpp
│   │   │       ├── ⚡ CaminhoPersistenteUtils.hpp
│   │   │       ├── ⚡ ComboBoxUtils.cpp
│   │   │       ├── ⚡ ComboBoxUtils.hpp
│   │   │       ├── ⚡ DialogTranslationUtils.cpp
│   │   │       ├── ⚡ DialogTranslationUtils.hpp
│   │   │       ├── ⚡ IconUtils.cpp
│   │   │       └── ⚡ IconUtils.hpp
│   │   ├── ⚡ agenda_host_bridge.hpp
│   │   ├── ⚡ agenda_plugin.cpp
│   │   ├── ⚡ agenda_plugin.hpp
│   │   ├── ⚙️ agenda_plugin.json
│   │   └── ⚡ agenda_plugin_interface.hpp
│   ├── 📁 feynman
│   │   ├── 📁 Feynman
│   │   │   ├── 📁 language
│   │   │   │   ├── ⚡ tr_01_gerenciadorTraducao.cpp
│   │   │   │   └── ⚡ tr_01_gerenciadorTraducao.hpp
│   │   │   ├── 📁 utils
│   │   │   │   ├── ⚡ ApplicationPathUtils.cpp
│   │   │   │   ├── ⚡ ApplicationPathUtils.hpp
│   │   │   │   ├── ⚡ CaminhoPersistenteUtils.cpp
│   │   │   │   ├── ⚡ CaminhoPersistenteUtils.hpp
│   │   │   │   ├── ⚡ EventBus.cpp
│   │   │   │   ├── ⚡ EventBus.hpp
│   │   │   │   ├── ⚡ GerenciadorBotoesUI.cpp
│   │   │   │   ├── ⚡ GerenciadorBotoesUI.hpp
│   │   │   │   ├── ⚡ IconUtils.cpp
│   │   │   │   ├── ⚡ IconUtils.hpp
│   │   │   │   ├── ⚡ MessageBox.cpp
│   │   │   │   └── ⚡ MessageBox.hpp
│   │   │   ├── ⚡ agregador_01_Feynman.hpp
│   │   │   ├── ⚡ fey_01_FeynmanWorker.cpp
│   │   │   ├── ⚡ fey_01_FeynmanWorker.hpp
│   │   │   ├── ⚡ fey_02_carregar_conceitos.hpp
│   │   │   ├── ⚡ fey_03_setup_ui.cpp
│   │   │   ├── ⚡ fey_03_setup_ui.hpp
│   │   │   ├── ⚡ fey_04_atualizar_traducoes.cpp
│   │   │   ├── ⚡ fey_04_atualizar_traducoes.hpp
│   │   │   ├── ⚡ fey_05_novo_conceito.hpp
│   │   │   ├── ⚡ fey_06_limpar_campos.cpp
│   │   │   ├── ⚡ fey_06_limpar_campos.hpp
│   │   │   ├── ⚡ fey_07_salvar_conceito_atual.cpp
│   │   │   ├── ⚡ fey_07_salvar_conceito_atual.hpp
│   │   │   ├── ⚡ fey_08_editar_rotulos_dialog.cpp
│   │   │   ├── ⚡ fey_08_editar_rotulos_dialog.hpp
│   │   │   ├── ⚡ fey_08_selecionar_conceito.hpp
│   │   │   ├── ⚡ fey_09_deletar_conceito.hpp
│   │   │   ├── ⚡ fey_10_atualizar_lista.hpp
│   │   │   ├── ⚡ fey_12_menu_contexto.hpp
│   │   │   ├── ⚡ fey_13_receber_conceito_mapa_mental.hpp
│   │   │   ├── ⚡ fey_14_on_conceitos_loaded.hpp
│   │   │   ├── ⚡ fey_15_stop_feynman_thread.hpp
│   │   │   ├── ⚡ fey_16_cleanup.hpp
│   │   │   ├── ⚡ fey_17_limpar_abas.hpp
│   │   │   ├── ⚡ fey_18_arquivo_vinculado.hpp
│   │   │   ├── ⚡ feynman_metodo_helpers.cpp
│   │   │   ├── ⚡ feynman_metodo_helpers.hpp
│   │   │   ├── ⚡ feynman_support.cpp
│   │   │   └── ⚡ feynman_support.hpp
│   │   ├── ⚡ feynman_app.cpp
│   │   ├── ⚡ feynman_app.hpp
│   │   ├── ⚡ feynman_host_bridge.hpp
│   │   ├── ⚡ feynman_plugin.cpp
│   │   ├── ⚡ feynman_plugin.hpp
│   │   ├── ⚙️ feynman_plugin.json
│   │   ├── ⚡ feynman_plugin_interface.hpp
│   │   ├── ⚡ mod_05_metodo_feynman.cpp
│   │   └── ⚡ mod_05_metodo_feynman.hpp
│   ├── 📁 gantt
│   │   ├── 📁 Gantt
│   │   │   ├── 📁 api
│   │   │   │   ├── ⚡ HttpServer.cpp
│   │   │   │   ├── ⚡ HttpServer.hpp
│   │   │   │   ├── ⚡ RestApiController.cpp
│   │   │   │   └── ⚡ RestApiController.hpp
│   │   │   ├── 📁 application
│   │   │   │   └── 📁 services
│   │   │   │       ├── ⚡ AuditService.cpp
│   │   │   │       ├── ⚡ AuditService.hpp
│   │   │   │       ├── ⚡ Bim4DService.cpp
│   │   │   │       ├── ⚡ Bim4DService.hpp
│   │   │   │       ├── ⚡ CsvExchangeService.cpp
│   │   │   │       ├── ⚡ CsvExchangeService.hpp
│   │   │   │       ├── ⚡ FieldProgressService.cpp
│   │   │   │       ├── ⚡ FieldProgressService.hpp
│   │   │   │       ├── ⚡ GanttSimulationWorker.cpp
│   │   │   │       ├── ⚡ GanttSimulationWorker.hpp
│   │   │   │       ├── ⚡ GanttTaskIntegrationBridge.cpp
│   │   │   │       ├── ⚡ GanttTaskIntegrationBridge.hpp
│   │   │   │       ├── ⚡ IEisenhowerHost.hpp
│   │   │   │       ├── ⚡ PlanningEngineService.cpp
│   │   │   │       ├── ⚡ PlanningEngineService.hpp
│   │   │   │       ├── ⚡ ScheduleService.cpp
│   │   │   │       ├── ⚡ ScheduleService.hpp
│   │   │   │       ├── ⚡ SecurityService.cpp
│   │   │   │       ├── ⚡ SecurityService.hpp
│   │   │   │       ├── ⚡ WebhookDispatcher.cpp
│   │   │   │       └── ⚡ WebhookDispatcher.hpp
│   │   │   ├── 📁 domain
│   │   │   │   ├── 📁 models
│   │   │   │   │   ├── ⚡ AuditLog.cpp
│   │   │   │   │   ├── ⚡ AuditLog.hpp
│   │   │   │   │   ├── ⚡ Baseline.cpp
│   │   │   │   │   ├── ⚡ Baseline.hpp
│   │   │   │   │   ├── ⚡ BimLink.cpp
│   │   │   │   │   ├── ⚡ BimLink.hpp
│   │   │   │   │   ├── ⚡ Dependency.cpp
│   │   │   │   │   ├── ⚡ Dependency.hpp
│   │   │   │   │   ├── ⚡ GanttEnums.hpp
│   │   │   │   │   ├── ⚡ ProgressUpdate.cpp
│   │   │   │   │   ├── ⚡ ProgressUpdate.hpp
│   │   │   │   │   ├── ⚡ Schedule.cpp
│   │   │   │   │   ├── ⚡ Schedule.hpp
│   │   │   │   │   ├── ⚡ ScheduleItem.cpp
│   │   │   │   │   ├── ⚡ ScheduleItem.hpp
│   │   │   │   │   ├── ⚡ WorkCalendar.cpp
│   │   │   │   │   └── ⚡ WorkCalendar.hpp
│   │   │   │   └── 📁 rules
│   │   │   │       ├── ⚡ CpmEngine.cpp
│   │   │   │       ├── ⚡ CpmEngine.hpp
│   │   │   │       ├── ⚡ DependencyValidator.cpp
│   │   │   │       └── ⚡ DependencyValidator.hpp
│   │   │   ├── 📁 infrastructure
│   │   │   │   ├── 📁 connectors
│   │   │   │   │   ├── ⚡ MsProjectXmlConnector.cpp
│   │   │   │   │   └── ⚡ MsProjectXmlConnector.hpp
│   │   │   │   └── 📁 persistence
│   │   │   │       ├── 📁 migrations
│   │   │   │       │   └── 📄 V1__initial_gantt_bim_schema.sql
│   │   │   │       ├── ⚡ GanttDatabase.cpp
│   │   │   │       └── ⚡ GanttDatabase.hpp
│   │   │   ├── 📁 language
│   │   │   │   ├── ⚡ GanttTranslationWorker.cpp
│   │   │   │   ├── ⚡ GanttTranslationWorker.hpp
│   │   │   │   ├── ⚡ tr_01_gerenciadorTraducao.cpp
│   │   │   │   └── ⚡ tr_01_gerenciadorTraducao.hpp
│   │   │   └── 📁 ui
│   │   │       ├── ⚡ BiDirectionalResizeHeaderView.hpp
│   │   │       ├── ⚡ Bim4DSimulationPanel.cpp
│   │   │       ├── ⚡ Bim4DSimulationPanel.hpp
│   │   │       ├── ⚡ CsvMappingDialog.cpp
│   │   │       ├── ⚡ CsvMappingDialog.hpp
│   │   │       ├── ⚡ EisenhowerTaskImportDialog.cpp
│   │   │       ├── ⚡ EisenhowerTaskImportDialog.hpp
│   │   │       ├── ⚡ GanttCellHoverDelegate.hpp
│   │   │       ├── ⚡ GanttHolidaysDialog.cpp
│   │   │       ├── ⚡ GanttHolidaysDialog.hpp
│   │   │       ├── ⚡ GanttMainWindow.cpp
│   │   │       ├── ⚡ GanttMainWindow.hpp
│   │   │       ├── ⚡ GanttTimelineWidget.cpp
│   │   │       ├── ⚡ GanttTimelineWidget.hpp
│   │   │       ├── ⚡ GanttWbsTreeWidget.cpp
│   │   │       ├── ⚡ GanttWbsTreeWidget.hpp
│   │   │       ├── ⚡ GanttWorkingDaysDialog.cpp
│   │   │       ├── ⚡ GanttWorkingDaysDialog.hpp
│   │   │       ├── ⚡ TaskLinkDialog.cpp
│   │   │       └── ⚡ TaskLinkDialog.hpp
│   │   ├── ⚡ gantt_host_bridge.hpp
│   │   ├── ⚡ gantt_plugin.cpp
│   │   ├── ⚡ gantt_plugin.hpp
│   │   ├── ⚙️ gantt_plugin.json
│   │   └── ⚡ gantt_plugin_interface.hpp
│   └── 📁 pomodoro
│       ├── 📁 Pomodoro
│       │   ├── 📁 Alarme
│       │   │   ├── 📁 sound
│       │   │   │   ├── 🎵 mixkit-alarm-clock-beep-988.wav
│       │   │   │   ├── 🎵 mixkit-alarm-digital-clock-beep-989.wav
│       │   │   │   ├── 🎵 mixkit-alarm-tone-996.wav
│       │   │   │   ├── 🎵 mixkit-alert-alarm-1005.wav
│       │   │   │   ├── 🎵 mixkit-battleship-alarm-1001.wav
│       │   │   │   ├── 🎵 mixkit-casino-jackpot-alarm-and-coins-1991.wav
│       │   │   │   ├── 🎵 mixkit-casino-win-alarm-and-coins-1990.wav
│       │   │   │   ├── 🎵 mixkit-city-alert-siren-loop-1008.wav
│       │   │   │   ├── 🎵 mixkit-classic-alarm-995.wav
│       │   │   │   ├── 🎵 mixkit-classic-short-alarm-993.wav
│       │   │   │   ├── 🎵 mixkit-classic-winner-alarm-1997.wav
│       │   │   │   ├── 🎵 mixkit-critical-alarm-1004.wav
│       │   │   │   ├── 🎵 mixkit-data-scaner-2847.wav
│       │   │   │   ├── 🎵 mixkit-digital-clock-digital-alarm-buzzer-992.wav
│       │   │   │   ├── 🎵 mixkit-emergency-alert-alarm-1007.wav
│       │   │   │   ├── 🎵 mixkit-facility-alarm-908.wav
│       │   │   │   ├── 🎵 mixkit-facility-alarm-sound-999.wav
│       │   │   │   ├── 🎵 mixkit-game-notification-wave-alarm-987.wav
│       │   │   │   ├── 🎵 mixkit-interface-hint-notification-911.wav
│       │   │   │   ├── 🎵 mixkit-morning-clock-alarm-1003.wav
│       │   │   │   ├── 🎵 mixkit-retro-game-emergency-alarm-1000.wav
│       │   │   │   ├── 🎵 mixkit-rooster-crowing-in-the-morning-2462.wav
│       │   │   │   ├── 🎵 mixkit-scanning-sci-fi-alarm-905.wav
│       │   │   │   ├── 🎵 mixkit-security-facility-breach-alarm-994.wav
│       │   │   │   ├── 🎵 mixkit-short-rooster-crowing-2470.wav
│       │   │   │   ├── 🎵 mixkit-slot-machine-payout-alarm-1996.wav
│       │   │   │   ├── 🎵 mixkit-slot-machine-win-alarm-1995.wav
│       │   │   │   ├── 🎵 mixkit-sound-alert-in-hall-1006.wav
│       │   │   │   ├── 🎵 mixkit-space-shooter-alarm-1002.wav
│       │   │   │   ├── 🎵 mixkit-spaceship-alarm-998.wav
│       │   │   │   ├── 🎵 mixkit-street-public-alarm-997.wav
│       │   │   │   ├── 🎵 mixkit-vintage-warning-alarm-990.wav
│       │   │   │   └── 🎵 mixkit-warning-alarm-buzzer-991.wav
│       │   │   ├── ⚡ sound_01_config_dialog.cpp
│       │   │   └── ⚡ sound_01_config_dialog.hpp
│       │   ├── 📁 GerenciadorTarefas
│       │   │   ├── ⚡ gtar_01_carregar_tarefas.cpp
│       │   │   ├── ⚡ gtar_01_carregar_tarefas.hpp
│       │   │   ├── ⚡ gtar_02_salvar_tarefas.cpp
│       │   │   ├── ⚡ gtar_02_salvar_tarefas.hpp
│       │   │   ├── ⚡ gtar_03_setup_ui.cpp
│       │   │   ├── ⚡ gtar_03_setup_ui.hpp
│       │   │   ├── ⚡ gtar_04_atualizar_traducoes.cpp
│       │   │   ├── ⚡ gtar_04_atualizar_traducoes.hpp
│       │   │   ├── ⚡ gtar_05_criar_coluna.cpp
│       │   │   ├── ⚡ gtar_05_criar_coluna.hpp
│       │   │   ├── ⚡ gtar_06_on_drop_sincronizar.cpp
│       │   │   ├── ⚡ gtar_06_on_drop_sincronizar.hpp
│       │   │   ├── ⚡ gtar_07_on_rows_moved.cpp
│       │   │   ├── ⚡ gtar_07_on_rows_moved.hpp
│       │   │   ├── ⚡ gtar_08_sincronizar_status_pos_movimento.cpp
│       │   │   ├── ⚡ gtar_08_sincronizar_status_pos_movimento.hpp
│       │   │   ├── ⚡ gtar_09_adicionar_tarefa.cpp
│       │   │   ├── ⚡ gtar_09_adicionar_tarefa.hpp
│       │   │   ├── ⚡ gtar_10_adicionar_tarefa_interna.cpp
│       │   │   ├── ⚡ gtar_10_adicionar_tarefa_interna.hpp
│       │   │   ├── ⚡ gtar_11_adicionar_tarefa_externa.cpp
│       │   │   ├── ⚡ gtar_11_adicionar_tarefa_externa.hpp
│       │   │   ├── ⚡ gtar_12_mover_tarefa.cpp
│       │   │   ├── ⚡ gtar_12_mover_tarefa.hpp
│       │   │   ├── ⚡ gtar_13_atualizar_listas.cpp
│       │   │   ├── ⚡ gtar_13_atualizar_listas.hpp
│       │   │   ├── ⚡ gtar_14_mostrar_menu_contexto.cpp
│       │   │   ├── ⚡ gtar_14_mostrar_menu_contexto.hpp
│       │   │   ├── ⚡ gtar_15_ha_tarefas_marcadas.cpp
│       │   │   ├── ⚡ gtar_15_ha_tarefas_marcadas.hpp
│       │   │   ├── ⚡ gtar_16_remover_tarefa_por_item.cpp
│       │   │   ├── ⚡ gtar_16_remover_tarefa_por_item.hpp
│       │   │   ├── ⚡ gtar_17_remover_tarefas_marcadas.cpp
│       │   │   ├── ⚡ gtar_17_remover_tarefas_marcadas.hpp
│       │   │   ├── ⚡ gtar_18_resetar_contagem_botao.cpp
│       │   │   ├── ⚡ gtar_18_resetar_contagem_botao.hpp
│       │   │   ├── ⚡ gtar_19_mover_tarefas_marcadas.cpp
│       │   │   ├── ⚡ gtar_19_mover_tarefas_marcadas.hpp
│       │   │   ├── ⚡ gtar_20_mover_tarefa_por_item.cpp
│       │   │   ├── ⚡ gtar_20_mover_tarefa_por_item.hpp
│       │   │   ├── ⚡ gtar_21_ajustar_largura_combo.cpp
│       │   │   ├── ⚡ gtar_21_ajustar_largura_combo.hpp
│       │   │   ├── ⚡ gtar_22_aplicar_tema_dinamico_inputs.cpp
│       │   │   ├── ⚡ gtar_22_aplicar_tema_dinamico_inputs.hpp
│       │   │   ├── ⚡ gtar_23_eventFilter.cpp
│       │   │   ├── ⚡ gtar_23_eventFilter.hpp
│       │   │   ├── ⚡ gtar_24_update_rounded_mask.cpp
│       │   │   ├── ⚡ gtar_24_update_rounded_mask.hpp
│       │   │   ├── ⚡ gtar_25_prioridade_display.cpp
│       │   │   ├── ⚡ gtar_25_prioridade_display.hpp
│       │   │   ├── ⚡ gtar_26_FileDropLineEdit.cpp
│       │   │   ├── ⚡ gtar_26_FileDropLineEdit.hpp
│       │   │   ├── ⚡ gtar_27_RotatedTabButton.cpp
│       │   │   ├── ⚡ gtar_27_RotatedTabButton.hpp
│       │   │   ├── ⚡ gtar_28_redefinir_ciclos.cpp
│       │   │   ├── ⚡ gtar_28_redefinir_ciclos.hpp
│       │   │   └── ⚡ gtar_internal.hpp
│       │   ├── 📁 GerenciadorTempo
│       │   │   ├── ⚡ gtemp_01_setup_ui.cpp
│       │   │   ├── ⚡ gtemp_01_setup_ui.hpp
│       │   │   ├── ⚡ gtemp_02_atualizar_rotulo_botao_eisenhower.cpp
│       │   │   ├── ⚡ gtemp_02_atualizar_rotulo_botao_eisenhower.hpp
│       │   │   ├── ⚡ gtemp_03_setup_integracao_eisenhower.cpp
│       │   │   ├── ⚡ gtemp_03_setup_integracao_eisenhower.hpp
│       │   │   ├── ⚡ gtemp_04_on_tarefa_adicionada_gestao_tempo.cpp
│       │   │   ├── ⚡ gtemp_04_on_tarefa_adicionada_gestao_tempo.hpp
│       │   │   ├── ⚡ gtemp_05_on_event_bus_tarefa.cpp
│       │   │   ├── ⚡ gtemp_05_on_event_bus_tarefa.hpp
│       │   │   ├── ⚡ gtemp_06_aplicar_tema_dinamico_inputs.cpp
│       │   │   ├── ⚡ gtemp_06_aplicar_tema_dinamico_inputs.hpp
│       │   │   ├── ⚡ gtemp_07_eventFilter.cpp
│       │   │   ├── ⚡ gtemp_07_eventFilter.hpp
│       │   │   ├── ⚡ gtemp_08_atualizar_traducoes.cpp
│       │   │   ├── ⚡ gtemp_08_atualizar_traducoes.hpp
│       │   │   ├── ⚡ gtemp_09_registrar_pomodoro.cpp
│       │   │   ├── ⚡ gtemp_09_registrar_pomodoro.hpp
│       │   │   ├── ⚡ gtemp_10_cleanup.cpp
│       │   │   ├── ⚡ gtemp_10_cleanup.hpp
│       │   │   ├── ⚡ gtemp_11_del.cpp
│       │   │   ├── ⚡ gtemp_11_del.hpp
│       │   │   ├── ⚡ gtemp_12_atualizar_coluna_doing_timer.cpp
│       │   │   ├── ⚡ gtemp_12_atualizar_coluna_doing_timer.hpp
│       │   │   ├── ⚡ gtemp_13_ajustar_largura_minima_coluna_esquerda.cpp
│       │   │   ├── ⚡ gtemp_13_ajustar_largura_minima_coluna_esquerda.hpp
│       │   │   ├── ⚡ gtemp_14_interacoes_coluna_doing_timer.cpp
│       │   │   ├── ⚡ gtemp_14_interacoes_coluna_doing_timer.hpp
│       │   │   └── ⚡ gtemp_internal.hpp
│       │   ├── 📁 PomodoroTimer
│       │   │   ├── ⚡ pomo_01_setup_ui.cpp
│       │   │   ├── ⚡ pomo_01_setup_ui.hpp
│       │   │   ├── ⚡ pomo_02_atualizar_traducoes.cpp
│       │   │   ├── ⚡ pomo_02_atualizar_traducoes.hpp
│       │   │   ├── ⚡ pomo_03_atualizar_configuracoes.cpp
│       │   │   ├── ⚡ pomo_03_atualizar_configuracoes.hpp
│       │   │   ├── ⚡ pomo_04_toggle_timer.cpp
│       │   │   ├── ⚡ pomo_04_toggle_timer.hpp
│       │   │   ├── ⚡ pomo_05_atualizar_timer.cpp
│       │   │   ├── ⚡ pomo_05_atualizar_timer.hpp
│       │   │   ├── ⚡ pomo_06_atualizar_display.cpp
│       │   │   ├── ⚡ pomo_06_atualizar_display.hpp
│       │   │   ├── ⚡ pomo_07_executar_alerta.cpp
│       │   │   ├── ⚡ pomo_07_executar_alerta.hpp
│       │   │   ├── ⚡ pomo_08_finalizar_ciclo.cpp
│       │   │   ├── ⚡ pomo_08_finalizar_ciclo.hpp
│       │   │   ├── ⚡ pomo_09_resetar_timer.cpp
│       │   │   ├── ⚡ pomo_09_resetar_timer.hpp
│       │   │   ├── ⚡ pomo_10_resetar_ciclo.cpp
│       │   │   ├── ⚡ pomo_10_resetar_ciclo.hpp
│       │   │   ├── ⚡ pomo_11_pular_ciclo.cpp
│       │   │   ├── ⚡ pomo_11_pular_ciclo.hpp
│       │   │   ├── ⚡ pomo_12_aplicar_tema_dinamico.cpp
│       │   │   ├── ⚡ pomo_12_aplicar_tema_dinamico.hpp
│       │   │   ├── ⚡ pomo_13_eventFilter.cpp
│       │   │   ├── ⚡ pomo_13_eventFilter.hpp
│       │   │   ├── ⚡ pomo_14_aviso_metas_ciclos.cpp
│       │   │   ├── ⚡ pomo_14_aviso_metas_ciclos.hpp
│       │   │   └── ⚡ pomo_internal.hpp
│       │   ├── 📁 Tarefa
│       │   │   ├── ⚡ taref_01_to_dict.cpp
│       │   │   ├── ⚡ taref_01_to_dict.hpp
│       │   │   ├── ⚡ taref_02_from_dict.cpp
│       │   │   ├── ⚡ taref_02_from_dict.hpp
│       │   │   ├── ⚡ taref_03_normalizar_prioridade.cpp
│       │   │   └── ⚡ taref_03_normalizar_prioridade.hpp
│       │   ├── 📁 language
│       │   │   ├── ⚡ tr_01_gerenciadorTraducao.cpp
│       │   │   └── ⚡ tr_01_gerenciadorTraducao.hpp
│       │   ├── 📁 utils
│       │   │   ├── ⚡ ApplicationPathUtils.cpp
│       │   │   ├── ⚡ ApplicationPathUtils.hpp
│       │   │   ├── ⚡ CaminhoPersistenteUtils.cpp
│       │   │   ├── ⚡ CaminhoPersistenteUtils.hpp
│       │   │   ├── ⚡ EventBus.cpp
│       │   │   ├── ⚡ EventBus.hpp
│       │   │   ├── ⚡ FontManager.cpp
│       │   │   ├── ⚡ FontManager.hpp
│       │   │   ├── ⚡ GerenciadorBotoesUI.cpp
│       │   │   ├── ⚡ GerenciadorBotoesUI.hpp
│       │   │   ├── ⚡ IconUtils.cpp
│       │   │   ├── ⚡ IconUtils.hpp
│       │   │   ├── ⚡ MessageBox.cpp
│       │   │   ├── ⚡ MessageBox.hpp
│       │   │   ├── ⚡ SoundManager.cpp
│       │   │   └── ⚡ SoundManager.hpp
│       │   ├── ⚡ pomodoro_support.cpp
│       │   ├── ⚡ pomodoro_support.hpp
│       │   ├── ⚡ tmp_01_Tarefa.cpp
│       │   ├── ⚡ tmp_01_Tarefa.hpp
│       │   ├── ⚡ tmp_02_PomodoroTimer.cpp
│       │   ├── ⚡ tmp_02_PomodoroTimer.hpp
│       │   ├── ⚡ tmp_03_GerenciadorTarefas.cpp
│       │   ├── ⚡ tmp_03_GerenciadorTarefas.hpp
│       │   ├── ⚡ tmp_04_KanbanListWidget.cpp
│       │   ├── ⚡ tmp_04_KanbanListWidget.hpp
│       │   ├── ⚡ tmp_05_GerenciadorTempo.cpp
│       │   ├── ⚡ tmp_05_GerenciadorTempo.hpp
│       │   ├── ⚡ tmp_06_TimeWorker.cpp
│       │   └── ⚡ tmp_06_TimeWorker.hpp
│       ├── ⚡ pomodoro_app.cpp
│       ├── ⚡ pomodoro_app.hpp
│       ├── ⚡ pomodoro_host_bridge.hpp
│       ├── ⚡ pomodoro_plugin.cpp
│       ├── ⚡ pomodoro_plugin.hpp
│       ├── ⚙️ pomodoro_plugin.json
│       └── ⚡ pomodoro_plugin_interface.hpp
├── 📁 services
│   ├── 📁 Arquivo
│   │   ├── ⚡ agregador_01_Arquivo.hpp
│   │   ├── ⚡ arq_01_clear_all_lists.cpp
│   │   ├── ⚡ arq_01_clear_all_lists.hpp
│   │   ├── ⚡ arq_02_add_placeholders_after_clear.cpp
│   │   ├── ⚡ arq_02_add_placeholders_after_clear.hpp
│   │   ├── ⚡ arq_03_novo.cpp
│   │   ├── ⚡ arq_03_novo.hpp
│   │   ├── ⚡ arq_04_limpar_tudo.cpp
│   │   ├── ⚡ arq_04_limpar_tudo.hpp
│   │   ├── ⚡ arq_05_parse_text_date_time.cpp
│   │   ├── ⚡ arq_05_parse_text_date_time.hpp
│   │   ├── ⚡ arq_06_abrir_arquivo.cpp
│   │   ├── ⚡ arq_06_abrir_arquivo.hpp
│   │   ├── ⚡ arq_07_salvar_como.cpp
│   │   └── ⚡ arq_07_salvar_como.hpp
│   ├── 📁 MostrarMenuContexto
│   │   ├── ⚡ agregador_01_MostrarMenuContexto.hpp
│   │   ├── ⚡ mmc_01_compute_widget_height.cpp
│   │   ├── ⚡ mmc_01_compute_widget_height.hpp
│   │   ├── ⚡ mmc_02_apply_widget_min_height.cpp
│   │   ├── ⚡ mmc_02_apply_widget_min_height.hpp
│   │   ├── ⚡ mmc_03_effective_locale.cpp
│   │   ├── ⚡ mmc_03_effective_locale.hpp
│   │   ├── ⚡ mmc_04_apply_locale_to_dateedit.cpp
│   │   ├── ⚡ mmc_04_apply_locale_to_dateedit.hpp
│   │   ├── ⚡ mmc_05_is_completed_list.cpp
│   │   ├── ⚡ mmc_05_is_completed_list.hpp
│   │   ├── ⚡ mmc_06_quadrant_options.cpp
│   │   ├── ⚡ mmc_06_quadrant_options.hpp
│   │   ├── ⚡ mmc_07_target_list_for_quadrant.cpp
│   │   ├── ⚡ mmc_07_target_list_for_quadrant.hpp
│   │   ├── ⚡ mmc_08_find_source_item_for_calendar.cpp
│   │   ├── ⚡ mmc_08_find_source_item_for_calendar.hpp
│   │   ├── ⚡ mmc_09_base_text_from_item.cpp
│   │   ├── ⚡ mmc_09_base_text_from_item.hpp
│   │   ├── ⚡ mmc_10_build_display_and_tooltip.cpp
│   │   ├── ⚡ mmc_10_build_display_and_tooltip.hpp
│   │   ├── ⚡ mmc_11_edit_task_dialog.cpp
│   │   ├── ⚡ mmc_11_edit_task_dialog.hpp
│   │   ├── ⚡ mmc_12_move_to_quadrant.cpp
│   │   ├── ⚡ mmc_12_move_to_quadrant.hpp
│   │   ├── ⚡ mmc_13_mostrar_menu_contexto.cpp
│   │   └── ⚡ mmc_13_mostrar_menu_contexto.hpp
│   ├── 📁 RecursosProdutividade
│   │   ├── ⚡ recpro_01_utilitarios_compartilhados.cpp
│   │   ├── ⚡ recpro_01_utilitarios_compartilhados.hpp
│   │   ├── ⚡ recpro_02_busca_painel.cpp
│   │   ├── ⚡ recpro_02_busca_painel.hpp
│   │   ├── ⚡ recpro_03_recorrencias.cpp
│   │   ├── ⚡ recpro_03_recorrencias.hpp
│   │   ├── ⚡ recpro_04_lembretes_locais.cpp
│   │   ├── ⚡ recpro_04_lembretes_locais.hpp
│   │   ├── ⚡ recpro_05_relatorios_fluxos.cpp
│   │   ├── ⚡ recpro_05_relatorios_fluxos.hpp
│   │   ├── ⚡ recpro_06_revisoes_plugins.cpp
│   │   ├── ⚡ recpro_06_revisoes_plugins.hpp
│   │   ├── ⚡ recpro_07_filtros.cpp
│   │   └── ⚡ recpro_07_filtros.hpp
│   ├── ⚡ agregador_01_services.cpp
│   ├── ⚡ agregador_01_services.hpp
│   ├── ⚡ incore_01_initUI.cpp
│   ├── ⚡ incore_01_initUI.hpp
│   ├── ⚡ incore_02_add_placeholder.cpp
│   ├── ⚡ incore_02_add_placeholder.hpp
│   ├── ⚡ incore_03_criar_menu_configuracoes.cpp
│   ├── ⚡ incore_03_criar_menu_configuracoes.hpp
│   ├── ⚡ incore_04_definir_idioma.cpp
│   ├── ⚡ incore_04_definir_idioma.hpp
│   ├── ⚡ incore_05_atualizar_textos.cpp
│   ├── ⚡ incore_05_atualizar_textos.hpp
│   ├── ⚡ incore_06_atualizar_placeholders.cpp
│   ├── ⚡ incore_06_atualizar_placeholders.hpp
│   ├── ⚡ incore_07_MostrarMenuContexto.cpp
│   ├── ⚡ incore_07_MostrarMenuContexto.hpp
│   ├── ⚡ incore_08_exibir_sobre.cpp
│   ├── ⚡ incore_08_exibir_sobre.hpp
│   ├── ⚡ incore_09_Arquivo.cpp
│   ├── ⚡ incore_09_Arquivo.hpp
│   ├── ⚡ incore_10_task_list_widget.cpp
│   ├── ⚡ incore_10_task_list_widget.hpp
│   ├── ⚡ incore_11_atualizar_itens_tarefas.cpp
│   ├── ⚡ incore_11_atualizar_itens_tarefas.hpp
│   ├── ⚡ incore_12_Manual.cpp
│   ├── ⚡ incore_12_Manual.hpp
│   ├── ⚡ incore_13_prioridade_display.cpp
│   ├── ⚡ incore_13_prioridade_display.hpp
│   ├── ⚡ incore_14_EditTaskDialog.cpp
│   ├── ⚡ incore_14_EditTaskDialog.hpp
│   ├── ⚡ incore_15_ProductivityFeatures.cpp
│   ├── ⚡ incore_15_ProductivityFeatures.hpp
│   ├── ⚡ incore_16_manual_pt-BR.cpp
│   ├── ⚡ incore_16_manual_pt-BR.hpp
│   ├── ⚡ incore_17_manual_en-US.cpp
│   └── ⚡ incore_17_manual_en-US.hpp
├── 📁 ui
│   ├── 📁 CalendarDialog
│   │   ├── ⚡ agregador_01_CalendarDialog.cpp
│   │   ├── ⚡ agregador_01_CalendarDialog.hpp
│   │   ├── ⚡ caldial_01_closeEvent.cpp
│   │   ├── ⚡ caldial_01_closeEvent.hpp
│   │   ├── ⚡ caldial_02_open_linked_file.cpp
│   │   ├── ⚡ caldial_02_open_linked_file.hpp
│   │   ├── ⚡ caldial_03_collect_tasks.cpp
│   │   ├── ⚡ caldial_03_collect_tasks.hpp
│   │   ├── ⚡ caldial_04_update_task_list.cpp
│   │   ├── ⚡ caldial_04_update_task_list.hpp
│   │   ├── ⚡ caldial_05_on_calendar_context_menu.cpp
│   │   ├── ⚡ caldial_05_on_calendar_context_menu.hpp
│   │   ├── ⚡ caldial_06_prompt_edit_task_for_date.cpp
│   │   ├── ⚡ caldial_06_prompt_edit_task_for_date.hpp
│   │   ├── ⚡ caldial_07_prompt_remove_task_for_date.cpp
│   │   ├── ⚡ caldial_07_prompt_remove_task_for_date.hpp
│   │   ├── ⚡ caldial_08_update_badge_counts.cpp
│   │   ├── ⚡ caldial_08_update_badge_counts.hpp
│   │   ├── ⚡ caldial_09_on_language_changed.cpp
│   │   ├── ⚡ caldial_09_on_language_changed.hpp
│   │   ├── ⚡ caldial_10_apply_locale_to_calendar.cpp
│   │   ├── ⚡ caldial_10_apply_locale_to_calendar.hpp
│   │   ├── ⚡ caldial_11_get_task_dates.cpp
│   │   ├── ⚡ caldial_11_get_task_dates.hpp
│   │   ├── ⚡ caldial_12_apply_highlighted_dates.cpp
│   │   └── ⚡ caldial_12_apply_highlighted_dates.hpp
│   ├── ⚡ agregador_01_ui.cpp
│   ├── ⚡ agregador_01_ui.hpp
│   ├── ⚡ ui_01_SobreDialog.cpp
│   ├── ⚡ ui_01_SobreDialog.hpp
│   ├── ⚡ ui_02_OpcoesSobre.cpp
│   ├── ⚡ ui_02_OpcoesSobre.hpp
│   ├── ⚡ ui_03_add_task.cpp
│   ├── ⚡ ui_03_add_task.hpp
│   ├── ⚡ ui_04_handle_item_checked.cpp
│   ├── ⚡ ui_04_handle_item_checked.hpp
│   ├── ⚡ ui_05_move_item_between_lists.cpp
│   ├── ⚡ ui_05_move_item_between_lists.hpp
│   ├── ⚡ ui_06_remove_task.cpp
│   ├── ⚡ ui_06_remove_task.hpp
│   ├── ⚡ ui_07_save_tasks.cpp
│   ├── ⚡ ui_07_save_tasks.hpp
│   ├── ⚡ ui_08_load_tasks.cpp
│   ├── ⚡ ui_08_load_tasks.hpp
│   ├── ⚡ ui_09_Calendar.cpp
│   ├── ⚡ ui_09_Calendar.hpp
│   ├── ⚡ ui_10_BadgeCalendarWidget.cpp
│   ├── ⚡ ui_10_BadgeCalendarWidget.hpp
│   ├── ⚡ ui_11_NativeLineEditFrame.cpp
│   ├── ⚡ ui_11_NativeLineEditFrame.hpp
│   ├── ⚡ ui_12_CalendarDialog.cpp
│   ├── ⚡ ui_12_CalendarDialog.hpp
│   ├── ⚡ ui_13_RotatedTabButton.cpp
│   ├── ⚡ ui_13_RotatedTabButton.hpp
│   ├── ⚡ ui_14_CalendarPanel.cpp
│   ├── ⚡ ui_14_CalendarPanel.hpp
│   ├── ⚡ ui_15_edit_task.cpp
│   ├── ⚡ ui_15_edit_task.hpp
│   ├── ⚡ ui_16_YearCalendarWidget.cpp
│   └── ⚡ ui_16_YearCalendarWidget.hpp
├── 📁 utils
│   ├── ⚡ AccentTabStyle.hpp
│   ├── ⚡ AppConfig.hpp
│   ├── ⚡ ApplicationPathUtils.cpp
│   ├── ⚡ ApplicationPathUtils.hpp
│   ├── ⚡ BarraTitulosUtils.cpp
│   ├── ⚡ BarraTitulosUtils.hpp
│   ├── ⚡ CaminhoPersistenteUtils.cpp
│   ├── ⚡ CaminhoPersistenteUtils.hpp
│   ├── ⚡ FeynmanLabelsConfig.cpp
│   ├── ⚡ FeynmanLabelsConfig.hpp
│   ├── ⚡ FontUtils.hpp
│   ├── ⚡ IconUtils.cpp
│   ├── ⚡ IconUtils.hpp
│   ├── ⚡ LinkedTitleUnderlineDelegate.cpp
│   ├── ⚡ LinkedTitleUnderlineDelegate.hpp
│   ├── ⚡ LogManager.cpp
│   ├── ⚡ LogManager.hpp
│   ├── ⚡ PluginRegistry.cpp
│   ├── ⚡ PluginRegistry.hpp
│   ├── ⚡ TrialManager.cpp
│   ├── ⚡ TrialManager.hpp
│   ├── ⚡ UnifiedTaskStore.cpp
│   └── ⚡ UnifiedTaskStore.hpp
├── ⚡ eisenhower_app.cpp
├── ⚡ eisenhower_app.hpp
└── 📄 windows_app_icon.rc.in
```

**Root Path:** `c:\Users\ferna\APLICATIVOS\CPP\EisenPo_cpp\.github`

```
├── 📁 ISSUE_TEMPLATE
│   ├── 📝 bug_report.md
│   ├── 📝 feature_request.md
│   └── 📝 question.md
├── 📁 workflows
│   └── ⚙️ update-readme-version.yml
└── 📝 PULL_REQUEST_TEMPLATE.md
```

**Root Path:** `c:\Users\ferna\APLICATIVOS\CPP\EisenPo_cpp\mocks`

```
├── 📁 icones
│   ├── 📄 calendar.ico
│   ├── 🖼️ calendar.png
│   ├── 📄 eisenpo.ico
│   ├── 🖼️ eisenpo.png
│   ├── 📄 eisenpo1.ico
│   ├── 🖼️ eisenpo1.png
│   ├── 🖼️ eisenpo2.png
│   ├── 🖼️ eisenpo_1080-1080.png
│   ├── 🖼️ eisenpo_1440-2160.png
│   ├── 🖼️ eisenpo_150-150.png
│   ├── 🖼️ eisenpo_1920-1080.png
│   ├── 🖼️ eisenpo_2160-1440.png
│   ├── 🖼️ eisenpo_2160-2160.png
│   ├── 🖼️ eisenpo_300-300.png
│   ├── 🖼️ eisenpo_3840-2160.png
│   ├── 🖼️ eisenpo_584-800.png
│   ├── 🖼️ eisenpo_71-71.png
│   ├── 🖼️ eisenpo_en-US.png
│   └── 🖼️ eisenpo_pt-BR.png
├── 📄 CMakeLists.txt
├── ⚡ CompileTranslationsGUI.cpp
├── ⚡ CompileTranslationsGUI.hpp
├── ⚡ CompileTranslationsTerminal.cpp
├── ⚡ CompileTranslationsTerminal.hpp
├── ⚡ find_VersionEditor.cpp
└── ⚡ find_VersionEditor.hpp
```

**Root Path:** `c:\Users\ferna\APLICATIVOS\CPP\EisenPo_cpp\images`

```
└── 📁 interfaces
    ├── 📁 en_US
    │   ├── 🖼️ Captura de tela 2026-08-15 005858.png
    │   ├── 🖼️ Captura de tela 2026-08-15 005918.png
    │   ├── 🖼️ Captura de tela 2026-08-15 005945.png
    │   ├── 🖼️ Captura de tela 2026-08-15 010006.png
    │   └── 🖼️ Captura de tela 2026-08-15 010029.png
    └── 📁 pt_BR
        ├── 🖼️ Captura de tela 2026-08-15 005502.png
        ├── 🖼️ Captura de tela 2026-08-15 005526.png
        ├── 🖼️ Captura de tela 2026-08-15 005545.png
        ├── 🖼️ Captura de tela 2026-08-15 005633.png
        └── 🖼️ Captura de tela 2026-08-15 005713.png
```
