# Auditoria Técnica, Decisões Arquiteturais e Roadmap / Technical Audit, ADRs & Roadmap — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher a auditoria técnica em português</summary><br>

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

Este documento consolida a auditoria técnica factual do **EisenPo**, suas Decisões Arquiteturais (ADRs), verificação de build e empacotamento, limitações conhecidas e roadmap técnico evolutivo.

---

## A. Inventário Funcional Verificado no Código-Fonte

| Módulo / Subsistema | Localização (`source/`, `mocks/`) | Estado Atual Verificado | Observações Técnicas |
|---|---|---|---|
| **Host Matriz de Eisenhower & Calendário** | `source/app/`, `source/services/`, `source/ui/`, `source/models/` | Implementado e Integrado | 4 quadrantes, 4 layouts + PoCa ($2 \times K$), editor responsivo `EditTaskDialog`, import/export `JSON`/`CSV`/`XLSX`/`PDF`/`ICS`, manual embutido `F1` |
| **Persistência Unificada & Diagnóstico** | `source/utils/{UnifiedTaskStore,LogManager,CaminhoPersistenteUtils}.cpp` | Implementado e Integrado | Gravação atômica `QSaveFile` em `%LOCALAPPDATA%\EisenPo\tarefas.json`, migração legada de `%LOCALAPPDATA%\EisenhowerOrganizer`, logs em `logs/file_eisenpo_*.log` |
| **Módulo Pomodoro** | `source/modules/pomodoro/` | Integrado Nativamente em `EisenPo.exe` | Temporizador com metas individuais (`1`–`999`), diálogo preventivo de metas, alarmes `.wav` (`Qt6::Multimedia`) com carregamento sob demanda |
| **Módulo Feynman** | `source/modules/feynman/` | Integrado Nativamente em `EisenPo.exe` | 5 etapas configuráveis (`feynman_rotulos_config.json`), vínculos locais e **compatibilidade C++23 validada** (`concept_text` / `conceito`) |
| **Módulo Agenda (Registro de Atividades)** | `source/modules/agenda/` | Integrado Nativamente em `EisenPo.exe` | Persistência SQLite (`listas.db`), agendas aninhadas (`agendas/*.json`), modos de cor e 12 docs de conformidade no `Sobre` |
| **Módulo Gantt & BIM 4D** | `source/modules/gantt/` | Integrado Nativamente em `EisenPo.exe` | Clean Architecture 4 camadas, `CpmEngine` DAG, `WorkCalendar`, feriados de 18 países (Meeus/Jones/Butcher), SQLite `eisenpo_gantt.db`, 5 layouts, tooltips dinâmicos; subdiretório `api/` permanece dormante |
| **Catálogo & Compilação I18N** | `source/language/`, `mocks/CompileTranslations*.cpp` | Implementado e Integrado | Bilinguismo dinâmico completo `pt_BR` / `en_US` com resposta a `QEvent::LanguageChange` |
| **Auditoria de Versões & Documentos** | `mocks/find_VersionEditor.cpp` | Implementado e Integrado | Valida e sincroniza **88 especificações** de versão/data entre raiz, Inno Setup, manifestos e `source/assets/` |

---

## B. Fluxo Arquitetural End-to-End

```text
[Interface Host Qt 6.12.0 (EisenhowerMatrixApp / Layouts / PoCa)]
        │
        ├──> [UnifiedTaskStore] ──> Escrita atômica (QSaveFile) -> tarefas.json
        │            │
        │            ├──> [Pomodoro] (ciclos_totais, progresso discreto, Qt6::Multimedia)
        │            ├──> [Feynman]  (etapas, rótulos, arquivos de apoio locais)
        │            ├──> [Agenda]   (listas.db SQLite + agendas/*.json)
        │            └──> [Gantt & BIM 4D] (GanttTaskIntegrationBridge O(1))
        │                       ├──> CpmEngine (DAG, ES/EF/LS/LF, TF/FF)
        │                       ├──> WorkCalendar + Feriados 18 Países (Meeus)
        │                       └──> eisenpo_gantt.db (SQLite) + Bim4DSimulationPanel
        │
        └──> [Utilitários Mocks] -> CompileTranslations{GUI,Terminal} (.ts -> .qm)
                                 -> find_VersionEditor (--check / GUI: 88 specs)
```

---

## C. Auditoria de Build C++23, Toolchain e Empacotamento Externo

