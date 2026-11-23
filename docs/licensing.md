# Licenciamento e Esclarecimento Comercial / Licensing & Commercial Model — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher o documento de licenciamento em português</summary><br>

# EisenPo — Licenciamento, Termos de Código Aberto e Esclarecimento Comercial

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Licença do Código-Fonte:** GNU Affero General Public License v3 (AGPLv3)  
**Autor e Detentor dos Direitos Autorais:** Fernando Nillsson Cidade  

---

## 1. Visão Geral do Modelo de Duplo Licenciamento

O **EisenPo** adota um modelo transparente de **Duplo Licenciamento** que concilia as liberdades do software livre copyleft forte com a distribuição oficial assinada na Microsoft Store e os direitos mandatórios dos componentes de terceiros:

| Componente | Licença | Usos Permitidos |
|---|---|---|
| **Código-Fonte (Repositório Completo)** | [GNU AGPLv3](../LICENSE) | 100% Livre: Uso, estudo, compilação (`MSVC` ou `MinGW`), modificação, criação de forks e redistribuição sob termos copyleft da GNU AGPLv3 (preservada a proteção nominal da Seção 7(e) para `"EisenPo"`). |
| **Documentação em `source/assets/`** | Autoria do Projeto (Uso Livre para Desenvolvedores) | Elaborada para conformidade com a Microsoft Store; o autor **concede liberdade total** para que desenvolvedores alterem, excluam, adicionem e adaptem esses documentos em seus próprios forks e plataformas. |
| **Binários Compilados por Terceiros (via Código)** | GNU AGPLv3 | Totalmente livres para compilar, empacotar e redistribuir sob responsabilidade do compilador, cumprindo integralmente os termos da AGPLv3 e as atribuições LGPL/permissivas aplicáveis. |
| **Binário Oficial Assinado (Microsoft Store / Instaladores do Autor)** | [EULA Oficial](../source/assets/eisenhower/EULA/EULA_pt_BR.txt) + [CLC](../source/assets/eisenhower/CLC/CLC_pt_BR.txt) | Pacote oficial pré-compilado e disponibilizado pelo autor na [Microsoft Store (`9P289X0185C3`)](https://apps.microsoft.com/detail/9P289X0185C3) (e instaladores oficiais). Proibida a revenda comercial paga não autorizada, apropriação de marca ou redistribuição ilícita por terceiros do pacote assinado pelo autor, preservados os direitos LGPL sobre DLLs. |
| **Bibliotecas de Terceiros (`Qt 6.12.0`, `FFmpeg-n9.0.1`, `SQLite`, `MinGW-w64`, `MSVC CRT`)** | LGPLv3 / LGPLv2.1+ / Domínio Público / GPLv3+GCC Exception / Microsoft Redistributable | Distribuídas em conformidade estrita com suas licenças upstream ([`THIRD_PARTY_COMPONENT_NOTICES.txt`](../source/assets/LICENSES/THIRD_PARTY_COMPONENT_NOTICES.txt)). |

---

## 2. Direitos de Código Aberto (GNU AGPLv3)

Todo o código-fonte público do **EisenPo** — abrangendo o host da Matriz de Eisenhower (`source/app/`, `source/services/`, `source/ui/`, `source/models/`, `source/utils/`, `main.cpp`), os quatro módulos nativos integrados (`source/modules/pomodoro/`, `feynman/`, `agenda/` e `gantt/`), os catálogos de tradução (`source/language/`), os scripts CMake (`CMakeLists.txt`, `cmake/`, `CMakePresets.json`) e os utilitários em `mocks/` — está licenciado sob a **GNU Affero General Public License v3 (AGPLv3, identificador SPDX `AGPL-3.0-only`)**:

```text
EisenPo — Organizador de Tarefas, Estudos e Foco
Copyright (c) 2025-2026 Fernando Nillsson Cidade

Este programa é um software livre: você pode redistribuí-lo e/ou modificá-lo sob os termos da GNU Affero General Public License conforme publicada pela Free Software Foundation, seja a versão 3 da Licença, ou (a seu critério) qualquer versão posterior.

Este programa é distribuído na esperança de que seja útil, mas SEM QUALQUER GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou ADEQUAÇÃO A UM DETERMINADO FIM. Veja a GNU Affero General Public License para mais detalhes.

Você deve ter recebido uma cópia da GNU Affero General Public License junto com este programa. Se não, veja em: <https://www.gnu.org/licenses/>.
```

---

## 3. Documentação em `source/assets/` e Liberdade Total para Desenvolvedores

Os textos institucionais e regulatórios mantidos em `source/assets/` (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `NOTICES`, `PRIVACY_POLICY` e `RELEASE`) possuem finalidade e permissão expressas:

1. **Finalidade Original:** Elaborados por Fernando Nillsson Cidade para atender integralmente às exigências de publicação, transparência, acessibilidade (`MS-Store-ADA-v8.9-FINAL-EN.pdf`), privacidade e conformidade na **Microsoft Store** e legislação aplicável (LGPD/GDPR/CDC).
2. **Autorização Expressa para Desenvolvedores:** O autor **autoriza expressamente e concede liberdade total** para que desenvolvedores, contribuidores e mantenedores de forks alterem, excluam, adicionem ou adaptem livremente qualquer arquivo em `source/assets/` em suas próprias distribuições.
3. **Condição de Responsabilidade:** Cada desenvolvedor ou fork que publicar pacotes próprios responde de forma independente pelo cumprimento das diretrizes da respectiva loja/plataforma e da legislação vigente em sua jurisdição, devendo preservar apenas os avisos e obrigações das bibliotecas de terceiros efetivamente redistribuídas (`Qt 6.12.0`, `FFmpeg-n9.0.1`, runtime MinGW/MSVC, etc.).

---

## 4. Fronteira entre Compilações Independentes e o Pacote Oficial do Autor

1. **Compilações Próprias a partir do Código-Fonte:** Qualquer pessoa ou organização pode clonar o repositório, auditar o código, compilar com **MSVC 14.52 (`v145`)** ou **MinGW-w64 (`GCC 15.1.0`)** via `CMakePresets.json` e redistribuir suas próprias compilações, desde que cumpra a **GNU AGPLv3** (incluindo fornecimento do código-fonte correspondente e preservação dos avisos de terceiros).
2. **Pacote Oficial na Microsoft Store (`9P289X0185C3`) e Instaladores Assinados pelo Autor:** Os pacotes pré-compilados e empacotados oficialmente pelo autor (MSIX na Microsoft Store e instaladores Inno Setup oficiais) são regidos pelo **[EULA Oficial](../source/assets/eisenhower/EULA/EULA_pt_BR.txt)** e pelo **[Esclarecimento de Licenciamento Comercial — CLC](../source/assets/eisenhower/CLC/CLC_pt_BR.txt)** em harmonia com os Termos de Licença Padrão para Aplicativos da Microsoft Store e direitos irrenunciáveis do consumidor. É vedada a revenda comercial paga não autorizada ou republicação indevida por terceiros do pacote binário oficial do autor.
3. **Preservação Integral de Direitos LGPL:** Nenhuma cláusula do EULA ou CLC restringe direitos garantidos por licenças open source de terceiros — incluindo o direito sob **LGPLv3 / LGPLv2.1+** de substituir ou revincular bibliotecas compartilhadas compatíveis do **Qt 6.12.0** e **FFmpeg-n9.0.1**, bem como engenharia reversa estritamente limitada à depuração de modificações nessas bibliotecas LGPL (com oferta de código-fonte correspondente em [`OPEN_SOURCE_SOURCE_OFFER_pt_BR.txt`](../source/assets/LICENSES/OPEN_SOURCE_SOURCE_OFFER_pt_BR.txt)).

---

## 5. Checklist de Conformidade e Submissão Microsoft Store (MSIX / Partner Center)

Este checklist resume o mapeamento documental compatível com [`library/AppxManifest.xml`](../library/AppxManifest.xml) e as **Políticas da Microsoft Store (v7.20)**:

| Item do Partner Center | Status Documental / Referência no Repositório | Observação Operacional |
|---|---|---|
| **Identidade MSIX (`AppxManifest.xml`)** | `fernandoncidade.EisenPo` (`CN=F1FFF36E-09E6-42F5-BAE5-3124C3054234`, `x64`, `MinVersion 10.0.17763.0`) | Executável `EisenPo\EisenPo.exe` empacotado com os 4 módulos integrados e runtime Qt 6.12.0. |
| **Categoria (Obrigatório)** | Sugerido: *Produtividade* / *Educação* | Configurar na seção *Propriedades* do envio. |
| **URL da Política de Privacidade (Obrigatório para `runFullTrust`)** | [`docs/privacy-policy.md`](privacy-policy.md#ptbr) e [`Privacy_Policy_pt_BR.txt`](../source/assets/eisenhower/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt) | Aplicativos Win32/`runFullTrust` exigem URL pública de privacidade (Política 10.5.1). **A Microsoft NÃO fornece política padrão e não cobre este aplicativo sob a Declaração de Privacidade da Microsoft.** |
| **Site Oficial do Desenvolvedor** | `https://github.com/fernandoncidade/EisenPo_cpp` | Aponta para o repositório oficial do desenvolvedor (distinto da listagem na Store `9P289X0185C3`). |
| **Suporte ao Cliente** | `linceu_lighthouse@outlook.com` · [GitHub Issues](https://github.com/fernandoncidade/EisenPo_cpp/issues) | **A Microsoft NÃO presta suporte ao cliente para este aplicativo**; o autor (desenvolvedor individual) oferece atendimento voluntário best-effort. |
| **Capacidade Restrita (`runFullTrust`)** | `<rescap:Capability Name="runFullTrust" />` em `library/AppxManifest.xml` | Necessária apenas por ser aplicativo desktop Win32 em C++ / Qt 6 com acesso a diálogos padrão de abertura/salvamento local e armazenamento em `%LOCALAPPDATA%\EisenPo`, operando 100% offline. |
| **Desinstalação Limpa (Política 10.2.7)** | Documentada em `README.md`, `privacy-policy.md` e `EULA` | Remoção integral via **Configurações do Windows → Aplicativos** / Menu Iniciar, sem serviços residentes ou drivers. |
| **Acessibilidade** | [`MS-Store-ADA-v8.9-FINAL-EN.pdf`](../source/assets/LICENSES/MS-Store-ADA-v8.9-FINAL-EN.pdf) | Documentação complementar incluída na pasta `LICENSES`. |

### Textos Prontos para Preenchimento no Partner Center

- **Justificativa para Capacidade Restrita (`runFullTrust`):**
  > *EisenPo is a native Win32 C++ / Qt 6.12.0 desktop productivity application uniting the Eisenhower Matrix, Pomodoro, Feynman active learning, SQLite-backed Activity Registration, and Gantt & 4D BIM scheduling. It requires `runFullTrust` strictly to run its native Qt Win32 binaries and standard Windows file Open/Save dialogs for local user-selected JSON, CSV, XLSX, PDF, and ICS files. It executes 100% offline with zero background services, zero drivers, and zero telemetry.*

- **Direitos Autorais e Marcas Registradas (`pt-BR`, máx. 200 caracteres):**
  > `© 2025-2026 Fernando Nillsson Cidade. Código aberto GNU AGPLv3; pacote oficial Microsoft Store sob EULA do autor. Qt (LGPLv3), FFmpeg (LGPLv2.1+), SQLite.` *(154 caracteres)*

- **Sugestão de Palavras-chave (`pt-BR`, até 7 termos — Política 10.1.3):**
  `Matriz de Eisenhower`, `Pomodoro`, `Feynman`, `Gantt`, `Agenda`, `Produtividade`, `Offline`

---

## 6. Escopo Auxiliar, Limitações de Maturidade e Isenção Legal ("AS IS")

1. **Uso como Ferramenta Auxiliar:** O **EisenPo** destina-se ao uso como **ferramenta computacional auxiliar** de produtividade pessoal, acadêmica, gestão de tempo, estudos e planejamento de cronogramas — e **não** como instrumento definitivo ou único para decisões críticas de engenharia, prazos contratuais ou obrigações regulatórias.
2. **Imaturidade e Conferência Obrigatória:** O aplicativo pode conter limitações e eventuais falhas humanas no código-fonte, na sincronização entre módulos ou nos algoritmos de cálculo de cronogramas (Caminho Crítico/CPM, calendários de trabalho e simulação 4D), devendo resultados críticos ser conferidos manualmente ou validados com ferramentas consolidadas e supervisão de profissional habilitado.
3. **Fornecimento "AS IS":** Na extensão máxima permitida pela legislação aplicável (incluindo Lei nº 9.609/1998, EULA Oficial e Seções 15 e 16 da [GNU AGPLv3](../LICENSE), preservados direitos irrenunciáveis do consumidor), o software é fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sem garantias explícitas ou implícitas, assumindo o usuário exclusiva responsabilidade por eventuais decisões profissionais baseadas isoladamente nos resultados gerados.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse licensing in English</summary><br>

# EisenPo — Licensing, Open Source Terms & Commercial Clarification

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Source Code License:** GNU Affero General Public License v3 (AGPLv3)  
**Author and Copyright Holder:** Fernando Nillsson Cidade  

---

## 1. Dual-Licensing Architecture Overview

**EisenPo** adopts a transparent **Dual-Licensing** framework that reconciles strong-copyleft free software freedoms with the author's official signed Microsoft Store package and mandatory upstream third-party open-source rights:

| Component | License | Permitted Uses |
|---|---|---|
| **Source Code (Full Repository)** | [GNU AGPLv3](../LICENSE) | 100% Free: Use, inspect, compile (`MSVC` or `MinGW`), modify, fork, and redistribute under GNU AGPLv3 copyleft terms (reserving Section 7(e) brand/name protection for `"EisenPo"`). |
| **Documentation in `source/assets/`** | Project Authorship (Free for Developers) | Created to meet Microsoft Store compliance requirements; the author **grants complete freedom** for developers to alter, delete, add, and adapt these documents in their own forks and platforms. |
| **Third-Party Builds from Source** | GNU AGPLv3 | Fully free to build, package, and redistribute under the builder's own responsibility in full compliance with AGPLv3 and applicable LGPL/permissive attributions. |
| **Author's Official Signed Package (Microsoft Store / Official Installers)** | [Author's Official EULA](../source/assets/eisenhower/EULA/EULA_en_US.txt) + [CLC](../source/assets/eisenhower/CLC/CLC_en_US.txt) | Official precompiled package distributed by the author on the [Microsoft Store (`9P289X0185C3`)](https://apps.microsoft.com/detail/9P289X0185C3) (and official installers). Unauthorized paid commercial resale, brand hijacking, or illicit redistribution of the author's signed binary package is prohibited, preserving LGPL shared-library rights. |
| **Third-Party Libraries (`Qt 6.12.0`, `FFmpeg-n9.0.1`, `SQLite`, `MinGW-w64`, `MSVC CRT`)** | LGPLv3 / LGPLv2.1+ / Public Domain / GPLv3+GCC Exception / Microsoft Redistributable | Shipped in compliance with upstream terms ([`THIRD_PARTY_COMPONENT_NOTICES.txt`](../source/assets/LICENSES/THIRD_PARTY_COMPONENT_NOTICES.txt)). |

---

## 2. Open Source Rights (GNU AGPLv3)

All public **EisenPo** source code — encompassing the Eisenhower Matrix host (`source/app/`, `source/services/`, `source/ui/`, `source/models/`, `source/utils/`, `main.cpp`), all four native compiled productivity modules (`source/modules/pomodoro/`, `feynman/`, `agenda/`, and `gantt/`), translation catalogs (`source/language/`), CMake build/deploy scripts (`CMakeLists.txt`, `cmake/`, `CMakePresets.json`), and utilities in `mocks/` — is licensed under the **GNU Affero General Public License v3 (AGPLv3, SPDX identifier `AGPL-3.0-only`)**:

```text
EisenPo — Task, Study, and Focus Organizer
Copyright (c) 2025-2026 Fernando Nillsson Cidade

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
```

---

## 3. Documentation in `source/assets/` and Full Freedom for Developers

Institutional and compliance texts under `source/assets/` (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `NOTICES`, `PRIVACY_POLICY`, and `RELEASE`) serve a dedicated role with explicit developer freedom:

1. **Original Purpose:** Authored by Fernando Nillsson Cidade to satisfy official **Microsoft Store** publishing, accessibility (`MS-Store-ADA-v8.9-FINAL-EN.pdf`), privacy, support, and LGPD/GDPR compliance requirements.
2. **Express Authorization for Developers:** The author **expressly authorizes and grants full freedom** for developers, contributors, and forks to modify, delete, add, or adapt any document in `source/assets/` for their own builds and distributions.
3. **Independent Compliance Responsibility:** Developers and forks distributing independent builds remain solely responsible for ensuring compliance with target store guidelines and applicable local laws, while honoring third-party component notices (`Qt 6.12.0`, `FFmpeg-n9.0.1`, MinGW/MSVC runtimes, etc.).

---

## 4. Boundary Between Independent Source Builds and Official Store Packages

1. **Independent Builds from Source:** Any individual or organization may clone the repository, audit every file, compile using **MSVC 14.52 (`v145`)** or **MinGW-w64 (`GCC 15.1.0`)** via `CMakePresets.json`, and redistribute derivative builds provided they satisfy **GNU AGPLv3** obligations (including corresponding source availability and upstream notices).
2. **Author's Official Microsoft Store Binary (`9P289X0185C3`) & Official Signed Installers:** Precompiled packages officially signed and published by the author are governed by the author's **[Official EULA](../source/assets/eisenhower/EULA/EULA_en_US.txt)** and **[Commercial Licensing Clarification — CLC](../source/assets/eisenhower/CLC/CLC_en_US.txt)** in harmony with Microsoft Store Standard Application License Terms and non-waivable statutory consumer rights. Third parties may not commercially resell or republish the author's signed official package without written authorization.
3. **Full Preservation of LGPL Rights:** Nothing in the EULA or CLC restricts third-party open-source rights — notably recipients' rights under **LGPLv3 / LGPLv2.1+** to replace or relink compatible shared libraries of **Qt 6.12.0** and **FFmpeg-n9.0.1**, and to reverse engineer strictly to debug modifications to those LGPL libraries (with the corresponding source offer in [`OPEN_SOURCE_SOURCE_OFFER_en_US.txt`](../source/assets/LICENSES/OPEN_SOURCE_SOURCE_OFFER_en_US.txt)).

---

## 5. Microsoft Store MSIX Submission, Privacy & Support Checklist

This checklist summarizes Partner Center metadata aligned with [`library/AppxManifest.xml`](../library/AppxManifest.xml) and **Microsoft Store Policies (v7.20)**:

| Partner Center Item | Repository Reference / Status | Operational Notes |
|---|---|---|
| **MSIX Identity (`AppxManifest.xml`)** | `fernandoncidade.EisenPo` (`CN=F1FFF36E-09E6-42F5-BAE5-3124C3054234`, `x64`, `MinVersion 10.0.17763.0`) | Launches `EisenPo\EisenPo.exe` with all 4 integrated modules and Qt 6.12.0 runtime. |
| **Category (Required)** | Suggested: *Productivity* / *Education* | Configure under *Properties*. |
| **Privacy Policy URL (Required for `runFullTrust`)** | [`docs/privacy-policy.md`](privacy-policy.md#enus) and [`Privacy_Policy_en_US.txt`](../source/assets/eisenhower/PRIVACY_POLICY/Privacy_Policy_en_US.txt) | Win32/`runFullTrust` desktop apps require a privacy policy (Policy 10.5.1). **Microsoft does NOT provide a default policy and does NOT cover this app under Microsoft's Privacy Statement.** |
| **Developer Website** | `https://github.com/fernandoncidade/EisenPo_cpp` | Points to the developer's own website — distinct from the Store listing (`9P289X0185C3`). |
| **Customer Support** | `linceu_lighthouse@outlook.com` · [GitHub Issues](https://github.com/fernandoncidade/EisenPo_cpp/issues) | **Microsoft provides NO customer support for this app**; the author (individual developer) offers voluntary best-effort support. |
| **Restricted Capability (`runFullTrust`)** | `<rescap:Capability Name="runFullTrust" />` in `library/AppxManifest.xml` | Strictly needed because EisenPo is a native Win32 C++ / Qt 6 app using standard Windows file Open/Save dialogs and `%LOCALAPPDATA%\EisenPo`, running 100% offline. |
| **Clean Uninstallation (Policy 10.2.7)** | Documented in `README.md`, `privacy-policy.md`, and `EULA` | Clean removal via **Windows Settings → Apps** / Start Menu with zero residual background services or drivers. |
| **Accessibility Compliance** | [`MS-Store-ADA-v8.9-FINAL-EN.pdf`](../source/assets/LICENSES/MS-Store-ADA-v8.9-FINAL-EN.pdf) | Supporting accessibility documentation shipped in `LICENSES`. |

### Ready-to-Paste Texts for Partner Center

- **Restricted Capability Justification (`runFullTrust`):**
  > *EisenPo is a native Win32 C++ / Qt 6.12.0 desktop productivity application uniting the Eisenhower Matrix, Pomodoro, Feynman active learning, SQLite-backed Activity Registration, and Gantt & 4D BIM scheduling. It requires `runFullTrust` strictly to run its native Qt Win32 binaries and standard Windows file Open/Save dialogs for local user-selected JSON, CSV, XLSX, PDF, and ICS files. It executes 100% offline with zero background services, zero drivers, and zero telemetry.*

- **Copyright & Trademark Info (`en-US`, max. 200 chars):**
  > `© 2025-2026 Fernando Nillsson Cidade. Open source under GNU AGPLv3; official Microsoft Store binary under author EULA. Qt (LGPLv3), FFmpeg (LGPLv2.1+), SQLite.` *(156 chars)*

- **Suggested Keywords (`en-US`, up to 7 terms — Policy 10.1.3):**
  `Eisenhower Matrix`, `Pomodoro`, `Feynman`, `Gantt Chart`, `Task Manager`, `Productivity`, `Offline`

---

## 6. Auxiliary Scope, Maturity Limitations & Legal Disclaimer ("AS IS")

1. **Auxiliary Tool Scope:** **EisenPo** is intended as an **auxiliary computational tool** for personal productivity, academic organization, active study, activity logging, and schedule planning — and **not** as a sole or definitive legal/engineering instrument for contractual project schedules or regulatory compliance.
2. **Immaturity & Independent Verification:** The software may contain limitations and human errors in its source code, cross-module sync, or scheduling algorithms (Critical Path Method / CPM, working-day calendars, and 4D timeline simulation); critical outputs should be independently verified by qualified professionals.
3. **"AS IS" Supply:** To the maximum extent permitted by applicable law (including Brazilian Law No. 9.609/1998, the Official EULA, and [GNU AGPLv3](../LICENSE) Sections 15–16, preserving non-waivable statutory consumer rights), the software is provided **"AS IS"** without express or implied warranties, and the user retains sole responsibility for professional or production reliance on generated outputs.

</details>
