# Conformidade e Verificação / Compliance & Verification — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher o documento em português</summary><br>

# EisenPo — Matriz de Conformidade Regulatória, Open Source e Funcional

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

---

## 1. Conformidade Microsoft Store (MSIX / Políticas v7.20) & Acessibilidade

| Requisito / Política | Evidência no Repositório | Estado |
|---|---|:---:|
| **Manifesto MSIX (`AppxManifest.xml`)** | [`library/AppxManifest.xml`](../library/AppxManifest.xml) (`fernandoncidade.EisenPo`, `x64`, `MinVersion 10.0.17763.0`) | Documentado |
| **Capacidade Restrita `runFullTrust` (Política 10.6)** | Justificativa Win32 local-first para acesso a arquivos escolhidos pelo usuário ([`licensing.md`](licensing.md#ptbr)) | Documentado |
| **Política de Privacidade Própria (Política 10.5)** | [`privacy-policy.md`](privacy-policy.md#ptbr) e [`source/assets/eisenhower/PRIVACY_POLICY/`](../source/assets/eisenhower/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt) declarando independência da Microsoft | Documentado |
| **Suporte do Desenvolvedor Independente** | Canal próprio `linceu_lighthouse@outlook.com` + GitHub Issues (sem suporte Microsoft) | Documentado |
| **Desinstalação Limpa (Política 10.2.7)** | Remoção limpa via Configurações do Windows / Menu Iniciar, zero serviços em segundo plano | Documentado |
| **Acessibilidade (ADA / Microsoft Store)** | [`source/assets/LICENSES/MS-Store-ADA-v8.9-FINAL-EN.pdf`](../source/assets/LICENSES/MS-Store-ADA-v8.9-FINAL-EN.pdf) e suporte visual/cognitivo em `README.md` | Documentado |

---

## 2. Conformidade de Proteção de Dados e Privacidade (LGPD / GDPR)

- **LGPD (Lei nº 13.709/2018) e GDPR:** Todo tratamento de tarefas, agendas SQLite, estudos Feynman, sessões Pomodoro e cronogramas Gantt ocorre estritamente no dispositivo do usuário em `%LOCALAPPDATA%\EisenPo\`, sob custódia exclusiva do titular (execução local solicitada pelo usuário, zero coleta para servidores externos, portabilidade total em JSON/CSV/XLSX/PDF/ICS).

---

## 3. Matriz de Conformidade Open Source & Redistribuição Dual-Toolchain

O **EisenPo** atende simultaneamente aos deveres da **GNU AGPLv3** (para todo o código-fonte público e compilações próprias) e das licenças de componentes de terceiros distribuídos dinamicamente com os binários Windows:

| Componente / Runtime | Licença Upstream | Conformidade Aplicada na Distribuição | Documentos de Referência |
|---|---|---|---|
| **Código-Fonte Próprio (`EisenPo_cpp`)** | **GNU AGPLv3** (`AGPL-3.0-only`) | Arquivo integral [`LICENSE`](../LICENSE) na raiz e em `source/assets/LICENSES/` | [`LICENSE`](../LICENSE), [`licensing.md`](licensing.md#ptbr) |
| **Documentação em `source/assets/`** | Autoria com liberdade aos devs | Uso livre para alterar/excluir/adaptar em forks e plataformas de terceiros | [`COPYRIGHT`](../source/assets/eisenhower/COPYRIGHT/AVISO%20DE%20COPYRIGHT%20E%20MARCA%20REGISTRA_pt_BR.txt) |
| **Qt 6.12.0** (`Core`, `Gui`, `Widgets`, `Multimedia`, `Sql`, `Network`) | **GNU LGPLv3** | Vinculação dinâmica via DLLs separadas + `qt.conf`; direito expresso de substituição/relinkagem | [`GNU LESSER GENERAL PUBLIC LICENSE Version 3.txt`](../source/assets/LICENSES/GNU%20LESSER%20GENERAL%20PUBLIC%20LICENSE%20Version%203.txt), [`OPEN_SOURCE_SOURCE_OFFER_pt_BR.txt`](../source/assets/LICENSES/OPEN_SOURCE_SOURCE_OFFER_pt_BR.txt) |
| **FFmpeg-n9.0.1** (`avcodec-63`, `avformat-63`, `avutil-61`, `swresample-7`, `swscale-10`, `ffmpegmediaplugin.dll`) | **LGPL v2.1 ou posterior** | DLLs compartilhadas não ofuscadas do Qt 6.12.0 sem `--enable-gpl` nem `--enable-nonfree` | [`FFmpeg_LGPL_BUILD_CONFIGURATION.txt`](../source/assets/LICENSES/FFmpeg_LGPL_BUILD_CONFIGURATION.txt), [`GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.txt`](../source/assets/LICENSES/GNU%20LESSER%20GENERAL%20PUBLIC%20LICENSE%20Version%202.1.txt) |
| **SQLite** (`listas.db`, `eisenpo_gantt.db`, `qsqlite.dll`) | Domínio Público / SQLite Blessing | Banco relacional local embarcado sem restrições proprietárias | [`SQLite Blessing.txt`](../source/assets/LICENSES/SQLite%20Blessing.txt) |
| **MinGW-w64 15.1.0** (`libgcc_s_seh-1.dll`, `libstdc++-6.dll`, `libwinpthread-1.dll`) | GPLv3 + GCC Runtime Exception 3.1 + MinGW-w64 | Implantado apenas na variante MinGW via `cmake/copy_mingw_runtime.cmake` | [`GCC Runtime Library Exception 3.1.txt`](../source/assets/LICENSES/GCC%20Runtime%20Library%20Exception%203.1.txt), [`MinGW-w64 Runtime License.txt`](../source/assets/LICENSES/MinGW-w64%20Runtime%20License.txt) |
| **MSVC 14.52 (`v145`) CRT App-Local** (`msvcp140*.dll`, `vcruntime140*.dll`) | Termos Redistribuíveis Microsoft | Implantado apenas na variante MSVC sem embutir `vc_redist.x64.exe` nos instaladores Inno Setup | [`Microsoft Redistributable Runtime Notice.txt`](../source/assets/LICENSES/Microsoft%20Redistributable%20Runtime%20Notice.txt) |

---

## 4. Critérios de Verificação Funcional e Consistência Documental

- **Sincronização Versão/Documentação:** Auditável via `find_VersionEditor.exe --check --root .` cobrindo `README.md`, `OBSERVACAO.md`, `MANUAL.md`, `CONTRIBUTING.md`, `SECURITY.md`, `CODE_OF_CONDUCT.md`, todos os arquivos em `docs/*.md`, `library/EisenPo*.iss`, `library/EisenPo_cpp.txt`, `library/AppxManifest.xml`, manifestos de módulos, `incore_08_exibir_sobre.cpp` e os 72 documentos em `source/assets/eisenhower/`.
- **Validação dos Catálogos Bilíngues:** Auditável via `CompileTranslationsTerminal.exe --all` gerando `.qm` sincronizados para `eisenpo`, `pomodoro`, `feynman`, `agenda` e `gantt`.
- **Verificações Funcionais dos Algoritmos do Cronograma (`CpmEngine` / `WorkCalendar`):**
  - Detecção de ciclos em grafo de predecessoras antes da ordenação topológica DAG;
  - Progresso físico Pomodoro $\leftrightarrow$ Gantt avançando em degraus exatos `(ciclos_concluídos / ciclos_estimados) × 100%` (sem incremento inicial arbitrário de 10%);
  - Feriados móveis calculados pelo algoritmo de Meeus/Jones/Butcher em 18 países configuráveis;
  - Sincronização bidirecional e exclusão segura coordenada entre Matriz de Eisenhower, Pomodoro, Feynman, Agenda e Gantt.

> [!WARNING]
> **Caráter Auxiliar e Ausência de Certificação Definitiva ("AS IS"):** O cumprimento das diretrizes de distribuição e verificações funcionais descritas neste documento não transforma o **EisenPo** em instrumento certificado para decisões contratuais ou obras críticas. O aplicativo gratuito é fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")** (EULA Oficial, Lei nº 9.609/1998 e GNU AGPLv3 Seções 15–16), devendo resultados críticos de cronograma ou engenharia ser sempre validados manualmente ou com ferramentas consolidadas sob supervisão de profissional habilitado.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse compliance in English</summary><br>

# EisenPo — Regulatory, Open Source & Functional Compliance Matrix

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Author:** Fernando Nillsson Cidade  

---

## 1. Microsoft Store Compliance (MSIX / Policies v7.20) & Accessibility

| Requirement / Policy | Repository Evidence | Status |
|---|---|:---:|
| **MSIX Manifest (`AppxManifest.xml`)** | [`library/AppxManifest.xml`](../library/AppxManifest.xml) (`fernandoncidade.EisenPo`, `x64`, `MinVersion 10.0.17763.0`) | Documented |
| **Restricted Capability `runFullTrust` (Policy 10.6)** | Local-first Win32 rationale for standard Windows user-selected file dialogs ([`licensing.md`](licensing.md#enus)) | Documented |
| **Independent Privacy Policy (Policy 10.5)** | [`privacy-policy.md`](privacy-policy.md#enus) and [`source/assets/eisenhower/PRIVACY_POLICY/`](../source/assets/eisenhower/PRIVACY_POLICY/Privacy_Policy_en_US.txt) stating non-coverage by Microsoft | Documented |
| **Independent Developer Support** | Direct developer support (`linceu_lighthouse@outlook.com` + GitHub Issues; no Microsoft support) | Documented |
| **Clean Uninstallation (Policy 10.2.7)** | Clean removal via Windows Settings / Start Menu with zero residual background services | Documented |
| **Accessibility (ADA / Microsoft Store)** | [`source/assets/LICENSES/MS-Store-ADA-v8.9-FINAL-EN.pdf`](../source/assets/LICENSES/MS-Store-ADA-v8.9-FINAL-EN.pdf) plus low-friction UI/theme docs in `README.md` | Documented |

---

## 2. Privacy & Data Protection Compliance (LGPD / GDPR)

- **LGPD (Brazilian Law No. 13.709/2018) & GDPR:** All task, SQLite agenda, Feynman note, Pomodoro timer, and Gantt schedule processing stays strictly local on disk inside `%LOCALAPPDATA%\EisenPo\`, under sole user custody (user-requested local execution, zero remote collection, full portability across `.json`, `.csv`, `.xlsx`, `.pdf`, and `.ics`).

---

## 3. Open-Source & Dual-Toolchain Redistribution Compliance Matrix

**EisenPo** honors both **GNU AGPLv3** obligations (across public source code and third-party builds) and upstream third-party licenses redistributed in Windows runtime packages:

| Component / Runtime | Upstream License | Applied Distribution Compliance | Reference Documents |
|---|---|---|---|
| **Project Source Code (`EisenPo_cpp`)** | **GNU AGPLv3** (`AGPL-3.0-only`) | Full verbatim [`LICENSE`](../LICENSE) in root & `source/assets/LICENSES/` | [`LICENSE`](../LICENSE), [`licensing.md`](licensing.md#enus) |
| **Docs in `source/assets/`** | Authored with full dev freedom | Express freedom for forks to modify/delete/adapt for their own platforms | [`COPYRIGHT`](../source/assets/eisenhower/COPYRIGHT/COPYRIGHT%20AND%20TRADEMARK%20NOTICE_en_US.txt) |
| **Qt 6.12.0** (`Core`, `Gui`, `Widgets`, `Multimedia`, `Sql`, `Network`) | **GNU LGPLv3** | Shared-library deployment + `qt.conf`; explicit library replacement/relinking rights | [`GNU LESSER GENERAL PUBLIC LICENSE Version 3.txt`](../source/assets/LICENSES/GNU%20LESSER%20GENERAL%20PUBLIC%20LICENSE%20Version%203.txt), [`OPEN_SOURCE_SOURCE_OFFER_en_US.txt`](../source/assets/LICENSES/OPEN_SOURCE_SOURCE_OFFER_en_US.txt) |
| **FFmpeg-n9.0.1** (`avcodec-63`, `avformat-63`, `avutil-61`, `swresample-7`, `swscale-10`, `ffmpegmediaplugin.dll`) | **LGPL v2.1 or later** | Unobfuscated Qt 6.12.0 shared DLLs built without `--enable-gpl` or `--enable-nonfree` | [`FFmpeg_LGPL_BUILD_CONFIGURATION.txt`](../source/assets/LICENSES/FFmpeg_LGPL_BUILD_CONFIGURATION.txt), [`GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.txt`](../source/assets/LICENSES/GNU%20LESSER%20GENERAL%20PUBLIC%20LICENSE%20Version%202.1.txt) |
| **SQLite** (`listas.db`, `eisenpo_gantt.db`, `qsqlite.dll`) | Public Domain / SQLite Blessing | Embedded local relational storage | [`SQLite Blessing.txt`](../source/assets/LICENSES/SQLite%20Blessing.txt) |
| **MinGW-w64 15.1.0** (`libgcc_s_seh-1.dll`, `libstdc++-6.dll`, `libwinpthread-1.dll`) | GPLv3 + GCC Runtime Exception 3.1 + MinGW-w64 | Deployed strictly in MinGW builds via `cmake/copy_mingw_runtime.cmake` | [`GCC Runtime Library Exception 3.1.txt`](../source/assets/LICENSES/GCC%20Runtime%20Library%20Exception%203.1.txt), [`MinGW-w64 Runtime License.txt`](../source/assets/LICENSES/MinGW-w64%20Runtime%20License.txt) |
| **MSVC 14.52 (`v145`) App-Local CRT** (`msvcp140*.dll`, `vcruntime140*.dll`) | Microsoft Redistributable Terms | Deployed strictly app-local in MSVC builds without embedding `vc_redist.x64.exe` in Inno Setup installers | [`Microsoft Redistributable Runtime Notice.txt`](../source/assets/LICENSES/Microsoft%20Redistributable%20Runtime%20Notice.txt) |

---

## 4. Functional & Documentary Verification Criteria

- **Version/Docs Sync:** Verified via `find_VersionEditor.exe --check --root .` spanning `README.md`, `OBSERVACAO.md`, `MANUAL.md`, `CONTRIBUTING.md`, `SECURITY.md`, `CODE_OF_CONDUCT.md`, all `docs/*.md` files, `library/EisenPo*.iss`, `library/EisenPo_cpp.txt`, `library/AppxManifest.xml`, module manifests, `incore_08_exibir_sobre.cpp`, and all 72 compliance files under `source/assets/eisenhower/`.
- **Bilingual Catalog Verification:** Compiled via `CompileTranslationsTerminal.exe --all`, producing synchronized `.qm` binaries for `eisenpo`, `pomodoro`, `feynman`, `agenda`, and `gantt`.
- **Algorithmic & Workflow Checks (`CpmEngine` / `WorkCalendar`):**
  - Predecessor cycle interception prior to DAG topological sort;
  - Exact Pomodoro-to-Gantt physical progress stepping `(completed_cycles / target_cycles) × 100%` (no arbitrary 10% initial bump);
  - Movable Christian holidays computed via the Meeus/Jones/Butcher algorithm across 18 built-in countries;
  - Bidirectional sync and coordinated protected deletion across Eisenhower Matrix, Pomodoro, Feynman, Agenda, and Gantt.

> [!WARNING]
> **Auxiliary Nature & Non-Certification ("AS IS"):** Satisfying packaging policies and functional checks does not certify **EisenPo** for legally binding construction contracts, engineering works, or regulated compliance. This free software is supplied **"AS IS"** (Official EULA, Brazilian Law No. 9.609/1998, and GNU AGPLv3 Sections 15–16), and critical schedule/project metrics must always be verified independently against established tools under licensed professional oversight.

</details>
