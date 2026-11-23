# Guia de Contribuição / Contributing Guide — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher as instruções em português</summary><br>

# Contribuindo com o EisenPo

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  

Agradecemos profundamente seu interesse em contribuir com o **EisenPo**! Sua colaboração auxilia no avanço de uma suíte desktop livre, offline e acessível em **C++ / Qt 6.12.0** dedicada à organização estratégica de tarefas (Matriz de Eisenhower), sessões de foco (Pomodoro), aprendizado ativo (Feynman), Registro de Atividades (Agenda/SQLite) e Cronograma Gantt & BIM 4D.

> [!NOTE]
> **Sobre o Desenvolvimento, Suporte e Contribuições Externas:**  
> Este projeto é concebido, desenvolvido e mantido primariamente como uma iniciativa pessoal e educacional pelo autor, estudante de Engenharia Civil, que está em processo contínuo de aprendizado em programação C++/Qt e gerenciamento de cronogramas.  
> O envio de sugestões, abertura de *Issues* e submissão de *Pull Requests (PRs)* são sempre bem-vindos! Contudo, a revisão de código, o acompanhamento de discussões e a incorporação de contribuições ocorrem no ritmo do autor, em regime estritamente voluntário e sem compromisso de prazo ou SLA. Caso um chamado ou PR demore a receber resposta ou não seja incorporado ao branch principal, sinta-se plenamente livre para manter um *Fork* independente deste repositório e evoluir suas próprias versões nos exatos termos da licença **GNU AGPLv3** (bem como adaptar livremente a documentação em [`source/assets/`](source/assets/eisenhower/COPYRIGHT/AVISO%20DE%20COPYRIGHT%20E%20MARCA%20REGISTRA_pt_BR.txt)).  
> Por se tratar de um aplicativo gratuito de uso auxiliar ainda não maduro para uso definitivo ou crítico em produção — podendo conter limitações, falhas humanas e erros de lógica, cálculo de cronogramas ou persistência —, contribuições voltadas a testes automatizados, auditoria de segurança e revisão de algoritmos (como `CpmEngine`, `WorkCalendar` e `UnifiedTaskStore`) são especialmente valiosas ([`README.md`](README.md#ptbr) e [EULA Oficial](source/assets/eisenhower/EULA/EULA_pt_BR.txt)).

---

## 1. Código de Conduta

Ao participar deste projeto, você concorda em cumprir nosso [Código de Conduta](CODE_OF_CONDUCT.md). Trate todos os colaboradores com respeito, empatia e profissionalismo.

---

## 2. Formas de Contribuir

Você pode colaborar com o **EisenPo** de diversas maneiras:
- **Relatar Bugs:** Identifique falhas na sincronização entre Matriz de Eisenhower, Pomodoro, Feynman, Agenda e Gantt, inconsistências de cálculo no Caminho Crítico (CPM) / calendário de trabalho, problemas de layout ou falhas em exportação/importação (`JSON`, `CSV`, `XLSX`, `PDF`, `ICS`).
- **Acessibilidade e Usabilidade:** Proponha ajustes ergonômicos voltados à redução de ruído cognitivo (TDAH/dislexia), atalhos de teclado e legibilidade nos temas Claro e Escuro do Windows 10/11.
- **Qualidade de Código & Testes:** Implemente suítes de teste unitário (ex.: `CpmEngine`, `WorkCalendar`, `UnifiedTaskStore`) preservando conformidade estrita com **C++23**, isolamento offline e bilinguismo dinâmico.
- **Melhorar a Documentação:** Aprimore os manuais (`MANUAL.md`, `docs/user-guide.md`), a documentação arquitetural (`docs/architecture.md`) e guias de compilação.
- **Internacionalização (i18n):** Revise ou expanda os catálogos `.ts` em `source/language/translations/` e valide a compilação para `.qm` via `CompileTranslationsTerminal.exe`.

---

## 3. Configuração do Ambiente de Desenvolvimento

### 3.1. Pré-requisitos
- **Sistema Operacional:** Windows 10 (build `17763+`) ou Windows 11 x64;
- **Compilador:** **MSVC x64** (Visual Studio 18 / Build Tools `14.52.37025`, Windows SDK `10.0.28000.0`) ou **MinGW-w64 x64** (`C:\Qt\Tools\mingw1510_64`, GCC `15.1.0`);
- **Qt 6.12.0:** Kits `msvc2022_64` e/ou `mingw_64` com os módulos `Core`, `Gui`, `Widgets`, `Multimedia`, `Sql`, `Network` e `LinguistTools`;
- **Ferramentas de Build:** **CMake 3.28+**, **MSBuild / Visual Studio 18 2026** (MSVC puro) e **Ninja 1.13.2** (MinGW-w64);
- **Empacotamento (opcional):** **Inno Setup 7** (`ISCC.exe`) para compilar `library/EisenPo.iss`, `library/EisenPo_MSVC.iss` e `library/EisenPo_MinGW.iss`.

> [!NOTE]
> **Compatibilidade C++23 Verificada (`feynman`):** O projeto compila integralmente sob `CMAKE_CXX_STANDARD 23` (`CMAKE_CXX_SCAN_FOR_MODULES OFF`) tanto em **MSVC `14.52.37025`** quanto em **MinGW-w64 `GCC 15.1.0`** com Qt `6.12.0`, após a refatoração do identificador `concept` nos 5 arquivos de `source/modules/feynman/Feynman/` (`concept_text` / `conceito`). Consulte [`docs/technical-audit-and-roadmap.md`](docs/technical-audit-and-roadmap.md#ptbr).

### 3.2. Compilando com Presets Oficiais (`CMakePresets.json`)

Os presets de configuração e build estão separados para o aplicativo principal (`host-*`) e para os utilitários em `mocks/` (`mocks-*`), nos fluxos **MSVC x64** e **MinGW-w64 x64**:

```powershell
# Clonar o repositório
git clone https://github.com/fernandoncidade/EisenPo_cpp.git
cd EisenPo_cpp

# Configurar e compilar o aplicativo principal (MSVC: build/build_EisenPo_msvc | MinGW: build/build_EisenPo_mingw)
cmake --preset host-qt6-msvc-release
cmake --build --preset host-qt6-msvc-release

cmake --preset host-qt6-mingw-release
cmake --build --preset host-qt6-mingw-release

# Configurar e compilar os utilitários auxiliares em mocks/ (CompileTranslationsGUI, CompileTranslationsTerminal, find_VersionEditor)
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release
```

*(Veja também [`OBSERVACAO.md`](OBSERVACAO.md#ptbr) e [`CMakePresets.json`](CMakePresets.json) para presets PGO, comandos de compilação de traduções `.ts -> .qm`, auditoria `find_VersionEditor --check` e geração de instaladores Inno Setup sem embutir `vc_redist.x64.exe`.)*

---

## 4. Estrutura do Código-Fonte e Diretrizes Arquiteturais

1. **Separação Modular Nativa Integrada (`source/`):**
   - `main.cpp` e `source/app/`: Inicialização Qt 6, aplicação principal (`EisenhowerMatrixApp`), abas modulares e adaptadores de host;
   - `source/models/`: Modelos Qt de lista e dados compartilhados (`TaskRecordListModel`);
   - `source/modules/`: Os quatro módulos integrados nativamente ao executável `EisenPo.exe` — `pomodoro/`, `feynman/`, `agenda/` e `gantt/` (o módulo Gantt adota Clean Architecture em `domain/`, `application/`, `infrastructure/` e `ui/`);
   - `source/services/`: Lógica central de organização, produtividade (`RecursosProdutividade/`), ações de arquivo (`Arquivo/`), menu de contexto (`MostrarMenuContexto/`), edição de tarefas e manuais embutidos;
   - `source/ui/`: Diálogos de interface, calendários (`CalendarDialog/`, `YearCalendarWidget`), painel PoCa e janela `Sobre`;
   - `source/utils/`: Persistência atômica unificada (`UnifiedTaskStore`), caminhos locais (`CaminhoPersistenteUtils`), logs (`LogManager`), fontes, ícones e registro interno de módulos;
   - `source/language/`: Gerenciamento de idioma em tempo real e catálogo único `translations/*.ts`;
   - `source/assets/`: Documentação institucional e licenças (`eisenhower/` e `LICENSES/`), cuja adaptação em forks possui liberdade total concedida pelo autor;
   - `mocks/`: Ferramentas auxiliares independentes (`CompileTranslationsGUI`, `CompileTranslationsTerminal`, `find_VersionEditor`).
2. **Gerenciamento de Memória e RAII:** Prefira ponteiros inteligentes (`std::unique_ptr`, `std::shared_ptr`), alocação em pilha e árvore de propriedade pai-filho do `QObject`, evitando ponteiros brutos proprietários.
3. **Soberania de Dados e Privacidade Local-First:** Todas as alterações devem manter a operação **100% offline e local** em `%LOCALAPPDATA%\EisenPo`, sem telemetria, sem contas obrigatórias e sem envios de dados pela rede.
4. **Bilinguismo Dinâmico (`pt_BR` e `en_US`):** Novos textos visíveis ao usuário devem estar preparados para tradução dinâmica em tempo de execução (`QEvent::LanguageChange` / arquivos `.ts`).

---

## 5. Processo de Envio de Pull Requests

1. Crie uma branch específica a partir de `main` (`git checkout -b feat/minha-melhoria`).
2. Implemente suas alterações preservando o isolamento offline e o bilinguismo `pt_BR` / `en_US`.
3. Se alterar arquivos `.ts`, valide a compilação com `CompileTranslationsTerminal.exe`; se alterar arquivos versionados ou documentação em `source/assets/`, valide com `find_VersionEditor.exe --check --root .`.
4. Faça commits claros e objetivos (ex.: `fix(gantt): corrigir calculo de folga total em feriados moveis`).
5. Abra o Pull Request usando o modelo oficial (`.github/PULL_REQUEST_TEMPLATE.md`).

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse instructions in English</summary><br>

# Contributing to EisenPo

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  

Thank you very much for your interest in contributing to **EisenPo**! Your collaboration helps advance a free, offline, accessible **C++ / Qt 6.12.0** desktop suite dedicated to strategic task organization (Eisenhower Matrix), focused work sessions (Pomodoro), active learning (Feynman Method), Activity Registration (Agenda/SQLite), and Gantt & 4D BIM Scheduling.

> [!NOTE]
> **Development Context, Support & External Contributions:**  
> This project is conceived, developed, and maintained primarily as a personal and educational initiative by the author, a Civil Engineering student continuously learning C++/Qt software engineering and schedule management algorithms.  
> Suggestions, *Issues*, and *Pull Requests (PRs)* are always welcome! However, please note that code reviews, discussions, and merges take place at the author's own pace, on a strictly voluntary basis, with no guaranteed response timeframe or SLA. If an issue or PR takes time to be reviewed or is not merged into the main branch, please understand these time constraints. Under the **GNU AGPLv3** license, you have complete freedom to maintain an independent *Fork* of this repository and evolve your own versions (as well as full freedom to adapt or replace the compliance texts under [`source/assets/`](source/assets/eisenhower/COPYRIGHT/COPYRIGHT%20AND%20TRADEMARK%20NOTICE_en_US.txt)).  
> Because this is a free auxiliary application not yet mature enough for critical or definitive production reliance — and may contain limitations, human flaws, or defects in logic, schedule calculation, or persistence — contributions targeting automated unit testing, security audits, and algorithm verification (`CpmEngine`, `WorkCalendar`, `UnifiedTaskStore`) are especially valuable ([`README.md`](README.md#enus) and [Official EULA](source/assets/eisenhower/EULA/EULA_en_US.txt)).

---

## 1. Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md). Please treat all contributors with respect, empathy, and professionalism.

---

## 2. Ways to Contribute

You can contribute to **EisenPo** in several ways:
- **Bug Reports:** Identify issues in synchronization between the Eisenhower Matrix, Pomodoro, Feynman, Agenda, and Gantt modules, Critical Path Method (CPM) / working-calendar discrepancies, UI layout glitches, or import/export bugs (`JSON`, `CSV`, `XLSX`, `PDF`, `ICS`).
- **Accessibility & Ergonomics:** Suggest improvements reducing cognitive friction (ADHD/dyslexia), keyboard shortcuts, and Windows 10/11 Light/Dark theme readability.
- **Code Quality & Automated Tests:** Add CTest unit tests (e.g., for `CpmEngine`, `WorkCalendar`, `UnifiedTaskStore`) while preserving strict **C++23** compliance, offline operation, and dynamic bilingualism.
- **Documentation:** Improve guides (`MANUAL.md`, `docs/user-guide.md`), architecture notes (`docs/architecture.md`), and build documentation.
- **Internationalization (i18n):** Review or expand `.ts` translation catalogs under `source/language/translations/` and verify compilation to `.qm` via `CompileTranslationsTerminal.exe`.

---

## 3. Development Environment Setup

### 3.1. Prerequisites
- **Operating System:** Windows 10 (build `17763+`) or Windows 11 x64;
- **Compiler:** **MSVC x64** (Visual Studio 18 / Build Tools `14.52.37025`, Windows SDK `10.0.28000.0`) or **MinGW-w64 x64** (`C:\Qt\Tools\mingw1510_64`, GCC `15.1.0`);
- **Qt 6.12.0:** `msvc2022_64` and/or `mingw_64` kits with `Core`, `Gui`, `Widgets`, `Multimedia`, `Sql`, `Network`, and `LinguistTools`;
- **Build Tools:** **CMake 3.28+**, **MSBuild / Visual Studio 18 2026** (pure MSVC), and **Ninja 1.13.2** (MinGW-w64);
- **Installers (optional):** **Inno Setup 7** (`ISCC.exe`) for `library/EisenPo.iss`, `library/EisenPo_MSVC.iss`, and `library/EisenPo_MinGW.iss`.

> [!NOTE]
> **Verified C++23 Build (`feynman`):** The project builds cleanly end-to-end under `CMAKE_CXX_STANDARD 23` (`CMAKE_CXX_SCAN_FOR_MODULES OFF`) on both **MSVC `14.52.37025`** and **MinGW-w64 `GCC 15.1.0`** with Qt `6.12.0`, following refactoring of the legacy `concept` identifier in 5 files under `source/modules/feynman/Feynman/` (`concept_text` / `conceito`). See [`docs/technical-audit-and-roadmap.md`](docs/technical-audit-and-roadmap.md#enus).

### 3.2. Building with Official Presets (`CMakePresets.json`)

Configure and build presets are cleanly separated for the main application (`host-*`) and auxiliary utilities in `mocks/` (`mocks-*`), across both **MSVC x64** and **MinGW-w64 x64**:

```powershell
# Clone repository
git clone https://github.com/fernandoncidade/EisenPo_cpp.git
cd EisenPo_cpp

# Configure and build the main application (MSVC: build/build_EisenPo_msvc | MinGW: build/build_EisenPo_mingw)
cmake --preset host-qt6-msvc-release
cmake --build --preset host-qt6-msvc-release

cmake --preset host-qt6-mingw-release
cmake --build --preset host-qt6-mingw-release

# Configure and build auxiliary tools in mocks/ (CompileTranslationsGUI, CompileTranslationsTerminal, find_VersionEditor)
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release
```

*(See [`OBSERVACAO.md`](OBSERVACAO.md#enus) and [`CMakePresets.json`](CMakePresets.json) for PGO presets, `.ts -> .qm` compilation commands, `find_VersionEditor --check` auditing, and Inno Setup packaging without `vc_redist.x64.exe`.)*

---

## 4. Source Layout & Architectural Guidelines

1. **Integrated Native Modular Layout (`source/`):**
   - `main.cpp` & `source/app/`: Qt 6 entry point, main host (`EisenhowerMatrixApp`), module tabs, and host adapters;
   - `source/models/`: Shared Qt list/data models (`TaskRecordListModel`);
   - `source/modules/`: All four productivity modules compiled natively into `EisenPo.exe` — `pomodoro/`, `feynman/`, `agenda/`, and `gantt/` (the Gantt module follows 4-layer Clean Architecture across `domain/`, `application/`, `infrastructure/`, and `ui/`);
   - `source/services/`: Core organizer business logic, productivity suite (`RecursosProdutividade/`), file actions (`Arquivo/`), context menu (`MostrarMenuContexto/`), task editor, and built-in manual texts;
   - `source/ui/`: Dialogs, calendars (`CalendarDialog/`, `YearCalendarWidget`), PoCa sidebar, and the `About` window (`OpcoesSobre`);
   - `source/utils/`: Unified atomic store (`UnifiedTaskStore`), persistent paths (`CaminhoPersistenteUtils`), logging (`LogManager`), fonts, icons, and module registry;
   - `source/language/`: Runtime language switching and unified translation catalog `translations/*.ts`;
   - `source/assets/`: Institutional compliance docs and licenses (`eisenhower/` and `LICENSES/`), which developers are freely authorized to adapt in their own forks;
   - `mocks/`: Standalone developer utilities (`CompileTranslationsGUI`, `CompileTranslationsTerminal`, `find_VersionEditor`).
2. **Memory Safety & RAII:** Prefer smart pointers (`std::unique_ptr`, `std::shared_ptr`), stack allocation, and Qt `QObject` parent-child ownership rather than raw owning pointers.
3. **100% Local-First Sovereignty:** All contributions must preserve strictly offline local execution inside `%LOCALAPPDATA%\EisenPo`, with zero telemetry, zero mandatory user accounts, and zero unsolicited network connections.
4. **Dynamic Bilingualism (`pt_BR` and `en_US`):** User-facing strings must support live runtime switching (`QEvent::LanguageChange` / `.ts` catalogs).

---

## 5. Pull Request Submission Process

1. Create a branch from `main` (`git checkout -b feat/my-improvement`).
2. Implement your changes while preserving offline isolation and `pt_BR` / `en_US` bilingual support.
3. If modifying `.ts` catalogs, validate compilation with `CompileTranslationsTerminal.exe`; if updating versioned docs in `source/assets/` or root files, run `find_VersionEditor.exe --check`.
4. Use concise, descriptive commit messages (e.g., `fix(gantt): fix total float calculation across movable holidays`).
5. Submit the Pull Request using the official template (`.github/PULL_REQUEST_TEMPLATE.md`).

</details>
