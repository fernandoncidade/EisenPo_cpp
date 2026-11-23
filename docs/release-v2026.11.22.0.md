# Release Notes — EisenPo v2026.11.22.0

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher notas de lançamento em português</summary><br>

# EisenPo — Versão 2026.11.22.0 (Padronização Gantt/BIM 4D, Duplo Licenciamento, Qt 6.12.0 & Distribuição Limpa)

**Data de Lançamento:** 22 de novembro de 2026  
**Autor:** Fernando Nillsson Cidade (`linceu_lighthouse@outlook.com`)  
**Repositório:** `fernandoncidade/EisenPo_cpp`  
**Licenciamento:** GNU AGPLv3 (Código-Fonte Aberto) / EULA Oficial + CLC (Microsoft Store `9P289X0185C3`)  

---

## Principais Recursos e Consolidação da Versão 2026.11.22.0

### 1. Padronização Cromática Unificada Entre Tabela WBS, Diagrama de Gantt e Simulação 4D BIM
- Todas as atividades exibidas na **Tabela WBS** e os nós/caixas do **Diagrama de Gantt** adotaram a mesma codificação semântica de cores da **Simulação Temporal 4D BIM**:
  - `✅ Executado:` Verde (`#4caf50`) com contraste calibrado nas barras do Gantt para atividades 100% concluídas;
  - `🔄 Em Andamento:` Azul límpido (`#2196f3`) para atividades ativas dentro do prazo;
  - `⚠️ Atrasado:` Vermelho alerta (`#f44336`) para atrasos tanto no início quanto no término planejado;
  - `⏳ Futuro:` Cinza neutro (`#9e9e9e`) para atividades planejadas que ainda não atingiram a data de início.

### 2. Quadros Informativos (Tooltips) Detalhados e Tradução Dinâmica em Tempo Real
- Posicionar o mouse sobre qualquer barra ou nó na régua temporal do Gantt exibe o código EAP/WBS, status colorido, datas e horários formatados por região (`dd/MM/yyyy HH:mm` vs `MM/dd/yyyy HH:mm`), duração em dias úteis, progresso físico discreto vinculado aos ciclos Pomodoro, badge de **Caminho Crítico (CPM)** e **folga total (horas)**.
- Todos os textos respondem imediatamente a `QEvent::LanguageChange` ao alternar entre Português (Brasil) e Inglês (EUA) sem reiniciar, com recolhimento limpo de tooltips antigos.

### 3. Gestão Documental Completa do Módulo Agenda & Duplo Licenciamento Transparente
- **Cobertura Integral da Agenda (`plugin_agenda`):** Todos os 12 arquivos contratuais e de conformidade do módulo Agenda foram integrados à automação de versões (`mocks/find_VersionEditor.cpp`).
- **Arquitetura Completa de Duplo Licenciamento e Conformidade (`docs/` & `source/assets/`):**
  - Adoção formal da licença **`GNU AGPLv3`** para todo o código-fonte público no GitHub ([`LICENSE`](../LICENSE)), autorizando uso, estudo, compilação e criação de forks;
  - Concessão expressa de **liberdade total aos desenvolvedores** sobre a documentação em `source/assets/` para adaptação em forks e outras plataformas;
  - Delimitação clara do **[EULA Oficial](../source/assets/eisenhower/EULA/EULA_pt_BR.txt)** e **[CLC](../source/assets/eisenhower/CLC/CLC_pt_BR.txt)** aplicáveis exclusivamente ao pacote oficial assinado distribuído na Microsoft Store, acompanhada de declaração explícita de independência da Microsoft (privacidade e suporte), desinstalação limpa (Política 10.2.7), caráter auxiliar gratuito e isenção `"AS IS"`;
  - Criação da suíte documental bilíngue em `docs/` (`architecture.md`, `licensing.md`, `privacy-policy.md`, `security-model.md`, `compliance.md`, `user-guide.md`, `technical-audit-and-roadmap.md`, `suggested-issues.md`) e templates do GitHub (`.github/`).

