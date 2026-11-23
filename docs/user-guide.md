# Guia do Usuário / User Guide — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher o guia do usuário em português</summary><br>

# EisenPo — Guia do Usuário e Referência Rápida

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

> *(Para o manual completo passo a passo dentro do aplicativo, consulte também [`MANUAL.md`](../MANUAL.md#ptbr).)*

---

> [!WARNING]
> **Escopo Auxiliar Gratuito e Responsabilidade Exclusiva ("AS IS"):** O **EisenPo** é um aplicativo gratuito de uso auxiliar para organização pessoal, foco, estudos e planejamento de cronogramas. Por estar em evolução contínua e poder conter limitações ou eventuais erros de software, cronogramas contratuais ou decisões críticas devem sempre ser conferidos com ferramentas consolidadas sob responsabilidade do usuário ([`README.md`](../README.md#ptbr) e [EULA Oficial](../source/assets/eisenhower/EULA/EULA_pt_BR.txt)).

---

## 1. Matriz de Eisenhower e Layouts (`Host`)

- **Quatro Quadrantes Estratégicos:** Q1 (*Importante e Urgente*), Q2 (*Importante, mas Não Urgente*), Q3 (*Não Importante, mas Urgente*) e Q4 (*Não Importante e Não Urgente*), cada um com sub-listas de **Pendentes**, **Em Progresso** e **Concluídas**.
- **Painel "Nova Tarefa" em 3 Linhas + Duração:** Digite o título, selecione quadrante, prioridade, nível de domínio, data, horário e **Duração (dias)** com ajuste automático de largura dos seletores.
- **Cinco Disposições de Layout (`Configurações > Layout de Tarefas > Interface do Usuário`):**
  1. `Layout com Abas > Horizontal`
  2. `Layout com Abas > Vertical`
  3. `Layout em Quadrantes`
  4. `Matriz 2×2` (abas `Pendentes` e `Concluídas` dentro de cada quadrante)
  5. `PoCa` (coluna lateral esquerda unificada com Temporizador Pomodoro, Configurações e progressão sequencial $2 \times K$ de calendários mensais)
- **Edição Responsiva (`EditTaskDialog`):** Janela minimizável/maximizável que alterna automaticamente entre modos *Compacto* e *Largo* (com histerese anti-tremulação), mantendo notas expandidas à esquerda e controles de metadados ancorados à direita.
- **Vínculo de Arquivos Externos:** Anexe PDFs, planilhas ou documentos pelo diálogo de seleção ou arraste diretamente do Explorador do Windows; o arquivo só é aberto mediante clique explícito do usuário.

---

## 2. Módulos Integrados Nativamente

### 2.1. Módulo Pomodoro
- Ciclos configuráveis de foco, pausa curta e pausa longa, alarmes sonoros `.wav` (carregamento preguiçoso sem travar a abertura) e painel sincronizado com as tarefas da matriz.
- **Metas Individuais (1–999 ciclos):** Defina `Meta de Ciclos Pomodoro (🎯)` por tarefa; ao iniciar o relógio, a janela **Aviso de Metas de Ciclos (`⚠️`)** detecta divergências entre tarefas ativas e o ciclo do relógio, permitindo padronizar ou prosseguir com metas individuais.

### 2.2. Módulo Feynman (Aprendizado Ativo)
- Metodologia estruturada em 5 etapas (conceito, explicação simplificada, lacunas, revisão/domínio e arquivos locais vinculados) em visualizações em *Abas*, *Vertical* ou *Horizontal*.
- **`Configurações > Editar Rótulos`:** Renomeie livremente todas as etapas da Técnica de Feynman (persistido em `feynman_rotulos_config.json`).

### 2.3. Módulo Agenda — Registro de Atividades
- Acessível por `Ferramentas -> Registro de Atividades` com cadastro de curso, ementa, semestre, disciplina, turma, tipo avaliativo, datas/horários e notas em banco relacional local SQLite (`listas.db`).
- Suporta **agendas customizadas em árvore aninhada** (`agendas/*.json`), modos de cor independentes (`Black` adaptável ao tema do Windows e `Colorful`) e geração sincronizada de tarefas em `tarefas.json`.

### 2.4. Módulo Gantt & BIM 4D
- Acessível por `Ferramentas -> Cronograma de Gantt BIM 4D` com cálculo automático do **Caminho Crítico (CPM)**, análise de folgas livres/totais e detecção preventiva de dependências circulares (`FS`, `SS`, `FF`, `SF`).
- **Cinco Layouts (`Configurações > Layout > Tabelas`):** `Original` (50%/50% WBS/Gantt no topo + Simulação 4D na base), `Vertical Topo`, `Vertical Inferior`, `Horizontal Esquerda` e `Horizontal Direita` (salvos em `layout.json`).
- **Cores Unificadas & Tooltips Dinâmicos:** Código WBS, datas localizadas (`dd/MM/yyyy HH:mm` vs `MM/dd/yyyy HH:mm`), progresso Pomodoro discreto (`(concluídos / estimados) × 100%`), folga e indicadores cromáticos (`✅ Executado`, `🔄 Em Andamento`, `⚠️ Atrasado`, `⏳ Futuro`) retraduzidos ao vivo.
- **Calendário Laboral & Feriados de 18 Países:** Configure dias úteis (seg–dom), jornada (`WorkCalendar`) e aplique feriados nacionais fixos e móveis (Páscoa por algoritmo de Meeus/Jones/Butcher) para 18 países.
- **Linhas de Base, Linha Fantasma & Simulação 4D:** Compare o planejado com o executado e acompanhe a evolução construtiva vinculada a `IFC GUID / UniqueId`, com importação/exportação `.csv`.

---

## 3. Ferramentas de Produtividade, Importação/Exportação e Atalhos

- **Ferramentas Integradas:** Busca global (`Ctrl+Shift+G`), painel *Hoje / Próximos 7 dias* (`Ctrl+Shift+H`), filtros avançados (`Ctrl+Alt+F`), recorrências em lote com prévia e desfazimento (`Gerar próximas recorrências`), lembretes locais com bandeja do sistema, fluxo integrado de estudo, templates de tarefas, revisões espaçadas, métricas de histórico e **Gerenciador de Logs** (`file_eisenpo_*.log`).
- **Importação e Exportação:** `JSON` (backup completo), `CSV`, planilhas Excel `XLSX` (abas por quadrante), relatórios `PDF` e calendários `ICS`.
- **Atalhos Principais:**
  - `Ctrl+N` (Nova sessão) · `Ctrl+O` (Abrir/Importar) · `Ctrl+S` (Salvar) · `Ctrl+Shift+S` (Salvar como/Exportar)
  - `Ctrl+M` (Calendário) · `Ctrl+Shift+E` (Em Progresso / Pomodoro) · `Ctrl+Shift+G` (Busca Global) · `Ctrl+Shift+H` (Hoje / 7 dias) · `Ctrl+Alt+F` (Filtros Avançados)
  - `Ctrl+Shift+A` (Sobre) · `Ctrl+Shift+M` (Manual Completo) · `Ctrl+Q` (Sair)

---

## 4. Desinstalação Limpa (Windows / Microsoft Store)

Desinstale o **EisenPo** a qualquer momento via **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou **Menu Iniciar → Desinstalar**), sem deixar serviços em segundo plano. Em instalações não empacotadas em MSIX, seus arquivos pessoais permanecem sob controle exclusivo do usuário em `%LOCALAPPDATA%\EisenPo\` (além das exportações salvas em outras pastas) e podem ser removidos livremente quando desejado.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse user guide in English</summary><br>

# EisenPo — User Guide & Quick Reference

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Author:** Fernando Nillsson Cidade  

> *(For the full step-by-step operations manual included with the project, see [`MANUAL.md`](../MANUAL.md#enus).)*

---

> [!WARNING]
> **Free Auxiliary Scope & Sole Responsibility ("AS IS"):** **EisenPo** is a free auxiliary application for personal task organization, focus, study, and schedule planning. Because it remains under ongoing development and may contain limitations or software defects, contractual schedules or critical decisions should always be independently verified against established tools under user responsibility ([`README.md`](../README.md#enus) and [Official EULA](../source/assets/eisenhower/EULA/EULA_en_US.txt)).

---

## 1. Eisenhower Matrix & Workspace Layouts (`Host`)

- **Four Strategic Quadrants:** Q1 (*Important & Urgent*), Q2 (*Important, Not Urgent*), Q3 (*Not Important, Urgent*), and Q4 (*Not Important & Not Urgent*), each holding **Pending**, **In Progress**, and **Completed** lists.
- **Ergonomic 3-Row "New Task" Panel + Duration:** Enter description, select quadrant, priority, mastery level, date, time, and **Duration (days)** with dynamic dropdown width sizing.
- **Five Workspace Layouts (`Settings > Task Layout > User Interface`):**
  1. `Tabbed Layout > Horizontal`
  2. `Tabbed Layout > Vertical`
  3. `Quadrants Layout`
  4. `2×2 Matrix Layout` (`Pending` and `Completed` tabs inside each quadrant)
  5. `PoCa` (unified left sidebar combining Pomodoro Timer, Settings, and $2 \times K$ sequential monthly calendars)
- **Responsive Task Editor (`EditTaskDialog`):** Minimizable/maximizable editor smoothly transitioning across a $680\text{–}760\text{ px}$ hysteresis band between *Compact* and *Wide* modes.
- **External File Linking:** Attach PDFs, spreadsheets, or files via dialog or drag-and-drop from Windows Explorer; external files are opened solely on explicit user click.

---

## 2. Natively Integrated Modules

### 2.1. Pomodoro Module
- Configurable focus, short-break, and long-break timers, `.wav` alarms (lazy-initialized for instant app startup), and task board synced with the matrix.
- **Individual Cycle Targets (1–999 cycles):** Assign per-task `Target Pomodoro Cycles (🎯)`; starting the timer checks active tasks via **Cycle Target Warning (`⚠️`)** to reset targets or proceed as planned.

### 2.2. Feynman Module (Active Learning)
- Structured 5-step study flow (concept, plain-language explanation, gap identification, review/mastery cycles, and linked local study files) across *Tabs*, *Vertical*, or *Horizontal* views.
- **`Settings > Edit Labels`:** Freely customize all Feynman step labels (saved in `feynman_rotulos_config.json`).

### 2.3. Agenda — Activity Registration Module
- Available via `Tools -> Activity Registration` for academic/professional schedules (course, syllabus, semester, subject, class, assessment type, grades) backed by local SQLite (`listas.db`).
- Supports hierarchical custom nested schedules (`agendas/*.json`), independent color themes (`Black` adapting to Windows Light/Dark mode and `Colorful`), and live sync with `tarefas.json`.

### 2.4. Gantt & 4D BIM Module
- Available via `Tools -> Gantt 4D BIM Schedule`, featuring analytical **Critical Path Method (CPM)** scheduling, free/total float tracking, and circular dependency prevention (`FS`, `SS`, `FF`, `SF`).
- **Five Persistent Layouts (`Settings > Layout > Tables`):** `Original` (50%/50% WBS/Gantt split + bottom 4D Simulation), `Top Vertical`, `Bottom Vertical`, `Left Horizontal`, and `Right Horizontal` (`layout.json`).
- **Unified Status Styling & Dynamic Tooltips:** Hover tooltips display WBS code, localized dates (`MM/dd/yyyy HH:mm` vs `dd/MM/yyyy HH:mm`), discrete Pomodoro progress (`(completed / target) × 100%`), float, and color badges (`✅ Executed`, `🔄 In Progress`, `⚠️ Delayed`, `⏳ Future`) updated live on language switch.
- **Work Calendar & 18-Country Holidays:** Customize working weekdays (Mon–Sun), shift hours (`WorkCalendar`), and one-click national holidays across 18 countries with Meeus/Jones/Butcher movable Easter holidays.
- **Baselines, Ghost Line & 4D Simulation:** Compare planned vs. actual dates and drive 4D timelines linked to `IFC GUID / UniqueId`, with `.csv` import/export.

---

## 3. Productivity Suite, Import/Export & Shortcuts

- **Productivity Features:** Global Search (`Ctrl+Shift+G`), Today / Next 7 Days (`Ctrl+Shift+H`), Advanced Filters (`Ctrl+Alt+F`), batch Recurrences with restore snapshot, Local Reminders (desktop & tray), Integrated Study Workflow, Task Templates, Spaced Reviews, History Metrics, and **Log Manager** (`file_eisenpo_*.log`).
- **Import & Export:** `JSON` (full backup), `CSV`, Excel `XLSX` (sheets per quadrant), structured `PDF` reports, and `.ics` calendars.
- **Key Shortcuts:**
  - `Ctrl+N` (New session) · `Ctrl+O` (Open/Import) · `Ctrl+S` (Save) · `Ctrl+Shift+S` (Save as/Export)
  - `Ctrl+M` (Calendar) · `Ctrl+Shift+E` (In Progress / Pomodoro) · `Ctrl+Shift+G` (Global Search) · `Ctrl+Shift+H` (Today / Next 7 days) · `Ctrl+Alt+F` (Advanced Filters)
  - `Ctrl+Shift+A` (About) · `Ctrl+Shift+M` (User Manual) · `Ctrl+Q` (Quit)

---

## 4. Clean Uninstallation (Windows / Microsoft Store)

Cleanly uninstall **EisenPo** at any time via **Windows Settings → Apps → Installed Apps** (or **Start Menu → Uninstall**), leaving zero background services running. In unpackaged desktop installs, user data stays under your sole control inside `%LOCALAPPDATA%\EisenPo\` (plus exports saved elsewhere) and can be deleted manually at any time.

</details>
