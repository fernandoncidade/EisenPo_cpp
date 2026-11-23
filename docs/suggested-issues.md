# Sugestões de Issues e Roadmap / Suggested Issues & Roadmap — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher sugestões em português</summary><br>

# EisenPo — Backlog Técnico e Sugestões para Contribuidores

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  

Este documento reúne tópicos técnicos, modernizações de código e melhorias de produtividade e segurança identificadas para orientar colaboradores interessados no **EisenPo**.

---

### [CONCLUÍDA] Issue 1: Renomear Identificador `concept` no Módulo Feynman para Compatibilidade Plena com C++23
- **Área:** `feynman` / `c++23`
- **Status:** **Concluída e Verificada** (MSVC `14.52.37025` e MinGW-w64 `15.1.0`)
- **Descrição:** O identificador `concept` nos 5 arquivos de `source/modules/feynman/Feynman/` (`feynman_metodo_helpers.{hpp,cpp}`, `fey_01_FeynmanWorker.cpp`, `fey_03_setup_ui.cpp` e `fey_07_salvar_conceito_atual.cpp`) foi renomeado para `DescriptionParts::concept_text` e `conceito` (preservando intactas todas as chaves JSON/internas `"concept"`), e `CMAKE_CXX_SCAN_FOR_MODULES OFF` foi definido nos `CMakeLists.txt`.
- **Benefício Alcançado:** Todos os 4 presets ativos (`host-qt6-{msvc,mingw}-release` e `mocks-{msvc,mingw}-release`) compilam limpos em C++23 estrito (MSVC puro via MSBuild / `Visual Studio 18 2026` e MinGW via Ninja).

---

### Issue 2: Criar Suíte de Testes Automatizados (`CTest`) para `CpmEngine`, `WorkCalendar` e `UnifiedTaskStore`
- **Área:** `tests` / `qualidade`
- **Dificuldade:** Média
- **Descrição:** Adicionar um diretório `tests/` e alvos CTest cobrindo cálculo Forward/Backward Pass e folgas do `CpmEngine`, detecção de ciclos em DAG, cálculo de feriados móveis pelo algoritmo de Meeus/Jones/Butcher nos 18 países de `WorkCalendar`, e round-trip atômico de `UnifiedTaskStore`.
- **Benefício:** Regressões algorítmicas passam a ser detectadas automaticamente antes de novos lançamentos.

---

### Issue 3: Isolar ou Remover Código Dormante de API HTTP (`Gantt/api/`) para Eliminar Dependência `Qt6::Network`
- **Área:** `gantt` / `seguranca` / `deploy`
- **Dificuldade:** Baixa / Média
- **Descrição:** `source/modules/gantt/Gantt/api/HttpServer.{hpp,cpp}` e `RestApiController.{hpp,cpp}` nunca são instanciados pelo executável `EisenPo.exe`, mas fazem o alvo vincular `Qt6::Network` (adicionando `Qt6Network.dll` e plugins TLS à distribuição externa). Isolar esses arquivos atrás de uma opção CMake desligada por padrão (ex.: `EISENPO_ENABLE_GANTT_LOCAL_API=OFF`) ou movê-los para ramo experimental limpa ainda mais o pacote final.
- **Benefício:** Redução adicional do tamanho da distribuição externa e eliminação total de superfície de rede vinculada ao binário.

---

### Issue 4: Fixar SHAs de GitHub Actions e Adicionar Verificação Automática de Build no CI
- **Área:** `.github/workflows` / `ci`
- **Dificuldade:** Média
- **Descrição:** Fixar ações por SHA de 40 caracteres no workflow `.github/workflows/update-readme-version.yml` e adicionar fluxo de verificação em PRs rodando `find_VersionEditor --check`, validação de traduções `.ts` -> `.qm` e build automatizado em Windows (`MSVC` e `MinGW`).
- **Benefício:** Maior segurança na cadeia de suprimentos e garantia contínua de sincronia documental.

---