### 4. Modernização de Toolchains (`Qt 6.12.0` / `MSVC 14.52` / `MinGW 15.1.0`) e Distribuição Externa Limpa
- Atualização e validação integral dos scripts CMake, presets (`CMakePresets.json`) e configurações de IDE para **Qt 6.12.0** (`FFmpeg-n9.0.1` em LGPLv2.1+), **Visual Studio 18 / MSVC `14.52.37025` (`v145`)**, **Windows SDK `10.0.28000.0`**, **MinGW-w64 `GCC 15.1.0`**, **Ninja `1.13.2`** e padrão estrito **C++23** (`CMAKE_CXX_SCAN_FOR_MODULES OFF`, refatoração do identificador `concept` nos 5 arquivos de `source/modules/feynman/Feynman/` e correção de `generated/qt.conf` no deploy single-config Ninja), com todos os 4 presets ativos compilados e verificados (MSVC puro via MSBuild `18.12.3`: `114,97 MB` / `49,34 MB` no instalador; MinGW via Ninja `1.13.2`: `129,48 MB` / `53,70 MB` no instalador) — detalhes na [auditoria técnica](technical-audit-and-roadmap.md#ptbr).
- Otimização extrema das distribuições externas e instaladores Inno Setup (`library/EisenPo.iss`, `EisenPo_MSVC.iss`, `EisenPo_MinGW.iss`): exclusão de banners promocionais de loja, `*.pdb`, plugins não utilizados (`qsvg`, `qpdf`, `qml`, drivers SQL extras, `qopensslbackend`) e **remoção integral do `vc_redist.x64.exe` (~18 MB)** graças à implantação *app-local* isolada das 5 DLLs exatas do `Microsoft.VC145.CRT`.

### 5. Base Consolidada de Versões Anteriores (`v2026.9.27.0` e anteriores)
- Integração nativa dos módulos Pomodoro, Feynman, Agenda (SQLite) e Gantt & BIM 4D diretamente ao executável `EisenPo.exe` (sem DLLs dinâmicas externas), catálogos `.qm` unificados, gravação atômica assíncrona em `%LOCALAPPDATA%\EisenPo`, layout PoCa ($2 \times K$), metas individuais Pomodoro (1–999) e gerenciador de feriados de 18 países (algoritmo de Meeus/Jones/Butcher).

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse release notes in English</summary><br>

# EisenPo — Version 2026.11.22.0 (Gantt/4D BIM Unification, Dual Licensing, Qt 6.12.0 & Clean Deployment)

**Release Date:** November 22, 2026  
**Author:** Fernando Nillsson Cidade (`linceu_lighthouse@outlook.com`)  
**Repository:** `fernandoncidade/EisenPo_cpp`  
**Licensing:** GNU AGPLv3 (Open Source Codebase) / Official EULA + CLC (Microsoft Store `9P289X0185C3`)  

---

## Highlights & Consolidation of Version 2026.11.22.0

### 1. Unified Chromatic Status Styling Across WBS Table, Gantt Chart & 4D BIM Simulation
- All WBS table entries and Gantt timeline activity nodes/bars share the exact color language established in **4D BIM Temporal Simulation**:
  - `✅ Executed:` Green (`#4caf50`) with calibrated contrast on timeline bars for 100% completed activities;
  - `🔄 In Progress:` Clear blue (`#2196f3`) for active on-schedule work;
  - `⚠️ Delayed:` Alert red (`#f44336`) for overdue start or finish dates;
  - `⏳ Future:` Neutral gray (`#9e9e9e`) for planned upcoming items.

### 2. Rich Runtime-Retranslatable Timeline Tooltips
- Hovering over any Gantt bar or milestone displays full WBS code, status badge, locale-aware start/finish dates (`MM/dd/yyyy HH:mm` vs `dd/MM/yyyy HH:mm`), working-day duration, discrete Pomodoro-driven physical progress, **Critical Path (CPM)** flag, and **Total Float (hours)**.
- All tooltip strings respond dynamically to `QEvent::LanguageChange` across English and Portuguese without restarting, dismissing stale tooltips cleanly.

### 3. Complete Agenda Compliance Automation & Transparent Dual-Licensing Architecture
- **Full Agenda (`plugin_agenda`) Coverage:** All 12 contractual, notice, and privacy documents for Activity Registration are tracked automatically by `mocks/find_VersionEditor.cpp`.
- **Complete Dual-Licensing & Governance Documentation (`docs/` & `source/assets/`):**
  - Formal adoption of **GNU AGPLv3** across the public repository source code ([`LICENSE`](../LICENSE)), guaranteeing full rights to inspect, compile, modify, and fork;
  - Express author grant giving **complete freedom to developers** over `source/assets/` compliance documentation for forks and third-party platforms;
  - Clear boundary for the **[Official EULA](../source/assets/eisenhower/EULA/EULA_en_US.txt)** and **[CLC](../source/assets/eisenhower/CLC/CLC_en_US.txt)** governing the author's digitally signed Microsoft Store release (`9P289X0185C3`), with explicit Microsoft privacy/support non-coverage statements, Policy 10.2.7 clean uninstall, auxiliary scope, and `"AS IS"` warranty disclaimers;
  - Creation of the complete bilingual documentation suite under `docs/` (`architecture.md`, `licensing.md`, `privacy-policy.md`, `security-model.md`, `compliance.md`, `user-guide.md`, `technical-audit-and-roadmap.md`, `suggested-issues.md`) and `.github/` templates.

### 4. Dual-Toolchain Modernization (`Qt 6.12.0` / `MSVC 14.52` / `MinGW 15.1.0`) & Clean External Deployment
- Updated and end-to-end verified CMake scripts, `CMakePresets.json`, and IDE configs for **Qt 6.12.0** (`FFmpeg-n9.0.1` under LGPLv2.1+), **Visual Studio 18 / MSVC `14.52.37025` (`v145`)**, **Windows SDK `10.0.28000.0`**, **MinGW-w64 `GCC 15.1.0`**, **Ninja `1.13.2`**, and strict **C++23** (`CMAKE_CXX_SCAN_FOR_MODULES OFF`, `concept` identifier refactored across 5 files in `source/modules/feynman/Feynman/`, and `generated/qt.conf` single-config Ninja fix), with all 4 active presets cleanly built and verified (pure MSVC via MSBuild `18.12.3`: `114.97 MB` / `49.34 MB` installer; MinGW via Ninja `1.13.2`: `129.48 MB` / `53.70 MB` installer) — see the [technical audit](technical-audit-and-roadmap.md#enus).
- Ultra-clean external distributions and Inno Setup scripts (`library/EisenPo.iss`, `EisenPo_MSVC.iss`, `EisenPo_MinGW.iss`): stripped promotional store marketing banners, `*.pdb`, unused plugins (`qsvg`, `qpdf`, `qml`, extra SQL drivers, `qopensslbackend`), and **eliminated `vc_redist.x64.exe` (~18 MB)** through isolated app-local deployment of the exact 5 `Microsoft.VC145.CRT` DLLs.

### 5. Consolidated Ecosystem Foundation (`v2026.9.27.0` and earlier)
- Direct native compilation of Pomodoro, Feynman, Agenda (SQLite), and Gantt & 4D BIM inside `EisenPo.exe` (zero plugin DLLs), unified `.qm` translation deployment, async atomic storage under `%LOCALAPPDATA%\EisenPo`, $2 \times K$ PoCa sidebar, individual Pomodoro cycle targets (1–999), and 18-country Holiday Manager powered by Meeus/Jones/Butcher Easter dates.

</details>
