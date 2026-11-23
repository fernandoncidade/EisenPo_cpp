# Arquitetura do Sistema / System Architecture — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher a arquitetura em português</summary><br>

# EisenPo — Arquitetura de Sistema e Especificações Técnicas

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Padrão Tecnológico:** C++23 | Qt 6.12.0 Widgets, Multimedia, Sql, Network, LinguistTools | SQLite  
**Plataforma Alvo:** Windows 11 / Windows 10 x64 (`MSVC 14.52` & `MinGW-w64 15.1.0`)  
**Autor:** Fernando Nillsson Cidade  

---

## 1. Filosofia Arquitetural e Unificação Modular Nativa

A partir da consolidação arquitetural (`v2026.9.27.0`–`v2026.9.28.0`), o **EisenPo** eliminou o carregamento dinâmico de plugins via DLL (`QPluginLoader` / `plugins/*.dll`), passando a compilar e vincular nativamente o host principal e os quatro módulos de produtividade dentro de um único executável de alto desempenho (`EisenPo.exe`):

```text
+---------------------------------------------------------------------------------+
|                    CAMADA DE APRESENTAÇÃO HOST (Qt 6.12.0 Widgets)              |
|  EisenhowerMatrixApp | Tabbed / Quadrants / 2x2 / PoCa (Pomodoro + 2 Calend.)   |
|  Calendários (BadgeCalendarWidget, YearCalendarWidget) | EditTaskDialog (Wide/C) |
|  Produtividade (Busca Ctrl+Shift+G, Filtros, Recorrências, Lembretes, Sobre/F1)  |
+---------------------------------------------------------------------------------+
         |                    |                      |                     |
         v                    v                      v                     v
+----------------+   +-----------------+   +-------------------+   +--------------+
| Módulo Pomodoro|   | Módulo Feynman  |   |   Módulo Agenda   |   |Gantt & BIM 4D|
| Temporizador,  |   | 5 Etapas Ativas,|   | Registro de Ativ.,|   | 4 Camadas CA:|
| Metas 1-999,   |   | Rótulos Custom.,|   | Cursos/Notas,     |   | domain/      |
| Alarmes .wav   |   | Arquivos Apoio  |   | Listas Aninhadas  |   | application/ |
| (QtMultimedia) |   |                 |   | SQLite (listas.db)|   | infra/ & ui/ |
+----------------+   +-----------------+   +-------------------+   +--------------+
         |                    |                      |                     |
         +--------------------+----------+-----------+---------------------+
                                         v
+---------------------------------------------------------------------------------+
|            SERVIÇOS COMPARTILHADOS, SINCRONIZAÇÃO & I18N (source/utils/)         |
|  UnifiedTaskStore (gravação atômica assíncrona) | EisenhowerGanttHostAdapter    |
|  GanttTaskIntegrationBridge (Cache O(1) QHash) | GerenciadorTraducao (.ts->.qm) |
|  PluginRegistry | LogManager (file_eisenpo_*.log) | CaminhoPersistenteUtils     |
+---------------------------------------------------------------------------------+
                                         |
                                         v
+---------------------------------------------------------------------------------+
|              PERSISTÊNCIA LOCAL 100% OFFLINE (%LOCALAPPDATA%\EisenPo)           |
|  tarefas.json | layout.json | language.json | feynman_rotulos_config.json       |
|  listas.db (SQLite) | eisenpo_gantt.db (SQLite) | agenda_config.json / agendas/ |
+---------------------------------------------------------------------------------+
```

---

## 2. Arquitetura dos Módulos Nativos e Padrões Aplicados

### 2.1. Núcleo Host (`source/app/`, `source/services/`, `source/ui/`, `source/models/`)
- **`EisenhowerMatrixApp` e Layouts Dinâmicos:** Organiza tarefas em 4 quadrantes (`Importante × Urgente`) com sub-listas de Pendentes, Em Progresso e Concluídas; suporta alternância instantânea entre **Abas Horizontais**, **Abas Verticais**, **Quadrantes**, **Matriz 2×2** e o painel lateral integrado **PoCa** (Temporizador Pomodoro + Configurações + progressão sequencial $2 \times K$ de calendários).
- **Edição Responsiva (`EditTaskDialog`):** Alternância fluida com banda de histerese ($680\text{–}760\text{ px}$) entre modo **Compacto** e **Largo**, mantendo descrição expandida à esquerda e controles ancorados à direita.
- **`UnifiedTaskStore`:** Centraliza leitura, gravação atômica assíncrona com `QSaveFile` em `tarefas.json` e sinalização de alterações entre Eisenhower, Pomodoro, Feynman, Agenda e Gantt sem travar a thread de interface.

