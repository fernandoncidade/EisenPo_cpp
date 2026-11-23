# Modelo de Segurança e Resiliência / Security Model — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher o modelo de segurança em português</summary><br>

# EisenPo — Modelo de Segurança, Integridade de Dados e Resiliência

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

---

## 1. Princípios de Segurança e Garantias Arquiteturais

O **EisenPo** é projetado para execução autônoma em estações de trabalho Windows, ambientes acadêmicos e fluxos de produtividade individuais com cinco garantias estruturais:

1. **Soberania Local e Operação Offline em Tempo de Execução:** Todo o processamento da Matriz de Eisenhower, Pomodoro, Feynman, Agenda e Gantt & BIM 4D ocorre localmente na máquina do usuário sem envio de telemetria, criação de contas em nuvem ou solicitações HTTP externas. *(Observação de auditoria: existem arquivos fonte não instanciados em [`source/modules/gantt/Gantt/api/`](../source/modules/gantt/Gantt/api/HttpServer.cpp) que vinculam `Qt6::Network`, mas nenhuma instância de servidor ou socket de rede é iniciada pelo `EisenPo.exe` em execução.)*
2. **Persistência Atômica Segura (`QSaveFile`):** A gravação de tarefas (`UnifiedTaskStore`), preferências de layout (`layout.json`) e arquivos de configuração utiliza commits atômicos via `QSaveFile`, reduzindo drasticamente o risco de arquivos truncados em quedas de energia.
3. **Integridade Transacional em SQLite (`listas.db` e `eisenpo_gantt.db`):** Os bancos de dados locais da Agenda e do Gantt & BIM 4D utilizam SQLite embarcado via driver oficial `qsqlite.dll` do Qt 6.12.0 com migrações versionadas de esquema.
4. **Proteção Algorítmica contra Ciclos e Corrupção Lógica:** O motor de Caminho Crítico (`CpmEngine` no módulo Gantt) executa verificação preventiva de dependências circulares antes da ordenação topológica DAG, impedindo loops infinitos em redes malformadas.
5. **Empacotamento Binário Limpo e Isolado (MSVC x64 vs. MinGW-w64 x64):** Scripts CMake (`cmake/copy_msvc_runtime.cmake`, `cmake/copy_mingw_runtime.cmake`, `cmake/prune_optional_root_runtime_dlls.cmake`, `cmake/qt.conf.in`) e instaladores Inno Setup garantem carregamento local estrito do runtime correspondente (`Microsoft.VC145.CRT` ou `mingw1510_64`), eliminando colisões de DLLs cruzadas, drivers SQL não utilizados, plugins QML/SVG/PDF supérfluos e redistribuíveis externos redundantes (`vc_redist.x64.exe`).

---

## 2. Superfície de Ataque e Modelo de Ameaças

