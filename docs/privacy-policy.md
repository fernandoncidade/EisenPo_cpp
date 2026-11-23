# Política de Privacidade / Privacy Policy — EisenPo

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher a Política de Privacidade em português</summary><br>

# POLÍTICA DE PRIVACIDADE — EISENPO

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Desenvolvedor / Controlador:** Fernando Nillsson Cidade (Desenvolvedor Individual / Pessoa Física)  
**E-mail de Contato e Privacidade:** `linceu_lighthouse@outlook.com`  
**Site Oficial do Projeto:** [https://github.com/fernandoncidade/EisenPo_cpp](https://github.com/fernandoncidade/EisenPo_cpp)

---

## 1. Declaração de Independência frente à Política de Privacidade da Microsoft

> [!IMPORTANT]
> **A Microsoft NÃO fornece uma política de privacidade padrão para o EisenPo, e este aplicativo NÃO é coberto por nenhuma política de privacidade da Microsoft.**
> - Esta Política de Privacidade aplica-se exclusiva e integralmente ao software **EisenPo** em todas as suas formas de distribuição (pacote Microsoft Store MSIX [`9P289X0185C3`](https://apps.microsoft.com/detail/9P289X0185C3), instaladores Windows Inno Setup ou compilações independentes a partir do código-fonte).
> - Dados coletados separadamente pela própria Microsoft ao navegar, baixar, instalar, atualizar, desinstalar, avaliar ou publicar comentários na loja **Microsoft Store** ou mediante recursos diagnósticos opcionais do sistema operacional **Windows** são regidos exclusivamente pela [Política de Privacidade da Microsoft](https://www.microsoft.com/pt-br/privacy/privacystatement).
> - Além disso, **a Microsoft não presta suporte técnico ao cliente** pelos usuários do **EisenPo**; dúvidas sobre privacidade ou operação do aplicativo devem ser encaminhadas diretamente ao desenvolvedor por e-mail ou pelo repositório oficial no GitHub.

---

## 2. Arquitetura Local-First e Ausência de Coleta de Dados Pessoais

O **EisenPo** foi projetado para operar **100% localmente e offline** na estação de trabalho do usuário:

1. **Zero Coleta de Dados Pessoais (PII) para Servidores Externos:** O aplicativo **NÃO** coleta remotamente, solicita em nuvem, transmite, vende ou compartilha nome civil, CPF/CNPJ, endereço, telefone, e-mail, localização geográfica (GPS/IP), histórico de navegação ou identificadores publicitários.
2. **Zero Telemetria, Zero Contas e Zero Rede Ativa:** O fluxo operacional do aplicativo não exige login ou criação de contas e não inclui SDKs analíticos, rastreadores, anúncios nem envio em segundo plano. *(Nota técnica transparente: o executável inclui módulos runtime padrão do Qt 6.12.0 — inclusive `Qt6Network.dll` devido ao código fonte auxiliar não instanciado em `source/modules/gantt/Gantt/api/` — e nenhuma conexão externa ou porta de escuta é aberta em tempo de execução.)*
3. **Sem Coleta de Dados de Crianças ou Dados Sensíveis:** Em conformidade com as Políticas da Microsoft Store (Seções 10.5.5 a 10.5.8), o aplicativo não realiza coleta direcionada ou envio remoto de dados financeiros, biométricos, de saúde ou dados pessoais de crianças e adolescentes.

---

## 3. Inventário de Dados Armazenados Localmente no Dispositivo

Todos os dados criados nos cinco módulos integrados permanecem exclusivamente no diretório persistente local do usuário em `%LOCALAPPDATA%\EisenPo\` (com migração automática transparente do caminho legado `%LOCALAPPDATA%\EisenhowerOrganizer\`, ou no contêiner isolado equivalente quando empacotado via MSIX):

| Arquivo / Estrutura Local | Módulo(s) | Conteúdo Armazenado Localmente |
|---|---|---|
| `tarefas.json` | Matriz de Eisenhower, Pomodoro, Feynman, Agenda, Gantt | Base unificada de tarefas (`UnifiedTaskStore`), quadrantes, prioridades, níveis de domínio, datas, horários, durações, ciclos Pomodoro e caminhos de arquivos vinculados. |
| `layout.json` | Interface Host e Gantt & BIM 4D | Preferências visuais de layout (`Tabbed`, `Quadrants`, `2x2`, `PoCa`), visibilidade/ordem de abas e disposição de tabelas do Gantt. |
| `language.json` | Todo o sistema | Código do idioma ativo na interface (`pt_BR` ou `en_US`). |
| `feynman_rotulos_config.json` | Feynman | Rótulos personalizados para as etapas do Método Feynman. |
| `listas.db` | Agenda (Registro de Atividades) | Banco relacional **SQLite** local com cursos, disciplinas, semestres, categorias e agendas hierárquicas. |
| `agenda_config.json` e `agendas/*.json` | Agenda | Modos de cor (`Black` / `Colorful`), últimas seleções e listas customizadas criadas pelo usuário. |
| `eisenpo_gantt.db` | Gantt & BIM 4D | Banco relacional **SQLite** local com EAP/WBS, dependências CPM, linhas de base, calendários/feriados e vínculos de simulação 4D BIM. |
| `recorrencias_config.json`, `recorrencias_estado_original.json`, `lembretes_locais_config.json` | Ferramentas de Produtividade | Parâmetros locais de geração de recorrências e lembretes locais em área de trabalho / bandeja do sistema. |
| `tarefas_gerais_font_config.json`, `arquivo_vinculado_font_config.json` | Tipografia | Preferências locais de fonte para tarefas comuns e tarefas com arquivo vinculado. |
| `logs\file_eisenpo_*.log` | Diagnóstico (`LogManager`) | Logs cronológicos locais de execução e diagnóstico técnico (apenas no dispositivo do usuário). |
| `trial_info.json` e `HKCU\SOFTWARE\EisenPo` *(somente builds de avaliação)* | Controle local de avaliação (`TrialManager`) | Quando uma compilação de avaliação estiver ativa (`LIBERAR_USO_DEFINITIVO = false`), armazena localmente apenas o carimbo de data da primeira execução no registro local do usuário e JSON local, sem qualquer comunicação pela internet. |

- **Importações e Exportações Explicitamente Acionadas pelo Usuário:** A leitura/gravação de arquivos `.json`, `.csv`, `.xlsx`, `.pdf` e `.ics` ou abertura de arquivos vinculados ocorrem **unicamente** nos caminhos selecionados pelo próprio usuário nos diálogos padrão do sistema.

---

## 4. Capacidade Declarada no Manifesto MSIX (`runFullTrust`)

Em conformidade com a Política 10.6 da Microsoft Store, [`library/AppxManifest.xml`](../library/AppxManifest.xml) declara exclusivamente `<rescap:Capability Name="runFullTrust" />`, estritamente necessária porque o **EisenPo** é uma aplicação desktop nativa Win32 em C++ / Qt 6.12.0 que utiliza acesso padrão ao sistema de arquivos do usuário (diálogos Abrir/Salvar e persistência local em `%LOCALAPPDATA%\EisenPo`). Nenhuma permissão de câmera, microfone, localização GPS ou lista de contatos é solicitada.

---

## 5. Direitos do Titular, Controle e Desinstalação Limpa (Política 10.2.7 / LGPD / GDPR)

Em conformidade com a **LGPD (Lei nº 13.709/2018 — art. 7º, V e IX)**, com o **GDPR** e com a **Política 10.2.7 da Microsoft Store**:
- **Soberania e Exclusão Imediata:** O usuário pode editar, exportar (JSON/CSV/XLSX/PDF/ICS), copiar ou apagar livremente seus arquivos em `%LOCALAPPDATA%\EisenPo` a qualquer momento, resultando em exclusão imediata dos dados no dispositivo.
- **Desinstalação Limpa:** Você pode desinstalar o **EisenPo** de forma limpa a qualquer momento por **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou **Menu Iniciar → Desinstalar**), removendo os binários do pacote sem deixar serviços residentes ou agentes em segundo plano.

---

## 6. Contato para Privacidade e Suporte

- **Controlador / Autor:** Fernando Nillsson Cidade
- **E-mail:** `linceu_lighthouse@outlook.com`
- **Issues / Repositório:** [https://github.com/fernandoncidade/EisenPo_cpp/issues](https://github.com/fernandoncidade/EisenPo_cpp/issues)

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse Privacy Policy in English</summary><br>

# PRIVACY POLICY — EISENPO

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Developer / Controller:** Fernando Nillsson Cidade (Individual Developer)  
**Privacy & Support Email:** `linceu_lighthouse@outlook.com`  
**Official Project Website:** [https://github.com/fernandoncidade/EisenPo_cpp](https://github.com/fernandoncidade/EisenPo_cpp)

---

## 1. Independence from Microsoft Privacy Statement

> [!IMPORTANT]
> **Microsoft does NOT provide a default privacy policy for EisenPo, and this application is NOT covered by any Microsoft privacy policy.**
> - This Privacy Policy applies exclusively to **EisenPo** across all distribution channels (Microsoft Store MSIX package [`9P289X0185C3`](https://apps.microsoft.com/detail/9P289X0185C3), Inno Setup Windows installers, or independent builds from source code).
> - Data processed separately by Microsoft when you browse, download, install, update, uninstall, rate, or review applications on the **Microsoft Store**, or via optional **Windows** diagnostic telemetry, is governed strictly by the [Microsoft Privacy Statement](https://www.microsoft.com/en-us/privacy/privacystatement).
> - In addition, **Microsoft does not provide customer or technical support** for **EisenPo**; privacy or application questions must be sent straight to the developer via email or the official GitHub repository.

---

## 2. Local-First Architecture & Zero Personal Data Collection

**EisenPo** is engineered to run **100% locally and offline** on the user's computer:

1. **Zero Personal Data Collection (PII) to External Servers:** The software does **NOT** remotely collect, request, upload, share, or sell legal names, tax/national IDs, physical addresses, phone numbers, email addresses, geolocation (GPS/IP), browsing history, or advertising identifiers.
2. **Zero Telemetry, Zero Accounts & Zero Active Networking:** Application workflows require no login and contain zero analytics SDKs, trackers, ads, or background HTTP traffic. *(Transparent technical note: packaged binaries ship standard Qt 6.12.0 runtime libraries — including `Qt6Network.dll` due to uninstantiated dormant source code in `source/modules/gantt/Gantt/api/` — and zero outbound requests or listening sockets are opened at runtime.)*
3. **No Collection of Children's or Sensitive Data:** Under Microsoft Store Policies (Sections 10.5.5–10.5.8), the application never collects financial, health, biometric, or children's personal data.

---

## 3. Inventory of Locally Stored Data on Your Device

All records produced across all five integrated modules reside strictly inside `%LOCALAPPDATA%\EisenPo\` (automatically migrating legacy `%LOCALAPPDATA%\EisenhowerOrganizer\`, or the equivalent isolated container under MSIX):

| Local File / Structure | Module(s) | Local Content Stored |
|---|---|---|
| `tarefas.json` | Eisenhower Matrix, Pomodoro, Feynman, Agenda, Gantt | Unified task store (`UnifiedTaskStore`), quadrants, priority/mastery levels, dates, times, durations, Pomodoro counts, and linked file paths. |
| `layout.json` | Host UI & Gantt & 4D BIM | Visual layout modes (`Tabbed`, `Quadrants`, `2x2`, `PoCa`), tab order/visibility, and Gantt table splitter arrangements. |
| `language.json` | Application-wide | Active runtime UI language code (`pt_BR` or `en_US`). |
| `feynman_rotulos_config.json` | Feynman | User-customized step labels for the Feynman Method workflow. |
| `listas.db` | Agenda (Activity Registration) | Embedded local **SQLite** database storing subjects, courses, semesters, categories, and hierarchical agendas. |
| `agenda_config.json` & `agendas/*.json` | Agenda | Color modes (`Black` / `Colorful`), recent selections, and custom user schedules. |
| `eisenpo_gantt.db` | Gantt & 4D BIM | Embedded local **SQLite** database storing WBS/EAP trees, CPM dependencies, baselines, work calendars/holidays, and 4D BIM simulation bindings. |
| `recorrencias_config.json`, `recorrencias_estado_original.json`, `lembretes_locais_config.json` | Productivity Suite | Local recurrence rules, restore snapshot, and local desktop/tray reminder settings. |
| `tarefas_gerais_font_config.json`, `arquivo_vinculado_font_config.json` | Typography | Local font preferences for standard tasks and file-linked tasks. |
| `logs\file_eisenpo_*.log` | Diagnostics (`LogManager`) | Local chronological runtime logs strictly on disk inside `%LOCALAPPDATA%\EisenPo\logs\`. |
| `trial_info.json` & `HKCU\SOFTWARE\EisenPo` *(evaluation builds only)* | Local trial check (`TrialManager`) | When an evaluation build is enabled (`LIBERAR_USO_DEFINITIVO = false`), stores only the local first-run timestamp in current-user registry and JSON with zero network access. |

- **Explicit User-Triggered Import & Export:** Reading/writing `.json`, `.csv`, `.xlsx`, `.pdf`, or `.ics` files and opening linked external documents happen **only** at user-selected paths via standard system dialogs.

---

## 4. Capability Declared in the MSIX Manifest (`runFullTrust`)

For Microsoft Store MSIX packaging (Policy 10.6), [`library/AppxManifest.xml`](../library/AppxManifest.xml) declares only `<rescap:Capability Name="runFullTrust" />`, strictly required because **EisenPo** is a native Win32 C++ / Qt 6.12.0 desktop application accessing standard Windows file dialogs and `%LOCALAPPDATA%\EisenPo`. Zero camera, microphone, location, or contacts capabilities are declared or accessed.

---

## 5. Data Subject Rights, Control & Clean Uninstallation (Policy 10.2.7 / LGPD / GDPR)

In compliance with **Brazilian LGPD (Law No. 13.709/2018)**, **GDPR**, and **Microsoft Store Policy 10.2.7**:
- **Full Data Ownership:** Users can inspect, copy, export (`.json`, `.csv`, `.xlsx`, `.pdf`, `.ics`), or delete any file in `%LOCALAPPDATA%\EisenPo` at any time.
- **Clean Uninstallation:** You may cleanly uninstall **EisenPo** at any time via **Windows Settings → Apps → Installed Apps** (or **Start Menu → Uninstall**), removing application binaries completely without leaving background services or kernel drivers behind.

---

## 6. Privacy & Support Contact

- **Controller / Author:** Fernando Nillsson Cidade
- **Email:** `linceu_lighthouse@outlook.com`
- **Support / Issues:** [https://github.com/fernandoncidade/EisenPo_cpp/issues](https://github.com/fernandoncidade/EisenPo_cpp/issues)

</details>