### 2.2. Módulo Pomodoro (`source/modules/pomodoro/`)
- Gerencia ciclos de foco, pausa curta e pausa longa com metas individuais por tarefa (`ciclos_totais` de `1` a `999`) e diálogo preventivo de alinhamento (`pomo_14_aviso_metas_ciclos.cpp`).
- Subsistema de áudio `Qt6::Multimedia` (`Alarme/sound/*.wav`) com **inicialização preguiçosa sob demanda (*lazy initialization*)**, evitando bloqueios na abertura do aplicativo.
- Calcula progresso físico discreto por ciclos concluídos:
  $$P_{\text{físico}} = \mathrm{clamp}\left(\frac{c_{\text{concluídos}}}{c_{\text{estimados}}} \times 100\%,\; 0\%,\; 100\%\right)$$

### 2.3. Módulo Feynman (`source/modules/feynman/`)
- Estrutura estudos ativos em etapas configuráveis (conceito, explicação simplificada, lacunas, revisão/domínio e arquivos locais vinculados).
- Persistência de rótulos personalizados em `feynman_rotulos_config.json` (`FeynmanLabelsConfig`) mantendo retrocompatibilidade ao interpretar descrições salvas anteriormente.

### 2.4. Módulo Agenda — Registro de Atividades (`source/modules/agenda/`)
- Cadastro avaliativo acadêmico e profissional (curso, ementa, semestre, disciplina, turma, tipo e nota), agendas customizadas hierárquicas (`agendas/*.json`), banco relacional SQLite local `listas.db` e preferências de cor independentes em `agenda_config.json`.

### 2.5. Módulo Gantt & BIM 4D (`source/modules/gantt/`)
Projetado em **Clean Architecture de 4 camadas**:
1. **`domain/` & `domain/rules/`:** Entidades de projeto/cronograma, regras de calendário laboral (`WorkCalendar`), gestor de feriados de **18 países** com algoritmo astronômico de **Meeus/Jones/Butcher** para cálculo da Páscoa e feriados móveis, e o motor analítico **`CpmEngine`** (Critical Path Method em grafo direcionado acíclico — DAG com verificação preventiva de ciclos, cálculo *Forward Pass* $ES/EF$, *Backward Pass* $LS/LF$, folga total $FT = LS - ES$, folga livre $FF$ e identificação de atividades críticas).
2. **`application/services/`:** `ScheduleService`, `PlanningEngineService`, `FieldProgressService`, `Bim4DService`, `CsvExchangeService` e `GanttTaskIntegrationBridge` (cache em memória `O(1)` via `QHash<QString, TaskLinkSummary>`).
3. **`infrastructure/persistence/`:** Persistência transacional SQLite em `eisenpo_gantt.db` com versionamento de esquema (`V1__initial_gantt_bim_schema.sql`).
4. **`ui/`:** `GanttMainWindow` (5 layouts persistidos em `layout.json`), `GanttWbsTreeWidget`, `GanttTimelineWidget` (escala Dia/Semana/Mês, Linha de Base / Linha Fantasma, tooltips dinâmicos retraduzidos e cores unificadas de status `#4caf50`, `#2196f3`, `#f44336`, `#9e9e9e`) e `Bim4DSimulationPanel` (barra responsiva de 1–3 linhas vinculando cronograma a identificadores `IFC GUID / UniqueId`).

*(Nota: o subdiretório `Gantt/api/` contém classes auxiliares `HttpServer` e `RestApiController` compiladas no código-fonte, porém não instanciadas nem executadas durante a operação do aplicativo.)*

---

## 3. Pipeline de Tradução Dinâmica e Empacotamento Dual Toolchain

