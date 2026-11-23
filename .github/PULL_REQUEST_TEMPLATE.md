# Modelo de Pull Request / Pull Request Template

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

> [!NOTE]
> **Aviso ao Colaborador / Contributor Notice:**  
> 🇧🇷 *Este repositório é um projeto pessoal mantido pelo autor em ritmo próprio de aprendizado em C++/Qt e produtividade. Pull Requests são bem-vindos, mas a análise e incorporação de código ocorrem de forma voluntária e sem compromisso de prazo. Caso não haja manifestação ou merge, sinta-se plenamente livre para manter e utilizar suas melhorias em seu próprio fork sob a licença GNU AGPLv3.*  
> 🇺🇸 *This repository is a personal project maintained by the author at their own pace of learning in C++/Qt and productivity engineering. Pull Requests are welcome, but review and merging take place voluntarily without any guaranteed timeframe. If there is no response or merge, please feel free to maintain and use your improvements in your own fork under the GNU AGPLv3 license.*

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher o template em português</summary><br>

## Descrição
Descreva sucintamente o objetivo deste PR, os módulos afetados e qual issue ele soluciona.

## Tipo de Alteração
- [ ] Correção de bug no Host (`EisenPo` / Matriz de Eisenhower / Calendário / `UnifiedTaskStore`)
- [ ] Alteração em módulo nativo (`Pomodoro`, `Feynman`, `Agenda`, `Gantt & BIM 4D`)
- [ ] Melhoria de interface, ergonomia, acessibilidade (TDAH/dislexia) ou temas Claro/Escuro
- [ ] Internacionalização e tradução (`source/language/translations/*.ts` -> `.qm`)
- [ ] Build, CMake, presets (`CMakePresets.json`), empacotamento limpo ou Inno Setup
- [ ] Atualização de documentação (`README.md`, `MANUAL.md`, `docs/`, `source/assets/`)

## Verificação e Compilação
- [ ] Testado com MSVC x64 (`cmake --preset host-qt6-msvc-release`) e/ou MinGW-w64 x64 (`cmake --preset host-qt6-mingw-release`)
- [ ] Ferramentas auxiliares verificadas (`mocks-msvc-release` / `mocks-mingw-release`)
- [ ] Traduções validadas com `CompileTranslationsTerminal.exe` (quando houver mudanças em `.ts`)
- [ ] Consistência documental verificada com `find_VersionEditor.exe --check` (quando aplicável)

## Conformidade Arquitetural
- [ ] A aplicação permanece estritamente **100% local e offline**, sem telemetria ou conexões de rede automáticas.
- [ ] A persistência em `%LOCALAPPDATA%\EisenPo` (`tarefas.json`, SQLite `listas.db` / `eisenpo_gantt.db`, configurações) mantém compatibilidade e gravação segura.
- [ ] Textos visíveis ao usuário respondem corretamente à troca dinâmica de idioma (`pt_BR` / `en_US`).

## Issues Relacionadas
Corrige #(número da issue)

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse template in English</summary><br>

## Description
Briefly describe the purpose of this PR, affected modules, and which issue it addresses.

## Type of Change
- [ ] Bug fix in Host (`EisenPo` / Eisenhower Matrix / Calendar / `UnifiedTaskStore`)
- [ ] Change in native module (`Pomodoro`, `Feynman`, `Agenda`, `Gantt & 4D BIM`)
- [ ] UI, ergonomics, accessibility (ADHD/dyslexia), or Light/Dark theme enhancement
- [ ] Internationalization / Localization (`source/language/translations/*.ts` -> `.qm`)
- [ ] Build system, CMake presets (`CMakePresets.json`), clean deployment, or Inno Setup scripts
- [ ] Documentation updates (`README.md`, `MANUAL.md`, `docs/`, `source/assets/`)

## Build & Verification
- [ ] Built with MSVC x64 (`cmake --preset host-qt6-msvc-release`) and/or MinGW-w64 x64 (`cmake --preset host-qt6-mingw-release`)
- [ ] Auxiliary tools verified (`mocks-msvc-release` / `mocks-mingw-release`)
- [ ] Translations validated via `CompileTranslationsTerminal.exe` (if `.ts` files changed)
- [ ] Documentation consistency validated via `find_VersionEditor.exe --check` (if applicable)

## Architectural Compliance
- [ ] The software remains strictly **100% local-first and offline** with zero telemetry or unsolicited network calls.
- [ ] Persistence in `%LOCALAPPDATA%\EisenPo` (`tarefas.json`, SQLite `listas.db` / `eisenpo_gantt.db`, configs) remains safe and backward-compatible.
- [ ] User-facing strings update dynamically on runtime language switches (`pt_BR` / `en_US`).

## Related Issues
Fixes #(issue number)

</details>
