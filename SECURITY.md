# Política de Segurança / Security Policy — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher a política em português</summary><br>

# Política de Segurança

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  

## 1. Versões com Suporte Oficial

Atualizações e correções de segurança são consideradas para a linha de lançamento mais recente:

| Versão | Suportada |
|:---:|:---:|
| `v2026.9.x` | :white_check_mark: |
| `< v2026.9` | :x: |

---

## 2. Como Relatar uma Vulnerabilidade

Levamos a integridade dos dados locais, a estabilidade e a segurança do **EisenPo** com atenção e transparência.

Se você identificou uma potencial vulnerabilidade de segurança (como erro de análise ao importar arquivos `.json`, `.csv`, `.xlsx`, `.pdf` ou `.ics`, corrupção de memória, manipulação insegura de caminhos no vínculo de arquivos ou falha de isolamento em bancos SQLite locais):

1. **NÃO divulgue publicamente a vulnerabilidade em issues ou discussões abertas.**
2. Envie o relato de forma privada por meio do recurso **GitHub Security Advisories** na aba [Security](https://github.com/fernandoncidade/EisenPo_cpp/security/advisories/new) do repositório ou contate o mantenedor diretamente via e-mail (`linceu_lighthouse@outlook.com`).
3. Por favor, inclua em seu relato:
   - Descrição detalhada da vulnerabilidade encontrada;
   - Passo a passo reprodutível e, quando aplicável, um arquivo de exemplo sanitizado (`.json`, `.csv`, `.xlsx`, `.pdf`, `.ics`);
   - Impacto estimado nos fluxos locais do aplicativo ou em ferramentas auxiliares (`mocks/`);
   - Sugestões técnicas de correção ou mitigação, caso possua.

### Prazos e Processo de Resposta
- O mantenedor envidará os melhores esforços para analisar e confirmar o recebimento de relatos de vulnerabilidade em tempo hábil, considerando o caráter estritamente voluntário e pessoal da manutenção do projeto (sem SLA comercial).
- Após avaliação e correção, será publicada uma nota de atualização coordenada quando pertinente.

---

## 3. Compromissos Arquiteturais de Segurança

- **100% Autônomo, Local e Offline em Tempo de Execução:** Todo o ecossistema do **EisenPo** (Matriz de Eisenhower, Pomodoro, Feynman, Agenda e Gantt & BIM 4D) opera exclusivamente no dispositivo local do usuário, com persistência em `%LOCALAPPDATA%\EisenPo` (`tarefas.json`, `listas.db`, `eisenpo_gantt.db`, configurações e logs locais). O executável não possui telemetria, contas de usuário nem comunicação com servidores remotos. *(Nota de auditoria transparente: o diretório `source/modules/gantt/Gantt/api/` contém código dormante de `HttpServer`/`RestApiController` herdado na árvore de código-fonte vinculando `Qt6::Network`, porém nenhuma porta TCP/UDP é aberta nem nenhum servidor HTTP é instanciado ou iniciado durante a execução normal do `EisenPo.exe`.)*
- **Persistência Atômica & Bancos SQLite Locais:** O armazenamento unificado de tarefas (`UnifiedTaskStore`), layouts e configurações utiliza escrita segura atômica (`QSaveFile`) para evitar corrupção em quedas de energia ou falhas de gravação, e o armazenamento relacional de Agenda (`listas.db`) e Gantt (`eisenpo_gantt.db`) opera localmente em SQLite.
- **Vínculo de Arquivos Sob Ação Explícita do Usuário:** Ao vincular arquivos externos em tarefas da Matriz de Eisenhower ou estudos do módulo Feynman, o aplicativo armazena apenas o caminho local informado e abre o recurso unicamente mediante clique ou comando explícito do usuário via mecanismo padrão do sistema operacional.
- **Isolamento Estrito de Runtime (MSVC x64 vs. MinGW-w64 x64):** Os scripts CMake de implantação (`copy_msvc_runtime.cmake`, `copy_mingw_runtime.cmake`, `prune_optional_root_runtime_dlls.cmake`, `qt.conf`) e instaladores Inno Setup garantem isolamento rigoroso entre variantes MSVC 14.52 e MinGW 15.1.0, removendo arquivos temporários/símbolos `.pdb` e redistribuíveis redundantes do pacote final.
- **Logs Locais Transparentes:** Diagnósticos de execução são gravados apenas localmente em `%LOCALAPPDATA%\EisenPo\logs\file_eisenpo_*.log`, sem transmissão externa.

> [!NOTE]
> **Escopo desta Política vs. Confiabilidade para Decisões Críticas ("AS IS"):** Esta Política de Segurança trata de segurança do software em execução local e **não certifica adequação ou maturidade definitiva do aplicativo como instrumento único para decisões profissionais, acadêmicas, contratuais ou cronogramas de engenharia/obras**. O **EisenPo** é um aplicativo gratuito de uso auxiliar fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sujeito a limitações e eventuais falhas humanas no código e nos algoritmos, devendo resultados críticos ser conferidos de forma independente ([`README.md`](README.md#ptbr), [EULA Oficial](source/assets/eisenhower/EULA/EULA_pt_BR.txt) e GNU AGPLv3 Seções 15–16).

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse the policy in English</summary><br>

# Security Policy

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  

## 1. Supported Versions

Security reviews and updates focus on the current release line:

| Version | Supported |
|:---:|:---:|
| `v2026.9.x` | :white_check_mark: |
| `< v2026.9` | :x: |

---

## 2. Reporting a Vulnerability

We take local data integrity, stability, and application security in **EisenPo** seriously.

If you believe you have discovered a security vulnerability (such as malformed-file parsing errors when importing `.json`, `.csv`, `.xlsx`, `.pdf`, or `.ics` files, memory safety defects, unsafe path handling in linked files, or local SQLite storage flaws):

1. **Do NOT disclose security vulnerabilities through public GitHub issues or discussions.**
2. Report the vulnerability privately via **GitHub Security Advisories** in the repository's [Security tab](https://github.com/fernandoncidade/EisenPo_cpp/security/advisories/new) or email the maintainer directly at `linceu_lighthouse@outlook.com`.
3. Please include:
   - Detailed description of the vulnerability;
   - Reproducible steps and, where relevant, a sanitized sample input file (`.json`, `.csv`, `.xlsx`, `.pdf`, `.ics`);
   - Estimated impact on local application workflows or helper tools (`mocks/`);
   - Suggested mitigations or patches, if available.

### Response Process
- The maintainer will make best-effort attempts to acknowledge and assess reports in a timely manner, reflecting the strictly voluntary and personal maintenance of this project (without a commercial SLA).
- Once verified and remediated, a release update and coordinated disclosure will be published where applicable.

---

## 3. Core Architectural Security Commitments

- **100% Autonomous, Local & Offline at Runtime:** The entire **EisenPo** ecosystem (Eisenhower Matrix, Pomodoro, Feynman, Agenda, and Gantt & 4D BIM) executes locally on the user's workstation, storing data inside `%LOCALAPPDATA%\EisenPo` (`tarefas.json`, `listas.db`, `eisenpo_gantt.db`, local configs, and logs). The running binary has zero telemetry, zero user accounts, and zero remote server communication. *(Transparent audit note: the directory `source/modules/gantt/Gantt/api/` contains dormant `HttpServer`/`RestApiController` code linked against `Qt6::Network`, but no TCP/UDP listening socket is opened and no HTTP server is instantiated or started during normal `EisenPo.exe` execution.)*
- **Atomic Local Persistence & Embedded SQLite:** `UnifiedTaskStore`, layout, and configuration writers rely on atomic `QSaveFile` commits to prevent corruption during crashes or power interruptions, and local Agenda (`listas.db`) and Gantt (`eisenpo_gantt.db`) relational data stay inside local SQLite databases.
- **Explicit User Action for Linked Files:** When attaching external files to Eisenhower tasks or Feynman study topics, only local paths are stored and external files are opened solely upon explicit user click/command through OS handlers.
- **Strict Runtime Toolchain Isolation (MSVC x64 vs. MinGW-w64 x64):** CMake deployment scripts (`copy_msvc_runtime.cmake`, `copy_mingw_runtime.cmake`, `prune_optional_root_runtime_dlls.cmake`, `qt.conf`) and Inno Setup scripts enforce clean isolation between MSVC 14.52 and MinGW 15.1.0 flavors while pruning debug symbols (`.pdb`) and redundant redistributables.
- **Local Diagnostic Logs:** Runtime diagnostics stay strictly local in `%LOCALAPPDATA%\EisenPo\logs\file_eisenpo_*.log` without any external transmission.

> [!NOTE]
> **Security Scope vs. Critical Decision Maturity ("AS IS"):** This Security Policy covers local software execution security and **does not certify definitive production maturity or suitability as a sole tool for legally binding professional, contractual, or engineering project schedules**. **EisenPo** is a free auxiliary application supplied **"AS IS"**, subject to human flaws and potential software/algorithmic defects, and critical outputs must always be verified independently ([`README.md`](README.md#enus), [Official EULA](source/assets/eisenhower/EULA/EULA_en_US.txt), and GNU AGPLv3 Sections 15–16).

</details>