- **Catálogo Centralizado (`source/language/translations/`):** Todos os arquivos `.ts` (`eisenhower_organizer_*`, `pomodoro_*`, `feynman_*`, `agenda_*`, `languagegantt_*`/`gantt_*`) são compilados em `.qm` pelo Qt `lrelease.exe` (via `CompileTranslationsGUI` / `CompileTranslationsTerminal`) e implantados em `translations/` junto ao binário principal, com resposta imediata a `QEvent::LanguageChange` (inclusive em janelas auxiliares abertas e `GanttTranslationWorker` em `QThread`).
- **Dual Toolchain Windows x64 (`CMakeLists.txt`, `CMakePresets.json`, `cmake/`):** Suporta integralmente **MSVC x64 (`14.52.37025` / Windows SDK `10.0.28000.0`)** e **MinGW-w64 x64 (`GCC 15.1.0`)** sobre **Qt 6.12.0** e **Ninja 1.13.2**, com isolamento rigoroso de DLLs de runtime (*app-local*), configuração limpa via `qt.conf` (`Prefix = .`, `Plugins = plugins`, `Translations = translations`) e remoção automática de símbolos `.pdb`, drivers SQL/imagem supérfluos e `vc_redist*.exe`.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse architecture in English</summary><br>

# EisenPo — System Architecture & Technical Specifications

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Technology Stack:** C++23 | Qt 6.12.0 Widgets, Multimedia, Sql, Network, LinguistTools | SQLite  
**Target Platform:** Windows 11 / Windows 10 x64 (`MSVC 14.52` & `MinGW-w64 15.1.0`)  
**Author:** Fernando Nillsson Cidade  

---

## 1. Architectural Philosophy & Native Module Unification

Since the `v2026.9.27.0`–`v2026.9.28.0` unification, **EisenPo** replaced dynamic plugin DLL loading (`QPluginLoader` / `plugins/*.dll`) with direct in-tree compilation, linking the host organizer and all four productivity modules natively into a single high-speed executable (`EisenPo.exe`):

```text
+---------------------------------------------------------------------------------+
|                      HOST PRESENTATION LAYER (Qt 6.12.0 Widgets)                |
|  EisenhowerMatrixApp | Tabbed / Quadrants / 2x2 / PoCa (Pomodoro + Dual Cal.)   |
|  Calendars (BadgeCalendarWidget, YearCalendarWidget) | EditTaskDialog (Wide/C)  |
|  Productivity Suite (Global Search, Filters, Recurrences, Reminders, About/F1)  |
+---------------------------------------------------------------------------------+
         |                    |                      |                     |
         v                    v                      v                     v
+----------------+   +-----------------+   +-------------------+   +--------------+
| Pomodoro Module|   | Feynman Module  |   |   Agenda Module   |   |Gantt & 4D BIM|
| Timers, Targets|   | 5 Active Steps, |   | Activity Tracking,|   | 4-Layer CA:  |
| 1-999, .wav    |   | Custom Labels,  |   | Courses/Grades,   |   | domain/      |
| Alarms (QtMult)|   | Linked Files    |   | Nested SQLite DB  |   | application/ |
|                |   |                 |   | (listas.db)       |   | infra/ & ui/ |
+----------------+   +-----------------+   +-------------------+   +--------------+
         |                    |                      |                     |
         +--------------------+----------+-----------+---------------------+
                                         v
+---------------------------------------------------------------------------------+
|              SHARED SERVICES, SYNC ENGINE & I18N (source/utils/)                 |
|  UnifiedTaskStore (async atomic commits) | EisenhowerGanttHostAdapter           |
|  GanttTaskIntegrationBridge (O(1) QHash Cache) | GerenciadorTraducao (.ts->.qm) |
|  PluginRegistry | LogManager (file_eisenpo_*.log) | CaminhoPersistenteUtils     |
+---------------------------------------------------------------------------------+
                                         |
                                         v
+---------------------------------------------------------------------------------+
|              100% OFFLINE LOCAL PERSISTENCE (%LOCALAPPDATA%\EisenPo)            |
|  tarefas.json | layout.json | language.json | feynman_rotulos_config.json       |
|  listas.db (SQLite) | eisenpo_gantt.db (SQLite) | agenda_config.json / agendas/ |
+---------------------------------------------------------------------------------+
```

---

## 2. Native Module Architecture & Core Algorithms

### 2.1. Host Core (`source/app/`, `source/services/`, `source/ui/`, `source/models/`)
- **`EisenhowerMatrixApp` & Adaptive Layouts:** Organizes tasks across 4 quadrants (`Important × Urgent`) with Pending, In Progress, and Completed lists; dynamically switches between **Tabbed Horizontal**, **Tabbed Vertical**, **Quadrants**, **2×2 Matrix**, and the **PoCa** integrated sidebar (Pomodoro Timer + Settings + sequential $2 \times K$ dual calendars).
- **Responsive Task Editor (`EditTaskDialog`):** Dual-mode UI with anti-flicker hysteresis ($680\text{–}760\text{ px}$ band) transitioning smoothly between **Compact** and **Wide** modes.
- **`UnifiedTaskStore`:** Coordinates non-blocking atomic commits via `QSaveFile` to `tarefas.json` and live synchronization across Eisenhower, Pomodoro, Feynman, Agenda, and Gantt without UI stalls.