### Issue 5: Interoperabilidade Estendida no Gantt & BIM 4D (MS Project XML e Leitura IFC4 Direta) + Perfis TDAH/Dislexia
- **Área:** `gantt` / `acessibilidade` / `ui`
- **Dificuldade:** Avançada
- **Descrição:** Expandir a exportação/importação CSV atual do módulo Gantt & BIM 4D para suportar troca estruturada via MS Project XML e leitura direta de GUIDs e propriedades temporais de arquivos `.ifc`; paralelamente, oferecer perfis visuais de alto contraste e baixo ruído cognitivo focados em usuários com TDAH e dislexia.
- **Benefício:** Integração direta com fluxos BIM reais de obras/projetos e maior conforto cognitivo no uso diário.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse suggested issues in English</summary><br>

# EisenPo — Technical Backlog & Contributor Suggestions

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  

This document compiles key technical enhancements, code modernization tasks, and security/usability improvements mapped for contributors to **EisenPo**.

---

### [COMPLETED] Issue 1: Rename `concept` Identifier in the Feynman Module for Full C++23 Compatibility
- **Area:** `feynman` / `c++23`
- **Status:** **Completed & Verified** (MSVC `14.52.37025` and MinGW-w64 `15.1.0`)
- **Description:** Renamed the legacy `concept` member/parameter identifier across the 5 files under `source/modules/feynman/Feynman/` (`feynman_metodo_helpers.{hpp,cpp}`, `fey_01_FeynmanWorker.cpp`, `fey_03_setup_ui.cpp`, and `fey_07_salvar_conceito_atual.cpp`) to `DescriptionParts::concept_text` and `conceito` (keeping all JSON/internal `"concept"` literal keys intact), alongside `CMAKE_CXX_SCAN_FOR_MODULES OFF` in both `CMakeLists.txt` files.
- **Outcome:** All 4 active build presets (`host-qt6-{msvc,mingw}-release` and `mocks-{msvc,mingw}-release`) compile cleanly under strict C++23 (pure MSVC via MSBuild / `Visual Studio 18 2026` and MinGW via Ninja).

---

### Issue 2: Introduce Automated `CTest` Unit Test Suite for `CpmEngine`, `WorkCalendar` & `UnifiedTaskStore`
- **Area:** `tests` / `quality`
- **Difficulty:** Medium
- **Description:** Add a `tests/` directory and CTest targets covering `CpmEngine` Forward/Backward passes and float calculations, circular-dependency DAG interception, Meeus/Jones/Butcher movable holidays across all 18 built-in countries in `WorkCalendar`, and atomic `UnifiedTaskStore` round-trips.
- **Benefit:** Automated verification catching scheduling or persistence regressions prior to every release.

---

### Issue 3: Gate or Remove Dormant HTTP API Sources (`Gantt/api/`) to Drop Unused `Qt6::Network` Linkage
- **Area:** `gantt` / `security` / `deploy`
- **Difficulty:** Easy / Medium
- **Description:** `source/modules/gantt/Gantt/api/HttpServer.{hpp,cpp}` and `RestApiController.{hpp,cpp}` are never instantiated by `EisenPo.exe`, yet link `Qt6::Network` into the executable (pulling `Qt6Network.dll` and TLS plugins into external deployments). Gating them behind an off-by-default CMake option (e.g., `EISENPO_ENABLE_GANTT_LOCAL_API=OFF`) or moving them out of the main target makes runtime deployments even cleaner.
- **Benefit:** Leaner external runtime distributions and zero linked networking libraries.

---

### Issue 4: Pin GitHub Actions by Commit SHA & Add Automated PR Verification CI
- **Area:** `.github/workflows` / `ci`
- **Difficulty:** Medium
- **Description:** Pin workflow actions in `.github/workflows/update-readme-version.yml` to immutable 40-char commit SHAs and add PR checks executing `find_VersionEditor --check`, `.ts` -> `.qm` translation checks, and Windows (`MSVC` / `MinGW`) build verification.
- **Benefit:** Stronger CI supply-chain hygiene and continuous documentation synchronization.

---

### Issue 5: Extended Gantt & 4D BIM Interoperability (MS Project XML & Direct IFC4 Parsing) + ADHD/Dyslexia Visual Profiles
- **Area:** `gantt` / `accessibility` / `ui`
- **Difficulty:** Advanced
- **Description:** Extend the current Gantt CSV exchange with MS Project XML support and direct IFC4 element/GUID extraction for 4D simulations, alongside selectable low-noise, high-readability focus presets tailored for ADHD and dyslexia accessibility.
- **Benefit:** Seamless integration with real-world BIM authoring tools and improved cognitive ergonomics.

</details>