| Vetor Potencial | Nível de Risco | Medida de Mitigação Implementada |
|---|:---:|---|
| **Importação de Arquivos Malformados (`.json`, `.csv`, `.xlsx`, `.pdf`, `.ics`)** | Médio | Análise defensiva de documentos importados com tratamento de erros na camada de serviços (`arq_06_abrir_arquivo.cpp`, `CsvExchangeService`), preservando dados existentes antes de substituir o estado ativo. |
| **Vínculos Externos de Arquivos (Eisenhower e Feynman)** | Baixo | Apenas o caminho informado explicitamente pelo usuário é salvo; o programa nunca realiza varreduras automáticas em disco e só abre o arquivo mediante clique explícito do usuário via `QDesktopServices` / shell do Windows. |
| **Dependências Circulares em Cronogramas Gantt** | Baixo | Verificação preventiva de ciclos em grafo direcionado acíclico (DAG) antes do cálculo Forward/Backward Pass do `CpmEngine`. |
| **Código Dormante de API HTTP (`Gantt/api/`)** | Baixo (Informativo) | `HttpServer` e `RestApiController` não são instanciados nem ligados em `main.cpp`, `eisenhower_app.cpp` ou `gantt_plugin.cpp`, mantendo superfície de rede ativa **zero** em tempo de execução; seu isolamento ou remoção consta no roadmap ([`suggested-issues.md`](suggested-issues.md#ptbr)). |
| **Falhas em Bancos Locais SQLite e Preferências** | Baixo | Escrita atômica via `QSaveFile` em JSONs principais (`tarefas.json`, `layout.json`), permissões graváveis isoladas em `%LOCALAPPDATA%\EisenPo` e logs cronológicos locais sem envio externo (`LogManager`). |
| **Corrupção ou Exposição de Símbolos / Runtimes na Distribuição** | Baixo | Scripts de deploy limpo removem `*.pdb`, `vc_redist*.exe`, plugins não utilizados (`qopensslbackend`, drivers SQL externos, `iconengines`, `qml`) e impõem `qt.conf` com busca local restrita a `plugins/` e `translations/`. |

---

## 3. Recomendações de Segurança Operacional ao Usuário

- Realize backups periódicos do diretório `%LOCALAPPDATA%\EisenPo` ou exporte cópias em `.json` / `.xlsx` para projetos importantes.
- Ao importar planilhas `.xlsx`, `.csv`, `.pdf` ou cronogramas de fontes externas, confira os vínculos e conteúdos antes de incorporar ao cronograma principal.
- Utilize sempre compilações oficiais da [Microsoft Store (`9P289X0185C3`)](https://apps.microsoft.com/detail/9P289X0185C3), releases verificadas do repositório oficial ou suas próprias compilações a partir do código-fonte público.

---

## 4. Distinção Entre Segurança de Software e Confiabilidade de Planejamento ("AS IS")

As medidas descritas neste documento dizem respeito à segurança operacional da aplicação em execução local e **não representam certificação para uso definitivo em cronogramas contratuais, obras civis ou obrigações regulatórias críticas**. O **EisenPo** é fornecido como aplicativo gratuito auxiliar **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sujeito a limitações e eventuais falhas humanas nos algoritmos de agendamento, cálculo de caminho crítico (CPM), calendários e sincronização, devendo resultados críticos ser conferidos de forma independente ([`README.md`](../README.md#ptbr) e [EULA Oficial](../source/assets/eisenhower/EULA/EULA_pt_BR.txt)).

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse security model in English</summary><br>

# EisenPo — Security Model, Data Integrity & Resilience

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Author:** Fernando Nillsson Cidade  

---

## 1. Security Principles & Architectural Guarantees

**EisenPo** is built for autonomous execution on Windows workstations, academic environments, and personal productivity workflows around five core structural guarantees:

1. **Local Sovereignty & Offline Runtime:** All processing across the Eisenhower Matrix, Pomodoro, Feynman, Agenda, and Gantt & 4D BIM executes strictly on the local workstation with zero telemetry, zero cloud accounts, and zero outbound HTTP traffic. *(Audit note: dormant uninstantiated source files in [`source/modules/gantt/Gantt/api/`](../source/modules/gantt/Gantt/api/HttpServer.cpp) link `Qt6::Network`, but `EisenPo.exe` never instantiates the server or opens listening sockets at runtime.)*
2. **Atomic Persistence (`QSaveFile`):** Task storage (`UnifiedTaskStore`), layout state (`layout.json`), and configuration writers commit via atomic `QSaveFile` replacements, preventing partial writes or truncated files on crash or power loss.
3. **Embedded Transactional SQLite (`listas.db` & `eisenpo_gantt.db`):** Agenda and Gantt & 4D BIM relational stores use embedded local SQLite via Qt 6.12.0's `qsqlite.dll` with schema versioning.
4. **DAG Cycle & Logic Guards:** The Gantt Critical Path Method solver (`CpmEngine`) checks predecessor graphs for circular dependencies prior to topological traversal, preventing infinite loops on malformed schedules.
5. **Clean Isolated Binary Packaging (MSVC x64 vs. MinGW-w64 x64):** CMake deployment scripts (`cmake/copy_msvc_runtime.cmake`, `cmake/copy_mingw_runtime.cmake`, `cmake/prune_optional_root_runtime_dlls.cmake`, `cmake/qt.conf.in`) and Inno Setup installers enforce strict isolation between `Microsoft.VC145.CRT` and `mingw1510_64` DLLs, pruning debug `.pdb` files, unused SQL/image/QML/TLS plugins, and redundant `vc_redist.x64.exe` installers.

---

## 2. Attack Surface & Threat Model

| Threat Vector | Risk Level | Implemented Mitigation |
|---|:---:|---|
| **Malformed Import Files (`.json`, `.csv`, `.xlsx`, `.pdf`, `.ics`)** | Medium | Defensive parsing with error reporting in import services (`arq_06_abrir_arquivo.cpp`, `CsvExchangeService`), keeping active state intact if validation fails. |
| **External File Attachments (Eisenhower & Feynman)** | Low | Only explicitly chosen local paths are stored; the application performs zero filesystem crawling and opens attachments solely upon explicit user click via standard OS handlers. |
| **Circular Schedule Dependencies** | Low | DAG cycle detection intercepts circular links prior to `CpmEngine` forward/backward scheduling passes. |
| **Dormant HTTP API Sources (`Gantt/api/`)** | Low (Informational) | Neither `HttpServer` nor `RestApiController` is instantiated or started in `main.cpp`, `eisenhower_app.cpp`, or `gantt_plugin.cpp`, resulting in **zero active network surface** at runtime; explicit toggling or removal is tracked in [`suggested-issues.md`](suggested-issues.md#enus). |
| **Local SQLite & Preference State Corruption** | Low | Atomic `QSaveFile` replacement for primary JSON state, isolated `%LOCALAPPDATA%\EisenPo` storage, and local chronological logs (`LogManager`) with zero remote reporting. |
| **Deployment Symbol / Cross-Toolchain Leakage** | Low | Deployment scripts prune `*.pdb`, `vc_redist*.exe`, unused Qt modules/plugins (`qopensslbackend`, unused SQL drivers, `qml`), and lock Qt lookup to local `qt.conf` (`plugins/`, `translations/`). |

---

## 3. Operational Security Practices for Users

- Back up `%LOCALAPPDATA%\EisenPo` periodically or export `.json` / `.xlsx` archives for critical plans.
- Inspect third-party `.xlsx`, `.csv`, `.pdf`, or `.ics` schedules prior to importing them into active projects.
- Install only official [Microsoft Store (`9P289X0185C3`)](https://apps.microsoft.com/detail/9P289X0185C3) packages, verified repository installers, or binaries built directly from source.

---

## 4. Software Security vs. Planning Reliability ("AS IS")

The controls documented here address local application execution security and **do not certify definitive suitability for legally binding contractual schedules, engineering works, or regulated compliance commitments**. **EisenPo** is supplied as a free auxiliary tool **"AS IS"**, subject to human limitations and possible defects in scheduling logic, Critical Path calculations, calendars, or cross-module sync; critical plans must always be verified independently ([`README.md`](../README.md#enus) and [Official EULA](../source/assets/eisenhower/EULA/EULA_en_US.txt)).

</details>