### 2.2. Pomodoro Module (`source/modules/pomodoro/`)
- Manages focus, short break, and long break intervals with individual per-task targets (`ciclos_totais` `1`–`999`) and the preventive Cycle Target Warning dialog (`pomo_14_aviso_metas_ciclos.cpp`).
- `Qt6::Multimedia` audio alarms (`Alarme/sound/*.wav`) use **on-demand lazy initialization**, eliminating application startup lag.
- Computes discrete cycle-driven physical progress:
  $$P_{\text{physical}} = \mathrm{clamp}\left(\frac{c_{\text{completed}}}{c_{\text{estimated}}} \times 100\%,\; 0\%,\; 100\%\right)$$

### 2.3. Feynman Module (`source/modules/feynman/`)
- Structures active learning workflows across five customizable steps (concept, plain explanation, gap mapping, mastery review cycles, and local study attachments) backed by `feynman_rotulos_config.json` with backward-compatible description parsing.

### 2.4. Agenda / Activity Registration Module (`source/modules/agenda/`)
- Academic and workplace activity tracking (course, syllabus, semester, subject, class, assessment type, grade), hierarchical schedules (`agendas/*.json`), local SQLite storage in `listas.db`, and isolated color preferences in `agenda_config.json`.

### 2.5. Gantt & 4D BIM Module (`source/modules/gantt/`)
Engineered in **4-layer Clean Architecture**:
1. **`domain/` & `domain/rules/`:** Schedule entities, configurable workweek & shift engine (`WorkCalendar`), **18-country Holiday Manager** driven by the **Meeus/Jones/Butcher** Gregorian Easter algorithm for movable holidays, and the **`CpmEngine`** solver (DAG topological sort with circular dependency interception, Forward Pass $ES/EF$, Backward Pass $LS/LF$, Total Float $TF = LS - ES$, Free Float $FF$, and Critical Path highlighting).
2. **`application/services/`:** `ScheduleService`, `PlanningEngineService`, `FieldProgressService`, `Bim4DService`, `CsvExchangeService`, and `GanttTaskIntegrationBridge` (`O(1)` in-memory `QHash<QString, TaskLinkSummary>` cache).
3. **`infrastructure/persistence/`:** Local transactional SQLite persistence in `eisenpo_gantt.db` with schema versioning (`V1__initial_gantt_bim_schema.sql`).
4. **`ui/`:** `GanttMainWindow` (5 layout presets saved in `layout.json`), `GanttWbsTreeWidget`, `GanttTimelineWidget` (Day/Week/Month zoom, Baselines & Ghost Line, runtime-retranslatable tooltips, unified status color palette `#4caf50`, `#2196f3`, `#f44336`, `#9e9e9e`), and `Bim4DSimulationPanel` (responsive 1–3-line bar linking schedule items to `IFC GUID / UniqueId`).

*(Audit note: `Gantt/api/` contains uninstantiated `HttpServer` and `RestApiController` sources that are never launched during application execution.)*

---

## 3. Translation Pipeline & Dual-Toolchain Packaging

- **Unified Translation Catalog (`source/language/translations/`):** All `.ts` catalogs (`eisenhower_organizer_*`, `pomodoro_*`, `feynman_*`, `agenda_*`, `languagegantt_*`/`gantt_*`) compile via Qt `lrelease.exe` into `.qm` binaries deployed alongside `EisenPo.exe` in `translations/`, reacting live to `QEvent::LanguageChange` (including open child windows and `GanttTranslationWorker`).
- **Dual Windows x64 Toolchains (`CMakeLists.txt`, `CMakePresets.json`, `cmake/`):** Full support for **MSVC x64 (`14.52.37025` / Windows SDK `10.0.28000.0`)** and **MinGW-w64 x64 (`GCC 15.1.0`)** targeting **Qt 6.12.0** and **Ninja 1.13.2**, with strict app-local runtime isolation, clean `qt.conf` (`Prefix = .`, `Plugins = plugins`, `Translations = translations`), and automatic pruning of `.pdb` symbols, unused plugins, and `vc_redist*.exe`.

</details>