| Frente Auditada | Estado Verificado | Achado / Evidência Real | Ação Recomendada |
|---|---|---|---|
| **CMake / Presets Dual Toolchain (`MSVC` + `MinGW`)** | **Concluído e Verificado (Qt 6.12.0)** | Todos os 4 presets ativos (`host-qt6-{msvc,mingw}-release` e `mocks-{msvc,mingw}-release`) configuraram e compilaram limpos (`código 0`) com MSVC puro (`Visual Studio 18 2026` / MSBuild `18.12.3`, `MSVC 14.52.37025` `v145`, SDK `10.0.28000.0`) e `MinGW-w64 GCC 15.1.0` + `Ninja 1.13.2` (`CMAKE_CXX_SCAN_FOR_MODULES OFF` + `generated/qt.conf`) | Baseline oficial validado |
| **Compatibilidade C++23 (`CMAKE_CXX_STANDARD 23`)** | **Concluído e Verificado** | Identificador `concept` renomeado para `DescriptionParts::concept_text` e `conceito` nos 5 arquivos de `source/modules/feynman/Feynman/` (preservando intactas as chaves JSON `"concept"`). Zero erros C++23 em todo o projeto e módulos | Compilação limpa validada nas 2 toolchains |
| **Limpeza das Distribuições Externas (`D:\MISCELANEAS\VSCode\EisenPo\`)** | **Concluído e Auditado** | `EisenPo_cpp_msvc`: `176` arquivos / `114,97 MB` (`120.558.651 bytes`, exatas 5 DLLs CRT MSVC, 0 DLL MinGW); `EisenPo_cpp_mingw`: `174` arquivos / `129,48 MB` (`135.765.835 bytes`, exatas 3 DLLs MinGW, 0 DLL MSVC); `mocks_msvc`: `18` / `30,87 MB` (`32.373.319 bytes`); `mocks_mingw`: `16` / `37,45 MB` (`39.266.263 bytes`); `Qt6Core.dll == 6.12.0.0`, 0 `*.pdb`, 0 banners, 0 `vc_redist` | Paridade e isolamento 100% confirmados |
| **Tamanho de Instaladores Inno Setup (`MSVC` vs `MinGW`)** | **Concluído e Medido** | `EisenPo_msvc_v2026.11.22.0.exe` (**`49,34 MB`** — `51.739.686 bytes`) e `EisenPo_mingw_v2026.11.22.0.exe` (**`53,70 MB`** — `56.304.557 bytes`); remoção total de `vc_redist.x64.exe` (~18 MB) mantendo MSVC menor que MinGW | Compilados via `ISCC.exe` com êxito |
| **Suíte de Testes Automatizados (`CTest`)** | Pendente | Ainda não existe diretório `tests/` nem alvos CTest no repositório | Introduzir suíte CTest unitária (Issue #2) |

---

## D. Registro de Decisões Arquiteturais (ADRs)

- **ADR-001 — Unificação Nativa dos 4 Módulos em `EisenPo.exe`:** Eliminação de `QPluginLoader` e DLLs separadas em `plugins/` em favor da compilação direta sob `source/modules/`, zerando falhas de carregamento, latência de abertura e falsos positivos em antivírus.
- **ADR-002 — Persistência Unificada Atômica (`UnifiedTaskStore`):** Uso de `QSaveFile` e armazenamento local único em `%LOCALAPPDATA%\EisenPo\` com migração automática de dados legados (`EisenhowerOrganizer`).
- **ADR-003 — Clean Architecture + SQLite no Gantt & BIM 4D:** Desacoplamento entre `domain/` (`CpmEngine`, `WorkCalendar`), `application/services/` (`GanttTaskIntegrationBridge` `O(1)`), `infrastructure/` (`eisenpo_gantt.db`) e `ui/` (`GanttMainWindow`).
- **ADR-004 — Dual Toolchain Qt 6.12.0 + Runtime App-Local sem `vc_redist.x64.exe`:** Preservação de paridade total entre **MSVC 14.52** e **MinGW 15.1.0**, com isolamento estrito de DLLs e redução de ~18 MB no instalador Inno Setup MSVC (`49,33 MB`).
- **ADR-005 — Duplo Licenciamento Transparente (`GNU AGPLv3` + EULA Oficial Store):** Código-fonte público sob copyleft forte (`GNU AGPLv3`), liberdade explícita aos desenvolvedores sobre os documentos em `source/assets/`, e proteção de marca/distribuição oficial gratuita na Microsoft Store sob EULA próprio.

---

## E. Limitações Conhecidas e Escopo Experimental

1. **Compatibilidade C++23 em Feynman (Resolvida nesta revisão):** A colisão histórica do identificador `concept` nos 5 arquivos de `source/modules/feynman/Feynman/` foi eliminada (`concept_text` / `conceito`), desbloqueando a compilação estrita `/std:c++23` e `-std=c++23`.
2. **Código HTTP Dormante em `source/modules/gantt/Gantt/api/`:** `HttpServer` e `RestApiController` permanecem compilados e inativos (nunca instanciados pelo host), podendo ser removidos ou tornados explicitamente configuráveis em revisão futura.
3. **Ausência de Suíte CTest Automatizada:** A verificação matemática de `CpmEngine` e `WorkCalendar` depende hoje de validação manual/funcional na interface até a introdução de testes automatizados.
4. **Mecanismo Local `TrialManager` em Builds de Avaliação:** `source/utils/TrialManager.cpp` controla localmente o período de avaliação quando compilado sem `LIBERAR_USO_DEFINITIVO = true`, operando estritamente offline (`trial_info.json` + `HKCU\SOFTWARE\EisenPo`).
5. **Escopo Auxiliar e Isenção ("AS IS"):** O aplicativo destina-se a auxílio pessoal, acadêmico e de organização/planejamento, não substituindo software certificado nem supervisão profissional legalmente habilitada em cronogramas contratuais críticos ([EULA Oficial](../source/assets/eisenhower/EULA/EULA_pt_BR.txt)).

---

## F. Roadmap Técnico Consolidado

| Prioridade | Frente Técnica | Entregas Planejadas | Critério de Aceitação |
|---|---|---|---|
| **P0 (Concluído & Verificado)** | Migração Qt 6.12.0, Dual Licensing & C++23 | Presets MSVC 14.52 / MinGW 15.1.0, refatoração `concept` em `feynman`, correção `generated/qt.conf`, distribuição limpa (`114,81 MB` MSVC / `129,46 MB` MinGW) e instaladores (`49,33 MB` / `53,70 MB`) sem `vc_redist.x64.exe` | Builds limpos (`código 0`) dos 6 presets + `ISCC.exe` + auditoria binária concluídos |
| **P1** | Cobertura Automatizada CTest | Suíte de testes unitários para `CpmEngine`, `WorkCalendar`, algoritmo de Meeus, `UnifiedTaskStore` e importadores CSV/JSON | `ctest --output-on-failure` verde em MSVC e MinGW |
| **P2** | Limpeza da Superfície `Qt6::Network` | Remover ou isolar atrás de opção CMake explicitamente desligada os arquivos dormantes em `Gantt/api/` | Redução opcional da dependência `Qt6Network.dll` na distribuição |
| **P3** | Interoperabilidade BIM/Planejamento | Importação/exportação MS Project XML e leitura geométrica real de entidades IFC4 para a Simulação 4D | Round-trip de cronogramas e vínculo direto com GUIDs IFC de modelos reais |
| **P4** | Acessibilidade & Temas TDAH/Dislexia | Perfis visuais de baixo estímulo, atalhos configuráveis e contraste aprimorado para alto DPI | Validação contínua nos temas Claro e Escuro do Windows 11 |

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse technical audit in English</summary><br>

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Author:** Fernando Nillsson Cidade  

This document summarizes the factual technical audit of **EisenPo**, its Architectural Decision Records (ADRs), build and packaging verification findings, known limitations, and evolution roadmap.

---

## A. Verified Functional Inventory

| Module / Subsystem | Location (`source/`, `mocks/`) | Current Verified Status | Technical Notes |
|---|---|---|---|
| **Eisenhower Matrix Host & Calendars** | `source/app/`, `source/services/`, `source/ui/`, `source/models/` | Implemented & Integrated | 4 quadrants, 4 layouts + PoCa ($2 \times K$), responsive `EditTaskDialog`, `JSON`/`CSV`/`XLSX`/`PDF`/`ICS` import/export, built-in `F1` manual |
| **Unified Persistence & Diagnostics** | `source/utils/{UnifiedTaskStore,LogManager,CaminhoPersistenteUtils}.cpp` | Implemented & Integrated | Atomic `QSaveFile` commits to `%LOCALAPPDATA%\EisenPo\tarefas.json`, legacy migration from `%LOCALAPPDATA%\EisenhowerOrganizer`, logs in `logs/file_eisenpo_*.log` |
| **Pomodoro Module** | `source/modules/pomodoro/` | Natively Built into `EisenPo.exe` | Timers with individual cycle targets (`1`–`999`), Cycle Target Warning dialog, lazy-loaded `.wav` audio alarms (`Qt6::Multimedia`) |
| **Feynman Module** | `source/modules/feynman/` | Natively Built into `EisenPo.exe` | 5 customizable steps (`feynman_rotulos_config.json`), linked local files, and **verified C++23 compatibility** (`concept_text` / `conceito`) |
| **Agenda (Activity Registration)** | `source/modules/agenda/` | Natively Built into `EisenPo.exe` | Local SQLite DB (`listas.db`), nested custom schedules (`agendas/*.json`), independent color themes, 12 compliance docs in `About` |
| **Gantt & 4D BIM Module** | `source/modules/gantt/` | Natively Built into `EisenPo.exe` | 4-layer Clean Architecture, DAG `CpmEngine`, `WorkCalendar`, 18-country Holiday Manager (Meeus/Jones/Butcher), SQLite `eisenpo_gantt.db`, 5 layouts, live tooltips; `api/` remains dormant |
| **I18N Catalogs & Compilers** | `source/language/`, `mocks/CompileTranslations*.cpp` | Implemented & Integrated | Full dynamic bilingualism `pt_BR` / `en_US` reacting to `QEvent::LanguageChange` |
| **Version & Docs Auditor** | `mocks/find_VersionEditor.cpp` | Implemented & Integrated | Audits and updates **88 version/date specs** across root docs, Inno Setup scripts, manifests, and `source/assets/` |

---

## B. End-to-End Architecture Flow

```text
[Host Presentation Qt 6.12.0 (EisenhowerMatrixApp / Layouts / PoCa)]
        │
        ├──> [UnifiedTaskStore] ──> Atomic QSaveFile commits -> tarefas.json
        │            │
        │            ├──> [Pomodoro] (ciclos_totais, discrete progress, Qt6::Multimedia)
        │            ├──> [Feynman]  (steps, custom labels, local linked files)
        │            ├──> [Agenda]   (listas.db SQLite + agendas/*.json)
        │            └──> [Gantt & 4D BIM] (GanttTaskIntegrationBridge O(1))
        │                       ├──> CpmEngine (DAG, ES/EF/LS/LF, TF/FF)
        │                       ├──> WorkCalendar + 18-Country Holidays (Meeus)
        │                       └──> eisenpo_gantt.db (SQLite) + Bim4DSimulationPanel
        │
        └──> [Mock Utilities] -> CompileTranslations{GUI,Terminal} (.ts -> .qm)
                              -> find_VersionEditor (--check / GUI: 88 specs)
```

---

## C. C++23 Build, Toolchain & External Deployment Audit

| Audited Area | Verified State | Real Finding / Evidence | Recommended Action |
|---|---|---|---|
| **Dual Toolchain CMake/Presets (`MSVC` + `MinGW`)** | **Completed & Verified (Qt 6.12.0)** | All 4 active presets (`host-qt6-{msvc,mingw}-release` and `mocks-{msvc,mingw}-release`) configured and built cleanly (`exit code 0`) with pure MSVC (`Visual Studio 18 2026` / MSBuild `18.12.3`, `MSVC 14.52.37025` `v145`, SDK `10.0.28000.0`) and `MinGW-w64 GCC 15.1.0` + `Ninja 1.13.2` (`CMAKE_CXX_SCAN_FOR_MODULES OFF` + `generated/qt.conf`) | Official baseline verified |
| **C++23 Compatibility (`CMAKE_CXX_STANDARD 23`)** | **Completed & Verified** | Legacy `concept` identifier renamed to `DescriptionParts::concept_text` and `conceito` across the 5 files in `source/modules/feynman/Feynman/` (keeping `"concept"` JSON keys unchanged); zero C++23 build errors project-wide | Clean builds verified across both toolchains |
| **Clean External Distributions (`D:\MISCELANEAS\VSCode\EisenPo\`)** | **Completed & Audited** | `EisenPo_cpp_msvc`: `176` files / `114.97 MB` (`120,558,651 bytes`, exact 5 MSVC CRT DLLs, 0 MinGW DLL); `EisenPo_cpp_mingw`: `174` files / `129.48 MB` (`135,765,835 bytes`, exact 3 MinGW DLLs, 0 MSVC DLL); `mocks_msvc`: `18` / `30.87 MB` (`32,373,319 bytes`); `mocks_mingw`: `16` / `37.45 MB` (`39,266,263 bytes`); `Qt6Core.dll == 6.12.0.0`, 0 `*.pdb`, 0 store banners, 0 `vc_redist` | Full isolation and parity confirmed |
| **Inno Setup Installer Balance (`MSVC` vs `MinGW`)** | **Completed & Measured** | `EisenPo_msvc_v2026.11.22.0.exe` (**`49.34 MB`** — `51,739,686 bytes`) and `EisenPo_mingw_v2026.11.22.0.exe` (**`53.70 MB`** — `56,304,557 bytes`); `vc_redist.x64.exe` (~18 MB) eliminated, making the MSVC installer smaller than MinGW | Compiled via `ISCC.exe` with exit code 0 |
| **Automated Test Suite (`CTest`)** | Pending | No `tests/` directory or CTest suite exists in the repository yet | Introduce CTest unit tests (Issue #2) |

---

## D. Architectural Decision Records (ADRs)

- **ADR-001 — Native In-Tree Unification into `EisenPo.exe`:** Replaced `QPluginLoader` and separate plugin DLLs under `plugins/` with direct static/object compilation under `source/modules/`, eliminating load failures, startup lag, and false-positive antivirus blocks.
- **ADR-002 — Unified Atomic Task Persistence (`UnifiedTaskStore`):** Adopted `QSaveFile` atomic writes in `%LOCALAPPDATA%\EisenPo\` with transparent migration from legacy `EisenhowerOrganizer` directories.
- **ADR-003 — Clean Architecture + SQLite for Gantt & 4D BIM:** Enforced strict decoupling across `domain/` (`CpmEngine`, `WorkCalendar`), `application/services/` (`GanttTaskIntegrationBridge` `O(1)`), `infrastructure/` (`eisenpo_gantt.db`), and `ui/` (`GanttMainWindow`).
- **ADR-004 — Qt 6.12.0 Dual Toolchain + App-Local Runtime without `vc_redist.x64.exe`:** Preserved full parity between **MSVC 14.52** and **MinGW 15.1.0**, isolating runtime DLLs and cutting ~18 MB from the MSVC Inno Setup installer (`49.33 MB`).
- **ADR-005 — Transparent Dual Licensing (`GNU AGPLv3` + Official Store EULA):** Strong-copyleft source availability (`GNU AGPLv3`), explicit developer freedom over compliance assets in `source/assets/`, and brand/package protection for the official free Microsoft Store release.

---

## E. Known Limitations & Experimental Scope

1. **C++23 Compatibility in Feynman (Resolved in This Revision):** The legacy `concept` identifier collision across the 5 files under `source/modules/feynman/Feynman/` has been resolved (`concept_text` / `conceito`), enabling clean `/std:c++23` and `-std=c++23` builds.
2. **Dormant HTTP API Sources (`source/modules/gantt/Gantt/api/`):** `HttpServer` and `RestApiController` remain compiled-in yet completely uninstantiated/idle at runtime.
3. **No Automated CTest Suite Yet:** Schedule math (`CpmEngine`, `WorkCalendar`) currently relies on UI/manual verification pending unit-test introduction.
4. **Local `TrialManager` Evaluation Hook:** When built without `LIBERAR_USO_DEFINITIVO = true`, `source/utils/TrialManager.cpp` checks local-only timestamps (`trial_info.json` + `HKCU\SOFTWARE\EisenPo`) completely offline.
5. **Auxiliary Tool Disclaimer ("AS IS"):** Built as a free auxiliary productivity, study, and planning aid; never replaces certified project controls or licensed professional review ([Official EULA](../source/assets/eisenhower/EULA/EULA_en_US.txt)).

---

## F. Consolidated Technical Roadmap

| Priority | Workstream | Planned Deliverables | Acceptance Criterion |
|---|---|---|---|
| **P0 (Completed & Verified)** | Qt 6.12.0 Migration, Dual Licensing & C++23 | MSVC 14.52 / MinGW 15.1.0 presets, `concept` refactoring in `feynman`, `generated/qt.conf` fix, clean deployments (`114.81 MB` MSVC / `129.46 MB` MinGW) and installers (`49.33 MB` / `53.70 MB`) without `vc_redist.x64.exe` | Clean builds (`exit code 0`) across all 6 presets + `ISCC.exe` + binary audit passed |
| **P1** | Automated CTest Coverage | Unit tests covering `CpmEngine`, `WorkCalendar`, Meeus Easter algorithm, `UnifiedTaskStore`, and CSV/JSON parsing | Green `ctest --output-on-failure` on both MSVC and MinGW |
| **P2** | `Qt6::Network` Surface Cleanup | Remove or gate dormant `Gantt/api/` sources behind an off-by-default CMake option | Optional removal of `Qt6Network.dll` from runtime deployment |
| **P3** | BIM / Project Interoperability | MS Project XML import/export and real IFC4 structural/element parsing for 4D Simulation | Real-world IFC GUID timeline round-trip |
| **P4** | ADHD/Dyslexia Accessibility Profiles | Low-stimulation focus themes, customizable shortcuts, and high-DPI ergonomics | Continuous refinement across Windows 11 Light/Dark themes |

</details>
