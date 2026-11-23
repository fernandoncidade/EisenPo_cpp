<!-- Multilanguage README.md for EisenPo -->

# EisenPo — Organizador de Tarefas, Estudos e Foco (Eisenhower, Pomodoro, Feynman, Agenda & Gantt BIM 4D)

**Ecossistema desktop nativo de produtividade, gestão estratégica de tarefas, aprendizado ativo e cronogramas de projetos para Windows, desenvolvido em C++ moderno (`CMAKE_CXX_STANDARD 23`), Qt 6.12.0 Widgets/Multimedia/Sql e SQLite embarcado. Reúne nativamente no executável principal a Matriz de Eisenhower (4 quadrantes, 4 modos de layout e coluna lateral integrada `PoCa` 2×K), Temporizador Pomodoro com metas individuais e alarmes `.wav`, Método de Estudo Ativo Feynman com rótulos customizáveis, Registro de Atividades (Agenda em SQLite) e Cronograma de Gantt (EAP/WBS, motor CPM DAG, Calendário Laboral, Feriados de 18 países e Simulação Construtivo-Temporal 4D BIM), operando 100% local e offline em `%LOCALAPPDATA%\EisenPo` (`pt-BR` / `en-US`).**

[![Latest Release](https://img.shields.io/badge/Release-v2026.11.22.0-blue.svg)](docs/release-v2026.11.22.0.md)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=c%2B%2B)](CMakeLists.txt)
[![Qt 6.12.0](https://img.shields.io/badge/Qt-6.12.0-41CD52?logo=qt)](CMakeLists.txt)
[![MSVC 14.52 | MinGW 15.1.0](https://img.shields.io/badge/Toolchain-MSVC_14.52_%7C_MinGW_15.1.0-0078D4)](CMakePresets.json)
[![Platform Windows](https://img.shields.io/badge/Platform-Windows_11_%2F_10-0078D4?logo=windows)](https://apps.microsoft.com/detail/9P289X0185C3)
[![Microsoft Store](https://img.shields.io/badge/Microsoft_Store-9P289X0185C3-0078D4?logo=microsoft)](https://apps.microsoft.com/detail/9P289X0185C3)
[![License GNU AGPLv3](https://img.shields.io/badge/License-GNU_AGPLv3-blue.svg)](LICENSE)
[![Official Store EULA](https://img.shields.io/badge/License-Official_Store_EULA_%2B_CLC-purple.svg)](docs/licensing.md)
[![100% Offline](https://img.shields.io/badge/Security-100%25_Offline-success.svg)](docs/security-model.md)

<p align="center">
  <a href="docs/user-guide.md">📖 Guia do Usuário / User Guide</a> •
  <a href="docs/architecture.md">🏗️ Arquitetura / Architecture</a> •
  <a href="docs/compliance.md">📋 Conformidade / Compliance</a> •
  <a href="docs/security-model.md">🛡️ Privacidade &amp; Segurança</a> •
  <a href="docs/licensing.md">⚖️ Duplo Licenciamento / Dual Licensing</a> •
  <a href="https://apps.microsoft.com/detail/9P289X0185C3">🪟 Microsoft Store</a><br>
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> •
  <a href="#enus">🇺🇸 English (US)</a>
</p>

---

<p align="center">
  <img src="docs/images/banner.png" alt="EisenPo Banner" width="360">
</p>

---

## <a id="ptbr"></a><a id="português-br"></a>🇧🇷 Português (BR)

> **Observação:** Este repositório refere-se à versão oficial **v2026.11.22.0** do projeto **EisenPo** (implementação nativa em **C++ / Qt 6.12.0 / SQLite**), disponibilizado sob arquitetura transparente de **Duplo Licenciamento** (**GNU AGPLv3** para o código-fonte público no GitHub e **EULA Oficial + CLC** para o pacote assinado distribuído na Microsoft Store, com execução 100% local e offline). Adquira ou instale a edição oficial pelo link: [Instalar via Microsoft Store](https://apps.microsoft.com/detail/9P289X0185C3).

<details>
<summary><b>Clique para expandir / recolher a documentação em Português (BR)</b></summary><br>

# EisenPo — Organizador de Tarefas, Estudos e Foco

Versão: v2026.11.22.0<br>
Data técnica desta revisão: 22 de novembro de 2026<br>
Autor: Fernando Nillsson Cidade<br>

O **EisenPo** (Versão `2026.11.22.0`) é uma aplicação desktop nativa para Windows desenvolvida em **C++ e Qt 6.12.0** para organização estratégica de tarefas, foco cronometrado, estudo ativo, registro avaliativo e planejamento de cronogramas com Caminho Crítico (CPM) e Simulação 4D BIM.

Concebido originalmente para mitigar ruído cognitivo em rotinas afetadas por TDAH e dislexia, o aplicativo reúne cinco pilares integrados em um único processo nativo, com soberania integral de dados locais (`%LOCALAPPDATA%\EisenPo`):

```text
EisenPo.exe (Qt 6.12.0 + Módulos Nativos Integrados)
 ├── Matriz de Eisenhower (Q1–Q4, Abas/Quadrantes/2×2, PoCa 2×K, JSON/CSV/XLSX/PDF/ICS)
 ├── Módulo Pomodoro (Ciclos 1–999 por tarefa, Alerta de Metas, Áudio .wav lazy-loaded)
 ├── Módulo Feynman (5 Etapas Configuráveis, Rótulos Customizáveis, Arquivos Vinculados)
 ├── Módulo Agenda (Registro de Atividades, Cursos/Notas, Listas Aninhadas, SQLite listas.db)
 └── Módulo Gantt & BIM 4D (EAP/WBS, Motor CPM DAG, WorkCalendar, Feriados 18 Países, SQLite eisenpo_gantt.db)
```

---

### ⚖️ Manifesto e Autorização Legal do Titular dos Direitos Autorais

> **Declaração Expressa de Fernando Nillsson Cidade:**  
> Eu, **Fernando Nillsson Cidade**, na qualidade de único autor, criador original e titular exclusivo dos direitos autorais do projeto **EisenPo**, **autorizo o uso, estudo, cópia, modificação, compilação e distribuição do código-fonte público deste repositório** nos exatos termos do arquivo **[LICENSE](LICENSE)** (**GNU Affero General Public License v3 — GNU AGPLv3**, identificador SPDX `AGPL-3.0-only`), ressalvada a proteção nominal de nome e marca sob a Seção 7(e) (`"EisenPo"`).  
> Qualquer versão modificada, extensão ou obra derivada do código-fonte público deve preservar a atribuição da autoria original e disponibilizar o código-fonte correspondente sob a mesma licença **GNU AGPLv3**.  
> **Liberdade para Documentação Institucional em `source/assets/`:** Os textos de conformidade contidos em [`source/assets/`](source/assets/eisenhower/COPYRIGHT/AVISO%20DE%20COPYRIGHT%20E%20MARCA%20REGISTRA_pt_BR.txt) (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `NOTICES`, `PRIVACY_POLICY` e `RELEASE`) foram elaborados para viabilizar a distribuição oficial na Microsoft Store; o autor **autoriza expressamente** que desenvolvedores e forks alterem, excluam, adicionem ou adaptem livremente tais documentos em suas próprias distribuições, observada a legislação de suas jurisdições e diretrizes de cada plataforma.  
> **Limite do Binário Oficial Assinado (Microsoft Store e Instaladores Oficiais):** A edição empacotada e disponibilizada oficialmente pelo autor é regida pelo respectivo **[EULA Oficial](source/assets/eisenhower/EULA/EULA_pt_BR.txt)** (em harmonia com os Termos de Licença Padrão para Aplicativos da Microsoft Store e legislação consumerista) e pelo **[Esclarecimento de Licenciamento Comercial — CLC](source/assets/eisenhower/CLC/CLC_pt_BR.txt)**. É vedada a revenda comercial paga não autorizada, a apropriação indevida da marca ou a republicação ilícita por terceiros do pacote binário oficial assinado pelo autor, preservados os direitos concedidos pelas licenças open source de terceiros (`LGPLv3` / `LGPLv2.1+`).  
> **Canal de Suporte, Independência da Microsoft e Isenção Legal ("AS IS"):** A **Microsoft NÃO presta suporte ao cliente nem cobre este software sob a Política de Privacidade da Microsoft**. Todo eventual suporte técnico voluntário do autor ocorre **exclusivamente por e-mail (`linceu_lighthouse@outlook.com`) e pelas abas Issues/Discussions do GitHub**, conforme disponibilidade e sem garantia de urgência ou SLA (salvo disposição contratual escrita distinta). O software destina-se ao uso como **ferramenta auxiliar** e é fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sem garantias além das imperativas por lei; decisões contratuais ou cronogramas críticos de engenharia devem sempre ser conferidos de forma independente com ferramentas consolidadas e supervisão profissional habilitada. Consulte [`docs/licensing.md`](docs/licensing.md#ptbr).

---

### 📌 Ficha Técnica do Projeto

<p align="center">
  <img src="docs/images/logo.png" alt="Ícone EisenPo" width="128">
</p>

- **Nome Oficial:** EisenPo — Organizador de Tarefas, Estudos e Foco (`EisenPo.exe`)
- **Autor e Mantenedor:** Fernando Nillsson Cidade *(Desenvolvedor Individual / Pessoa Física)*
- **E-mail Oficial de Suporte (best-effort, sem SLA comercial):** `linceu_lighthouse@outlook.com`
- **Repositório / Site Oficial:** [https://github.com/fernandoncidade/EisenPo_cpp](https://github.com/fernandoncidade/EisenPo_cpp)
- **Download Oficial (Microsoft Store):** [Instalar via Microsoft Store (`9P289X0185C3`)](https://apps.microsoft.com/detail/9P289X0185C3)
- **Identidade MSIX (`library/AppxManifest.xml`):** `fernandoncidade.EisenPo` · Windows 10 (`17763+`) / Windows 11 x64 (`runFullTrust`)
- **Idiomas Suportados em Tempo de Execução:** Português (Brasil — `pt_BR`) · English (United States — `en_US`)
- **Toolchains Suportadas (`CMakePresets.json`):** **MSVC x64** (`14.52.37025` / Windows SDK `10.0.28000.0`) e **MinGW-w64 x64** (`GCC 15.1.0`) com **Qt 6.12.0**, **CMake 3.28+**, **MSBuild (`Visual Studio 18 2026`)** e **Ninja 1.13.2**
- **Utilitários Auxiliares (`mocks/`):** `CompileTranslationsTerminal.exe`, `CompileTranslationsGUI.exe` e `find_VersionEditor.exe`

---

### 🌟 Principais Recursos e Módulos Integrados Nativamente

| Módulo | Integração | Capacidade Principal |
|---|---|---|
| **EisenPo (Eisenhower)** | Host Integrado | 4 quadrantes estratégicos, sub-listas Pendente/Em Progresso/Concluída, 4 layouts + painel `PoCa` ($2 \times K$), painel "Nova Tarefa" em 3 linhas com Duração, `EditTaskDialog` responsivo e exportação/importação JSON, CSV, XLSX, PDF e ICS. |
| **Pomodoro** | Nativo em `EisenPo.exe` | Temporizador com ciclos individuais (1–999 por tarefa), diálogo preventivo `⚠️ Aviso de Metas de Ciclos`, carregamento preguiçoso (*lazy*) e alarmes `.wav` calibrados (`Qt6::Multimedia`). |
| **Feynman** | Nativo em `EisenPo.exe` | Fluxo estruturado em 5 etapas com rótulos personalizáveis (`feynman_rotulos_config.json`), vínculos de arquivos de estudo e sincronia com `UnifiedTaskStore`. |
| **Agenda (Registro de Atividades)** | Nativo em `EisenPo.exe` | Cadastro de atividades avaliativas (curso, ementa, semestre, disciplina, notas), agendas hierárquicas (`agendas/*.json`), modos de cor independentes e persistência em SQLite (`listas.db`). |
| **Gantt & BIM 4D** | Nativo em `EisenPo.exe` | Clean Architecture em 4 camadas, EAP/WBS, motor analítico **CPM** (DAG + prevenção de ciclos), progresso físico discreto via Pomodoro, 5 layouts persistentes (`layout.json`), **Calendário Laboral + Feriados de 18 países (algoritmo de Meeus/Jones/Butcher)**, Baselines/Linha Fantasma, tooltips dinâmicos retraduzíveis, cores unificadas e Simulação Construtivo-Temporal 4D (`IFC GUID`, `eisenpo_gantt.db`, CSV). |

---

### 📸 Capturas de Tela da Interface (`pt-BR`)

Capturas oficiais da interface em **Português (Brasil)** armazenadas em [`images/interfaces/pt_BR/`](images/interfaces/pt_BR):

<p align="center">
  <img src="images/interfaces/pt_BR/Captura%20de%20tela%202026-08-15%20005502.png" alt="EisenPo Português Visão Principal" width="880"><br><br>
  <img src="images/interfaces/pt_BR/Captura%20de%20tela%202026-08-15%20005633.png" alt="EisenPo Português Módulos Integrados" width="880">
</p>

---

### 🛡️ Distribuição Microsoft Store, Privacidade e Desinstalação Limpa

> [!IMPORTANT]
> **Conformidade Microsoft Store (Políticas v7.20 / MSIX):**
> - **Privacidade Independente (Política 10.5):** **A Microsoft NÃO fornece uma política de privacidade padrão para o EisenPo, e este software NÃO é coberto pela [Política de Privacidade da Microsoft](https://www.microsoft.com/pt-br/privacy/privacystatement).** Todo processamento de tarefas, estudos, bancos SQLite e cronogramas ocorre localmente em `%LOCALAPPDATA%\EisenPo\` sob a [Política de Privacidade do EisenPo](docs/privacy-policy.md#ptbr).
> - **Suporte do Desenvolvedor:** **A Microsoft NÃO presta suporte ao cliente para este aplicativo.** Dúvidas são atendidas exclusivamente pelo autor (`linceu_lighthouse@outlook.com` / [GitHub Issues](https://github.com/fernandoncidade/EisenPo_cpp/issues)).
> - **Capacidade `runFullTrust` (Política 10.6):** Declarada em [`library/AppxManifest.xml`](library/AppxManifest.xml) estritamente para execução Win32 nativa em Qt 6 e diálogos padrão de abertura/salvamento local, com zero telemetria e zero conexões de rede.
> - **Desinstalação Limpa (Política 10.2.7):** Remoção integral a qualquer momento via **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou Menu Iniciar → Desinstalar).

---

### 🚀 Compilação a partir do Código-Fonte e Empacotamento Limpo

#### Pré-requisitos
- **Sistema Operacional:** Windows 10 (`17763+`) ou Windows 11 x64;
- **Compiladores Suportados:** **MSVC x64** (Visual Studio 18 / Build Tools `14.52.37025`, Windows SDK `10.0.28000.0`) **e** **MinGW-w64 x64** (`C:\Qt\Tools\mingw1510_64`, GCC `15.1.0`);
- **Qt 6.12.0:** Módulos `Core`, `Gui`, `Widgets`, `Multimedia`, `Sql`, `Network` e `LinguistTools` (`msvc2022_64` e `mingw_64`);
- **Ferramentas de Build:** **CMake 3.28+**, **MSBuild / Visual Studio 18 2026** (MSVC puro), **Ninja 1.13.2** (MinGW-w64) e (opcional) **Inno Setup 7** (`ISCC.exe`).

> [!NOTE]
> **Compatibilidade C++23 Concluída (`CMAKE_CXX_STANDARD 23`):** Todo o projeto (`EisenPo.exe`, todos os 4 módulos nativos incluindo `feynman`, e os utilitários `mocks/`) compila de forma limpa e estrita sob **C++23** (`CMAKE_CXX_SCAN_FOR_MODULES OFF`) em **MSVC `14.52.37025`** e **MinGW-w64 `15.1.0`** com Qt `6.12.0`. Detalhes em [`docs/technical-audit-and-roadmap.md`](docs/technical-audit-and-roadmap.md#ptbr).

#### Compilação via Presets Oficiais (`CMakePresets.json`)
```powershell
git clone https://github.com/fernandoncidade/EisenPo_cpp.git
cd EisenPo_cpp

# Host EisenPo via MSVC x64 puro (gera também distribuição limpa em D:/MISCELANEAS/VSCode/EisenPo/EisenPo_cpp_msvc)
cmake --preset host-qt6-msvc-release
cmake --build --preset host-qt6-msvc-release

# Host EisenPo via MinGW-w64 x64 (gera também distribuição limpa em D:/MISCELANEAS/VSCode/EisenPo/EisenPo_cpp_mingw)
cmake --preset host-qt6-mingw-release
cmake --build --preset host-qt6-mingw-release

# Utilitários auxiliares (CompileTranslationsGUI, CompileTranslationsTerminal, find_VersionEditor)
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release

# Gerar instaladores Inno Setup compactos (sem embutir vc_redist.x64.exe; CRT MSVC app-local)
& "C:\Program Files\Inno Setup 7\ISCC.exe" .\library\EisenPo.iss
```

---

### 💾 Persistência Local, Exportação e Atalhos

- **Raiz Local 100% Offline (`%LOCALAPPDATA%\EisenPo`, migrando `%LOCALAPPDATA%\EisenhowerOrganizer` automaticamente):** `tarefas.json`, `layout.json`, `language.json`, `feynman_rotulos_config.json`, `listas.db` (SQLite), `eisenpo_gantt.db` (SQLite), `agenda_config.json`, `agendas/*.json`, `recorrencias_*.json`, `lembretes_locais_config.json`, `*_font_config.json` e `logs/file_eisenpo_*.log`.
- **Importação / Exportação:** `JSON`, `CSV`, `XLSX`, `PDF` e `ICS`.
- **Atalhos Principais:** `Ctrl+N`, `Ctrl+O`, `Ctrl+S`, `Ctrl+Shift+S`, `Ctrl+M` (Calendário), `Ctrl+Shift+E` (Pomodoro), `Ctrl+Shift+B` (Gantt BIM 4D), `Ctrl+Shift+G` (Busca Global), `Ctrl+Shift+H` (Hoje/7 dias), `Ctrl+Alt+F` (Filtros), `Ctrl+Shift+A` (Sobre com busca, licenças e política de privacidade), `Ctrl+Shift+M` (Manual interativo com índice e busca), `Ctrl+Q`.

---

### 📚 Documentação Técnica e Conformidade

- 📖 [Guia do Usuário](docs/user-guide.md#ptbr) · [Manual Completo de Operações (`MANUAL.md`)](MANUAL.md#ptbr)
- 🏗️ [Arquitetura do Sistema e Módulos Nativos](docs/architecture.md#ptbr)
- 📋 [Conformidade Regulatória e Open Source](docs/compliance.md#ptbr)
- 🔒 [Política de Privacidade e Soberania de Dados](docs/privacy-policy.md#ptbr)
- ⚖️ [Modelo de Duplo Licenciamento e Checklist Microsoft Store](docs/licensing.md#ptbr)
- 🛡️ [Modelo de Segurança e Resiliência](docs/security-model.md#ptbr)
- 🔬 [Auditoria Técnica, ADRs e Roadmap](docs/technical-audit-and-roadmap.md#ptbr) · [Sugestões de Issues](docs/suggested-issues.md#ptbr)
- 🚀 [Notas de Lançamento da Versão 2026.11.22.0](docs/release-v2026.11.22.0.md#ptbr) · [Observação Técnica Completa](OBSERVACAO.md#ptbr)

---

### ⚖️ Licenciamento, Terceiros e Comunidade

- **Código-Fonte Público (GitHub):** Licenciado sob **GNU Affero General Public License v3 (GNU AGPLv3)** ([`LICENSE`](LICENSE));
- **Pacote Oficial Microsoft Store:** Regido por **[EULA Oficial](source/assets/eisenhower/EULA/EULA_pt_BR.txt)** e **[CLC](source/assets/eisenhower/CLC/CLC_pt_BR.txt)** ([Baixar na Microsoft Store](https://apps.microsoft.com/detail/9P289X0185C3)), respeitando integralmente direitos LGPL de terceiros e legislação consumerista;
- **Componentes de Terceiros:** Qt 6.12.0 (`LGPLv3`), FFmpeg-n9.0.1 (`LGPLv2.1+`), SQLite (Domínio Público / Blessing), MinGW-w64 15.1.0 + GCC Runtime Library Exception 3.1 e Runtime MSVC `14.52` (*app-local*) — veja [`source/assets/eisenhower/NOTICES/NOTICE_pt_BR.txt`](source/assets/eisenhower/NOTICES/NOTICE_pt_BR.txt) e [`source/assets/LICENSES/THIRD_PARTY_COMPONENT_NOTICES.txt`](source/assets/LICENSES/THIRD_PARTY_COMPONENT_NOTICES.txt);
- **Suporte e Contribuições:** Canal oficial via `linceu_lighthouse@outlook.com` e GitHub — veja [`CONTRIBUTING.md`](CONTRIBUTING.md#ptbr).

</details>

---

## <a id="enus"></a><a id="english-us"></a>🇺🇸 English (US)

> **Note:** This repository refers to the official **v2026.11.22.0** release of **EisenPo** (native **C++ / Qt 6.12.0 / SQLite** implementation), provided under a transparent **Dual-Licensing** framework (**GNU AGPLv3** for the public open-source repository on GitHub and the author's **Official EULA + CLC** for the signed package distributed via the Microsoft Store, executing 100% locally and offline). Install the official edition at: [Install via Microsoft Store](https://apps.microsoft.com/detail/9P289X0185C3).

<details>
<summary><b>Click to expand / collapse English (US) documentation</b></summary><br>

# EisenPo — Task, Study, and Focus Organizer

Version: v2026.11.22.0<br>
Technical revision date: November 22, 2026<br>
Author: Fernando Nillsson Cidade<br>

**EisenPo** (Version `2026.11.22.0`) is a native Windows desktop productivity suite engineered in **C++ and Qt 6.12.0** uniting strategic task organization, focused execution timers, active study methodologies, academic/professional activity logging, and project scheduling with Critical Path Method (CPM) and 4D BIM simulation.

Originally conceived to reduce cognitive noise for workflows impacted by ADHD and dyslexia, the suite embeds five tightly integrated pillars inside a single native executable with full local data sovereignty (`%LOCALAPPDATA%\EisenPo`):

```text
EisenPo.exe (Qt 6.12.0 + Natively Integrated Modules)
 ├── Eisenhower Matrix (Q1–Q4, Tabbed/Quadrants/2×2, PoCa 2×K, JSON/CSV/XLSX/PDF/ICS)
 ├── Pomodoro Module (Per-task 1–999 targets, Target Warning, Lazy-Loaded .wav Alarms)
 ├── Feynman Module (5 Configurable Steps, Custom Labels, Linked Local Study Files)
 ├── Agenda Module (Activity Registration, Courses/Grades, Nested Lists, SQLite listas.db)
 └── Gantt & 4D BIM Module (WBS/EAP, DAG CPM Engine, WorkCalendar, 18-Country Holidays, SQLite eisenpo_gantt.db)
```

---

### ⚖️ Copyright Holder Manifesto & Legal Authorization

> **Express Statement by Fernando Nillsson Cidade:**  
> I, **Fernando Nillsson Cidade**, as the sole author, original creator, and exclusive copyright holder of the **EisenPo** project, **authorize the use, study, copying, modification, compilation, and distribution of the public source code in this repository** strictly under the **[LICENSE](LICENSE)** terms (**GNU Affero General Public License v3 — GNU AGPLv3**, SPDX identifier `AGPL-3.0-only`), reserving name and trademark protection under Section 7(e) (`"EisenPo"`).  
> Any modified version, extension, or derivative work based on the public source code must retain original authorship attribution and make its corresponding source code available under **GNU AGPLv3**.  
> **Freedom for Institutional Documentation in `source/assets/`:** Compliance texts located under [`source/assets/`](source/assets/eisenhower/COPYRIGHT/COPYRIGHT%20AND%20TRADEMARK%20NOTICE_en_US.txt) (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `NOTICES`, `PRIVACY_POLICY`, and `RELEASE`) were authored to fulfill official Microsoft Store submission requirements; the author **expressly grants developers and forks complete freedom** to alter, delete, add, or adapt these documents in their own distributions in accordance with applicable local laws and platform policies.  
> **Boundary for the Author's Signed Official Binary (Microsoft Store & Official Installers):** The package officially built and published by the author is governed by the author's **[Official EULA](source/assets/eisenhower/EULA/EULA_en_US.txt)** (harmonized with Microsoft Store Standard Application License Terms and non-waivable statutory consumer rights) and **[Commercial Licensing Clarification — CLC](source/assets/eisenhower/CLC/CLC_en_US.txt)**. Unauthorized paid commercial resale, trademark misappropriation, or illicit redistribution of the author's official signed binary package is strictly prohibited, preserving third-party open-source rights (`LGPLv3` / `LGPLv2.1+`).  
> **Support Channel, Microsoft Independence & Legal Disclaimer ("AS IS"):** **Microsoft does NOT provide customer support and does NOT cover this software under Microsoft's Privacy Statement.** Voluntary developer support is provided **exclusively via email (`linceu_lighthouse@outlook.com`) and GitHub Issues/Discussions** on a best-effort basis without urgency commitments or SLA (unless separately agreed in writing). The software is an **auxiliary tool** provided **"AS IS"** without warranties beyond mandatory law; critical contractual or engineering schedules must always be verified independently against established tools and qualified professionals. See [`docs/licensing.md`](docs/licensing.md#enus).

---

### 📌 Project Fact Sheet

<p align="center">
  <img src="docs/images/logo.png" alt="EisenPo Icon" width="128">
</p>

- **Official Name:** EisenPo — Task, Study, and Focus Organizer (`EisenPo.exe`)
- **Author & Maintainer:** Fernando Nillsson Cidade *(Individual Developer)*
- **Official Support Email (best-effort, no commercial SLA):** `linceu_lighthouse@outlook.com`
- **Official Repository / Website:** [https://github.com/fernandoncidade/EisenPo_cpp](https://github.com/fernandoncidade/EisenPo_cpp)
- **Official Download (Microsoft Store):** [Install via Microsoft Store (`9P289X0185C3`)](https://apps.microsoft.com/detail/9P289X0185C3)
- **MSIX Identity (`library/AppxManifest.xml`):** `fernandoncidade.EisenPo` · Windows 10 (`17763+`) / Windows 11 x64 (`runFullTrust`)
- **Runtime Dynamic Languages:** English (United States — `en_US`) · Português (Brasil — `pt_BR`)
- **Supported Toolchains (`CMakePresets.json`):** **MSVC x64** (`14.52.37025` / Windows SDK `10.0.28000.0`) and **MinGW-w64 x64** (`GCC 15.1.0`) targeting **Qt 6.12.0**, **CMake 3.28+**, **MSBuild (`Visual Studio 18 2026`)**, and **Ninja 1.13.2**
- **Auxiliary Utilities (`mocks/`):** `CompileTranslationsTerminal.exe`, `CompileTranslationsGUI.exe`, and `find_VersionEditor.exe`

---

### 🌟 Key Capabilities & Natively Integrated Modules

| Module | Integration | Core Capability |
|---|---|---|
| **EisenPo (Eisenhower)** | Built into Host | 4 strategic quadrants, Pending/In Progress/Completed lists, 4 workspace layouts + `PoCa` ($2 \times K$) sidebar, 3-row New Task bar with Duration, responsive `EditTaskDialog`, and JSON, CSV, XLSX, PDF & ICS import/export. |
| **Pomodoro** | Native in `EisenPo.exe` | Timer with individual cycle targets (`1`–`999` per task), preventive `⚠️ Cycle Target Warning`, lazy audio initialization, and `.wav` alarms (`Qt6::Multimedia`). |
| **Feynman** | Native in `EisenPo.exe` | Structured 5-step active study workflow with customizable step labels (`feynman_rotulos_config.json`), linked local files, and live `UnifiedTaskStore` sync. |
| **Agenda (Activity Registration)** | Native in `EisenPo.exe` | Academic/workplace activity logging (course, syllabus, semester, subject, grades), hierarchical schedules (`agendas/*.json`), independent color themes, and embedded SQLite (`listas.db`). |
| **Gantt & 4D BIM** | Native in `EisenPo.exe` | 4-layer Clean Architecture, hierarchical WBS/EAP, DAG **CPM** solver with cycle protection, discrete Pomodoro-driven physical progress, 5 persistent table layouts (`layout.json`), **WorkCalendar + 18-country Holiday Manager (Meeus/Jones/Butcher Easter algorithm)**, Baselines/Ghost Line, live tooltips, unified status colors, and 4D construction simulation (`IFC GUID`, `eisenpo_gantt.db`, CSV). |

---

### 📸 Interface Screenshots (`en-US`)

Official English screenshots stored in [`images/interfaces/en_US/`](images/interfaces/en_US):

<p align="center">
  <img src="images/interfaces/en_US/Captura%20de%20tela%202026-08-15%20005858.png" alt="EisenPo English Main View" width="880"><br><br>
  <img src="images/interfaces/en_US/Captura%20de%20tela%202026-08-15%20010006.png" alt="EisenPo English Integrated Modules" width="880">
</p>

---

### 🛡️ Microsoft Store Compliance, Privacy & Clean Uninstallation

> [!IMPORTANT]
> **Microsoft Store Compliance (Policies v7.20 / MSIX):**
> - **Independent Privacy Policy (Policy 10.5):** **Microsoft does NOT provide a default privacy policy for EisenPo, and this software is NOT covered by the [Microsoft Privacy Statement](https://www.microsoft.com/en-us/privacy/privacystatement).** All task, study, SQLite, and schedule processing occurs locally under `%LOCALAPPDATA%\EisenPo\` governed by the [EisenPo Privacy Policy](docs/privacy-policy.md#enus).
> - **Developer Support:** **Microsoft does NOT provide customer support for this app.** Inquiries go directly to the author (`linceu_lighthouse@outlook.com` / [GitHub Issues](https://github.com/fernandoncidade/EisenPo_cpp/issues)).
> - **`runFullTrust` Capability (Policy 10.6):** Declared in [`library/AppxManifest.xml`](library/AppxManifest.xml) solely for native Win32 C++ / Qt 6 execution and standard Windows Open/Save dialogs with zero telemetry and zero network calls.
> - **Clean Uninstallation (Policy 10.2.7):** Remove cleanly anytime via **Windows Settings → Apps → Installed Apps** (or Start Menu → Uninstall).

---

### 🚀 Building from Source & Clean Packaging

#### Prerequisites
- **Operating System:** Windows 10 (`17763+`) or Windows 11 x64;
- **Supported Compilers:** **MSVC x64** (Visual Studio 18 / Build Tools `14.52.37025`, Windows SDK `10.0.28000.0`) **and** **MinGW-w64 x64** (`C:\Qt\Tools\mingw1510_64`, GCC `15.1.0`);
- **Qt 6.12.0:** `Core`, `Gui`, `Widgets`, `Multimedia`, `Sql`, `Network`, and `LinguistTools` modules (`msvc2022_64` and `mingw_64`);
- **Build Tools:** **CMake 3.28+**, **MSBuild / Visual Studio 18 2026** (pure MSVC), **Ninja 1.13.2** (MinGW-w64), and (optional) **Inno Setup 7** (`ISCC.exe`).

> [!NOTE]
> **Full C++23 Build Verified (`CMAKE_CXX_STANDARD 23`):** The entire suite (`EisenPo.exe`, all 4 native modules including `feynman`, and `mocks/` utilities) compiles cleanly under strict **C++23** (`CMAKE_CXX_SCAN_FOR_MODULES OFF`) across **MSVC `14.52.37025`** and **MinGW-w64 `15.1.0`** targeting Qt `6.12.0`. See [`docs/technical-audit-and-roadmap.md`](docs/technical-audit-and-roadmap.md#enus).

#### Official CMake Presets (`CMakePresets.json`)
```powershell
git clone https://github.com/fernandoncidade/EisenPo_cpp.git
cd EisenPo_cpp

# EisenPo Host via pure MSVC x64 (also deploys clean package to D:/MISCELANEAS/VSCode/EisenPo/EisenPo_cpp_msvc)
cmake --preset host-qt6-msvc-release
cmake --build --preset host-qt6-msvc-release

# EisenPo Host via MinGW-w64 x64 (also deploys clean package to D:/MISCELANEAS/VSCode/EisenPo/EisenPo_cpp_mingw)
cmake --preset host-qt6-mingw-release
cmake --build --preset host-qt6-mingw-release

# Auxiliary utilities (CompileTranslationsGUI, CompileTranslationsTerminal, find_VersionEditor)
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release

# Compile compact Inno Setup installers (no bundled vc_redist.x64.exe; app-local MSVC CRT)
& "C:\Program Files\Inno Setup 7\ISCC.exe" .\library\EisenPo.iss
```

---

### 💾 Local Persistence, Export & Shortcuts

- **100% Offline Root (`%LOCALAPPDATA%\EisenPo`, auto-migrating legacy `%LOCALAPPDATA%\EisenhowerOrganizer`):** `tarefas.json`, `layout.json`, `language.json`, `feynman_rotulos_config.json`, `listas.db` (SQLite), `eisenpo_gantt.db` (SQLite), `agenda_config.json`, `agendas/*.json`, `recorrencias_*.json`, `lembretes_locais_config.json`, `*_font_config.json`, and `logs/file_eisenpo_*.log`.
- **Import / Export:** `JSON`, `CSV`, `XLSX`, `PDF`, and `ICS`.
- **Key Shortcuts:** `Ctrl+N`, `Ctrl+O`, `Ctrl+S`, `Ctrl+Shift+S`, `Ctrl+M` (Calendar), `Ctrl+Shift+E` (Pomodoro), `Ctrl+Shift+B` (Gantt 4D BIM), `Ctrl+Shift+G` (Global Search), `Ctrl+Shift+H` (Today/7 days), `Ctrl+Alt+F` (Filters), `Ctrl+Shift+A` (About dialog with live search, licenses, and privacy policy), `Ctrl+Shift+M` (Interactive manual with index and search), `Ctrl+Q`.

---

### 📚 Technical Documentation & Compliance

- 📖 [User Guide](docs/user-guide.md#enus) · [Full Operations Manual (`MANUAL.md`)](MANUAL.md#enus)
- 🏗️ [System Architecture & Native Modules](docs/architecture.md#enus)
- 📋 [Regulatory & Open-Source Compliance](docs/compliance.md#enus)
- 🔒 [Privacy Policy & Data Sovereignty](docs/privacy-policy.md#enus)
- ⚖️ [Dual-Licensing Model & Microsoft Store Checklist](docs/licensing.md#enus)
- 🛡️ [Security Model & Resilience](docs/security-model.md#enus)
- 🔬 [Technical Audit, ADRs & Roadmap](docs/technical-audit-and-roadmap.md#enus) · [Suggested Issues](docs/suggested-issues.md#enus)
- 🚀 [Release Notes v2026.11.22.0](docs/release-v2026.11.22.0.md#enus) · [Technical Observation](OBSERVACAO.md#ptbr)

---

### ⚖️ Licensing, Third Parties & Community

- **Public Source Code (GitHub):** Licensed under **GNU Affero General Public License v3 (GNU AGPLv3)** ([`LICENSE`](LICENSE));
- **Official Microsoft Store Binary Package:** Governed by **[Official EULA](source/assets/eisenhower/EULA/EULA_en_US.txt)** and **[CLC](source/assets/eisenhower/CLC/CLC_en_US.txt)** ([Download on Microsoft Store](https://apps.microsoft.com/detail/9P289X0185C3)), fully preserving third-party LGPL rights and non-waivable statutory consumer laws;
- **Third-Party Components:** Qt 6.12.0 (`LGPLv3`), FFmpeg-n9.0.1 (`LGPLv2.1+`), SQLite (Public Domain / Blessing), MinGW-w64 15.1.0 + GCC Runtime Library Exception 3.1, and app-local MSVC `14.52` CRT — see [`source/assets/eisenhower/NOTICES/NOTICE_en_US.txt`](source/assets/eisenhower/NOTICES/NOTICE_en_US.txt) and [`source/assets/LICENSES/THIRD_PARTY_COMPONENT_NOTICES.txt`](source/assets/LICENSES/THIRD_PARTY_COMPONENT_NOTICES.txt);
- **Support & Contributions:** Reach out via `linceu_lighthouse@outlook.com` or GitHub — see [`CONTRIBUTING.md`](CONTRIBUTING.md#enus).

</details>
