# MANUAL - EisenPo

**Versão documentada / Documented version:** `2026.11.22.0`  
**Data / Date:** `22 de novembro de 2026` / `November 22, 2026`  
**Ambiente Tecnológico / Tech Stack:** C++23 | Qt 6.12.0 Widgets / Multimedia / Sql | SQLite (`listas.db`, `eisenpo_gantt.db`)  
**Plataforma / Platform:** Windows 11 / Windows 10 x64 (`MSVC 14.52` & `MinGW-w64 15.1.0`)  
**Autor / Author:** Fernando Nillsson Cidade (`linceu_lighthouse@outlook.com`)  
**Repositório Oficial / Official Repository:** https://github.com/fernandoncidade/EisenPo_cpp  
**Microsoft Store:** https://apps.microsoft.com/detail/9P289X0185C3  

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">Português (BR)</a> |
  <a href="#enus">English (US)</a>
</p>

---

## <a id="ptbr"></a>Português (Brasil)

> **Duplo Licenciamento, Privacidade e Isenção ("AS IS"):**
> - **Código Aberto (GitHub) vs. Pacote Oficial Microsoft Store:** Todo o código-fonte deste repositório é licenciado sob **GNU Affero General Public License v3 (`GNU AGPLv3`, [`LICENSE`](LICENSE))**, enquanto o pacote binário oficial na Microsoft Store (`9P289X0185C3`) é regido pelo **[EULA Oficial](source/assets/eisenhower/EULA/EULA_pt_BR.txt)** e **[CLC](source/assets/eisenhower/CLC/CLC_pt_BR.txt)**; desenvolvedores possuem **liberdade total** para adaptar a documentação em `source/assets/`. Veja [`docs/licensing.md`](docs/licensing.md#ptbr).
> - **Independência da Microsoft & Desinstalação Limpa:** **A Microsoft NÃO fornece política de privacidade nem suporte ao cliente para o EisenPo.** Toda persistência ocorre **100% offline** em `%LOCALAPPDATA%\EisenPo` ([`docs/privacy-policy.md`](docs/privacy-policy.md#ptbr)), com remoção limpa pelas **Configurações do Windows → Aplicativos**.
> - **Escopo Auxiliar ("AS IS"):** Aplicativo gratuito destinado ao auxílio na produtividade, estudo e planejamento de cronogramas (fornecido `"AS IS"` nos termos do EULA Oficial, Lei nº 9.609/1998 e GNU AGPLv3 Seções 15–16); cronogramas contratuais e decisões críticas exigem conferência independente.

Este manual descreve o comportamento operacional e técnico do **EisenPo** (Eisenhower Organizer). O objetivo é detalhar como utilizar todos os recursos da aplicação, atalhos, configurações e como os módulos integrados se comportam no ambiente de trabalho. *(Veja também o resumo ilustrado em [`docs/user-guide.md`](docs/user-guide.md#ptbr).)*

## 1. Visão geral

O **EisenPo** é um ecossistema desktop completo para organização estratégica de tarefas, foco em execução, aprendizado estruturado e acompanhamento de atividades avaliativas.

Ele reúne a Matriz de Eisenhower e todos os módulos complementares de produtividade integrados nativamente no executável principal:

- `Eisenhower`: planejamento e priorização de tarefas por importância e urgência (Host integrado).
- `Pomodoro`: execução focada de tarefas com blocos de trabalho, pausas curtas/longas e alarmes sonoros configuráveis.
- `Feynman`: estudo ativo por conceitos, explicações simplificadas, lacunas e sínteses finais.
- `Registro de Atividades`: cadastro avaliativo detalhado, listas hierárquicas e agendas com criação de tarefas sincronizadas.
- `Gantt & BIM 4D`: gestão profissional de cronogramas, EAP/WBS hierárquica, análise de caminho crítico (CPM), linha do tempo horizontal interativa e simulação construtiva 4D vinculada a elementos IFC.

Todos os módulos fazem parte permanente e nativa da aplicação, operando diretamente sem a necessidade de DLLs externas ou pastas de plugins adicionais.

---

### 1.1. NOTAS DE LANÇAMENTO: Versão 2026.11.22.0

Data: 22 de novembro de 2026

1. **Padronização Cromática Unificada de Status na Tabela WBS e no Diagrama de Gantt**:
   - **Identidade Visual Integrada com a Simulação 4D**:
     - Todas as atividades listadas na "Tabela WBS" e representadas visualmente no "Diagrama de Gantt" agora compartilham rigorosamente a mesma paleta de cores aplicada aos indicadores de status do quadro "Simulação Temporal - 4D BIM" vinculado à tabela "Simulação 4D".
     - `✅ Executado:` Verde (`#4caf50`) para atividades totalmente concluídas (100% de avanço físico).
     - `🔄 Em Andamento:` Azul (`#2196f3`) para atividades atualmente em execução dentro do prazo planejado.
     - `⚠️ Atrasado:` Vermelho (`#f44336`) para atividades com início em atraso (quando a data atual ultrapassa o início planejado sem que a tarefa tenha sido iniciada) ou atraso no término (quando a data atual ultrapassa a data de conclusão prevista sem que a atividade tenha sido finalizada).
     - `⏳ Futuro:` Cinza neutro (`#9e9e9e`) para atividades agendadas que ainda não atingiram sua data de início.
   - **Harmonização nas Caixas e Barras de Atividades no Gantt**:
     - As caixas ou nós de atividades na linha do tempo horizontal refletem a mesma cromática de status. Para atividades concluídas (`✅ Executado:`), foi calibrada uma tonalidade verde suave e refinada com contraste equilibrado, diferenciando com clareza e elegância as tarefas finalizadas daquelas ainda em andamento ou atrasadas.

2. **Tooltips Informativos Ricos, Detalhados e Retraduzidos Dinamicamente na Linha do Tempo**:
   - **Quadro Completo ao Posicionar o Mouse**:
     - Ao posicionar o cursor do mouse sobre qualquer caixa ou nó de atividade na linha do tempo do Gantt, um tooltip informativo estilizado exibe instantaneamente um resumo estruturado da atividade:
       - Código identificador da EAP/WBS;
       - Badge colorido com o status operacional (`Executado`, `Em Andamento`, `Atrasado` ou `Futuro`);
       - Datas de Início e Término formatadas de acordo com o padrão regional ativo (`dd/MM/yyyy HH:mm` em português e `MM/dd/yyyy HH:mm` em inglês);
       - Duração total em dias inteiros;
       - Progresso físico percentual concluído;
       - Condição do Caminho Crítico destacada (`SIM` em vermelho para atividades críticas ou `Não` em cinza para atividades com folga);
       - Folga total disponível em horas.
   - **Retradução Dinâmica em Tempo Real**:
     - Ao alternar o idioma do sistema em `Configurações -> Idioma`, todos os textos, rótulos, formatos de data e indicadores dos tooltips são atualizados imediatamente em tempo de execução, garantindo localização perfeita sem necessidade de reiniciar.

3. **Ampliação do Utilitário de Versão para Documentos Contratuais da Agenda**:
   - O editor de versões interno agora gerencia de forma completa e automatizada todos os 12 documentos de termos, licenças, avisos legais e políticas de privacidade vinculados ao módulo de Registro de Atividades e Agenda (`plugin_agenda`) nos idiomas Português e Inglês.

4. **Como Consultar Duplo Licenciamento, Privacidade e Notas na Janela "Opções → Sobre" (`Ctrl+Shift+A`)**:
   - Abra `Opções → Sobre` (ou pressione `Ctrl+Shift+A`) para acessar a central completa de transparência e documentação legal do aplicativo.
   - Alterne entre as abas principais **Histórico**, **Detalhes**, **Licenças**, **Contrato**, **Direitos Autorais**, **Avisos**, **Política de Privacidade** e **Notas de versão**.
   - Dentro das seções documentais com sub-abas, navegue individualmente entre **Geral**, **Matriz Eisenhower**, **Pomodoro**, **Feynman**, **Registro de Atividades** e **Gantt & BIM 4D**, além de utilizar a barra **Buscar:** integrada com setas de resultado anterior/próximo e os links oficiais em **Licenças**.

5. **Como Utilizar o Manual Interativo com Busca Rápida (`Opções → Manual` / `Ctrl+Shift+M`)**:
   - Abra `Opções → Manual` (ou pressione `Ctrl+Shift+M`) para abrir o guia interativo.
   - Utilize a lista de seções à esquerda ou os links clicáveis do **Índice** para ir diretamente ao tópico desejado.
   - Digite qualquer palavra-chave no campo **Buscar:** (`Buscar texto no manual...`) no topo da janela e pressione `Enter` ou clique nos botões **Resultado anterior** / **Próximo resultado** para percorrer todos os trechos destacados em tempo real.

6. **Estabilidade Contínua no Método Feynman e Fluxo Integrado**:
   - As cinco etapas do Método Feynman (**Conceito**, **Explique com Simplicidade**, **Lacunas Identificadas**, **Revisão e Simplificação Final** e **Descrição** — personalizáveis em `Configurações → Editar Rótulos`) permanecem preservadas e sincronizadas em duas vias com as tarefas da Matriz de Eisenhower e do Pomodoro.

7. **Operação 100% Local/Offline e Desinstalação Limpa**:
   - Todos os dados de tarefas, agendas, cronogramas, configurações e logs ficam armazenados localmente em `%LOCALAPPDATA%\EisenPo`, sem telemetria, sem contas online e com remoção limpa pelas **Configurações do Windows → Aplicativos**.

---

### 1.2. NOTAS DE LANÇAMENTO: Versão 2026.9.27.0

Data: 27 de setembro de 2026

1. **Unificação Nativa dos Módulos no Executável Principal**:
   - **Integração Completa e Definitiva**:
     - Os quatro módulos complementares de produtividade (`Pomodoro`, `Feynman`, `Registro de Atividades / Agenda` e `Cronograma de Gantt & BIM 4D`) agora são alvos de compilação direta integrados nativamente ao executável principal do EisenPo.
     - Não há mais dependência de arquivos `.dll` dinâmicos externos (`PomodoroPlugin.dll`, `FeynmanPlugin.dll`, `AgendaPlugin.dll`, `GanttPlugin.dll`) nem da pasta `plugins/` para o funcionamento destes módulos.
   - **Como Utilizar os Módulos Nativos**:
     - *Menu Ferramentas*: Acesse imediatamente `Ferramentas -> Registro de Atividades` e `Ferramentas -> Cronograma de Gantt BIM 4D`. As janelas abrem instantaneamente ao clique do mouse sem tempo de carregamento de bibliotecas externas.
     - *Alternância de Modos Visuais*: Em `Configurações -> Layout de Tarefas -> Interface do Usuário`, alterne com total fluidez entre os ambientes `EisenPo`, `Pomodoro`, `Feynman` e o layout integrado `PoCa` (Pomodoro + Calendário Duplo).
     - *Gerenciador de Plugins Atualizado*: Ao abrir `Ferramentas -> Gerenciador de plugins`, os módulos nativos são reportados com status permanente de integração e prontidão operacional no núcleo do sistema.
   - **Confiabilidade e Desempenho no Windows**:
     - Eliminação completa de riscos de bloqueio acidental por antivírus, falhas de caminho de biblioteca ou descompassos de versões entre executáveis e extensões.

2. **Centralização e Unificação Completa dos Catálogos de Tradução**:
   - **Repositório Centralizado de Traduções**:
     - Todos os arquivos de tradução (`.ts` e `.qm`) de todos os módulos (`eisenhower_organizer`, `pomodoro`, `feynman`, `agenda` e `gantt`) foram centralizados e unificados na pasta oficial `source/language/translations/`.
     - O processo de compilação e empacotamento agora gera e copia automaticamente todos os arquivos `.qm` diretamente para o diretório `translations/` do executável, assegurando que 100% dos textos estejam sempre presentes e alinhados.
   - **Como Utilizar a Tradução Dinâmica**:
     - Abra `Configurações -> Idioma` e selecione `Português (Brasil)` ou `English (United States)`.
     - Todos os textos de todas as janelas principais, diálogos auxiliares, tabelas de cronograma, mensagens de aviso e botões de produtividade atualizam-se instantaneamente em tempo de execução sem requerer a reinicialização da aplicação.
   - **Ferramentas de Compilação de Traduções Atualizadas**:
     - Os utilitários `CompileTranslationsTerminal` e `CompileTranslationsGUI` contam com suporte aprimorado para compilação modular (`--module <id>`) ou compilação global (`--all`), garantindo consistência total na geração de arquivos de idiomas.

3. **Persistência Assíncrona e Motor de Dados Concorrente Sem Bloqueio da Interface**:
   - **Gravação Silenciosa e Rápida**:
     - O salvamento das tarefas no repositório persistente unificado (`tarefas.json`) e a sincronização em tempo real com o Cronograma de Gantt operam com gravação atômica assíncrona, eliminando qualquer micro-congelamento visual ao adicionar, editar, arrastar ou concluir tarefas.
   - **Sincronização Contínua**:
     - Todas as alterações realizadas na Matriz de Eisenhower (datas, durações, prioridades e ciclos Pomodoro concluídos) refletem-se instantaneamente no gráfico de Gantt e na régua temporal, mantendo o cálculo de progresso físico discreto sempre exato.

---

### 1.3. NOTAS DE LANÇAMENTO: Versão 2026.9.26.0

Data: 26 de setembro de 2026

1. **Persistência Completa do Layout das Tabelas do Cronograma Gantt & BIM 4D no Arquivo `layout.json`**:
   - **Disposições de Layout Disponíveis**:
     - Através do menu superior da janela do cronograma (`Ferramentas -> Cronograma de Gantt BIM 4D`), no caminho `Configurações -> 📐 Layout -> 📊 Tabelas`, o usuário pode alternar a qualquer momento entre 5 modos geométricos de exibição:
       1. `Original`: divisão horizontal equilibrada (50%/50%) entre a Tabela WBS e o Diagrama de Gantt na metade superior e o painel de Simulação 4D na base inferior;
       2. `Vertical Topo`: Tabelas "WBS" e "Simulação 4D" posicionadas lado a lado na metade superior da janela, com o "Diagrama de Gantt" ocupando toda a metade inferior;
       3. `Vertical Inferior`: "Diagrama de Gantt" ocupando a metade superior da janela, com as tabelas "WBS" e "Simulação 4D" lado a lado na metade inferior;
       4. `Horizontal Esquerda`: Tabelas "WBS" e "Simulação 4D" empilhadas verticalmente no painel esquerdo, e o "Diagrama de Gantt" ocupando todo o painel direito;
       5. `Horizontal Direita`: "Diagrama de Gantt" ocupando todo o painel esquerdo, e as tabelas "WBS" e "Simulação 4D" empilhadas verticalmente no painel direito.
   - **Gravação Segura e Persistente (`layout.json`)**:
     - Cada seleção realizada pelo usuário é salva imediatamente no arquivo de configuração do sistema `C:\Users\<usuario>\AppData\Local\EisenPo\layout.json` sob a propriedade `"table_layout_mode"`;
     - A gravação é feita de maneira atômica e não-destrutiva (`QSaveFile`), preservando e mesclando todas as demais preferências já gravadas pelo usuário na aplicação (como as opções de layout de tarefas da tela principal);
   - **Restauração Automática na Inicialização**:
     - Ao iniciar a aplicação e abrir a janela "EisenPo − Gestão de Cronograma, Gantt & BIM 4D", o software lê automaticamente o arquivo `layout.json` e restabelece a disposição de tabelas escolhida, posicionando divisores e checkmarks de menu sem necessidade de reconfiguração manual.

2. **Abertura Imediata e Desempenho Instantâneo em Todas as Ferramentas de Produtividade**:
   - **Eliminação de Travamentos e Esperas**:
     - Todos os recursos disponíveis no menu `Ferramentas`:
       - `Registro de Atividades` (avaliações e agendas acadêmicas);
       - `Cronograma de Gantt BIM 4D` (gestão física e simulação construtiva);
       - `Gerar próximas recorrências` (criação em lote de repetições);
       - `Ativar lembretes locais` (notificações locais e avisos na bandeja);
       - `Criar revisões espaçadas` (retenção e fixação de aprendizado);
     - Passam a abrir de forma instantânea ao clique do mouse, sem nenhum congelamento da interface principal.
   - **Extração de Dados em Memória $O(1)$ sem I/O Redundante em Disco**:
     - O sistema extrai a coleção unificada de tarefas diretamente das estruturas de dados e caches em memória, eliminando ciclos forçados de salvamento e recarregamento desnecessário de arquivos no disco rígido.
   - **Construção Visual Protegida e Prévia em Passagem Única**:
     - As janelas de diálogo (como *Recorrências* e *Lembretes Locais*) protegem a emissão de sinais durante sua montagem e renderizam a prévia e as tabelas em passagem única, sem recalcular layouts repetidamente.
   - **Sincronização Condicional Inteligente do Cronograma Gantt**:
     - Quando a janela do Gantt estiver minimizada ou oculta, sincronizações de tarefas em segundo plano evitam atualizações visuais desnecessárias, aplicando a reconstrução completa apenas quando a janela for trazida para visualização.
   - **Resolução Nativa de Bibliotecas Dinâmicas do Windows (DLLs e SQLite)**:
     - O ecossistema de bibliotecas e plugins de banco de dados (como o driver `qsqlite.dll`) agora segue o fluxo nativo de resolução do sistema operacional, garantindo carregamento rápido e estável em qualquer ambiente Windows.

---

### 1.4. NOTAS DE LANÇAMENTO: Versão 2026.9.25.0

Data: 25 de setembro de 2026

1. **Cálculo Físico Discreto de Progresso no Cronograma Gantt por Ciclos Pomodoro**:
   - O avanço percentual (%) das tarefas vinculadas ao cronograma agora é calculado de modo rigoroso e proporcional com base nos blocos Pomodoro executados em relação à meta planejada:
     $$\text{Progresso Físico} = \left(\frac{\text{Pomodoros Concluídos}}{\text{Meta de Pomodoros}}\right) \times 100\%$$
   - **Degraus Discretos e Previsíveis**:
     - Para uma meta de 4 pomodoros: os degraus de progresso avançam estritamente em **0%**, **25%**, **50%**, **75%** e **100%**;
     - Para uma meta de 5 pomodoros: os degraus de progresso avançam estritamente em **0%**, **20%**, **40%**, **60%**, **80%** e **100%**;
     - Tarefas com qualquer outra meta inteira (1 a 999 ciclos) progridem proporcionalmente de forma exata.
   - **Eliminação do Avanço Arbitrário Inicial de 10%**:
     - Foi totalmente removido o avanço arbitrário de 10% que anteriormente era atribuído ao vincular ou iniciar uma tarefa;
     - A atividade parte rigorosamente de **0%** de progresso físico quando nenhum ciclo foi executado.
   - **Conclusão Integral da Atividade**:
     - A tarefa alcança 100% de progresso físico quando todos os pomodoros previstos na meta são concluídos ou quando o usuário marca a tarefa como concluída na Matriz de Eisenhower.
2. **Sincronização Instantânea em Tempo Real com o Cronograma Gantt**:
   - Qualquer modificação realizada nas tarefas na interface principal (edição de descrição, prazos, prioridade, movimentação entre quadrantes, conclusão ou avanço de pomodoros) é propagada imediatamente e em tempo real para a janela do cronograma Gantt, sem exigir ações manuais ou recarregamentos.
3. **Tradução Dinâmica Completa em Tempo de Execução das Janelas de Ferramentas**:
   - **Janela "Gerar próximas recorrências"** (`Ferramentas -> Gerar próximas recorrências`):
     - Tradução instantânea ao vivo de todos os botões de ação: `"Restaurar padrão"`, `"Gerar ocorrências"`, `"Restaurar tarefas anteriores"`, `"Salvar definições"` e `"Fechar"`;
     - Tradução em tempo de execução da mensagem indicativa na tabela de prévia vazia: `"Nenhuma tarefa pronta para gerar ocorrências com as configurações atuais."`;
     - Formatação de datas localizada dinamicamente conforme o idioma e localidade ativa (`QLocale`).
   - **Janela "Ativar lembretes locais"** (`Ferramentas -> Ativar lembretes locais`):
     - Tradução instantânea ao vivo de todos os botões de ação: `"Limpar ativos"`, `"Restaurar padrão"`, `"Aplicar lembretes"`, `"Salvar definições"`, `"Fechar"` e `"Testar aviso"`;
     - Tradução dinâmica do título do quadro de opções `"Quando alertar"`;
     - Tradução dinâmica dos botões de rádio `"Usar data e horário de cada tarefa"` e `"Definir um único horário personalizado"`, além de todas as opções de notificação e seleção.
   - **Janelas "Fluxo integrado de estudo", "Templates" e "Criar revisões espaçadas"**:
     - Atualização instantânea de todos os botões, categorias, campos de entrada, descrições, tabelas e mensagens de retorno ao alternar o idioma do aplicativo entre Português e Inglês, sem necessidade de reiniciar.

---

### 1.5. NOTAS DE LANÇAMENTO: Versão 2026.9.23.0

Data: 23 de setembro de 2026

1. **Inicialização Instantânea Sem Congelamentos**:
   - Abertura imediata do aplicativo com liberação imediata dos controles manuais e do cursor do mouse, eliminando completamente a lentidão e o congelamento de vários segundos que ocorria ao iniciar o software.
   - Carregamento sob demanda (*lazy initialization*) dos motores multimídia e de áudio, mantendo o loop de eventos principal livre para resposta imediata.
2. **Novo Layout Integrado PoCa (Pomodoro + Calendário Duplo)**:
   - Integração direta em uma única coluna lateral à esquerda da Matriz de Eisenhower através do menu `Configurações -> Layout de Tarefas -> Interface do Usuário -> PoCa -> Ativar` (ou `Desativar`).
   - Empilhamento harmonioso de 4 quadros na coluna lateral esquerda:
     1. Quadro **"Temporizador"** (Pomodoro);
     2. Quadro **"Configurações"** (Pomodoro);
     3. Quadro **"Calendário 1"** (mês atual de referência);
     4. Quadro **"Calendário 2"** (mês subsequente sequencial).
   - **Espaçamento Rente Vertical Uniforme**: O segundo calendário posiciona-se rente logo abaixo do primeiro com espaçamento uniforme de 6px (idêntico à distância entre Configurações e o primeiro calendário), eliminando vãos visuais excessivos.
3. **Alinhamento Simétrico das Molduras dos 4 Quadros**:
   - As bordas laterais externas (esquerda e direita) das molduras dos quatro quadros ("Temporizador", "Configurações", "Calendário 1" e "Calendário 2") possuem alinhamento milimétrico simétrico absoluto entre si, formando uma coluna esteticamente perfeita.
4. **Sequenciamento Dinâmico $2 \times K$ de Meses na Replicação Horizontal**:
   - Ao arrastar a barra divisória vertical (*splitter*) para a direita, os calendários se replicam horizontalmente em novas colunas de modo matricial contínuo ($2 \times K$):
     - **Linha 1** (superior): exibe os meses em progressão horizontal da esquerda para a direita ($M, M+1, M+2, \dots$);
     - **Linha 2** (inferior): continua a progressão sequencial a partir do término da Linha 1 ($M+K, M+K+1, M+K+2, \dots$).
     - *Exemplo com 2 colunas ($2 \times 2 = 4$ calendários)*:
       - Linha 1: Calendário 1 = Mês de Setembro; Calendário 3 (ao lado) = Mês de Outubro.
       - Linha 2: Calendário 2 (abaixo do 1) = Mês de Novembro; Calendário 4 (abaixo do 3) = Mês de Dezembro.
     - A dinâmica estende-se automaticamente para 3 ou mais colunas conforme o espaço ampliado.
5. **Proteção e Dimensionamento Preventivo da Barra de Rolagem**:
   - Ao abrir o aplicativo nas dimensões padrão mínimas com o layout PoCa ativo, a largura útil da coluna lateral reserva margem de segurança para a barra de rolagem vertical, impedindo que a moldura direita de qualquer um dos quatro quadros seja sobreposta ou coberta pela barra de rolagem.
   - Proporção calibrada com os quadrantes da Matriz de Eisenhower, garantindo legibilidade integral e fluidez visual imediata.

---

### NOTAS DE LANÇAMENTO: Versão 2026.9.21.0

Data: 21 de setembro de 2026

1. **Resposta Instantânea e Sem Esperas ao Aplicar Edições de Tarefas**:
   - Resposta imediata ao clicar no botão **"OK"** da janela **"✏️ Editar Tarefa"**: a confirmação de sucesso é fechada no mesmo instante, sem reter o cursor, congelar a interface ou impor qualquer atraso para o usuário retomar o trabalho.
   - Desacoplamento do salvamento em disco e do recálculo do cronograma: as alterações são gravadas de forma assíncrona em segundo plano, liberando imediatamente os controles manuais da tela.
2. **Exibição Completa de Tarefas no Calendário com Status `[Em Progresso]`**:
   - O quadro **"Lista Global de Tarefas:"** e os balões informativos de datas (*tooltips*) no Calendário agora reportam fielmente as atividades ativas em andamento com a etiqueta `[Em Progresso]`, harmonizando o ciclo de vida completo de tarefas junto a `[Concluída]` e `[Pendente]`.
   - Visualização detalhada do esforço despendido: exibe a quantidade de blocos pomodoro concluídos e o total planejado em cada tarefa.
   - Interatividade rápida e contextual: clique duplo para abrir arquivos externos vinculados e menu de contexto com botão direito para gerenciar a tarefa diretamente.
3. **Tradução Dinâmica Instantânea em Tempo de Execução**:
   - Retradução imediata de todas as etiquetas do Calendário (`[Em Progresso]` e `[In Progress]`), balões de ajuda e textos operacionais ao trocar o idioma entre Português e Inglês, sem necessidade de reiniciar a aplicação.

---

### NOTAS DE LANÇAMENTO: Versão 2026.9.20.0

Data: 20 de setembro de 2026

1. **Meta de Ciclos Pomodoro Individual por Tarefa (🎯) e Cálculo Preciso de Progresso**:
   - Definição personalizada da quantidade de ciclos de foco planejados para cada tarefa (de 1 a 999) diretamente no novo campo **"Meta de Ciclos Pomodoro (🎯):"** da janela **"✏️ Editar Tarefa"** (na Matriz de Eisenhower e no módulo Pomodoro).
   - Eliminação de qualquer dependência de valores fixos globais (como 4 ciclos): o percentual de conclusão é calculado rigorosamente pela fórmula $\text{Progresso} = (\text{Ciclos Concluídos} / \text{Meta de Ciclos}) \times 100\%$, sem atingir 100% prematuramente nem travar em valores incorretos.
   - Uma tarefa só transita automaticamente para Concluída ("Done") ao atingir integralmente a sua própria meta de ciclos de foco.
2. **Janela Interativa "⚠️ Aviso de Metas de Ciclos" ao Iniciar o Timer Pomodoro**:
   - Ao clicar para iniciar o temporizador Pomodoro (`toggle_timer`) com tarefas na coluna "Em Progresso", o aplicativo verifica se alguma tarefa possui meta divergente da quantidade de ciclos da sessão atual do timer (`Ciclo 1/N`).
   - Caso haja divergências, a janela **"⚠️ Aviso de Metas de Ciclos"** é exibida automaticamente com uma tabela analítica de 5 colunas:
     - `Tarefa`: descrição da atividade em andamento;
     - `Módulo / Origem`: identificação clara da origem da tarefa (`🍅 Pomodoro`, `🏛️ Eisenhower` ou `🧠 Feynman`);
     - `Meta (🎯)`: meta individual de ciclos configurada na tarefa;
     - `Ciclo Timer (⚙️)`: quantidade de ciclos configurada no temporizador;
     - `Comportamento no Fluxo`: explicação textual do que acontecerá (se concluirá antes do término do timer ou se permanecerá em andamento ao fim da sessão).
   - Três botões de ação ágeis com dicas informativas (*tooltips*):
     - `🔄 Redefinir todas para N ciclos`: ajusta a meta de todas as tarefas da lista para coincidir com os ciclos do timer e inicia a contagem imediatamente, sincronizando em tempo real as tarefas no Pomodoro e suas irmãs no EisenPo;
     - `▶️ Iniciar mesmo assim`: inicia o temporizador mantendo as metas individuais de cada tarefa, respeitando o ritmo próprio de conclusão de cada uma;
     - `❌ Cancelar`: cancela o acionamento mantendo todas as configurações inalteradas.
3. **Tradução Dinâmica Completa e Retradução ao Vivo de Janelas Abertas**:
   - Suporte completo a localização dinâmica bilíngue (`pt_BR` e `en_US`) para todos os novos controles, mensagens de aviso de metas, rótulos e colunas.
   - Retradução instantânea da janela "✏️ Editar Tarefa" em tempo de execução: ao alternar o idioma do aplicativo no menu principal com o diálogo já aberto na tela, todos os textos e botões são atualizados ao vivo sem necessidade de fechar ou reabrir a janela.
   - Atualização dinâmica de todos os textos informativos (*tooltips*) de tarefas na Matriz de Eisenhower, na coluna Doing do Pomodoro e na Lista Global.
4. **Sincronização Bidirecional Contínua dos Calendários Extras**:
   - No painel lateral do Calendário (quando configurado para múltiplos meses), a navegação pelas setas de avançar e retroceder mês em qualquer calendário secundário (2º, 3º mês, etc.) recalcula e sincroniza automaticamente todos os demais meses exibidos de forma encadeada e contínua.
5. **Otimização de Desempenho no Arraste e Solte (Drag-and-Drop) de Tarefas**:
   - Eliminação de chamadas redundantes e síncronas de escrita em disco por item durante a movimentação de tarefas entre quadrantes e listas de status, tornando o reposicionamento múltiplo ágil, suave e sem congelamentos na interface.
6. **Abertura Ultrarrápida com Sincronização Sob Demanda no Cronograma Gantt & BIM 4D**:
   - Abertura imediata ($O(1)$) da janela do cronograma quando nenhuma tarefa foi modificada desde a última sessão, executando reconciliação completa apenas quando alterações reais forem detectadas.

---

### NOTAS DE LANÇAMENTO: Versão 2026.9.14.0

Data: 14 de setembro de 2026

1. **Configuração de Layout das Tabelas e do Diagrama (`Configurações > Layout > Tabelas`)**:
   - Submenu dedicado na barra de menus da janela "EisenPo − Gestão de Cronograma, Gantt & BIM 4D" com cinco modos de organização visual das tabelas e do diagrama:
     - `Original`: divisão horizontal proporcional 50%/50% entre a Tabela WBS e o Diagrama de Gantt na parte superior, e painel de Simulação 4D na parte inferior;
     - `Vertical Topo`: Tabelas "Tabela WBS" e "Simulação 4D" dispostas lado a lado (50%/50% horizontal) no topo da tela, com o "Diagrama de Gantt" na parte inferior;
     - `Vertical Inferior`: "Diagrama de Gantt" na parte superior da tela, com as tabelas "Tabela WBS" e "Simulação 4D" dispostas lado a lado (50%/50% horizontal) na parte inferior;
     - `Horizontal Esquerda`: Tabelas "Tabela WBS" e "Simulação 4D" empilhadas verticalmente (50%/50%) no lado esquerdo da tela, e o "Diagrama de Gantt" ocupando todo o lado direito;
     - `Horizontal Direita`: "Diagrama de Gantt" ocupando todo o lado esquerdo da tela, e as tabelas "Tabela WBS" e "Simulação 4D" empilhadas verticalmente (50%/50%) no lado direito.
   - Seleção com marca exclusiva (*checkmark*) e persistência automática da preferência do usuário em `QSettings`.
2. **Divisão Proporcional Meio-a-Meio (50% / 50%) entre WBS e Gantt**:
   - Alocação equilibrada do espaço horizontal: a Tabela WBS e o Diagrama de Gantt dividem rigorosamente meio-a-meio (50%/50%) a largura útil disponível, assegurando visualização confortável para os dados analíticos e para a régua temporal.
3. **Parentesco de Janelas de Diálogo e Suporte a Múltiplas Telas (Multi-Monitor)**:
   - Todas as janelas de diálogo, confirmações de exclusão de atividades e mensagens do cronograma passam a ser vinculadas e centralizadas diretamente sobre a janela do Gantt.
   - Ao excluir atividades da janela do cronograma em monitores secundários, a caixa de diálogo abre sobre a respectiva janela sem puxar a tela principal do EisenPo para frente.
   - Sincronização segura de exclusão bidirecional preservada com a Matriz de Eisenhower e Pomodoro.
4. **Persistência Estrita da Linha Fantasma (`👁️ Linha Fantasma`)**:
   - Correção na projeção da Linha de Base no gráfico de Gantt: a criação de novas tarefas no EisenPo ou a adição de novas atividades no cronograma não mais oculta ou cancela a renderização da Linha Fantasma.
5. **Barra de Controle da Simulação 4D Adaptativa em 3 Modos (`SingleLine`, `TwoLines`, `ThreeLines`)**:
   - Reorganização dinâmica dos controles ao arrastar divisores verticais (*splitters*):
     - Modo Linha Única (largura >= 1200px): botões, Data de Referência, controle deslizante e badges em linha única;
     - Modo Duas Linhas (largura entre 780px e 1200px): botões e Data de Referência na primeira linha, e os badges de status ("Total:", "Executado:", "Em Andamento:", "Atrasado:", "Futuro:") na linha inferior;
     - Modo Três Linhas (largura < 780px): primeira linha com botões ("▶️ Iniciar Simulação", "⏹️ Parar Simulação", "🔄 Reiniciar Simulação"), segunda linha com Data de Referência e barra deslizante, e terceira linha com os badges de status.
   - Ajuste dinâmico automático da altura do container (~58px, ~84px, ~114px) impedindo vazamentos ou sobreposição sobre a tabela.
6. **Largura Mínima Rígida dos Botões de Simulação Baseada na Extensão do Texto**:
   - Cálculo automático da largura mínima dos botões de simulação com base na métrica da fonte e no texto traduzido, eliminando cortes de palavras mesmo ao movimentar splitters ou alternar o idioma do sistema.
7. **Acoplamento Compacto entre Rótulo e Seletor de "Data de Referência:"**:
   - Posicionamento direto do seletor de calendário a 3px do texto descritivo "Data de Referência:", eliminando folgas excessivas e prevenindo colisões contra o botão de reiniciar ou contra o controle deslizante em todos os layouts.
8. **Tradução Dinâmica e Bilinguismo Completo em Tempo de Execução**:
   - Suporte completo a localização dinâmica para os novos menus de layout, diálogos e controles em Português e Inglês com atualização instantânea ao vivo.

---

### NOTAS DE LANÇAMENTO: Versão 2026.9.13.0

Data: 13 de setembro de 2026

1. **Configuração de Dias Úteis e Jornada de Trabalho no Cronograma (`Configurações > Dias Úteis`)**:
   - Diálogo dedicado `Configurar Dias Úteis e Jornada de Trabalho` acessível pelo menu `Configurações -> Dias Úteis` na janela do Cronograma Gantt.
   - Seleção livre e independente de cada um dos 7 dias da semana (Segunda a Domingo) para determinar quais são considerados dias úteis pelo projeto.
   - Configuração do horário de início e término da jornada diária de trabalho com cálculo e exibição automática em tempo real da carga horária útil (horas e minutos).
   - Recálculo dinâmico das datas de término de todas as atividades do cronograma e motor CPM considerando os dias úteis configurados.
2. **Gerenciamento de Feriados com Calendário Interativo e Biblioteca de 18 Países (`Configurações > Feriados`)**:
   - Diálogo dedicado `Gerenciar Feriados e Dias Não Úteis` acessível pelo menu `Configurações -> Feriados`.
   - Calendário interativo visual para navegação por mês e ano e seleção direta de datas de folga.
   - Lista suspensa com feriados oficiais de 18 nações (Brasil, Estados Unidos, Portugal, Argentina, Canadá, Reino Unido, França, Alemanha, Itália, Espanha, Japão, China, México, Chile, Colômbia, Uruguai, Paraguai e Austrália) e botão `Aplicar` para inserção automática, com cálculo exato de feriados móveis (Carnaval, Sexta-feira Santa, Páscoa, Corpus Christi) e fixos.
   - Tabela de feriados com estilo WBS, redimensionamento bidirecional pelas duas bordas dos cabeçalhos e realce visual adaptativo aos temas Claro e Escuro do Windows 11.
   - Tratamento estrito de feriados cadastrados como dias não úteis pelo motor do cronograma.
3. **Novo Layout Ergonômico em 3 Linhas do Quadro "Nova Tarefa"**:
   - Linha 1: Descrição da tarefa e seletor de quadrante de destino;
   - Linha 2: Listas suspensas de `Prioridade:` e `Nível de Domínio:`, com dimensionamento automático e dinâmico da largura ajustado em tempo de execução ao maior texto da respectiva lista;
   - Linha 3: Checkbox `Vincular data`, checkbox `Vincular horário`, novo campo de `Duração:` (em dias) e botão `Adicionar Tarefa`.
   - Proteção estrita de largura e contenção contra movimentação do divisor vertical (*splitter*), impedindo sobreposições ou quebras sobre o controle de duração.
4. **Campo de Duração em Dias Integrado ao Ecossistema**:
   - Definição explícita da duração de tarefas em dias no cadastro, edição e sincronização contínua com Pomodoro e Calendário.
5. **Tradução Dinâmica Completa (Bilinguismo Nativo)**:
   - Suporte a localização instantânea em tempo real para todas as novas interfaces gráficas, menus, diálogos e cabeçalhos em Português (`pt_BR`) e Inglês (`en_US`).

---

### NOTAS DE LANÇAMENTO: Versão 2026.9.8.0

Data: 8 de setembro de 2026

1. **Barra de Menus Integrada no Módulo de Cronograma, Gantt & BIM 4D**:
   - Barra de menus nativa e elegante na janela principal do módulo com dois menus principais:
     - **Menu Arquivo**: reúne as ações de criação (`➕ Nova Atividade`), edição detalhada de prazos e propriedades (`✏️ Editar`), exclusão com recálculo automático de rede (`❌ Excluir`), importação de dados de planilhas (`📥 Importar CSV`) e exportação do cronograma completo (`📤 Exportar CSV`).
     - **Menu Configurações**: recursos de engenharia, navegação e integração, incluindo dependências (`🔗 Vinculo/Predecessora`), recálculo imediato do caminho crítico (`⚡ Recalcular CPM`), congelamento de planejamento (`📌 Salvar Baseline`), análise comparativa de variações de prazo (`📊 Comparar Baseline`), exibição visual da linha de base (`👁️ Linha Fantasma`), seletores de escala temporal (`🔍 Zoom Dia`, `🔍 Zoom Semana`, `🔍 Zoom Mês`), alternância rápida de visualização (`📋 Tabela WBS` e `🏗️ Simulação 4D`), e sincronização/importação com a Matriz de Eisenhower (`🔄 Sincronizar com EisenPo` e `📋 Importar do EisenPo`).
   - Tradução dinâmica imediata em tempo de execução para todos os títulos de menus, submenus e botões ao alternar entre Português e Inglês.
2. **Redimensionamento Bidirecional de Colunas e Linhas**:
   - Controle dimensional intuitivo: posicione o cursor na borda direita do cabeçalho da coluna e arraste para a direita para expandir ou para a esquerda para reduzir; posicione na borda esquerda do cabeçalho e arraste para a esquerda para expandir ou para a direita para reduzir.
   - O comportamento bidirecional funciona de forma idêntica nas colunas da "📋 Tabela WBS" e da "🏗️ Simulação 4D", estendendo-se também às divisórias horizontais do cabeçalho vertical na tabela de Simulação 4D para ajuste suave da altura das linhas.
3. **Seleção Precisa e Múltipla de Células, Colunas e Linhas**:
   - Correção definitiva da seleção individual: ao clicar sobre qualquer célula da tabela, a seleção ocorre exatamente sobre o item clicado, eliminando qualquer salto para células vizinhas.
   - Suporte a seleção contígua (Shift+Clique ou arrasto) e descontínua (Ctrl+Clique) de células individuais.
   - Seleção de colunas inteiras com um clique no cabeçalho horizontal superior (com suporte a múltiplas colunas via Ctrl/Shift) e seleção de linhas completas clicando no cabeçalho vertical.
4. **Realce Visual Dinâmico Adaptativo ao Tema do Windows 11**:
   - No Tema Escuro: ao passar o mouse ou selecionar células, o realce aplica um tom de cinza suave ligeiramente mais claro que o fundo padrão da célula, eliminando ofuscamentos por branco ou saturação excessiva de azul.
   - No Tema Claro: o realce aplica um tom de cinza elegante ligeiramente mais escuro que o fundo padrão da célula, evitando escurecimento excessivo (preto) e preservando a harmonia visual.
   - Propagação consistente do realce para células únicas, múltiplos blocos, linhas e colunas completas, mantendo legibilidade perfeita e cores vibrantes dos indicadores de status (`✅ Executado:`, `🔄 Em Andamento:`, `⚠️ Atrasado:` e `⏳ Futuro:`).
5. **Linha de Base (Baseline), Linha Fantasma (Ghost Line) e Recálculo Forçado de CPM**:
   - Possibilidade de salvar o plano original como Linha de Base e compará-lo com o cronograma executivo atual para monitorar atrasos ou adiantamentos.
   - A Linha Fantasma projeta diretamente na régua de Gantt as barras da linha de base para visualização comparativa imediata.
   - Recálculo sob demanda de folgas, datas extremas e identificação do caminho crítico.
6. **Conformidade Legal e Documental Completa do Plugin Gantt**:
   - Integração completa dos 12 novos documentos legais e informativos (CLC, COPYRIGHT, EULA, PRIVACY_POLICY, NOTICES e ABOUT) em Português e Inglês na janela "Sobre".
   - Atualização do utilitário `find_VersionEditor` para gerenciamento automatizado das versões documentais.

---

### NOTAS DE LANÇAMENTO: Versão 2026.9.7.0

Data: 7 de setembro de 2026

1. **Novo Módulo de Gestão de Cronograma, Gantt & BIM 4D**:
   - Módulo avançado para planejamento físico, acompanhamento temporal e simulação virtual da construção civil, carregado a partir do plugin dinâmico sob `plugins/gantt`.
   - **Estrutura Analítica do Projeto (EAP / WBS)**: decomposição hierárquica em entregáveis e pacotes de trabalho, com recálculo automático de durações, datas e avanço ponderado das atividades-resumo.
   - **Motor de Caminho Crítico (CPM)**: cálculo analítico de datas mais cedo (*Early Start / Early Finish*) e mais tarde (*Late Start / Late Finish*), folga livre e folga total, com destaque visual em barras vermelhas para atividades críticas no gráfico e na árvore.
   - **Proteção Preventiva contra Dependências Circulares**: detecção em tempo real de ciclos de precedência com bloqueio inteligente e notificação clara ao usuário.
   - **Linha do Tempo e Gráfico de Gantt Interativo**: visualização temporal contínua com escalas por Dia, Semana e Mês, indicação da data atual (*Today Line*) e setas direcionadas de dependência.
   - **Integração Bidirecional com a Matriz de Eisenhower**: vinculação direta de tarefas dos quadrantes ao cronograma pelo menu de contexto (`Vincular ao Cronograma Gantt`), sincronização automática de status e exibição do indicador `🔗 Vinculada`.
   - **Simulação Construtiva 4D BIM (IFC)**: aba dedicada para vincular identificadores únicos de elementos construtivos (`IFC GUID / UniqueId`) aos itens da EAP, reprodutor interativo (Play, Pause, Step e Scrubber) e cálculo dinâmico de status construtivo (Planejado, Em Construção, Concluído, Em Atraso, Antecipado).
   - **Importação e Exportação CSV**: suporte completo a intercâmbio de cronogramas com softwares externos como MS Project, Primavera e Excel.
2. **Otimização Extrema de Desempenho e Tradução Instantânea sem Travamentos**:
   - Implementação de tabela hash em memória $O(1)$ (`QHash<QString, TaskLinkSummary>`) no adaptador de host e na ponte de integração, eliminando completamente acessos e consultas pesadas em disco no SQLite durante a troca de idioma.
   - Processamento multithread do arquivo de tradução `.qm` através do worker dedicado `GanttTranslationWorker` operando em `QThread` secundária.
   - Transição de idioma imediata (< 5ms) na interface gráfica principal, sem congelamentos da GUI.
3. **Localização Dinâmica em Tempo Real na Linha do Tempo e Tabelas**:
   - Tradução em tempo de execução dos meses do ano e dias da semana na régua temporal do Gantt.
   - Atualização imediata dos indicadores de vínculo (`🔗 Vinculada` / `🔗 Linked`) na árvore WBS.
   - Tradução dinâmica imediata dos cabeçalhos da tabela de simulação 4D (`Status 4D` / `4D Status` e `GUID IFC / Id Único` / `IFC GUID / Unique ID`) e dos estados de avanço sem necessidade de reabrir diálogos ou reiniciar o software.
4. **Rastreamento de Versão Automatizado**:
   - Integração das definições do plugin Gantt (`gantt_plugin.json` e `gantt_plugin_interface.hpp`) ao utilitário `find_VersionEditor`.

---

### NOTAS DE LANÇAMENTO: Versão 2026.9.6.0

Data: 6 de setembro de 2026

1. **Redimensionamento Inteligente e Responsivo da Janela "Editar Tarefa"**:
   - Inclusão dos botões de **Maximizar** e **Minimizar** na barra de títulos da janela de edição de tarefas.
   - Redimensionamento adaptativo dinâmico: ao expandir a largura da janela para um formato amplo ($\ge 760\text{px}$) ou ao maximizá-la, a caixa de texto "Descrição (opcional):" é reposicionada e expandida à esquerda para proporcionar amplo espaço de leitura e digitação de notas longas. Os campos com dimensões congeladas ("Tarefa:", "Prioridade:", "Nível de Domínio:", "Quantidade de Pomodoros (🍅):", "Arquivo vinculado (arraste ou escolha):", "Escolher...", "Abrir", "Remover", seletores de data/horário e botões "Cancelar" e "OK") são alinhados perfeitamente no canto inferior direito.
   - Retorno suave e automático ao formato vertical compacto quando a largura da janela for estreitada para menos de 680px.
   - Estabilidade aprimorada com faixa de histerese inteligente entre 680px e 760px, eliminando tremores ou oscilações ao arrastar tanto a borda direita quanto a esquerda.
   - Proteção de altura mínima garantida: previne sobreposição ou vazamento dos controles de descrição sobre os widgets superiores ou botões inferiores ao estreitar a janela verticalmente pelas bordas superior e inferior.
2. **Personalização de Rótulos do Método Feynman ("Configurações > Editar Rótulos")**:
   - Barra de menus nativa integrada na própria janela "Descrição Feynman" com o menu `Configurações > Editar Rótulos`.
   - Janela de edição organizada em matriz intuitiva de 3 colunas ("Padrão", "Atual", "Novo") para personalizar os títulos das 5 etapas do método ("1️⃣ Conceito", "2️⃣ Explique com Simplicidade", "3️⃣ Lacunas Identificadas", "4️⃣ Revisão e Simplificação Final" e "5️⃣ Descrição").
   - Botão `Aplicar` para salvar e propagar imediatamente os novos rótulos nas abas, quadros e descrições compostas, botão `Restaurar` para restabelecer os títulos padrão originais e botão `Fechar`.
   - Botão dedicado `Fechar` adicionado no rodapé da janela "Descrição Feynman" para facilitar o encerramento rápido.
   - Persistência automática em `feynman_rotulos_config.json` no armazenamento local do usuário, com suporte a tradução dinâmica e compatibilidade total com tarefas legadas.
3. **Reorganização Intuitiva do Menu de Contexto**:
   - Nova sequência otimizada das opções exibidas ao clicar com o botão direito sobre qualquer tarefa:
     1. `✏️ Editar Tarefa`
     2. `🪢 Descrição Feynman`
     3. `🔁 Recorrência`
     4. `🎨 Colorir tarefas`
     5. `🔤 Configurações de Fonte`
     6. `🔀 Mover para outro quadrante`
     7. `🗑️ Remover Tarefa`
4. **Alinhamento Simétrico de Bordas e Divisores Verticais**:
   - Alinhamento lateral milimétrico das linhas de contorno (esquerda e direita) do quadro "Nova Tarefa" com as bordas das molduras dos quadrantes abaixo.
   - Espessura perfeitamente sincronizada e uniforme de todas as barras verticais divisórias (*Vertical Splitters*) entre Calendário, Pomodoro e Quadrantes.

---

### NOTAS DE LANÇAMENTO: Versão 2026.8.17.0

Data: 17 de agosto de 2026

1. **Novos Modos de Layout em Abas: Horizontal e Vertical**:
   - No menu `Configurações -> Layout de Tarefas -> Interface do Usuário -> EisenPo -> Layout em Abas`, escolha entre as orientações `Horizontal` (colunas de tarefas lado a lado) e `Vertical` (empilhamento vertical compacto por quadrante).
   - Dimensionamento inteligente com cálculo dinâmico de largura mínima baseado na extensão dos títulos das abas e colunas de status.
2. **Alinhamento Perfeito e Estabilidade do Quadro "Nova Tarefa"**:
   - Organização estável e consistente em 2 linhas de controles para entrada de tarefas (descrição, seletor de quadrante, prioridade, domínio, data/horário e botão de adição).
   - Eliminação de quebras desordenadas ou vazamentos visuais ao redimensionar a janela ou movimentar barras divisórias.
   - Alinhamento lateral simétrico das bordas esquerda e direita do quadro com as extremidades dos quadrantes.
3. **Sistema de Alarme Sonoro do Pomodoro Aperfeiçoado**:
   - Configuração de toques de alarme em `Configurações -> Configurações Específicas -> Pomodoro -> Alarme`.
   - Seleção de arquivos `.wav` personalizados com botões de teste prévio (`▶️ Tocar` e `⏹️ Parar`) e indicador de duração em segundos.
   - Ajuste do intervalo de repetição do alarme (em segundos) e volume de saída calibrado.
   - Reprodução confiável e imediata do áudio configurado ao término dos ciclos de foco e descanso.
   - Modo Automático inteligente com limite configurável de repetições antes do avanço autônomo entre sessões.
4. **Tradução Dinâmica Instantânea em Tempo de Execução**:
   - Tradução em tempo real das abas de status ("Pendentes", "Em Progresso" e "Concluídas") e contadores sem necessidade de reiniciar.

---

### NOTAS DE LANÇAMENTO: Versão 2026.8.15.0

Data: 15 de agosto de 2026

Lançamento oficial da suíte **EisenPo** (versão 2026.8.15.0) — construída em C++17 e Qt 6.11.1:

1. **Consolidação dos 4 Pilares de Produtividade**:
   - Integração coesa entre Matriz de Eisenhower, Método Pomodoro, Técnica Feynman e Registro de Atividades/Agendas.
2. **Armazenamento e Persistência Otimizados**:
   - Persistência 100% local sob `%LOCALAPPDATA%/EisenPo`, com rotina de migração transparente para dados legados de `%LOCALAPPDATA%/EisenhowerOrganizer`.
3. **Gerenciamento Centralizado de Logs**:
   - Diagnósticos unificados em `logs/file_eisenpo_*.log` com monitor em tempo real sob `Ferramentas -> Gerenciador de logs`.
4. **Exclusão Segura e Purga Síncrona**:
   - Diálogos de confirmação protegidos contra cliques acidentais e sincronização instantânea de estado entre os módulos.
5. **Arquitetura Nativa de Alto Desempenho**:
   - Compilação otimizada para x64 com suporte a MinGW e MSVC/Ninja, garantindo inicialização veloz e baixo consumo de memória.

Personalização visual aprimorada do Registro de Atividades e reorganização de menus:

1. **Configuração Independente de Cores por Módulo**:
   - Possibilidade de selecionar os modos `Preto` ou `Coloridas` individualmente para cada ambiente (`EisenPo`, `Pomodoro` ou `Feynman`).
   - Opção `Geral` para aplicar simultaneamente a preferência de cor a todos os três módulos.
2. **Reorganização de Menus de Cores**:
   - Estruturação de submenus `Preto` e `Coloridas` no menu `Configurações -> Registro de Atividades -> Cores` da janela principal e no menu `Configurações -> Cores` da janela `Cadastro de Atividades`.
   - Suporte completo a tradução dinâmica e atualização visual das tarefas em tempo real.

Refinamentos na sincronização visual das tarefas e melhorias na organização dos filtros:

1. **Sincronização Visual e Cores Consistentes**:
   - Atribuição persistente de cores para atividades recém-criadas na Agenda, eliminando variações ao reiniciar.
   - Sincronização inteligente de fontes acionada pelo menu de contexto (clique com botão direito) sem alterar as demais tarefas não selecionadas.
2. **Organização e Limpeza de Filtros**:
   - Padronização e unificação de filtros de datas e horários em todos os módulos, eliminando opções duplicadas.
   - Filtros mais precisos: campos com textos longos não geram mais opções poluídas nos menus de filtros.

Reorganização completa da barra de menus sob `Configurações` e centralização de preferências:

1. **Estrutura Reorganizada do Menu Configurações**:
   - Sequência lógica: `Layout de Tarefas`, `Configurações de Fonte`, `Configurações Específicas`, `Registro de Atividades`, `Idioma`.
   - Menu `Configurações Específicas` concentrando parâmetros dedicados de plugins (ex.: `Pomodoro -> Alarme, Modo automático, Reportar Pomodoros`).
2. **Traduções Dinâmicas**:
   - Todas as novas chaves e caminhos de menus possuem suporte a tradução imediata em tempo de execução.

Expansões de personalização de fontes e tradução dinâmica em tempo de execução:

1. **Configuração de Fontes Independente por Plugin**:
   - Menus de configuração de fonte diretamente nos caminhos dos plugins (`Configurações de Fonte -> Feynman / Pomodoro`).
   - Aplicação exclusiva no respectivo plugin sem interferência da sincronização global.
2. **Tradução Dinâmica em Tempo de Execução**:
   - Suporte a `changeEvent(QEvent::LanguageChange)` em todos os widgets, botões, diálogos de edição, menus contextuais e contadores dinâmicos.

Configurações independentes de fonte para tarefas gerais e arquivos vinculados:

1. **Configurações de Fonte Especializadas**:
   - `Tarefas em Geral`: altera a aparência das tarefas sem arquivo externo vinculado.
   - `Arquivo Vinculado`: altera a aparência das tarefas com arquivo externo vinculado.
2. **Personalização Completa**:
   - Ajuste de família de fonte, estilo, tamanho, negrito, itálico, sublinhado e tachado, com botão `Restaurar`.

---

## 2. Tela principal

Na janela principal do EisenPo, você encontra:

- **Barra de menus superior**: `Arquivo`, `Configurações`, `Ferramentas`, `Opções`.
- **Abas dos módulos disponíveis**: `Eisenhower`, `Pomodoro`, `Feynman` (conforme plugins carregados).
- **Contador do módulo ativo**: no canto superior direito, mostrando tarefas selecionadas e total de itens.
- **Área principal de tarefas Eisenhower**: área central com campos de criação rápida, seletores de prioridade/domínio e os quadrantes de tarefas.
- **Painel lateral do Calendário e Lista Global**: visualização temporal e lista abrangente de tarefas agendadas.
- **Janela auxiliar Cadastro de Atividades**: quando o módulo Registro de Atividades estiver aberto pelo menu `Ferramentas`.
- **Painéis auxiliares**: painel `Em Progresso` (Pomodoro) e `Minhas Tarefas` (Feynman).
- **Janela Sobre (`Opções → Sobre` / `Ctrl+Shift+A`)**: reúne histórico, detalhes técnicos, notas de versão, **Política de Privacidade**, **Contrato (EULA Oficial)**, **CLC**, **Direitos Autorais**, **Avisos** e **Licenças** de código aberto para todos os módulos, com busca em tempo real.
- **Manual Interativo (`Opções → Manual` / `Ctrl+Shift+M`)**: exibe o guia completo com índice lateral clicável, campo **Buscar:** e botões de navegação anterior/próximo, retraduzido ao vivo em Português e Inglês.

---

## 3. Abas da interface

As abas de trabalho no topo da janela principal adaptam-se aos módulos disponíveis:

- `Eisenhower` (Sempre integrada)
- `Pomodoro` (Plugin opcional)
- `Feynman` (Plugin opcional)

Em `Configurações -> Layout de Tarefas -> Interface do Usuário`, você pode configurar as preferências visuais de cada módulo.

---

## 4. Contador do módulo ativo

O contador no canto superior direito atualiza-se dinamicamente ao alternar entre abas e selecionar tarefas:

- Exibe a contagem de tarefas selecionadas e o total de tarefas presentes na visão atual (ex.: `EisenPo: Tarefas selecionadas: 1 | Total de tarefas: 12`).
- Acompanha instantaneamente a troca de idioma (`Selected tasks` / `Tarefas selecionadas`).

---

## 5. Módulo Eisenhower (Matriz de Priorização)

O método Eisenhower divide tarefas em 4 quadrantes estratégicos:

- **Quadrante 1 (Importante e Urgente)**: Crises, prazos imediatos e emergências (Fazer Agora).
- **Quadrante 2 (Importante, Não Urgente)**: Planejamento, estudos, saúde e projetos de longo prazo (Agendar).
- **Quadrante 3 (Não Importante, Urgente)**: Interrupções, reuniões operacionais e solicitações pontuais (Delegar/Otimizar).
- **Quadrante 4 (Não Importante, Não Urgente)**: Distrações e atividades de baixo valor (Eliminar).

Cada tarefa registra:
- Descrição principal e notas detalhadas;
- Quadrante de destino;
- Prioridade (Urgente, Alta, Média, Baixa, Nenhuma);
- Nível de Domínio (Iniciante, Intermediário, Avançado);
- Data e horário vinculados;
- Duração prevista em dias;
- Caminho de arquivo externo anexado/vinculado;
- Estado de conclusão (Pendente ou Concluída);
- Cor personalizada de exibição.

---

## 6. Criar tarefas e o Quadro "Nova Tarefa"

Para adicionar uma nova tarefa:

1. Digite o título/descrição da tarefa no campo principal da **Primeira Linha** e selecione o quadrante desejado (1 a 4).
2. Na **Segunda Linha**, selecione a `Prioridade:` e o `Nível de Domínio:` desejados.
3. Na **Terceira Linha**, marque `Vincular data` e `Vincular horário` se a tarefa tiver prazo definido e informe a `Duração:` prevista (em dias inteiros).
4. Clique no botão **Adicionar Tarefa** ou pressione a tecla `Enter`.

### 6.1. Ergonomia e Disposição em Três Linhas
O quadro **Nova Tarefa** organiza seus controles em três linhas funcionais e elegantes:
- **Linha 1**: Campo de entrada de texto para a descrição e seletor do quadrante de destino;
- **Linha 2**: Listas suspensas de `Prioridade:` e `Nível de Domínio:`. Ambas possuem **dimensionamento automático e dinâmico** da largura em tempo de execução: o sistema calcula dinamicamente o espaço necessário com base no maior texto contido em cada lista, garantindo leitura perfeita sem truncamento e sem espaços vazios exagerados, mesmo ao alternar o idioma da interface;
- **Linha 3**: Opções de `Vincular data`, `Vincular horário`, controle de `Duração:` (em dias) e o botão **Adicionar Tarefa**.
- **Proteção Rígida de Divisores Verticais**: O layout possui limites rígidos de largura mínima e contenção para evitar que o botão `Adicionar Tarefa` ou os controles vizinhos sobreponham ou estoure sobre a caixa de `Duração:` durante o ajuste manual das barras divisórias (*splitters*) da interface.
- **Alinhamento Simétrico**: As bordas externas esquerda e direita do quadro coincidem com exatidão milimétrica com os contornos dos quadrantes posicionados logo abaixo em todos os modos visuais.

---

## 7. Editar tarefas e Redimensionamento Inteligente

Para editar uma tarefa existente:

1. Clique com o botão direito sobre a tarefa em qualquer lista ou quadrante e escolha **✏️ Editar Tarefa**.
2. Na janela de diálogo, você pode alterar:
   - Título da tarefa (`Tarefa:`);
   - Descrição detalhada e notas (`Descrição (opcional):`);
   - Prioridade (`Urgente`, `Alta`, `Média`, `Baixa`, `Nenhuma`);
   - Nível de Domínio (`Iniciante`, `Intermediário`, `Avançado`);
   - Quantidade de Pomodoros concluídos (`🍅`);
   - Meta de Ciclos Pomodoro (`Meta de Ciclos Pomodoro (🎯):`), permitindo definir de 1 a 999 ciclos de foco planejados para a conclusão da tarefa;
   - Arquivo externo vinculado (`Arquivo vinculado (arraste ou escolha):`), com botões `Escolher...`, `Abrir` e `Remover`, além de suporte para arrastar e soltar arquivos diretamente do Windows Explorer;
   - Vínculos opcionais de data (`Vincular data`) e horário (`Vincular horário`).
3. Clique em **OK** para salvar as alterações (o aplicativo exibe confirmação instantânea e fecha a janela sem esperas, gravando os dados e sincronizando em segundo plano) ou **Cancelar** para descartar.

### 7.1. Recursos da Janela "Editar Tarefa"

- **Confirmação Instantânea e Persistência em Segundo Plano**: Ao clicar no botão "OK", as alterações na tarefa são aplicadas de imediato na interface visual e a janela de confirmação de sucesso fecha no mesmo instante. A gravação das tarefas em disco e o recálculo do cronograma prosseguem de modo assíncrono em segundo plano, liberando imediatamente os controles e eliminando qualquer atraso na interação do usuário.
- **Botões de Maximizar e Minimizar**: A janela de diálogo conta com os botões nativos de maximizar e minimizar no canto superior direito da barra de títulos, permitindo expandir a janela para tela cheia ou minimizá-la conforme a conveniência de trabalho.
- **Modo Amplo / Expandido (Wide Mode)**: Ao expandir a largura da janela para $\ge 760\text{px}$ ou ao clicar no botão de maximizar, o layout se reorganiza inteligentemente de forma automática em tempo de execução:
  - A caixa de texto de anotações **"Descrição (opcional):"** ganha destaque prioritário ocupando todo o lado esquerdo da janela, oferecendo uma área ampla e confortável para leitura e edição de textos longos.
  - Todos os campos e controles associados com dimensões congeladas ("Tarefa:", "Prioridade:", "Nível de Domínio:", "Quantidade de Pomodoros (🍅):", "Meta de Ciclos Pomodoro (🎯):", campos de arquivo externo, seletores de data/horário e botões "Cancelar" e "OK") são deslocados e fixados no canto inferior direito, alinhados à base.
- **Modo Compacto (Compact Mode)**: Ao estreitar a janela para uma largura menor que 680px, os widgets retornam de forma suave e instantânea para a disposição vertical clássica original empilhada.
- **Estabilidade Total e Ausência de Oscilações**: A transição é protegida por uma faixa de histerese inteligente (entre 680px e 760px), impedindo qualquer tipo de tremor, piscar ou oscilação ao arrastar a borda esquerda ou direita.
- **Proteção Contra Vazamentos e Limite Mínimo de Altura**: Ao arrastar as bordas externas superior e inferior estreitando a altura da janela, a aplicação respeita limites dimensionais rígidos, garantindo que o bloco de descrição nunca sobreponha ou esmague os campos superiores e os botões inferiores.
- **Retradução Dinâmica ao Vivo de Diálogos Abertos**: Caso o idioma do sistema seja modificado em `Configurações -> Idioma` enquanto o diálogo "Editar Tarefa" estiver aberto, todos os rótulos, botões e campos da interface atualizam-se instantaneamente em tempo de execução sem requerer fechamento da janela.

---

## 8. Concluir, mover, selecionar e remover

- **Menu de Contexto Otimizado**: Ao clicar com o botão direito sobre qualquer tarefa, as seguintes opções são apresentadas na ordem estratégica consolidada:
  1. `✏️ Editar Tarefa` — Abre a janela de edição com suporte a redimensionamento inteligente.
  2. `🪢 Descrição Feynman` — Abre a janela de estudo e formulação de conceitos do Método Feynman.
  3. `🔁 Recorrência` — Define repetições programadas (diárias, semanais ou mensais).
  4. `🎨 Colorir tarefas` — Aplica cores customizadas de destaque ao texto da tarefa.
  5. `🔤 Configurações de Fonte` — Ajusta tipografia (família, tamanho e estilos) de tarefas comuns ou vinculadas.
  6. `🔀 Mover para outro quadrante` — Reclassifica a tarefa para outro quadrante da Matriz de Eisenhower.
  7. `🗑️ Remover Tarefa` — Exclui a tarefa com confirmação protegida (foco padrão no botão `Não`).
- **Concluir Tarefa**: Marque a caixa de seleção da tarefa. Ela será transferida automaticamente para a aba `Concluídas`.
- **Reabrir Tarefa**: Desmarque a caixa de seleção de uma tarefa concluída. Ela retornará imediatamente para `Pendente`.
- **Seleção Múltipla**: Utilize `Ctrl + Clique` para selecionar itens alternados ou `Shift + Clique` para selecionar blocos contínuos.
- **Arraste e Solte (Drag-and-Drop) de Alto Desempenho**: Arraste tarefas individuais ou seleções múltiplas entre quadrantes, abas de estado ou painéis. A movimentação ocorre de forma instantânea e fluida, sem travamentos na interface decorrentes de acessos repetitivos em disco.
- **Remover Tarefas**: Clique com o botão direito sobre a tarefa (ou grupo selecionado) e selecione `Remover Tarefa`. A janela de confirmação abre com foco inicial em `Não` para evitar perdas acidentais.

---

## 9. Layout de tarefas da Matriz de Eisenhower

Em `Configurações -> Layout de Tarefas -> Interface do Usuário -> EisenPo` e `PoCa`, escolha entre:

1. **Layout em Abas > Horizontal**: Exibe os quadrantes em abas superiores com as colunas de status dispostas horizontalmente lado a lado, oferecendo uma leitura panorâmica ideal para telas amplas.
2. **Layout em Abas > Vertical**: Exibe um quadrante por vez em abas com formato vertical compacto.
3. **Layout em Quadrantes**: Exibe os quatro quadrantes simultaneamente dispostos em colunas verticais expandidas.
4. **Layout Matriz 2x2**: Visão panorâmica simultânea dos 4 quadrantes dispostos em grade 2x2.
5. **Layout PoCa (Pomodoro + Calendário Duplo)**: Ativação em `Configurações -> Layout de Tarefas -> Interface do Usuário -> PoCa -> Ativar (ou Desativar)`.

### 9.1. Como usar o Layout Matriz 2x2

No modo Matriz 2x2:
- Cada quadrante possui abas internas individuais: `Pendente` (na cor do quadrante) e `Concluídas` (em azul).
- A conclusão de tarefas pode ser feita marcando a caixa de seleção ou arrastando o item diretamente para a aba `Concluídas`.
- Arrastar tarefas para a área de outro quadrante ajusta automaticamente a classificação e a prioridade da tarefa.

### 9.2. Como usar o Layout PoCa (Pomodoro + Calendário Duplo)

O layout integrado **PoCa** unifica na coluna lateral esquerda todo o ecossistema de foco em execução e visão temporal ao lado da Matriz de Eisenhower:

- **Ativação e Acesso**: Acesse o menu `Configurações -> Layout de Tarefas -> Interface do Usuário -> PoCa` e clique em `Ativar` para exibir a coluna integrada ou `Desativar` para retornar ao modo padrão.
- **Estrutura Empilhada de Quatro Quadros**:
  1. **Temporizador Pomodoro**: relógio de contagem regressiva, indicadores de foco, pausas e botões de controle de ciclos;
  2. **Configurações**: controles operacionais de duração de foco, tempos de descanso e preferências do temporizador;
  3. **Calendário 1**: primeiro calendário mensal (mês atual de referência com marcações e datas de compromissos);
  4. **Calendário 2**: segundo calendário mensal (mês subsequente contínuo, perfeitamente sincronizado com o primeiro).
- **Alinhamento Simétrico das Molduras**: As bordas laterais externas (esquerda e direita) dos 4 quadros possuem alinhamento milimétrico simétrico entre si, formando uma coluna limpa, elegante e geometricamente coesa.
- **Espaçamento Rente Vertical (6px)**: O segundo calendário posiciona-se rente logo abaixo do primeiro com espaçamento uniforme de 6px (exatamente a mesma distância existente entre o quadro de Configurações e o primeiro calendário), eliminando vãos visuais excessivos.
- **Replicação Dinâmica e Sequência Matricial $2 \times K$ de Meses**:
  - Ao arrastar a barra divisória vertical (*splitter*) para a direita ampliando a coluna lateral, os calendários se replicam horizontalmente em novas colunas;
  - A ordem dos meses segue uma matriz matemática sequencial contínua ($2 \times K$):
    - **Linha 1** (superior): exibe os meses em progressão horizontal da esquerda para a direita ($M, M+1, M+2, \dots$);
    - **Linha 2** (inferior): continua sequencialmente a partir do término da primeira linha ($M+K, M+K+1, M+K+2, \dots$).
    - *Exemplo*: com 2 colunas visíveis ($2 \times 2 = 4$ calendários):
      - Linha 1: Calendário 1 = Mês de Setembro; Calendário 3 (ao lado) = Mês de Outubro.
      - Linha 2: Calendário 2 (abaixo do 1) = Mês de Novembro; Calendário 4 (abaixo do 3) = Mês de Dezembro.
- **Proteção e Dimensionamento Preventivo da Barra de Rolagem**:
  - Nas dimensões iniciais mínimas de abertura da aplicação com o layout PoCa ativo, o painel lateral calcula automaticamente a margem de segurança para acomodar a barra de rolagem vertical.
  - As molduras direitas dos quadros não sofrem corte, invasão ou sobreposição pela barra de rolagem, mantendo a proporção visual harmoniosa com os quadrantes da Matriz de Eisenhower.
- **Inicialização Instantânea Sem Congelamentos**:
  - A abertura do aplicativo ocorre de forma imediata e fluida, liberando instantaneamente a interação manual e o cursor do mouse sem os travamentos de inicialização que ocorriam anteriormente.

### 9.3. Tradução Dinâmica das Abas de Status

As abas de status (`Pendentes`, `Em Progresso` e `Concluídas`) adaptam-se instantaneamente em tempo de execução ao alternar o idioma em `Configurações -> Idioma`.

### 9.4. Ordenação de Tarefas

Sob `Configurações -> Layout de Tarefas`, você pode ordenar as listas por:
- **Organizar por Data** (Crescente / Decrescente)
- **Organizar por Horário** (Crescente / Decrescente)
- **Organizar por Ordem Alfabética** (Crescente / Decrescente)

---

## 10. Painéis visíveis

Em `Configurações -> Layout de Tarefas -> Mostrar Painéis`:

- **Calendário** (`Ctrl+M`): Ativa ou oculta o painel lateral com o calendário e a Lista Global de Tarefas.
- **Pomodoro** (`Ctrl+Shift+E`): Ativa ou oculta o painel lateral de tarefas em progresso do Pomodoro.

As barras divisórias verticais (*Vertical Splitters*) que separam o Calendário, o Pomodoro e os Quadrantes de Tarefas possuem espessura uniforme e perfeitamente sincronizada em toda a interface.

---

## 11. Calendário e Lista Global

- O calendário exibe marcadores visuais automáticos nos dias com tarefas agendadas.
- Filtros disponíveis por **Dia**, **Semana**, **Mês** e **Ano**.
- A **Lista Global de Tarefas** exibe todas as atividades do período filtrado, identificando claramente seu status no ciclo de vida: `[Concluída]`, `[Pendente]` e `[Em Progresso]` para tarefas em andamento.
- Para cada tarefa listada, são informados o horário agendado, a descrição da atividade, o status operacional e a contagem de pomodoros concluídos em relação à meta planejada.
- **Tradução Dinâmica ao Vivo**: Ao alternar o idioma do aplicativo entre Português e Inglês, as etiquetas de status (`[Em Progresso]` / `[In Progress]`) e todas as dicas de ferramentas (*tooltips*) do painel do Calendário atualizam-se instantaneamente em tempo de execução sem necessidade de reiniciar o programa.
- **Interatividade Ágil**: O clique duplo sobre qualquer item da Lista Global abre diretamente o arquivo externo vinculado. Clicar com o botão direito em um item da lista (concluído, pendente ou em progresso) permite editar a tarefa, abrir arquivos vinculados ou removê-la com máxima rapidez.
- O divisor vertical que separa o painel do Calendário da área de quadrantes mantém rigorosamente a mesma espessura padronizada do divisor do Pomodoro.
- **Sincronização Bidirecional dos Calendários Extras**: Quando o painel está configurado para exibir múltiplos meses simultâneos, navegar pelas setas de mês em qualquer um dos calendários secundários (2º mês, 3º mês, etc.) recalcula e atualiza de forma síncrona e contínua todos os calendários do painel, preservando a coerência visual da sequência temporal.

---

## 12. Personalização visual e fontes

Sob `Configurações -> Configurações de Fonte`:

- **Tarefas em Geral**: Define fonte, estilo, tamanho, negrito, itálico, sublinhado e tachado para tarefas sem arquivos vinculados.
- **Arquivo Vinculado**: Define tipografia específica para tarefas que contêm arquivos externos anexados.
- **Colorir tarefas**: Permite selecionar uma ou várias tarefas e aplicar uma cor personalizada com o seletor completo de cores.
- **Restaurar**: Restaura os padrões tipográficos recomendados para a categoria.

---

## 13. Módulo Pomodoro e Sistema de Alarme

O módulo Pomodoro organiza a execução do trabalho em ciclos cronometrados:

- **Blocos de Foco**: Tempo de concentração contínua (padrão de 25 minutos).
- **Pausas Curtas**: Descanso rápido entre blocos (padrão de 5 minutos).
- **Pausas Longas**: Descanso estendido após completar um ciclo de blocos (padrão de 15 minutos).
- **Quadro de Tarefas**: Registro e sincronização de pomodoros estimados versus executados.

### 13.1. Configuração do Alarme Sonoro

Em `Configurações -> Configurações Específicas -> Pomodoro -> Alarme`:
1. Escolha a campainha ou som de aviso para término de foco e descanso na lista suspensa.
2. Utilize o botão `▶️ Tocar` para testar o áudio e `⏹️ Parar` para interromper, visualizando a duração do som em segundos.
3. Configure o **Intervalo de repetição do alarme** (em segundos) para definir a frequência dos toques periódicos.
4. Clique em `💾 Salvar` para persistir as alterações.
5. Ao término de qualquer sessão, o som configurado é reproduzido imediatamente com volume calibrado.

### 13.2. Modo Automático

Em `Configurações -> Configurações Específicas -> Pomodoro -> Modo automático`:
- Ative as transições automáticas entre blocos de foco e pausas.
- Configure o número máximo de repetições do alarme antes do avanço autônomo para o próximo ciclo.

### 13.3. Metas Individuais de Ciclos e Janela "⚠️ Aviso de Metas de Ciclos"

O Pomodoro do EisenPo elimina dependências de contagens fixas de ciclos, permitindo total controle sobre o planejamento de esforço de cada tarefa:

- **Meta de Ciclos Individual por Tarefa (🎯)**:
  - Cada tarefa possui sua própria meta de ciclos (configurável de 1 a 999) na janela **"✏️ Editar Tarefa"** através do campo **"Meta de Ciclos Pomodoro (🎯):"**.
  - O cálculo do percentual de conclusão é estrito e proporcional:
    $$\text{Percentual} = \left(\frac{\text{Ciclos Concluídos}}{\text{Meta de Ciclos Planejada}}\right) \times 100\%$$
  - Uma tarefa só transita automaticamente para a aba de Concluídas ("Done") quando atinge sua própria meta de ciclos (e não mais prematuramente quando o timer encerra uma rodada padrão).
- **Detecção Inteligente e Janela "⚠️ Aviso de Metas de Ciclos"**:
  - Ao iniciar o temporizador Pomodoro com tarefas presentes na coluna "Em Progresso", o sistema compara a meta individual de cada tarefa ativa com a quantidade de ciclos programada na sessão do timer (`Ciclo 1/N`).
  - Se houver divergência em uma ou mais tarefas (por exemplo, timer configurado para 4 ciclos e tarefas com metas de 2 ou 10 ciclos), a janela **"⚠️ Aviso de Metas de Ciclos"** é exibida automaticamente antes de iniciar a contagem.
- **Tabela Analítica de Discrepâncias**:
  - A janela exibe uma tabela clara e detalhada com 5 colunas:
    - **Tarefa**: nome da atividade em andamento;
    - **Módulo / Origem**: procedência da tarefa (`🍅 Pomodoro`, `🏛️ Eisenhower` ou `🧠 Feynman`);
    - **Meta (🎯)**: meta de ciclos configurada individualmente na tarefa;
    - **Ciclo Timer (⚙️)**: quantidade de ciclos configurada na sessão atual do timer;
    - **Comportamento no Fluxo**: esclarecimento visual do fluxo (ex.: `🎯 < ⚙️ Concluirá ao atingir X ciclos (antes do timer)` ou `🎯 > ⚙️ Não concluirá (permanece Em Progresso; timer para em Y ciclos)`).
- **Opções de Ação e Alinhamento**:
  - `🔄 Redefinir todas para N ciclos`: ajusta simultaneamente a meta de todas as tarefas da lista para coincidir com os ciclos do timer e inicia a contagem imediatamente, sincronizando as tarefas no Pomodoro e suas correspondentes no EisenPo;
  - `▶️ Iniciar mesmo assim`: inicia o temporizador preservando as metas individuais de cada tarefa, respeitando a conclusão independente de cada atividade;
  - `❌ Cancelar`: aborta a inicialização do temporizador mantendo o estado de todas as tarefas inalterado.
- **Tradução Dinâmica Instantânea**:
  - Todos os títulos, colunas, mensagens explicativas, botões e dicas (*tooltips*) da janela contam com suporte bilíngue nativo e atualizam-se instantaneamente ao alternar o idioma do aplicativo.

---

## 14. Módulo Feynman

O método Feynman estimula o aprendizado profundo através da síntese e simplificação:

1. **Conceito**: Definição objetiva do tema ou problema a ser dominado.
2. **Explique com Simplicidade**: Redação da explicação em linguagem direta e acessível, como se ensinasse a um iniciante.
3. **Lacunas Identificadas**: Mapeamento claro de dúvidas, inconsistências conceituais e pontos que exigem revisão adicional.
4. **Revisão e Simplificação Final**: Refinamento e síntese definitiva do conteúdo após pesquisa e esclarecimento.

Visualização adaptável em **Abas**, **Vertical** ou **Horizontal** em `Configurações -> Layout de Tarefas -> Interface do Usuário -> Feynman`.

### 14.1. Personalização dos Rótulos das Etapas ("Configurações > Editar Rótulos")

A janela **"Descrição Feynman"** conta com uma barra de menus própria e um sistema nativo de edição de rótulos:

- **Como acessar**: Na janela "Descrição Feynman", acesse o menu superior `Configurações -> Editar Rótulos`.
- **Estrutura da Janela "Editar Rótulos"**: A janela exibe uma matriz intuitiva e organizada de 3 colunas e 5 linhas estruturada com widgets nativos:
  - **Coluna "Padrão"**: Exibe em modo somente leitura a denominação original do sistema para cada uma das 5 etapas ("1️⃣ Conceito", "2️⃣ Explique com Simplicidade", "3️⃣ Lacunas Identificadas", "4️⃣ Revisão e Simplificação Final" e "5️⃣ Descrição").
  - **Coluna "Atual"**: Exibe em modo somente leitura o rótulo atualmente ativo no seu ambiente (seja ele o padrão ou uma personalização anterior).
  - **Coluna "Novo"**: Campos editáveis com botões para limpar e sugestões dinâmicas onde você pode digitar o novo nome desejado para qualquer uma das etapas.
- **Botões de Ação**:
  - `Aplicar`: Salva as alterações, grava no arquivo local `feynman_rotulos_config.json`, atualiza imediatamente os títulos das abas, dos quadros e das descrições compostas de todas as tarefas associadas, transfere o novo nome para a coluna "Atual" e limpa o campo "Novo".
  - `Restaurar`: Redefine instantaneamente todos os rótulos para os valores padrão originais do sistema.
  - `Fechar`: Encerra a janela de edição de rótulos.
- **Botão "Fechar" no Rodapé do Feynman**: A janela "Descrição Feynman" dispõe de um botão dedicado `Fechar` ao lado de `Salvar Conceito` e `Limpar` para encerrar a janela de forma ágil e conveniente.
- **Persistência e Compatibilidade Total**: As preferências de rótulos ficam gravadas em `%LOCALAPPDATA%/EisenPo/feynman_rotulos_config.json`. Tarefas antigas criadas com rótulos anteriores permanecem totalmente legíveis e compatíveis.
- **Tradução Dinâmica**: Tanto o menu quanto a janela de rótulos adaptam-se instantaneamente em tempo de execução ao alternar o idioma do aplicativo entre Português e Inglês.

---

## 15. Plugin Registro de Atividades e Agendas

Aberto através de `Ferramentas -> Registro de Atividades` ou `Configurações -> Registro de Atividades`:

### 15.1. Janela Cadastro de Atividades
- Cadastro com campos de curso, ementa, semestre, disciplina, turma, tipo de atividade, sequência, prioridade, nível de domínio, data, horário e arquivo vinculado.
- Botão **Registrar Definições** converte o cadastro em tarefa unificada compartilhada no `tarefas.json`.

### 15.2. Agendas Personalizadas e Listas Aninhadas
- Crie novas agendas temáticas pelo menu interno da janela.
- Crie listas aninhadas em formato de árvore hierárquica (ex.: Curso > Semestre > Disciplina). A seleção do item pai filtra dinamicamente as opções disponíveis na lista dependente.
- Suporte a esquemas visuais com modo `Preto` e modo `Coloridas` selecionáveis por módulo (`EisenPo`, `Pomodoro`, `Feynman` ou `Geral`).

---

## 16. Módulo Cronograma, Gantt & BIM 4D

Disponível através de `Ferramentas -> Gestão de Cronograma, Gantt & BIM 4D`, pelo atalho `Ctrl+Shift+B` ou pelo menu de contexto (clique direito) sobre qualquer tarefa da Matriz de Eisenhower (`Vincular ao Cronograma Gantt` / `Abrir no Gantt`):

### 16.1. Visão Geral da Interface e Barra de Menus
A janela principal do módulo integra em um único ambiente de trabalho:
- **Barra de Menus Superior**:
  - **Menu Arquivo**:
    - `➕ Nova Atividade`: abre o diálogo para adicionar uma nova atividade ou entrega na EAP/WBS.
    - `✏️ Editar`: edita as propriedades da atividade selecionada (código, descrição, duração, datas, progresso).
    - `❌ Excluir`: remove com segurança a atividade selecionada e recalcula a rede de predecessoras.
    - `📥 Importar CSV`: carrega um cronograma completo a partir de arquivo de planilha CSV estruturado.
    - `📤 Exportar CSV`: exporta todo o cronograma atual para intercâmbio com o MS Project, Primavera ou Excel.
  - **Menu Configurações**:
    - `🔗 Vinculo/Predecessora`: define e ajusta as relações de dependência Término-para-Início entre tarefas.
    - `⚡ Recalcular CPM`: força a reavaliação analítica do caminho crítico, atualizando folgas e datas extremas.
    - `📌 Salvar Baseline`: armazena o snapshot da Linha de Base para controle de alterações e auditoria de prazos.
    - `📊 Comparar Baseline`: confronta o cronograma executivo com o plano original gravado, indicando variações.
    - `👁️ Linha Fantasma`: liga ou desliga as barras da Linha Fantasma diretamente na régua de Gantt.
    - `🔍 Zoom Dia`, `🔍 Zoom Semana` e `🔍 Zoom Mês`: calibra instantaneamente a resolução temporal da régua.
    - `📋 Tabela WBS` e `🏗️ Simulação 4D`: alterna rapidamente entre a tabela de engenharia e a aba de simulação 4D.
    - `Layout -> Tabelas`: configura a disposição geométrica das tabelas e do diagrama (Original, Vertical Topo, Vertical Inferior, Horizontal Esquerda e Horizontal Direita).
    - `📅 Dias Úteis`: abre a janela de configuração de dias úteis da semana e jornada de trabalho diária.
    - `🎉 Feriados`: abre a janela de gerenciamento de feriados e dias não úteis com calendário interativo e seletor de nações.
    - `🔄 Sincronizar com EisenPo`: atualiza em tempo real as tarefas vinculadas à Matriz de Eisenhower.
    - `📋 Importar do EisenPo`: importa em lote tarefas dos quadrantes da matriz como atividades do cronograma.
- **Painel Esquerdo (Árvore EAP/WBS)**: listagem estruturada das fases, entregáveis e tarefas com código WBS, descrição, duração em dias, datas planejadas, porcentagem de conclusão e indicador de caminho crítico.
- **Painel Direito (Gráfico de Gantt Interativo)**: visualização gráfica horizontal das barras de tarefas, progresso concluído, conexões de precedência e linha vertical indicadora da data atual (*Today Line*).
- **Aba de Simulação 4D BIM**: painel dedicado para acompanhamento visual da evolução da construção civil integrado a elementos 3D/IFC.

### 16.2. Estrutura EAP (WBS) e Análise de Caminho Crítico (CPM)
- **Hierarquia e Agrupamento**: organize o projeto em múltiplos níveis de detalhe. Atividades agrupadas sob um pacote-pai formam automaticamente uma barra-resumo que consolida o período de início e término e o progresso físico ponderado de suas etapas filhas.
- **Cálculo Analítico CPM**: o algoritmo de Caminho Crítico calcula com precisão matemática os limites temporais de cada tarefa: início e término mais cedo (*Early Start / Early Finish*), início e término mais tarde (*Late Start / Late Finish*), folga livre (*Free Float*) e folga total (*Total Float*).
- **Identificação do Caminho Crítico**: atividades cuja folga total é igual a zero determinam o prazo final do projeto e são exibidas automaticamente em destaque vermelho brilhante tanto na árvore quanto no gráfico temporal.
- **Padronização Cromática Unificada de Status na Tabela WBS**: todas as atividades listadas na Tabela WBS utilizam a mesma identidade visual e coloração de fonte estabelecida para as strings de status do quadro "Simulação Temporal - 4D BIM" vinculado à tabela "Simulação 4D": verde (#4caf50) para tarefas totalmente concluídas (`✅ Executado:`), azul (#2196f3) para tarefas em andamento dentro do prazo (`🔄 Em Andamento:`), vermelho (#f44336) para tarefas em atraso (`⚠️ Atrasado:` — cobrindo tanto o início em atraso quanto o atraso no término da atividade) e cinza neutro (#9e9e9e) para tarefas agendadas para o futuro que ainda não iniciaram (`⏳ Futuro:`). Essa padronização visual elimina ambiguidades e permite diagnosticar instantaneamente o estado da obra ou projeto diretamente nas linhas da tabela.
- **Proteção contra Dependências Circulares**: o sistema valida em tempo real qualquer nova dependência inserida. Se uma relação de precedência direta ou indireta formar um laço circular (ciclo fechado), a operação é imediatamente rejeitada com uma mensagem clara, impedindo inconsistências ou travamentos no cronograma.

### 16.3. Interação Avançada com as Tabelas EAP/WBS e Simulação 4D
- **Cinco Modos de Layout (Configurações -> Layout -> Tabelas)**:
  - `Original`: divisão horizontal proporcional 50%/50% entre a Tabela WBS e o Diagrama de Gantt na parte superior, e painel de Simulação 4D na parte inferior;
  - `Vertical Topo`: Tabelas "Tabela WBS" e "Simulação 4D" dispostas lado a lado (50%/50% horizontal) na parte superior da tela, com o "Diagrama de Gantt" na parte inferior;
  - `Vertical Inferior`: "Diagrama de Gantt" na parte superior da tela, com as tabelas "Tabela WBS" e "Simulação 4D" dispostas lado a lado (50%/50% horizontal) na parte inferior;
  - `Horizontal Esquerda`: Tabelas "Tabela WBS" e "Simulação 4D" empilhadas verticalmente (50%/50%) no lado esquerdo da tela, e o "Diagrama de Gantt" ocupando todo o lado direito;
  - `Horizontal Direita`: "Diagrama de Gantt" ocupando todo o lado esquerdo da tela, e as tabelas "Tabela WBS" e "Simulação 4D" empilhadas verticalmente (50%/50%) no lado direito.
  - A escolha é marcada com um checkmark visual e registrada imediatamente no arquivo de configuração `layout.json` (`C:\Users\<usuario>\AppData\Local\EisenPo\layout.json`) utilizando gravação atômica segura (`QSaveFile`). A cada reinicialização da janela do Gantt ou do aplicativo, a preferência é lida e restabelecida automaticamente sem perda de dados e sem afetar outras preferências.
- **Divisão Proporcional 50% / 50%**: no layout padrão e nos arranjos combinados, a Tabela WBS e o Diagrama de Gantt dividem rigorosamente meio-a-meio a largura útil da tela, garantindo ampla visibilidade para as colunas de dados e para a régua temporal.
- **Parentesco de Janelas e Suporte a Múltiplas Telas (Multi-Monitor)**: ao excluir atividades na janela do cronograma ou confirmar ações, todas as caixas de diálogo abrem centralizadas sobre a própria janela do Gantt, inclusive em monitores secundários, sem puxar a tela principal do EisenPo para a frente.
- **Redimensionamento Bidirecional de Colunas e Linhas**:
  - Arraste a **borda direita** de qualquer cabeçalho de coluna para a direita para expandir ou para a esquerda para reduzir a largura.
  - Arraste a **borda esquerda** de qualquer cabeçalho de coluna para a esquerda para expandir ou para a direita para reduzir a largura.
  - Na tabela "Simulação 4D", o mesmo ajuste bidirecional está disponível nas linhas arrastando as divisórias horizontais do cabeçalho vertical para cima ou para baixo.
- **Seleção Precisa e Múltipla**:
  - Clique simples em qualquer célula seleciona com exatidão o item clicado, sem saltos para posições adjacentes.
  - Selecione blocos contínuos de células arrastando com o mouse ou mantendo `Shift` pressionado.
  - Selecione células alternadas e descontínuas mantendo a tecla `Ctrl` pressionada.
  - Clique no cabeçalho horizontal superior para selecionar a coluna inteira (ou múltiplas colunas com `Ctrl`/`Shift`).
  - Clique no cabeçalho vertical numerado para selecionar a linha completa (ou múltiplas linhas com `Ctrl`/`Shift`).
- **Realce Visual Adaptativo ao Tema do Windows 11**:
  - **No Tema Escuro**: ao passar o mouse sobre as células ou selecioná-las, o sistema aplica um realce em cinza suave ligeiramente mais claro que a cor de fundo padrão da célula, garantindo contraste impecável sem ofuscar a visão.
  - **No Tema Claro**: ao passar o mouse sobre as células ou selecioná-las, o sistema aplica um realce em cinza sofisticado ligeiramente mais escuro que a cor de fundo padrão da célula, evitando áreas excessivamente escuras ou pretas.
  - **Propagação Perfeita**: o realce espalha-se harmonicamente por células isoladas, seleções múltiplas, colunas inteiras e linhas inteiras.
  - **Cores e Status Preservados**: a formatação preserva integralmente as cores de status originais: `✅ Executado:`, `🔄 Em Andamento:`, `⚠️ Atrasado:` e `⏳ Futuro:`.

### 16.4. Linha do Tempo e Gráfico de Gantt
- **Escalas Temporais Flexíveis**: alterne entre visualizações em dias, semanas ou meses para analisar desde cronogramas operacionais diários até visões macroestratégicas plurianuais de obras e projetos.
- **Navegação Contínua**: arraste e role a linha do tempo horizontalmente com suavidade.
- **Harmonização Cromática nas Caixas e Nós do Gantt**: as caixas ou nós de atividades na linha do tempo horizontal adotam a mesma identidade de cor das fontes de status da WBS e do 4D BIM (azul para em andamento, vermelho para atrasado, cinza para futuro), aplicando uma tonalidade verde suave e refinada (#4caf50 com opacidade e contraste balanceados) especificamente para atividades concluídas, permitindo distinção imediata e confortável em relação às demais atividades.
- **Tooltips Informativos Dinâmicos, Detalhados e Localizados**: ao repousar o cursor do mouse sobre qualquer caixa ou nó de atividade no Diagrama de Gantt, um quadro informativo completo exibe em tempo real o código WBS, o badge estilizado de status atual, as datas de início e término formatadas conforme o idioma selecionado (`dd/MM/yyyy HH:mm` em português e `MM/dd/yyyy HH:mm` em inglês), duração em dias inteiros, percentual de conclusão física, condição do Caminho Crítico com realce colorido (`SIM` em vermelho ou `Não` em cinza) e folga total em horas. Todos os rótulos, datas e valores são retraduzidos dinamicamente e sem demora em tempo de execução ao alternar o idioma nas configurações.
- **Linha Fantasma (Ghost Line) Persistente**: quando ativada no menu Configurações, desenha na linha do tempo barras semitransparentes correspondentes à Linha de Base gravada, permitindo comparar visualmente o planejado com o realizado. A renderização mantém-se contínua e estável mesmo após o cadastro de novas tarefas no EisenPo ou a criação de novas atividades no cronograma.
- **Sincronização de Cabeçalhos em Tempo Real**: os meses e dias da semana da linha do tempo atualizam-se dinamicamente ao vivo ao alternar entre Português e Inglês, sem exigir recarregamento.

### 16.5. Vinculando Tarefas da Matriz de Eisenhower ao Cronograma
- **Como Vincular**: clique com o botão direito sobre qualquer tarefa na Matriz de Eisenhower e escolha `🔗 Vincular ao Cronograma Gantt`.
- **Seleção Intuitiva**: no diálogo de vinculação, selecione o cronograma e o item da EAP desejado.
- **Indicador Visual**: o item da EAP associado exibe imediatamente a etiqueta `🔗 Vinculada` (ou `🔗 Linked`) acompanhada do código WBS correspondente.
- **Cálculo Físico Discreto de Progresso por Pomodoros**:
  - O avanço percentual (%) das tarefas vinculadas ao cronograma é calculado de modo exato e rigoroso com base na proporção entre os blocos Pomodoro executados e a meta de ciclos configurada na tarefa:
    $$\text{Progresso Físico} = \left(\frac{\text{Pomodoros Concluídos}}{\text{Meta de Pomodoros}}\right) \times 100\%$$
  - **Degraus Discretos e Previsíveis**:
    - Para uma tarefa com meta de 4 pomodoros: os degraus de progresso físico no Gantt avançam com precisão em **0%**, **25%**, **50%**, **75%** e **100%**;
    - Para uma tarefa com meta de 5 pomodoros: os degraus de progresso físico avançam em **0%**, **20%**, **40%**, **60%**, **80%** e **100%**;
    - Tarefas com qualquer outra meta inteira (1 a 999 ciclos) progridem proporcionalmente de forma exata.
  - **Eliminação do Avanço Arbitrário Inicial de 10%**:
    - Foi totalmente removido qualquer avanço arbitrário inicial ao vincular ou iniciar uma tarefa; a atividade parte rigorosamente de **0%** de progresso físico quando nenhum ciclo foi executado.
  - **Conclusão Integral da Atividade**:
    - A tarefa atinge 100% de avanço físico quando todos os pomodoros previstos em sua meta forem concluídos ou quando o usuário marcar a tarefa como concluída na Matriz de Eisenhower.
- **Sincronização Instantânea em Tempo Real**: qualquer alteração de tarefas na tela principal (edição de texto, prazos, prioridade, movimentação entre quadrantes, conclusão ou avanço de pomodoros) é propagada imediatamente e em tempo real para o cronograma Gantt aberto, mantendo a barra de Gantt e o progresso físico perfeitamente sincronizados sem necessidade de ações manuais.
- **Desempenho Instantâneo O(1)**: a consulta dos vínculos utiliza cache ultrarrápido em memória protegido por mutex, eliminando acessos a disco e permitindo que a troca de idiomas ou a reordenação de tarefas na matriz ocorra em milissegundos sem congelamentos.

### 16.6. Simulação Construtiva 4D BIM (IFC)
- **Vínculo com Elementos Construtivos**: conecte identificadores universais de elementos de modelos tridimensionais (IFC GUIDs ou UniqueIds) aos itens de cronograma correspondentes.
- **Barra de Controle Adaptativa em 3 Modos**: ao redimensionar os divisores verticais (*splitters*), o painel de simulação reorganiza dinamicamente seus controles em:
  - `Linha Única` (largura >= 1200px): botões de reprodução, seletor de Data de Referência, controle deslizante e badges de status em linha única;
  - `Duas Linhas` (largura entre 780px e 1200px): botões de simulação e Data de Referência na linha superior, e os badges de status ("Total:", "Executado:", "Em Andamento:", "Atrasado:", "Futuro:") na linha inferior;
  - `Três Linhas` (largura < 780px): primeira linha com botões ("▶️ Iniciar Simulação", "⏹️ Parar Simulação", "🔄 Reiniciar Simulação"), segunda linha com Data de Referência e barra deslizante temporal (*Scrubber*), e terceira linha com todos os badges de status.
  - A altura do container se ajusta automaticamente (~58px, ~84px, ~114px) sem vazar sobre a tabela ou truncar componentes.
- **Proteção Dimensional dos Botões de Simulação**: os botões "▶️ Iniciar Simulação", "⏹️ Parar Simulação" e "🔄 Reiniciar Simulação" têm sua largura mínima rigidamente bloqueada com base na métrica da fonte e no comprimento total de seus textos traduzidos mais margens de estilo, impedindo cortes de palavras ao trocar de idioma ou arrastar divisores.
- **Acoplamento Estrito de Data de Referência**: o seletor de calendário é posicionado a apenas 3px do rótulo "Data de Referência:", eliminando lacunas vazias e prevenindo qualquer sobreposição com o botão de reiniciar ou com o controle deslizante temporal.
- **Reprodutor de Simulação Interativo**: utilize os botões `▶️ Iniciar`, `⏸️ Pausar` e o controle deslizante temporal (*Scrubber*) para simular o avanço da obra ao longo do tempo.
- **Controle de Velocidade**: ajuste a velocidade da simulação (em passos por segundo) para acompanhar o ritmo da construção.
- **Classificação Dinâmica de Estados 4D**: a tabela de elementos classifica em tempo real a situação de cada peça construtiva na data simulada:
  - `Planejado` (*Planned*): elemento ainda não iniciado;
  - `Em Construção` (*In Construction*): elemento em fase ativa de execução física;
  - `Concluído` (*Constructed*): elemento finalizado;
  - `Em Atraso` (*Delayed*): data simulada ultrapassou o término planejado sem conclusão;
  - `Antecipado` (*Ahead of Schedule*): elemento executado antes do prazo previsto.
- **Tradução em Tempo Real de Rótulos e Colunas**: as colunas `Status 4D` e `GUID IFC / Id Único` e todos os estados de evolução construtiva traduzem-se instantaneamente ao vivo na tela.

### 16.7. Importação, Exportação CSV e Persistência Local
- **Interoperabilidade**: exporte o cronograma completo em arquivo `.csv` para abertura no Microsoft Excel, LibreOffice Calc, MS Project ou Primavera P6, ou importe cronogramas legados em lote.
- **Armazenamento Seguro**: os cronogramas, itens, dependências e vínculos BIM são gravados em banco de dados SQLite transacional próprio (`eisenpo_gantt.db`), localizado no diretório de persistência `%LOCALAPPDATA%/EisenPo`.
- **Webhooks e REST API**: suporte arquitetural para integração de cronogramas em rede e emissão de alertas automáticos via Webhooks e serviços web baseados na especificação OpenAPI 3.0.3.

### 16.8. Configuração de Dias Úteis e Jornada de Trabalho
Acesse através do menu `Configurações -> Dias Úteis` na janela do Cronograma Gantt:
- **Seleção Individual dos Dias da Semana**: O diálogo `Configurar Dias Úteis e Jornada de Trabalho` exibe caixas de seleção independentes para todos os 7 dias da semana (Segunda-feira, Terça-feira, Quarta-feira, Quinta-feira, Sexta-feira, Sábado e Domingo). Marque os dias que devem ser computados como dias úteis de trabalho e desmarque os dias de folga (por exemplo, permitindo trabalhar aos sábados ou configurar regimes de turno especiais).
- **Definição da Carga Horária e Horários da Jornada**: Ajuste os horários de início e término do expediente diário (padrão: 08:00 às 17:00).
- **Cálculo em Tempo Real da Carga Horária Diária**: O sistema calcula instantaneamente a duração útil da jornada de trabalho em horas e minutos e a exibe dinamicamente na tela (ex.: `8 horas e 0 minutos`).
- **Ações de Controle**:
  - `Restaurar Padrão`: reestabelece a jornada convencional de segunda a sexta-feira, das 08:00 às 17:00.
  - `Cancelar`: fecha o diálogo sem persistir alterações.
  - `OK`: salva as definições no calendário de trabalho (`WorkCalendar`) e dispara o recálculo automático de datas e caminhos críticos de todas as atividades do projeto.

### 16.9. Gerenciamento de Feriados e Calendário de Nações
Acesse através do menu `Configurações -> Feriados` na janela do Cronograma Gantt:
- **Calendário Visual Interativo**: Navegue facilmente pelos meses e anos utilizando os seletores do calendário. Clique sobre qualquer data para selecioná-la e visualizá-la no campo de data. Datas cadastradas como feriado são realçadas visualmente no calendário.
- **Cadastro e Exclusão Manual de Feriados**:
  - Para adicionar uma data especial ou feriado local: selecione a data no calendário, digite o nome ou descrição no campo `Descrição:` e clique no botão `➕ Adicionar Feriado`.
  - Para remover um feriado: selecione a linha correspondente na tabela de feriados e clique no botão `❌ Remover Feriado`.
- **Carga Automática de Feriados de 18 Países**:
  - Selecione a nação desejada na lista suspensa `Feriados por País:`. O catálogo integrado contempla 18 países:
    1. Brasil
    2. Estados Unidos
    3. Portugal
    4. Argentina
    5. Canadá
    6. Reino Unido
    7. França
    8. Alemanha
    9. Itália
    10. Espanha
    11. Japão
    12. China
    13. México
    14. Chile
    15. Colômbia
    16. Uruguai
    17. Paraguai
    18. Austrália
  - Clique no botão `Aplicar`: o aplicativo popula automaticamente a lista com todos os feriados nacionais oficiais do país escolhido para o ano corrente e subsequentes.
  - O mecanismo calcula analiticamente os feriados móveis dependentes da Páscoa (Carnaval, Sexta-feira Santa, Domingo de Páscoa e Corpus Christi), além de todos os feriados civis e religiosos de data fixa.
- **Tabela de Feriados com Ergonomia WBS**:
  - A tabela exibe colunas organizadas de `Data` e `Descrição`.
  - As colunas contam com redimensionamento bidirecional arrastando tanto a borda direita quanto a borda esquerda do cabeçalho.
  - O realce visual adapta-se suavemente aos temas Claro e Escuro do Windows 11, garantindo conforto visual e legibilidade perfeita.
- **Impacto no Cronograma**: Dias cadastrados como feriado recebem rigorosamente o mesmo tratamento de dias não úteis (folgas), sendo ignorados no avanço de tarefas e estendendo adequadamente o prazo final de atividades no gráfico de Gantt e no motor CPM.

---

## 17. Ferramentas de produtividade

Acessíveis no menu `Ferramentas`:

- **Abertura Instantânea e Desempenho em Memória**: Todos os módulos e janelas de diálogo do menu Ferramentas operam com extração direta em memória $O(1)$, dispensando ciclos pesados de gravação/leitura em disco e suprimindo eventos redundantes de interface, garantindo abertura imediata e fluida sem congelamentos.
- **Busca global** (`Ctrl+Shift+G`): Localiza tarefas por texto, descrição, tags ou arquivos em todos os módulos.
- **Hoje / Próximos 7 dias** (`Ctrl+Shift+H`): Painel de curto prazo com as entregas mais urgentes.
- **Filtros** (`Ctrl+Alt+F`): Filtragem rápida de tarefas por quadrante, prioridade, status e prazos.
- **Registro de Atividades**: Cadastro de avaliações, planos de estudo e agendas acadêmicas com persistência segura e carregamento imediato.
- **Cronograma de Gantt BIM 4D**: Gestão profissional de cronogramas, WBS, análise CPM e simulação construtiva 4D com sincronização sob demanda.
- **Gerar próximas recorrências**: Criação em lote de repetições (diárias, semanais, mensais) com prévia e proteção contra duplicadas.
- **Ativar lembretes locais**: Temporizadores de notificação dentro do app ou na bandeja do sistema com inicialização ágil.
- **Exportar calendário .ics**: Geração de arquivo iCalendar padrão para importar em calendários externos.
- **Histórico de produtividade**: Métricas e relatórios sobre tarefas concluídas e ciclos pomodoro.
- **Fluxo integrado de estudo**: Conversão de materiais de estudo em conceitos Feynman e tarefas de revisão.
- **Templates**: Modelos prontos de tarefas para estudos, projetos, trabalho e planejamento semanal.
- **Criar revisões espaçadas**: Algoritmo de repetição espaçada por nível de domínio (1, 3 e 7 dias) com processamento instantâneo em memória.
- **Gerenciador de plugins**: Monitor de módulos opcionais carregados, caminhos e diagnósticos.
- **Gerenciador de logs**: Leitor cronológico em tempo real dos arquivos `file_eisenpo_*.log`.

### 17.1. Como Configurar e Gerar Recorrências em Lote
Acesse pelo menu `Ferramentas -> Gerar próximas recorrências`:
- **Definição Individual Prévia**: Clique com o botão direito sobre qualquer tarefa no Eisenhower ou na Lista Global e escolha `🔁 Recorrência` para configurar a frequência individual (diária, semanal, mensal ou desativada) e opcionalmente uma data limite final de repetição.
- **Geração em Lote**:
  - Selecione os métodos de origem e destino (Eisenhower, Pomodoro, Feynman);
  - Escolha o período de aplicação (ex.: próximos 30, 60 ou 90 dias);
  - Defina o limite máximo de repetições por item;
  - Ative `Evitar duplicadas existentes` para proteger sua base contra itens repetidos na mesma data;
  - Use `Aplicar frequência padrão` para gerar ocorrências mesmo para itens que ainda não possuam frequência configurada individualmente;
  - Para tarefas sem horário, ative a opção de horário padrão e defina o horário de referência.
- **Ações e Botões**:
  - `Salvar definições`: grava as configurações e preferências para as próximas gerações;
  - `Gerar ocorrências`: processa a geração das novas tarefas recorrentes na base unificada;
  - `Restaurar padrão`: redefine todas as opções para as configurações recomendadas de fábrica;
  - `Restaurar tarefas anteriores`: desfaz a última geração executada utilizando o snapshot de segurança;
  - `Fechar`: fecha o diálogo.
- **Tradução Dinâmica ao Vivo e Localização**:
  - Todos os botões (`"Restaurar padrão"`, `"Gerar ocorrências"`, `"Restaurar tarefas anteriores"`, `"Salvar definições"`, `"Fechar"`), as caixas de seleção, a mensagem da tabela quando vazia (`"Nenhuma tarefa pronta para gerar ocorrências com as configurações atuais."`) e as formatações de data são atualizados dinamicamente ao alternar o idioma do aplicativo, sem fechar a janela.

### 17.2. Como Ativar e Gerenciar Lembretes Locais
Acesse pelo menu `Ferramentas -> Ativar lembretes locais`:
- **Configuração de Horários e Alertas**:
  - No quadro **"Quando alertar"**, escolha entre:
    - `Usar data e horário de cada tarefa`: utiliza estritamente o agendamento individual já configurado em cada tarefa;
    - `Definir um único horário personalizado`: programa todos os lembretes do lote para um horário unificado escolhido por você.
  - **Canais de Notificação**: marque `Exibir alerta no aplicativo` para janelas de notificação internas, `Notificar na bandeja do sistema` para avisos nativos na bandeja do Windows, ou ambos simultaneamente.
  - **Tratamento de Tarefas sem Horário**: marque `Horário padrão se faltar` para atribuir um horário automático a itens que contenham apenas data.
- **Ações e Botões**:
  - `Testar aviso`: dispara imediatamente um alerta de exemplo para verificar visual e sonoramente a notificação;
  - `Aplicar lembretes`: ativa os lembretes nos temporizadores locais da aplicação;
  - `Limpar ativos`: cancela e remove com um clique todos os lembretes que estejam agendados e ativos;
  - `Salvar definições`: persiste suas preferências de notificação;
  - `Restaurar padrão`: retorna todas as opções aos valores iniciais;
  - `Fechar`: encerra a janela.
- **Tradução Dinâmica em Tempo Real**:
  - Todos os botões (`"Limpar ativos"`, `"Restaurar padrão"`, `"Aplicar lembretes"`, `"Salvar definições"`, `"Fechar"`, `"Testar aviso"`), o rótulo do quadro (`"Quando alertar"`), os botões de rádio (`"Usar data e horário de cada tarefa"` e `"Definir um único horário personalizado"`) e todas as caixas de seleção traduzem-se imediatamente ao vivo ao alternar entre Português e Inglês.

### 17.3. Fluxo Integrado de Estudo, Templates e Revisões Espaçadas
- **Fluxo Integrado de Estudo** (`Ferramentas -> Fluxo integrado de estudo`):
  - Converte um tópico de estudo em aprendizado estruturado: cria automaticamente um conceito no Método Feynman para aprofundamento e agenda uma tarefa de consolidação e revisão na Matriz de Eisenhower para 2 dias depois.
  - Rótulos, campos e botões traduzem-se dinamicamente em tempo de execução.
- **Templates de Tarefas** (`Ferramentas -> Templates`):
  - Pacotes prontos de tarefas padronizadas para cenários comuns:
    - *Plano de Estudo*: fases de leitura, fichamento, resolução de exercícios e simulado;
    - *Desenvolvimento de Projeto*: etapas de levantamento de requisitos, arquitetura, desenvolvimento, testes e deploy;
    - *Planejamento Semanal*: alinhamento de prioridades, organização de rotina e retrospectiva;
    - *Revisão de Conteúdo*: blocos de revisão espaçada e autoexplicação.
  - Prévia instantânea do conteúdo e inserção direta nos quadrantes com um clique. Títulos, descrições e botões contam com retradução dinâmica ao vivo.
- **Criar Revisões Espaçadas** (`Ferramentas -> Criar revisões espaçadas`):
  - Analisa as tarefas da aba ativa e calcula intervalos científicos de repetição espaçada com base no nível de domínio (`Iniciante`: 1 dia, `Intermediário`: 3 dias, `Avançado`: 7 dias).
  - Permite revisar e editar as tarefas antes de transferi-las em lote para Eisenhower, Pomodoro e Feynman, com histórico e botão para desfazer a última transferência.
  - Atualização dinâmica de colunas e botões na troca de idioma em tempo de execução.

---

## 18. Arquivo, importação e exportação

- **Novo** (`Ctrl+N`): Inicia uma nova sessão de trabalho.
- **Abrir** (`Ctrl+O`): Importa dados de arquivos externos (JSON, CSV, XLSX, PDF).
- **Salvar** (`Ctrl+S`): Salva o estado atual na base local.
- **Salvar como** (`Ctrl+Shift+S`): Exporta tarefas nos formatos JSON, CSV, XLSX ou relatórios formatados em PDF.
- **Limpar** (`Ctrl+Shift+Del`): Remove todas as tarefas da visualização com confirmação de segurança.
- **Sair** (`Ctrl+Q`): Encerra o aplicativo.

---

## 19. Idiomas e traduções

Em `Configurações -> Idioma`:
- **Português (Brasil)** (`pt_BR`)
- **English (United States)** (`en_US`)

A alternância de idioma ocorre instantaneamente em tempo real em todas as janelas, diálogos, tabelas, abas e botões sem necessidade de reiniciar a aplicação.

---

## 20. Persistência e privacidade

- Diretório persistente local: `%LOCALAPPDATA%\EisenPo` (com migração automática de `%LOCALAPPDATA%\EisenhowerOrganizer`, se existir).
- Todos os arquivos (`tarefas.json`, `layout.json`, `language.json`, `feynman_rotulos_config.json`, `listas.db`, `agenda_config.json`, `agendas/*.json`, `eisenpo_gantt.db`, configurações de fontes/recorrências/lembretes e logs `logs/file_eisenpo_*.log`) são mantidos estritamente no seu computador.
- Não há envio de telemetria, rastreamento ou dados à nuvem; **a Microsoft NÃO cobre este aplicativo sob a Política de Privacidade da Microsoft** ([`docs/privacy-policy.md`](docs/privacy-policy.md#ptbr)).
- **Desinstalação Limpa:** Remova o **EisenPo** a qualquer momento via **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou Menu Iniciar → Desinstalar), sem deixar serviços em segundo plano.

---

## 21. Atalhos úteis

| Atalho | Ação |
| --- | --- |
| `Ctrl+N` | Nova sessão de tarefas |
| `Ctrl+O` | Abrir / Importar arquivo |
| `Ctrl+S` | Salvar tarefas |
| `Ctrl+Shift+S` | Salvar como / Exportar (XLSX, PDF, CSV, JSON) |
| `Ctrl+Shift+Del` | Limpar todas as tarefas |
| `Ctrl+Q` | Sair da aplicação |
| `Ctrl+M` | Mostrar / Ocultar Calendário |
| `Ctrl+Shift+E` | Mostrar / Ocultar painel Em Progresso (Pomodoro) |
| `Ctrl+Shift+B` | Abrir Cronograma Gantt & BIM 4D |
| `Ctrl+Shift+G` | Busca global de tarefas |
| `Ctrl+Shift+H` | Painel Hoje / Próximos 7 dias |
| `Ctrl+Alt+F` | Diálogo de Filtros avançados |
| `Ctrl+Shift+A` | Janela Sobre |
| `Ctrl+Shift+M` | Janela Manual de Utilização |

---

## 22. Resolução de problemas

- **Módulos e recursos integrados**:
  - Todos os 4 módulos essenciais (`Pomodoro`, `Feynman`, `Registro de Atividades / Agenda` e `Cronograma de Gantt & BIM 4D`) estão compilados e integrados nativamente dentro do executável principal do EisenPo.
  - Não há necessidade de instalar DLLs externas nem de manter pastas de plugins separadas. Você pode acessar qualquer módulo diretamente pelo menu `Ferramentas` ou pelos modos de visualização em `Configurações -> Layout de Tarefas`.
  - Caso deseje inspecionar o status das extensões e componentes, abra `Ferramentas -> Gerenciador de plugins` ou consulte `Ferramentas -> Gerenciador de logs`.
- **Arquivo vinculado não abre**:
  - Verifique se o arquivo não foi movido, renomeado ou excluído e se o Windows possui um programa padrão associado à sua extensão.
- **Lembretes locais não disparam**:
  - Certifique-se de manter o EisenPo aberto (ou minimizado na bandeja do sistema) e que o horário agendado pertence ao futuro.
- **O cronograma Gantt não abre ou não salva**:
  - Verifique se o diretório `%LOCALAPPDATA%\EisenPo` possui permissões normais de leitura e gravação para criar e acessar o banco `eisenpo_gantt.db` (e `listas.db` na Agenda).

---

## 23. Autor, Licenciamento e Suporte

- **Autor:** Fernando Nillsson Cidade
- **E-mail de Suporte (best-effort, sem suporte Microsoft):** `linceu_lighthouse@outlook.com`
- **Repositório (GNU AGPLv3):** https://github.com/fernandoncidade/EisenPo_cpp
- **Microsoft Store (EULA Oficial):** [Adquirir EisenPo na Microsoft Store](https://apps.microsoft.com/detail/9P289X0185C3) · [Licenciamento (`docs/licensing.md`)](docs/licensing.md#ptbr)

---

## <a id="enus"></a>English (US)

> **Dual Licensing, Privacy & Disclaimer ("AS IS"):**
> - **GitHub Open Source vs. Official Microsoft Store Package:** All repository source code is licensed under **GNU Affero General Public License v3 (`GNU AGPLv3`, [`LICENSE`](LICENSE))**, whereas the official signed Microsoft Store package (`9P289X0185C3`) is governed by the **[Official EULA](source/assets/eisenhower/EULA/EULA_en_US.txt)** and **[CLC](source/assets/eisenhower/CLC/CLC_en_US.txt)**; developers are granted **full freedom** to adapt institutional docs in `source/assets/`. See [`docs/licensing.md`](docs/licensing.md#enus).
> - **Microsoft Independence & Clean Uninstallation:** **Microsoft does NOT provide a privacy policy or customer support for EisenPo.** All local persistence stays **100% offline** in `%LOCALAPPDATA%\EisenPo` ([`docs/privacy-policy.md`](docs/privacy-policy.md#enus)), with clean removal via **Windows Settings → Apps**.
> - **Auxiliary Scope ("AS IS"):** Free software intended as an auxiliary aid for productivity, active learning, and schedule planning (provided `"AS IS"` under the Official EULA, Law No. 9.609/1998, and GNU AGPLv3 Sections 15–16); critical contractual or engineering schedules always require independent verification.

This user manual describes the operational and technical features of **EisenPo** (Eisenhower Organizer). It covers workflows, shortcuts, settings, and the behaviors of all integrated native modules. *(See also [`docs/user-guide.md`](docs/user-guide.md#enus).)*

## 1. Overview

**EisenPo** is a comprehensive native desktop productivity suite targeting modern **C++ (C++23)** and **Qt 6.12.0 Widgets / Multimedia / Sql** for strategic task management, timed focus execution, structured study methodologies, and academic/professional activity tracking.

- `Eisenhower`: Strategic task planning and prioritization by importance and urgency (Built-in host).
- `Pomodoro`: Deep focus execution intervals, short/long breaks, and configurable audio alarms.
- `Feynman`: Active study framework with concepts, plain-language explanations, gaps, and final synthesis.
- `Activity Registration`: Detailed academic tracking, hierarchical tree lists, and custom color-coded schedules.
- `Gantt & 4D BIM`: Professional project scheduling, hierarchical WBS, critical path analysis (CPM), interactive horizontal timeline, and 4D construction simulation linked to IFC BIM elements.

All modules are permanent, native components of the application, operating directly without external DLLs or supplementary plugin folders.

### 1.1. RELEASE NOTES: Version 2026.11.22.0

Date: November 22, 2026

1. **Unified Chromatic Status Standardization Across WBS Table and Gantt Chart**:
   - **Visual Identity Integrated with 4D BIM Simulation**:
     - All activities displayed in the "WBS Table" and visually represented on the "Gantt Chart" now share the exact same status color palette used in the "Temporal Simulation - 4D BIM" indicator badges of the "4D Simulation" table.
     - `✅ Executed:` Green (`#4caf50`) for completed activities (100% physical completion).
     - `🔄 In Progress:` Blue (`#2196f3`) for activities currently underway and on schedule.
     - `⚠️ Delayed:` Red (`#f44336`) for delayed activities (covering both delayed start where the current date exceeds the planned start without progress, and overdue completion where the current date exceeds the planned finish while incomplete).
     - `⏳ Future:` Neutral gray (`#9e9e9e`) for upcoming scheduled activities that have not reached their start date.
   - **Harmonized Activity Boxes and Gantt Nodes**:
     - Timeline activity boxes and milestone nodes adopt the same status chromatic identity. For completed tasks (`✅ Executed:`), a gentle, refined green tone (#4caf50 with balanced opacity and contrast) is applied, distinguishing finished deliverables clearly and elegantly from active or delayed items.

2. **Rich, Detailed, and Dynamically Retranslated Timeline Tooltips**:
   - **Comprehensive Overview on Mouse Hover**:
     - Hovering the mouse cursor over any activity box or timeline node in the Gantt Chart immediately opens a rich, styled informational tooltip showing:
       - WBS hierarchical item code;
       - Color-coded operational status badge (`Executed`, `In Progress`, `Delayed`, or `Future`);
       - Start and Finish dates formatted according to the active locale (`MM/dd/yyyy HH:mm` in English and `dd/MM/yyyy HH:mm` in Portuguese);
       - Total duration in integer days;
       - Physical completion percentage;
       - Critical Path condition highlighted (`YES` in bright red for critical tasks or `No` in neutral gray for non-critical tasks);
       - Total float in hours.
   - **Dynamic Runtime Retranslation**:
     - Switching interface language under `Settings -> Language` immediately and dynamically retranslates all tooltip labels, date formats, and status badges in real time without requiring an application restart.

3. **Version Editor Utility Expansion for Agenda Contractual Documents**:
   - The internal version management editor now fully automates version tracking and updates across all 12 legal notice, license agreement, terms, and privacy policy documents belonging to the Activity Registration and Agenda module (`plugin_agenda`) in both Portuguese and English.

4. **How to View Dual Licensing, Privacy Policy, and Release Notes in "Options → About" (`Ctrl+Shift+A`)**:
   - Open `Options → About` (or press `Ctrl+Shift+A`) to access the application's complete legal transparency and licensing hub.
   - Switch across the top tabs: **History**, **Details**, **Licenses**, **Agreement**, **Copyright**, **Notices**, **Privacy Policy**, and **Release Notes**.
   - Inside multi-tab document sections, switch individually between **General**, **Eisenhower Matrix**, **Pomodoro**, **Feynman**, **Activity Registration**, and **Gantt & BIM 4D**, use the built-in **Search:** bar with previous/next arrows, and consult official open-source reference links under **Licenses**.

5. **How to Use the Interactive Manual with Quick Search (`Options → Manual` / `Ctrl+Shift+M`)**:
   - Open `Options → Manual` (or press `Ctrl+Shift+M`) to launch the built-in instruction guide.
   - Use the left section list or clickable **Table of Contents** links to jump directly to any topic.
   - Type keywords into the top **Search:** box (`Search text in the manual...`) and press `Enter` or click **Previous result** / **Next result** to step through highlighted matches in real time.

6. **Continuous Stability in the Feynman Method and Integrated Workflow**:
   - All five steps of the Feynman Method (**Concept**, **Explain Simply**, **Identified Gaps**, **Final Review and Simplification**, and **Description** — customizable via `Settings → Edit Labels`) remain preserved and synchronized two-way with Eisenhower Matrix and Pomodoro tasks.

7. **100% Local/Offline Operation and Clean Uninstallation**:
   - All task files, agendas, schedules, settings, and diagnostic logs reside locally inside `%LOCALAPPDATA%\EisenPo` with zero telemetry, no required online accounts, and clean removal via **Windows Settings → Apps**.

---

### 1.2. RELEASE NOTES: Version 2026.9.27.0

Date: September 27, 2026

1. **Native Module Integration into Core Executable**:
   - **Full In-Tree Compilation**:
     - All four productivity modules (`Pomodoro`, `Feynman`, `Activity Registration / Agenda`, and `Gantt Schedule & 4D BIM`) are now native compilation targets compiled directly into the `EisenPo` desktop executable.
     - The dependency on external dynamic libraries (`PomodoroPlugin.dll`, `FeynmanPlugin.dll`, `AgendaPlugin.dll`, `GanttPlugin.dll`) and external `plugins/` directories has been permanently removed.
   - **How to Use Native Modules**:
     - *Tools Menu*: Open `Tools -> Activity Registration` and `Tools -> Gantt & 4D BIM Schedule` instantly without any dynamic library load overhead.
     - *User Interface Layouts*: Smoothly toggle between `EisenPo`, `Pomodoro`, `Feynman`, and the integrated `PoCa` (Pomodoro + Dual Calendar) modes via `Settings -> Task Layout -> User Interface`.
     - *Updated Plugin Manager*: In `Tools -> Plugin Manager`, modules are reported with native integration and operational readiness directly in the system core.
   - **Windows Reliability**:
     - Complete elimination of antivirus false-positive quarantine issues, library path search errors, or version desynchronization between host and plugins.

2. **Centralized and Unified Translation System**:
   - **Unified Translation Catalog**:
     - All translation files (`.ts` and `.qm`) across all modules (`eisenhower_organizer`, `pomodoro`, `feynman`, `agenda`, and `gantt`) are now centralized in `source/language/translations/`.
     - The automated build system compiles and deploys all `.qm` files directly to the executable's `translations/` directory, ensuring 100% string coverage at all times.
   - **How to Use Dynamic Translation**:
     - Navigate to `Settings -> Language` and pick either `Português (Brasil)` or `English (United States)`.
     - All open windows, auxiliary productivity dialogs, schedule tables, and context menus translate instantly at runtime without an application restart.
   - **Enhanced Translation Tooling**:
     - The `CompileTranslationsTerminal` and `CompileTranslationsGUI` developer utilities now feature robust targeted compilation (`--module <id>`) and global generation (`--all`) for flawless localization management.

3. **Asynchronous Task Persistence and Non-Blocking Performance**:
   - **Smooth Background File Operations**:
     - Saving tasks to the unified data store (`tarefas.json`) and propagating updates to the Gantt schedule occurs asynchronously in the background with atomic file protection, eliminating GUI pauses during creation, editing, drag-and-drop, or task completion.
   - **Continuous Real-Time Parity**:
     - Real-time synchronization guarantees that all task modifications (due dates, durations, priorities, and completed Pomodoro cycles) reflect immediately on the Gantt timeline and physical progress bars.

---

### 1.3. RELEASE NOTES: Version 2026.9.26.0

Date: September 26, 2026

1. **Persistent Table Layout in Gantt & 4D BIM Schedule via `layout.json`**:
   - **Available Layout Configurations**:
     - Through the Gantt window menu bar (`Tools -> Gantt & 4D BIM Schedule`), navigate to `Settings -> 📐 Layout -> 📊 Tables` to dynamically switch between 5 geometric arrangements:
       1. `Original`: balanced 50%/50% horizontal split between the WBS Table and Gantt Timeline in the upper area, with the 4D Simulation panel in the lower area;
       2. `Top Vertical`: "WBS Table" and "4D Simulation" side-by-side in the top half, with the "Gantt Timeline" occupying the entire bottom half;
       3. `Bottom Vertical`: "Gantt Timeline" in the top half, with the "WBS Table" and "4D Simulation" side-by-side in the bottom half;
       4. `Left Horizontal`: "WBS Table" and "4D Simulation" vertically stacked in the left panel, and the "Gantt Timeline" occupying the entire right panel;
       5. `Right Horizontal`: "Gantt Timeline" occupying the entire left panel, and the "WBS Table" and "4D Simulation" vertically stacked in the right panel.
   - **Secure Atomic Persistence (`layout.json`)**:
     - The selected table layout mode is immediately recorded to the system configuration file `C:\Users\<user>\AppData\Local\EisenPo\layout.json` under the `"table_layout_mode"` property;
     - Writes are performed atomically and non-destructively (`QSaveFile`), preserving and merging all existing user preferences (such as main Eisenhower window layout choices);
   - **Automatic Restoration on Startup**:
     - When launching the application and opening "EisenPo − Schedule Management, Gantt & 4D BIM", the system automatically loads `layout.json` and restores the selected table layout mode, configuring splitters and menu checkmarks without requiring manual reconfiguration.

2. **Instant Performance and Responsiveness Across Productivity Tools**:
   - **Freezes and Delays Eliminated**:
     - All tools accessible from the `Tools` menu:
       - `Activity Registration` (evaluations, study plans, and academic schedules);
       - `Gantt & 4D BIM Schedule` (project scheduling, CPM, and 4D simulation);
       - `Generate Next Recurrences` (batch creation of recurring events);
       - `Enable Local Reminders` (in-app alerts and system-tray notifications);
       - `Create Spaced Reviews` (active retention and spaced repetition);
     - Now open instantaneously upon clicking, without any freezing of the main user interface.
   - **$O(1)$ In-Memory Data Extraction without Redundant Disk I/O**:
     - Tasks are queried and extracted directly from in-memory cache structures, completely eliminating redundant disk write/read cycles.
   - **Signal-Flood Suppression and Single-Pass Rendering**:
     - Dialog windows (such as *Recurrences* and *Local Reminders*) suppress signal emissions during widget construction and render table previews in a single pass, avoiding costly repeated layout calculations.
   - **Smart On-Demand Gantt Synchronization**:
     - When the Gantt window is minimized or hidden, background task changes avoid costly UI widget repopulation until the window is displayed again.
   - **Native Windows Dynamic DLL & SQLite Driver Resolution**:
     - The dynamic library resolution pipeline honors native Windows system search rules, ensuring database drivers (`qsqlite.dll`) and native plugins initialize smoothly and rapidly.

---

### 1.4. RELEASE NOTES: Version 2026.9.25.0

Date: September 25, 2026

1. **Discrete Physical Progress Calculation in the Gantt Schedule by Pomodoro Cycles**:
   - The physical progress percentage (%) of schedule-linked tasks is now calculated strictly and proportionally based on completed Pomodoro blocks relative to the task's planned cycle target:
     $$\text{Physical Progress} = \left(\frac{\text{Completed Pomodoros}}{\text{Target Pomodoros}}\right) \times 100\%$$
   - **Predictable Discrete Increments**:
     - For a task with a target of 4 pomodoros: progress steps advance strictly across **0%**, **25%**, **50%**, **75%**, and **100%**;
     - For a task with a target of 5 pomodoros: progress steps advance strictly across **0%**, **20%**, **40%**, **60%**, **80%**, and **100%**;
     - Tasks with any other integer cycle target (1 to 999 cycles) progress proportionally in exact mathematical steps.
   - **Elimination of Arbitrary 10% Initial Progress**:
     - The arbitrary 10% progress increment previously assigned upon linking or starting a task has been completely eliminated;
     - The activity begins at strictly **0%** physical progress until Pomodoro work cycles are completed.
   - **Full Activity Completion**:
     - The task reaches 100% physical progress when all planned cycle pomodoros are finished or when the user marks the task as completed in the Eisenhower Matrix.
2. **Instantaneous Real-Time Synchronization with the Gantt Schedule**:
   - Any task modification made on the main screen (description edits, dates, priority, quadrant moves, completion toggle, or pomodoro increments) propagates immediately and in real time to the open Gantt schedule, keeping the Gantt bar and completion percentages in perfect sync without manual action.
3. **Comprehensive Dynamic Runtime Translation for Productivity Tools Dialogs**:
   - **"Generate Next Recurrences" Dialog** (`Tools -> Generate Next Recurrences`):
     - Live runtime translation of all action buttons: `"Restore defaults"`, `"Generate occurrences"`, `"Restore previous tasks"`, `"Save definitions"`, and `"Close"`;
     - Live translation of the empty preview placeholder text: `"No tasks ready to generate occurrences with current settings."`;
     - Dynamic locale-aware date formatting based on the active locale (`QLocale`).
   - **"Enable Local Reminders" Dialog** (`Tools -> Enable Local Reminders`):
     - Live runtime translation of all action buttons: `"Clear active"`, `"Restore defaults"`, `"Apply reminders"`, `"Save definitions"`, `"Close"`, and `"Test notification"`;
     - Dynamic translation of the options frame title `"When to alert"`;
     - Dynamic translation of radio buttons `"Use date and time from each task"` and `"Set a single custom time"`, as well as notification channel options.
   - **"Integrated Study Flow", "Templates", and "Create Spaced Reviews" Dialogs**:
     - Instantaneous translation of all buttons, categories, inputs, descriptions, tables, and feedback messages when switching between English and Portuguese at runtime without restarting.

---

### 1.5. RELEASE NOTES: Version 2026.9.23.0

Date: September 23, 2026

1. **Instantaneous Startup Without Freezes**:
   - Immediate application launch with instant unlocking of manual controls and the mouse cursor, completely eliminating the previous several-second startup lag and interface freezing.
   - On-demand lazy initialization of multimedia and audio subsystems, keeping Qt's primary event loop unblocked and immediately responsive to user interactions.
2. **New Integrated PoCa Layout (Pomodoro + Dual Calendar)**:
   - Unified directly into a single dedicated left sidebar alongside the Eisenhower Matrix via `Settings -> Task Layout -> User Interface -> PoCa -> Enable` (or `Disable`).
   - Four harmoniously stacked frames in the left column:
     1. **"Timer"** frame (Pomodoro countdown, focus/break indicators, and session controls);
     2. **"Settings"** frame (Pomodoro interval durations, rest times, and preferences);
     3. **"Calendar 1"** frame (reference current month with visual date markers);
     4. **"Calendar 2"** frame (subsequent sequential month, fully synchronized with Calendar 1).
   - **Uniform Flush Vertical Spacing**: The second calendar sits directly below the first with an identical 6px flush vertical spacing (matching the gap between Settings and the first calendar), eliminating unsightly visual gaps.
3. **Symmetrical Alignment Across All 4 Frames**:
   - The outer lateral borders (left and right) of all four frames ("Timer", "Settings", "Calendar 1", and "Calendar 2") feature absolute millimeter-level symmetrical alignment with each other, forming an aesthetically seamless and clean vertical column.
4. **Dynamic $2 \times K$ Sequential Month Progression on Horizontal Expansion**:
   - When dragging the vertical splitter to the right to widen the sidebar, calendars duplicate horizontally in new columns following an unbroken mathematical matrix progression ($2 \times K$):
     - **Row 1** (top row): displays months in horizontal sequence from left to right ($M, M+1, M+2, \dots$);
     - **Row 2** (bottom row): continues the sequence sequentially starting from the end of Row 1 ($M+K, M+K+1, M+K+2, \dots$).
     - *Example with 2 columns ($2 \times 2 = 4$ calendars)*:
       - Row 1: Calendar 1 = September; Calendar 3 (beside it) = October.
       - Row 2: Calendar 2 (below 1) = November; Calendar 4 (below 3) = December.
     - Extends dynamically for 3 or more columns as horizontal space expands.
5. **Scrollbar Safety Margins and Proportional Startup Dimensions**:
   - When opening the application at default minimum window dimensions with the PoCa layout active, the left sidebar automatically budgets adequate safety margin for the vertical scrollbar, ensuring that the right border of all four frames is never covered, clipped, or overlapped by the scrollbar.
   - Calibrated balance with Eisenhower Matrix quadrants, guaranteeing pristine readability and fluid operation right from startup.

---

### RELEASE NOTES: Version 2026.9.21.0

Date: September 21, 2026

1. **Instantaneous Response and Zero-Wait Task Editing**:
   - Immediate feedback upon clicking the **"OK"** button in the **"✏️ Edit Task"** dialog: the success prompt closes instantly on the spot without holding the mouse cursor, freezing the interface, or delaying the user from continuing work.
   - Background persistence and schedule synchronization: file saving and CPM recalculations proceed asynchronously in the background, freeing manual controls right away.
2. **Comprehensive Task Lifecycle Visibility in the Calendar with `[In Progress]` Status**:
   - The **"Global Task List:"** panel and calendar date tooltips now accurately display active in-progress tasks marked with the `[In Progress]` tag, alongside `[Completed]` and `[Pending]`.
   - Detailed effort tracking: shows the number of finished Pomodoro blocks relative to each task's planned cycle target.
   - Fast contextual interactivity: double-clicking an item in the list opens its linked external file directly, and right-clicking opens the context menu with immediate task location.
3. **Real-Time Bilingual Dynamic Translation**:
   - Instant live translation of all calendar status badges (`[In Progress]` / `[Em Progresso]`), tooltips, and prompts when toggling between English and Portuguese without restarting the application.

---

### RELEASE NOTES: Version 2026.9.20.0

Date: September 20, 2026

1. **Individual Pomodoro Cycle Targets (🎯)**:
   - Replaced the fixed 4-cycle assumption with an individual cycle target (ranging from 1 to 999 cycles) configurable per task.
   - Configured directly in the "Edit Task" dialog via the dedicated `Target Pomodoro Cycles (🎯):` field.
   - Dynamic progress percentage calculation: $(\text{completed} / \text{target}) \times 100\%$, strictly clamped to $[0.0\%, 100.0\%]$.
   - In-progress tasks only automatically move to the "Completed" tab when reaching their specific cycle target, respecting individual task complexities.
2. **Cycle Target Warning Dialog (`⚠️ Cycle Target Warning`)**:
   - Automatically displays when starting the timer if any task in progress has a cycle target different from the timer's current cycle limit (`Cycle 1/N`).
   - Detailed visual comparison table showing: Task Name, Module/Origin, Individual Target (🎯), Active Timer Limit (⚙️), and Execution Behavior.
   - 3 Clear Decision Options:
     - `🔄 Reset all to N cycles`: updates all active tasks to match the timer's limit and starts the countdown;
     - `▶️ Start anyway`: keeps individual custom targets and proceeds with the timer;
     - `❌ Cancel`: stops timer start, allowing adjustments without altering status.
3. **Multi-Month Calendar Synchronization**:
   - Real-time bidirectional synchronization across all displayed calendar months in the Calendar panel.
   - Navigating backwards or forwards in secondary month views updates all calendars continuously without visual gaps.
4. **Fluid Drag-and-Drop Task Management**:
   - Dragging and dropping tasks across quadrants, tabs, or lists is now completely fluid and lightweight.
   - Elimination of synchronous disk writes during drag-and-drop operations ensures instantaneous visual feedback.
5. **Instant Gantt Window Opening**:
   - Conditional synchronization between the Eisenhower Matrix and the Gantt module makes opening the Gantt window instant ($O(1)$) when tasks have not changed.
6. **Dynamic In-Place Retranslation**:
   - Live translation applied immediately to active dialogs, tooltips, and quadrant headers when changing languages at runtime.

---

### RELEASE NOTES: Version 2026.9.14.0

Date: September 14, 2026

1. **Table and Timeline Layout Configurations (`Settings > Layout > Tables`)**:
   - Dedicated submenu in the menu bar of the "EisenPo − Schedule Management, Gantt & 4D BIM" window offering five distinct visual arrangements:
     - `Original`: proportional 50%/50% horizontal split between the WBS Table and Gantt Timeline at the top, with the 4D Simulation table at the bottom;
     - `Top Vertical`: WBS Table and 4D Simulation table side-by-side (50%/50% horizontal) at the top, with the Gantt Timeline across the bottom;
     - `Bottom Vertical`: Gantt Timeline across the top, with WBS Table and 4D Simulation table side-by-side (50%/50% horizontal) at the bottom;
     - `Left Horizontal`: WBS Table and 4D Simulation table stacked vertically (50%/50%) on the left, with the Gantt Timeline occupying the entire right side;
     - `Right Horizontal`: Gantt Timeline occupying the entire left side, with WBS Table and 4D Simulation table stacked vertically (50%/50%) on the right.
   - Exclusive checkmark selection with automatic persistence across sessions via `QSettings`.
2. **Proportional 50% / 50% Space Division between WBS and Gantt**:
   - Balanced horizontal allocation: WBS Table and Gantt Timeline divide the available width strictly fifty-fifty (50%/50%), ensuring ample space for data columns and the timeline bar chart.
3. **Parented Dialog Windows and Multi-Monitor Support**:
   - All confirmation dialogs, activity deletion prompts, and operation notices are directly parented and centered on the Gantt window.
   - Eliminates unexpected foreground focus stealing by the main EisenPo host when deleting activities on secondary displays or multi-monitor setups.
   - Complete, safe bidirectional deletion synchronization preserved with Eisenhower Matrix and Pomodoro.
4. **Persistent Ghost Line Overlay (`👁️ Ghost Line`)**:
   - Resolved timeline baseline overlay persistence: adding new tasks in EisenPo or creating new activities in the schedule no longer cancels or hides the Ghost Line.
5. **3-Mode Adaptive 4D Simulation Control Bar (`SingleLine`, `TwoLines`, `ThreeLines`)**:
   - Responsive reflow when dragging vertical splitters:
     - Single-Line Mode (width >= 1200px): playback buttons, Reference Date picker, timeline scrubber, and KPI badges in a single row;
     - Two-Lines Mode (width 780px to 1200px): playback buttons and Reference Date picker on row 1, and KPI status badges ("Total:", "Executed:", "In Progress:", "Delayed:", "Future:") on row 2;
     - Three-Lines Mode (width < 780px): Row 1 = playback buttons ("▶️ Start Simulation", "⏹️ Stop Simulation", "🔄 Reset Simulation"); Row 2 = Reference Date group and timeline scrubber; Row 3 = KPI status badges.
   - Automatic dynamic container height adjustments (~58px, ~84px, ~114px) preventing any clipping or table overlap.
6. **Strict Button Minimum Width Sizing Based on Text Length**:
   - Automated minimum width enforcement for simulation buttons based on font metrics, text length, and styling, preventing truncated text when resizing splitters or switching languages.
7. **Tight Coupling for "Reference Date:" and Calendar Picker**:
   - Direct 3px spacing between the "Reference Date:" label and the date picker widget, eliminating excessive blank margins and avoiding collisions with the reset button or scrubber slider.
8. **Dynamic Real-Time Localization**:
   - Complete dynamic translation for all new layout submenus, dialogs, buttons, and badges in US English and Brazilian Portuguese without restart.

---

### RELEASE NOTES: Version 2026.9.13.0

Date: September 13, 2026

1. **Custom Working Days and Daily Work Shift Configuration (`Settings > Working Days`)**:
   - Dedicated dialog `Configure Working Days and Working Hours` accessible from the menu `Settings -> Working Days` in the Gantt Schedule window.
   - Independent selection of any of the 7 weekdays (Monday through Sunday) to define project working days and weekend/rest days.
   - Configuration of workday start and end hours with real-time dynamic calculation and display of total daily working hours and minutes.
   - Automatic schedule recalculation across all project activities and CPM critical path boundaries considering custom working calendars.
2. **Holiday Manager with Interactive Calendar and 18-Country Library (`Settings > Holidays`)**:
   - Dedicated dialog `Manage Holidays and Non-Working Days` accessible from the menu `Settings -> Holidays`.
   - Interactive visual calendar to navigate months/years and pick non-working holiday dates directly.
   - Dropdown menu featuring 18 nations (Brazil, United States, Portugal, Argentina, Canada, United Kingdom, France, Germany, Italy, Spain, Japan, China, Mexico, Chile, Colombia, Uruguay, Paraguay, and Australia) and an `Apply` button to batch-import official national holidays, including analytical calculation of movable holidays (Carnival, Good Friday, Easter Sunday, and Corpus Christi).
   - WBS-styled holiday table with bidirectional dual-border column resizing and theme-adaptive highlighting for Windows 11 Light and Dark themes.
   - Strict treatment of registered holidays as non-working days in schedule logic and CPM math.
3. **Ergonomic Three-Row "New Task" Panel Layout**:
   - Row 1: Task description input box and destination quadrant selector;
   - Row 2: `Priority:` and `Mastery Level:` dropdown menus with automatic dynamic runtime width calculation matching the widest item;
   - Row 3: `Link date`, `Link time`, new `Duration:` (in days) control, and the `Add Task` button.
   - Strict minimum width constraints and containment boundaries preventing the `Add Task` button and adjacent controls from clipping or overlapping onto the `Duration:` input when moving the vertical splitter.
4. **Task Duration in Days Integrated Across Ecosystem**:
   - Explicit task duration in full days configured during creation/editing and synchronized across Eisenhower, Pomodoro, and Calendar.
5. **Full Dynamic Translation (Native Bilingualism)**:
   - Complete runtime dynamic localization for all new GUI dialogs, menus, tables, and buttons between US English (`en_US`) and Brazilian Portuguese (`pt_BR`).

---

### RELEASE NOTES: Version 2026.9.8.0

Date: September 8, 2026

1. **Integrated Native Menu Bar for Schedule Management, Gantt & 4D BIM**:
   - Native menu bar organizing project management operations across two primary menus:
     - **File Menu**: fundamental activity workflows, including creation (`➕ New Activity`), detailed properties editing (`✏️ Edit`), safe deletion with automatic precedence recalculation (`❌ Delete`), and spreadsheet interoperability (`📥 Import CSV` and `📤 Export CSV`).
     - **Settings Menu**: advanced tools, navigation, and integration, featuring dependency links (`🔗 Link/Predecessor`), instant critical path calculation (`⚡ Recalculate CPM`), baseline freezing (`📌 Save Baseline`), variance comparison (`📊 Compare Baseline`), baseline overlay (`👁️ Ghost Line`), time scale pickers (`🔍 Zoom Day`, `🔍 Zoom Week`, `🔍 Zoom Month`), view toggles (`📋 WBS Table` and `🏗️ 4D Simulation`), and direct Eisenhower Matrix integration (`🔄 Sync with EisenPo` and `📋 Import from EisenPo`).
   - Dynamic real-time translation for all menu titles, items, and dialog buttons when switching languages.
2. **Bidirectional Column and Row Resizing**:
   - Intuitive dimensional resizing: drag the right border of any column header right to expand or left to shrink; drag the left border left to expand or right to shrink.
   - Applied symmetrically to both "📋 WBS Table" and "🏗️ 4D Simulation" columns, and extended to vertical header row dividers in the 4D simulation table.
3. **Precise Single, Multi-Cell, Full-Column, and Full-Row Selection**:
   - Single-cell selection fix: clicking any table cell targets that exact cell without unwanted jumps or shifting to neighboring cells.
   - Contiguous rectangular selection (drag or Shift+Click) and non-contiguous selection (Ctrl+Click).
   - Full column selection by clicking top horizontal headers (with multi-column support via Ctrl/Shift) and full row selection by clicking vertical headers.
4. **Adaptive Theme Highlighting for Windows 11**:
   - In Dark Theme: cell hover and selection apply a gentle lighter gray tone relative to cell background, eliminating harsh white glaring or obscured data.
   - In Light Theme: cell hover and selection apply an elegant darker gray tone relative to cell background, avoiding pitch-black fills.
   - Seamless highlight propagation across single cells, multi-selection blocks, full rows, and full columns, preserving original status badge colors (`✅ Executed:`, `🔄 In Progress:`, `⚠️ Delayed:`, and `⏳ Future:`).
5. **Project Baseline, Ghost Line, and On-Demand CPM Recalculation**:
   - Save project snapshots as Baselines and compare them against active execution to detect schedule variances and delays.
   - The Ghost Line renders baseline projection bars directly onto the Gantt timeline for immediate visual appraisal.
   - On-demand recalculation of floats, schedule boundaries, and critical path items.
6. **Comprehensive Gantt Plugin Legal and Notice Documentation**:
   - Integrated 12 new legal and informative documents (CLC, COPYRIGHT, EULA, PRIVACY_POLICY, NOTICES, and ABOUT) in Portuguese and English accessible via the About dialog.
   - Version tracking utility `find_VersionEditor` updated to manage all new documentation files.

---

### RELEASE NOTES: Version 2026.9.7.0

Date: September 7, 2026

1. **New Schedule Management, Gantt & 4D BIM Module**:
   - Advanced suite for physical planning, temporal tracking, and virtual 4D construction simulation, dynamically loaded from the plugin under `plugins/gantt`.
   - **Work Breakdown Structure (WBS)**: hierarchical decomposition into deliverables and work packages with automated duration, date, and weighted completion calculations for summary parent activities.
   - **Critical Path Method (CPM) Engine**: analytical calculation of Early Start/Finish, Late Start/Finish, Free Float, and Total Float, highlighting critical path activities with bright red bars in both the WBS tree and timeline.
   - **Preventive Circular Dependency Protection**: real-time cycle detection on predecessor links with intelligent blocking and clear alerts to ensure schedule consistency.
   - **Interactive Timeline and Gantt Chart**: continuous horizontal timeline with Day, Week, and Month scales, vertical 'Today Line', and directed dependency arrows.
   - **Bidirectional Eisenhower Matrix Task Linkage**: link matrix tasks directly to schedule activities via the context menu (`Link to Gantt Schedule`), automatic progress sync, and `🔗 Linked` visual indicator.
   - **4D BIM Construction Simulation (IFC)**: dedicated tab to map universal building element identifiers (`IFC GUID / UniqueId`) to WBS items, interactive simulation player (Play, Pause, Step, Scrubber), and real-time status categorization (Planned, In Construction, Constructed, Delayed, Ahead of Schedule).
   - **CSV Import and Export**: full interoperability with external tools such as MS Project, Primavera P6, and Excel.
2. **Extreme Performance Optimization & Freeze-Free Instant Language Switching**:
   - High-performance $O(1)$ in-memory hash table (`QHash<QString, TaskLinkSummary>`) in both the host adapter and integration bridge, eliminating heavy SQLite disk I/O and deserializations during language switches.
   - Multithreaded `.qm` translation loading via dedicated `GanttTranslationWorker` running on a separate `QThread`.
   - Sub-5ms instantaneous GUI language transitions with zero interface freezing.
3. **Real-Time Dynamic Localization in Timeline and Simulation Tables**:
   - Runtime translation of months and weekdays on the Gantt timeline ruler.
   - Live update of WBS task link indicators (`🔗 Vinculada` / `🔗 Linked`).
   - Instant dynamic localization of 4D simulation table headers (`4D Status` / `Status 4D` and `IFC GUID / Unique ID` / `GUID IFC / Id Único`) and construction status badges without requiring window reloads or app restarts.
4. **Automated Version Tracking**:
   - Gantt plugin definitions (`gantt_plugin.json` and `gantt_plugin_interface.hpp`) integrated into `find_VersionEditor`.

---

### RELEASE NOTES: Version 2026.9.6.0

Date: September 6, 2026

1. **Smart Responsive Resizing for the "Edit Task" Window**:
   - Integrated native **Maximize** and **Minimize** buttons in the title bar of the task editing dialog.
   - Dynamic adaptive resizing: when widening the window to a wide view ($\ge 760\text{px}$) or maximizing it, the "Description (optional):" text box automatically repositions and expands across the entire left side, providing generous reading and typing room for long notes. All frozen-dimension fields ("Task:", "Priority:", "Mastery Level:", "Estimated Pomodoros (🍅):", "Linked file (drag or choose):", "Choose...", "Open", "Remove", date/time pickers, and "Cancel" / "OK" buttons) anchor cleanly at the bottom-right corner.
   - Smooth and automatic return to the compact vertical stacked layout when narrowing the window below 680px.
   - Enhanced border dragging stability with an 80px smart hysteresis band (680px to 760px), completely eliminating jitter, flicker, or mode-toggling oscillation when resizing by the left or right borders.
   - Rigorous minimum height protection: prevents overlapping or widget crowding of the description editor over top fields or bottom action buttons when narrowing the window height from the top or bottom borders.
2. **Custom Feynman Step Labels ("Settings > Edit Labels")**:
   - Native menu bar embedded directly within the "Feynman Description" window under `Settings -> Edit Labels`.
   - Dedicated label customization dialog organized as a clean 3-column matrix ("Default", "Current", "New") to customize the 5 steps of the Feynman technique ("1️⃣ Concept", "2️⃣ Explain Simply", "3️⃣ Identified Gaps", "4️⃣ Final Review and Simplification", and "5️⃣ Description").
   - `Apply` button to immediately save and propagate new labels across tabs, step frames, and composite task descriptions; `Restore` button to revert to system defaults; and `Close` button.
   - Dedicated `Close` button added to the footer of the "Feynman Description" window alongside `Clear` and `Save Concept` for quick dismissal.
   - Automatic local persistence in `feynman_rotulos_config.json`, full dynamic runtime translation, and backward compatibility with all historical tasks.
3. **Intuitive Task Context Menu Reordering**:
   - Newly optimized strategic action order when right-clicking any task:
     1. `✏️ Edit Task`
     2. `🪢 Feynman Description`
     3. `🔁 Recurrence`
     4. `🎨 Color tasks`
     5. `🔤 Font Settings`
     6. `🔀 Move to another quadrant`
     7. `🗑️ Remove Task`
4. **Symmetrical Border Alignment and Synchronized Splitter Thickness**:
   - Pixel-perfect lateral alignment between the outer left and right border lines of the "New Task" frame and the quadrant frames below it.
   - Perfectly synchronized and uniform thickness for all vertical splitters separating the Calendar, Pomodoro, and Quadrant panels.

---

### RELEASE NOTES: Version 2026.8.17.0

Date: August 17, 2026

1. **New Tabbed Layout Modes: Horizontal and Vertical**:
   - Under `Settings -> Task Layout -> User Interface -> EisenPo -> Tabbed Layout`, choose between `Horizontal` (side-by-side task columns) and `Vertical` (compact vertical stacking per quadrant).
   - Smart dynamic minimum width calculations based on tab title lengths and status columns.
2. **Pixel-Perfect Alignment and Stable "New Task" Panel**:
   - Clean and consistent 2-row layout for task input controls (description, quadrant selector, priority, mastery, date/time, and add button).
   - Prevents overlapping or visual clipping when resizing the window or adjusting splitters.
   - Symmetrical lateral border alignment with quadrant boundaries across all visual modes.
3. **Enhanced Pomodoro Sound Alarm System**:
   - Alarm configuration under `Settings -> Specific Settings -> Pomodoro -> Alarm`.
   - Selection of `.wav` chimes with live preview controls (`▶️ Play` / `⏹️ Stop`) and duration counter in seconds.
   - Adjustable alarm repeat interval (in seconds) and calibrated audio output volume.
   - Reliable and immediate playback of selected audio when focus and break sessions finish.
   - Smart Automatic Mode with configurable maximum alarm repeats before autonomous cycle advancement.
4. **Instant Runtime Dynamic Translation**:
   - Real-time dynamic translation of status tabs ("Pending", "In Progress", and "Completed") and counters without requiring a restart.

---

### RELEASE NOTES: Version 2026.8.15.0

Date: August 15, 2026

Official release of **EisenPo** (version 2026.8.15.0) — built with C++17 and Qt 6.11.1:

1. **4 Core Productivity Pillars Consolidated**:
   - Seamless integration across Eisenhower Matrix, Pomodoro Technique, Feynman Method, and Activity Registration/Schedules.
2. **Optimized Local Persistence**:
   - 100% local persistence in `%LOCALAPPDATA%/EisenPo`, featuring transparent automated migration from legacy `%LOCALAPPDATA%/EisenhowerOrganizer` data.
3. **Centralized Log Manager**:
   - Unified diagnostics in `logs/file_eisenpo_*.log` with live monitoring under `Tools -> Log Manager`.
4. **Safe Deletion and Synchronous Multi-Module Purge**:
   - Contextual confirmation prompts with default negative focus and real-time state synchronization across all modules.
5. **High-Performance Native Architecture**:
   - Native 64-bit binaries optimized for MinGW and MSVC/Ninja, ensuring rapid startup and minimal memory footprint.

Visual customization enhancements in Activity Registration and menu structuring:

1. **Independent Module Color Control**:
   - Independent selection of `Black` or `Colorful` modes for each environment (`EisenPo`, `Pomodoro`, and `Feynman`).
   - `General` option to apply visual preferences across all modules simultaneously.
2. **Reorganized Color Menus**:
   - Structured submenus under `Settings -> Activity Registration -> Colors` in the main menu and `Settings -> Colors` in `Activity Registration`.
   - Full dynamic runtime translation and instant task visual updates.

Visual synchronization refinements and filter optimizations:

1. **Consistent Visual Synchronization**:
   - Persistent color assignment for newly created activities.
   - Smart context-menu font synchronization without affecting unselected tasks.
2. **Clean and Standardized Filters**:
   - Standardized date and time extraction across all modules, eliminating duplicate filter entries.
   - Long text fields excluded from column filter menus to maintain clean options.

Reorganized menu hierarchy under `Settings` and centralized configuration:

1. **Structured Settings Menu**:
   - Logical sequence: `Task Layout`, `Font Settings`, `Specific Settings`, `Activity Registration`, `Language`.
   - Dedicated `Specific Settings` submenu for plugin-exclusive parameters (e.g., `Pomodoro -> Alarm, Automatic mode, Report Pomodoros`).
2. **Runtime Translations**:
   - All new menu actions and paths support instant runtime translation.

Independent plugin font customization and runtime dynamic translation:

1. **Independent Font Settings per Plugin**:
   - Font menus directly under plugin paths (`Font Settings -> Feynman / Pomodoro`).
   - Exclusive application to the respective plugin regardless of global sync state.
2. **Runtime Dynamic Translation**:
   - `changeEvent(QEvent::LanguageChange)` implemented across dialogs, context menus, and top-right interface counters.

Independent font settings for general tasks and linked files:

1. **Specialized Font Settings**:
   - `General Tasks`: customize appearance of tasks without linked files.
   - `Linked File`: customize appearance of tasks with external file attachments.
2. **Full Typography Control**:
   - Adjust font family, style, size, bold, italic, underline, and strikeout with default `Restore` button.

---

## 2. Main screen

On the main window, you will find:

- **Top menu bar**: `File`, `Settings`, `Tools`, `Options`.
- **Module tabs**: `Eisenhower`, `Pomodoro`, `Feynman` (according to active plugins).
- **Active-module counter**: Top-right corner showing selected tasks and total item count.
- **Main Eisenhower task area**: Central workspace with fast creation fields, priority/mastery pickers, and quadrants.
- **Calendar & Global Task List panel**: Time-based navigation and comprehensive task list.
- **Activity Registration dialog**: Accessible directly via the `Tools` menu.
- **Side panels**: `In Progress` panel (Pomodoro) and `My Tasks` panel (Feynman).
- **About Window (`Options → About` / `Ctrl+Shift+A`)**: Centralizes history, technical details, release notes, **Privacy Policy**, **Agreement (Official EULA)**, **CLC**, **Copyright**, **Notices**, and open-source **Licenses** across all modules with live in-tab search.
- **Interactive Manual (`Options → Manual` / `Ctrl+Shift+M`)**: Displays the complete guide with a clickable section index, **Search:** input box, and previous/next navigation arrows, translated live across English and Portuguese.

---

## 3. Interface tabs

Module tabs at the top of the main window adapt to available components:

- `Eisenhower` (Built-in host)
- `Pomodoro` (Optional plugin)
- `Feynman` (Optional plugin)

Configure layout settings under `Settings -> Task Layout -> User Interface`.

---

## 4. Active-module counter

The counter in the top-right corner updates dynamically upon switching tabs or selecting tasks:

- Displays selected task count and total items (e.g., `EisenPo: Selected tasks: 1 | Total tasks: 12`).
- Translates instantly during runtime language switching.

---

## 5. Eisenhower Matrix Module

The Eisenhower Matrix prioritizes tasks across 4 strategic quadrants:

- **Quadrant 1 (Important & Urgent)**: Crises, deadlines, and emergencies (Do First).
- **Quadrant 2 (Important, Not Urgent)**: Planning, study, health, and development (Schedule).
- **Quadrant 3 (Not Important, Urgent)**: Interruptions and routine requests (Delegate).
- **Quadrant 4 (Not Important, Not Urgent)**: Distractions and time-wasters (Eliminate).

Each task captures:
- Title and detailed notes;
- Target quadrant;
- Priority (Urgent, High, Medium, Low, None);
- Mastery level (Beginner, Intermediate, Advanced);
- Scheduled date and time;
- Planned duration in days;
- Attached external file path;
- Completion state (Pending or Completed);
- Custom display color.

---

## 6. Creating tasks and the "New Task" Panel

To add a new task:

1. Type the task description into the main input field on the **First Row** and select the destination quadrant (1 to 4).
2. On the **Second Row**, choose the `Priority:` and `Mastery Level:` from their dropdown menus.
3. On the **Third Row**, optionally check `Link date` and `Link time`, enter the planned `Duration:` in days, and click **Add Task** or press `Enter`.

### 6.1. Ergonomic Three-Row Architecture
The **New Task** panel organizes input controls across three clearly arranged rows:
- **Row 1**: Task title input box and destination quadrant selector;
- **Row 2**: `Priority:` and `Mastery Level:` dropdown menus. Both feature **dynamic runtime width auto-sizing**: the interface calculates required width based on the longest text option, guaranteeing that no label is truncated or padded with excessive empty space, even when switching UI languages;
- **Row 3**: `Link date` and `Link time` checkboxes, the dedicated `Duration:` (in days) spin box, and the **Add Task** action button.
- **Strict Splitter Boundary Protection**: Reinforced minimum width constraints and containment boundaries prevent the `Add Task` button and neighboring controls from clipping, overflowing, or overlapping the `Duration:` input box when manually dragging vertical splitters.
- **Symmetrical Border Alignment**: Outer frame boundaries align with millimeter precision to the quadrant boundaries positioned directly beneath across all layout views.

---

## 7. Editing tasks and Smart Responsive Resizing

To edit an existing task:

1. Right-click on any task in any list or quadrant and select **✏️ Edit Task**.
2. In the dialog window, modify any fields:
   - Task title (`Task:`);
   - Detailed description and notes (`Description (optional):`);
   - Priority (`Urgent`, `High`, `Medium`, `Low`, `None`);
   - Mastery Level (`Beginner`, `Intermediate`, `Advanced`);
   - Target Pomodoro Cycles (`Target Pomodoro Cycles (🎯):`), defining how many cycles (1 to 999) this specific task needs for full completion;
   - Estimated Pomodoros (`🍅`);
   - Linked external file (`Linked file (drag or choose):`), with buttons `Choose...`, `Open`, and `Remove`, plus drag-and-drop file support from Windows Explorer;
   - Optional deadlines (`Link date`) and time scheduling (`Link time`).
3. Click **OK** to save changes (the application displays an instant confirmation notice and closes the prompt immediately, while saving to disk and syncing in the background) or **Cancel** to discard.

### 7.1. "Edit Task" Dialog Features

- **Instant Confirmation and Background Persistence**: Clicking the "OK" button immediately applies task modifications visually, and the success confirmation dialog closes on the spot without lag or UI freeze. Task persistence to local storage and schedule CPM recalculations proceed asynchronously in the background, instantly restoring control to the user.
- **Individual Cycle Target**: The dedicated `Target Pomodoro Cycles (🎯):` control enables setting individual targets for each activity. The progress percentage in the Pomodoro module is computed accurately based on this target, and tasks will only transition to `Completed` once this number of cycles is reached.
- **Dynamic In-Place Retranslation**: If the application language is changed while the dialog is open, all labels, buttons, tooltips, and prompts retranslate immediately without losing unsaved field data.
- **Maximize and Minimize Buttons**: The dialog window features native maximize and minimize buttons in the top-right title bar, allowing full-screen expansion or minimization as needed.
- **Wide Mode**: When expanding the window width to $\ge 760\text{px}$ or maximizing it, the layout dynamically adapts in real time:
  - The **"Description (optional):"** text editor moves to the left side and expands to fill the full vertical height, offering generous room for reading and editing long text notes.
  - All frozen-dimension fields and action controls ("Task:", "Priority:", "Mastery Level:", "Target Pomodoro Cycles (🎯):", "Estimated Pomodoros (🍅):", file attachment tools, date/time pickers, and "Cancel" / "OK" buttons) anchor neatly at the bottom-right corner aligned to the bottom.
- **Compact Mode**: When narrowing the window width below 680px, widgets smoothly and instantly return to the original vertical stacked arrangement.
- **Complete Sizing Stability**: The transition is governed by an 80px smart hysteresis band (680px to 760px), preventing any flickering, jitter, or oscillation when resizing by the left or right borders.
- **Negative Height Protection**: When narrowing the window height from the top or bottom borders, strict minimum dimensions are enforced, ensuring that the description text editor never overlaps or crowds top fields or bottom action buttons.

---

## 8. Complete, move, select, and remove

- **Optimized Context Menu**: Right-clicking any task presents actions in the newly streamlined order:
  1. `✏️ Edit Task` — Opens the smart-resizing task editor.
  2. `🪢 Feynman Description` — Opens the Feynman study and concept formulation window.
  3. `🔁 Recurrence` — Configures scheduled repetitions (daily, weekly, monthly).
  4. `🎨 Color tasks` — Applies custom highlight colors to task titles.
  5. `🔤 Font Settings` — Customizes typography for general or linked tasks.
  6. `🔀 Move to another quadrant` — Reclassifies the task to another Eisenhower quadrant.
  7. `🗑️ Remove Task` — Deletes tasks with protected confirmation (default focus on `No`).
- **Complete Task**: Check the box next to a task. It automatically moves to the `Completed` tab.
- **Reopen Task**: Uncheck a completed task. It immediately returns to `Pending`.
- **Multi-Selection**: Use `Ctrl + Click` to select individual items or `Shift + Click` to select a range.
- **Fluid Drag-and-Drop**: Drag single or multiple tasks between quadrants, state tabs, and panels. Heavy disk writes and redundant schedule recalculations during drag operations have been eliminated, providing an instantaneous, lag-free experience.
- **Remove Tasks**: Right-click on the task (or selected group) and choose `Remove Task`. The confirmation prompt focuses on `No` by default to prevent accidental data loss.

---

## 9. Eisenhower Matrix task layouts

Under `Settings -> Task Layout -> User Interface -> EisenPo` and `PoCa`, choose from:

1. **Tabbed Layout > Horizontal**: Displays quadrants in top tabs with status columns arranged horizontally side by side, optimal for wide screens.
2. **Tabbed Layout > Vertical**: Compact quadrant view in vertical orientation.
3. **Quadrants Layout**: Displays all four quadrants simultaneously in expanded vertical columns.
4. **2x2 Matrix Layout**: Simultaneous panoramic 2x2 grid overview.
5. **PoCa Layout (Pomodoro + Dual Calendar)**: Activated via `Settings -> Task Layout -> User Interface -> PoCa -> Enable (or Disable)`.

### 9.1. Using the 2x2 Matrix Layout

In 2x2 Matrix Mode:
- Each quadrant features dedicated `Pending` (quadrant color) and `Completed` (blue) tabs.
- Complete tasks by checking the box or dragging items directly to `Completed`.
- Dragging tasks across quadrants updates quadrant assignment and priority automatically.

### 9.2. How to use the PoCa Layout (Pomodoro + Dual Calendar)

The integrated **PoCa** layout unifies deep focus execution and temporal planning into a single left sidebar directly alongside the Eisenhower Matrix:

- **Activation and Access**: Navigate to `Settings -> Task Layout -> User Interface -> PoCa` and select `Enable` to display the unified column or `Disable` to return to the standard layout.
- **Four Harmoniously Stacked Frames**:
  1. **Pomodoro Timer**: countdown timer, focus/break status indicators, and session cycle controls;
  2. **Settings**: interval duration adjustments, rest lengths, and focus timer configuration;
  3. **Calendar 1**: primary monthly calendar (current reference month displaying event markers and scheduled dates);
  4. **Calendar 2**: secondary monthly calendar (subsequent sequential month, fully synchronized with Calendar 1).
- **Symmetrical Frame Alignment**: The outer lateral borders (left and right) of all four frames align with absolute millimeter-level precision with each other, presenting a cohesive, uncluttered aesthetic.
- **Uniform Flush Vertical Spacing (6px)**: The second calendar sits directly below the first with an identical 6px flush vertical spacing (matching the gap between Settings and the first calendar), eliminating blank vertical gaps.
- **Dynamic $2 \times K$ Sequential Month Progression**:
  - Dragging the vertical splitter to the right to widen the panel duplicates calendars horizontally into new columns;
  - The calendar progression follows an unbroken mathematical matrix sequence ($2 \times K$):
    - **Row 1** (top row): displays months in horizontal sequence from left to right ($M, M+1, M+2, \dots$);
    - **Row 2** (bottom row): continues the sequence sequentially starting after the end of Row 1 ($M+K, M+K+1, M+K+2, \dots$).
    - *Example*: with 2 visible columns ($2 \times 2 = 4$ calendars):
      - Row 1: Calendar 1 = September; Calendar 3 (beside it) = October.
      - Row 2: Calendar 2 (below 1) = November; Calendar 4 (below 3) = December.
- **Scrollbar Safety Margins and Sizing Protection**:
  - When opening the application at default minimum dimensions with the PoCa layout active, the left sidebar automatically budgets adequate safety margin for the vertical scrollbar.
  - Frame right borders are protected from clipping, overlap, or occlusion by the vertical scrollbar, preserving clear visibility and balance with the Eisenhower Matrix quadrants.
- **Instantaneous Startup Without Freezes**:
  - Application startup is instantaneous and smooth, freeing manual controls and mouse interactions right away without the startup delays or freezing of earlier versions.

### 9.3. Dynamic Status Tab Translation

Status tabs (`Pending`, `In Progress`, and `Completed`) update instantly at runtime when switching language in `Settings -> Language`.

### 9.4. Task Sorting

Under `Settings -> Task Layout`, sort lists by:
- **Sort by Date** (Ascending / Descending)
- **Sort by Time** (Ascending / Descending)
- **Sort Alphabetically** (Ascending / Descending)

---

## 10. Visible panels

Under `Settings -> Task Layout -> Show Panels`:

- **Calendar** (`Ctrl+M`): Toggle the side calendar and Global Task List.
- **Pomodoro** (`Ctrl+Shift+E`): Toggle the Pomodoro In Progress panel.

Vertical splitters dividing Calendar, Pomodoro, and Quadrants panels feature a synchronized, uniform thickness across the entire workspace.

---

## 11. Calendar and Global Task List

- Visual date highlights appear automatically on days with scheduled tasks.
- Filters available for **Day**, **Week**, **Month**, and **Year**.
- **Multi-Month Bidirectional Synchronization**: When displaying multiple calendar widgets side-by-side or navigating months, page changes in any month view automatically sync the other calendars in a smooth, continuous sequence (current month, next month, etc.) without gaps or duplicated months.
- The **Global Task List** displays all scheduled activities for the selected period, clearly identifying their full lifecycle status: `[Completed]`, `[Pending]`, and `[In Progress]` for active tasks.
- **Effort Tracking**: Each task item indicates scheduled time, title, lifecycle status, and completed Pomodoro intervals compared against its planned cycle target.
- **Real-Time Dynamic Translation**: Switching UI language between English and Portuguese immediately retranslates status labels (`[In Progress]` / `[Em Progresso]`) and all calendar tooltips on the fly without requiring an app restart.
- **Rapid Interactivity**: Double-clicking any task in the list opens its linked external file immediately. Right-clicking items (whether completed, pending, or in progress) enables rapid editing, file access, or task deletion directly from their source quadrant.
- The vertical splitter separating the Calendar panel from the quadrants area matches the standardized thickness of the Pomodoro splitter.

---

## 12. Visual styling and fonts

Under `Settings -> Font Settings`:

- **General Tasks**: Configure font family, style, size, bold, italic, underline, and strikeout for regular tasks.
- **Linked File**: Configure dedicated typography for tasks with attached external files.
- **Color Tasks**: Select one or multiple tasks and apply custom colors using the built-in color picker.
- **Restore**: Reset font settings to default values for the category.

---

## 13. Pomodoro Module and Alarm System

The Pomodoro module manages work sessions in structured focus cycles:

- **Focus Blocks**: Timed concentration intervals (default: 25 min).
- **Short Breaks**: Rest periods between intervals (default: 5 min).
- **Long Breaks**: Extended rest after completing a full cycle (default: 15 min).
- **Task Board**: Focus task tracking comparing estimated vs. completed pomodoro counts.

### 13.1. Sound Alarm Configuration

Under `Settings -> Specific Settings -> Pomodoro -> Alarm`:
1. Choose the chime or alert sound for focus and break completion from the drop-down list.
2. Use `▶️ Play` to preview the sound and `⏹️ Stop` to halt it, with duration displayed in seconds.
3. Set the **Alarm repeat interval** (in seconds) to adjust alert frequency.
4. Click `💾 Save` to apply and persist preferences immediately.
5. At the end of each session, the configured sound plays with calibrated volume.

### 13.2. Automatic Mode

Under `Settings -> Specific Settings -> Pomodoro -> Automatic Mode`:
- Enable autonomous transitions between focus intervals and breaks.
- Configure maximum alarm repetitions before closing alerts and advancing automatically.

### 13.3. Individual Cycle Targets and "⚠️ Cycle Target Warning" Window

- **Individual Cycle Targets (🎯)**:
  - Each task can have its own target cycle count configured (from 1 to 999 cycles) via `Target Pomodoro Cycles (🎯):` in the "Edit Task" dialog.
  - The traditional hardcoded 4-cycle limit is now replaced by this flexible per-task target.
  - Progress calculation is dynamic and proportional: $(\text{completed} / \text{target}) \times 100\%$, strictly limited between $0.0\%$ and $100.0\%$.
  - Tasks stay in the `In Progress` list and only move to `Completed` when their individual target is fulfilled.

- **"⚠️ Cycle Target Warning" Window**:
  - When clicking to start the timer (`▶️ Start`), if there are active tasks in the `In Progress` column whose individual targets differ from the timer's current cycle limit (`Cycle 1/N`), the system automatically displays the **⚠️ Cycle Target Warning** dialog.
  - **Comparison Table**: Displays a clean 5-column overview:
    1. `Task`: Task description.
    2. `Module / Origin`: Origin module (e.g., EisenPo, Pomodoro, Feynman, Schedule).
    3. `Target (🎯)`: Individual target cycle count for this task.
    4. `Timer Cycle (⚙️)`: Active timer cycle limit.
    5. `Execution Behavior`: Clear explanation of how the task will behave (e.g., whether it completes early, continues in progress, or aligns with the timer).
  - **Action Buttons**:
    - `🔄 Reset all to N cycles`: Quickly updates the target of all active tasks to match the timer's limit and starts the countdown immediately.
    - `▶️ Start anyway`: Keeps individual custom targets intact and begins the timer session.
    - `❌ Cancel`: Aborts the start operation, leaving tasks and timer state unchanged.

---

## 14. Feynman Module

The Feynman method promotes deep conceptual mastery through synthesis:

1. **Concept**: Clear objective definition of the topic to study.
2. **Explain Simply**: Write plain-language explanations as if teaching a beginner.
3. **Identified Gaps**: Map uncertainties, questions, and blind spots requiring further research.
4. **Final Review & Simplification**: Synthesize and refine key takeaways into permanent knowledge.

Adaptable layouts in **Tabs**, **Vertical**, or **Horizontal** under `Settings -> Task Layout -> User Interface -> Feynman`.

### 14.1. Custom Step Labels ("Settings > Edit Labels")

The **"Feynman Description"** window includes a built-in menu bar and native label editing interface:

- **How to access**: In the "Feynman Description" window, select the top menu `Settings -> Edit Labels`.
- **"Edit Labels" Dialog Structure**: The dialog features an intuitive 3-column by 5-row matrix using native Qt widgets:
  - **"Default" Column**: Displays the read-only original system titles for all 5 steps ("1️⃣ Concept", "2️⃣ Explain Simply", "3️⃣ Identified Gaps", "4️⃣ Final Review and Simplification", and "5️⃣ Description").
  - **"Current" Column**: Displays the currently active label in read-only mode (whether system default or previously customized).
  - **"New" Column**: Editable input fields with clear buttons and placeholders where you can enter custom names for any of the steps.
- **Action Buttons**:
  - `Apply`: Saves customized labels to `feynman_rotulos_config.json`, instantly updates titles on all Feynman tabs, step frames, and composite task descriptions, transfers new names to the "Current" column, and clears input boxes.
  - `Restore`: Resets all labels back to original system defaults.
  - `Close`: Closes the label editor dialog.
- **Footer "Close" Button**: A dedicated `Close` button is integrated into the bottom footer of the "Feynman Description" window alongside `Save Concept` and `Clear` for fast, convenient exit.
- **Persistence & Backward Compatibility**: Preferences persist in `%LOCALAPPDATA%/EisenPo/feynman_rotulos_config.json`. Historical tasks created under prior labels remain fully readable and functional.
- **Dynamic Translation**: Both the menu and the labels dialog update instantly at runtime when switching languages between English and Portuguese.

---

## 15. Activity Registration and Schedules

Opened via `Tools -> Activity Registration` or `Settings -> Activity Registration`:

### 15.1. Activity Registration Window
- Capture course, syllabus, semester, subject, class group, activity type, sequence, priority, mastery, date, time, and linked files.
- **Register Definitions** button saves activities as shared tasks in `tarefas.json`.

### 15.2. Custom Schedules and Nested Lists
- Create user-defined schedules via the window menu.
- Build hierarchical tree-nested lists (e.g., Course > Semester > Subject). Selecting a parent item automatically filters child options.
- Support for `Black` and `Colorful` display modes across modules (`EisenPo`, `Pomodoro`, `Feynman`, or `General`).

---

## 16. Schedule Management, Gantt & 4D BIM Module

Available from `Tools -> Schedule Management, Gantt & 4D BIM`, via shortcut `Ctrl+Shift+B`, or from the context menu (right-click) on any task in the Eisenhower Matrix (`Link to Gantt Schedule` / `Open in Gantt`):

### 16.1. Interface Overview and Menu Bar
The main module workspace integrates in a unified view:
- **Top Menu Bar**:
  - **File Menu**:
    - `➕ New Activity`: opens the creation dialog to add a deliverable or work package to the WBS.
    - `✏️ Edit`: modifies selected activity properties (WBS code, description, duration, dates, progress).
    - `❌ Delete`: safely deletes selected activity and automatically recalculates precedence links.
    - `📥 Import CSV`: loads complete schedules from structured CSV spreadsheet files.
    - `📤 Export CSV`: exports active schedule data for exchange with MS Project, Primavera, or Excel.
  - **Settings Menu**:
    - `🔗 Link/Predecessor`: defines and adjusts Finish-to-Start dependency relationships.
    - `⚡ Recalculate CPM`: forces analytical evaluation of the Critical Path Method, updating floats and boundary dates.
    - `📌 Save Baseline`: stores a project snapshot baseline for variance control and schedule auditing.
    - `📊 Compare Baseline`: analyzes active execution against the saved baseline, highlighting date variances.
    - `👁️ Ghost Line`: toggles baseline ghost bars on the Gantt timeline.
    - `🔍 Zoom Day`, `🔍 Zoom Week`, and `🔍 Zoom Month`: calibrates timeline ruler resolution.
    - `📋 WBS Table` and `🏗️ 4D Simulation`: toggles between engineering tables and 4D simulation.
    - `Layout -> Tables`: configures the geometric arrangement of data tables and the Gantt timeline (Original, Top Vertical, Bottom Vertical, Left Horizontal, and Right Horizontal).
    - `📅 Working Days`: opens the working days and daily work shift configuration dialog.
    - `🎉 Holidays`: opens the holiday and non-working days manager dialog with interactive calendar and country library.
    - `🔄 Sync with EisenPo`: synchronizes tasks linked to the Eisenhower Matrix in real time.
    - `📋 Import from EisenPo`: batch-imports matrix tasks as new schedule activities.
- **Left Panel (WBS Tree)**: structured list of project phases, deliverables, and work activities displaying WBS code, description, duration in days, planned dates, completion percentage, and critical path indicators.
- **Right Panel (Interactive Gantt Chart)**: horizontal graphic representation of task bars, completion progress, dependency connections, and vertical today marker (*Today Line*).
- **4D BIM Simulation Tab**: dedicated panel for visual temporal tracking of building construction linked to 3D/IFC elements.

### 16.2. WBS Structure and Critical Path Method (CPM)
- **Hierarchy and Work Packages**: organize complex projects into hierarchical deliverables. Activities grouped under a parent package automatically form a summary bar calculating overall start/finish dates and weighted physical completion.
- **Analytical CPM Calculations**: the Critical Path Method engine mathematically evaluates activity boundaries: Early Start / Early Finish, Late Start / Late Finish, Free Float, and Total Float.
- **Critical Path Highlighting**: activities with zero total float determine the overall project completion date and are highlighted in bright red in both the WBS tree and the Gantt chart.
- **Unified Chromatic Status Standardization in WBS Table**: schedule items in the WBS Table display font coloring strictly identical to the status indicators in the "Temporal Simulation - 4D BIM" panel linked to the "4D Simulation" table: green (#4caf50) for completed activities (`✅ Executed:`), blue (#2196f3) for in-progress activities on schedule (`🔄 In Progress:`), red (#f44336) for delayed activities (`⚠️ Delayed:` — covering both delayed start where current date exceeds planned start without progress, and overdue finish where current date exceeds planned finish while incomplete), and neutral gray (#9e9e9e) for upcoming activities not yet started (`⏳ Future:`). This unified visual identity allows project stakeholders to immediately assess task progress directly within the table rows.
- **Circular Dependency Prevention**: the engine performs real-time cycle detection. If a predecessor link would create a closed loop, the operation is blocked with a clear diagnostic message, safeguarding schedule integrity.

### 16.3. Advanced Interaction with WBS and 4D Simulation Tables
- **Five Table Layout Modes (Settings -> Layout -> Tables)**:
  - `Original`: proportional 50%/50% horizontal split between the WBS Table and Gantt Timeline at the top, with the 4D Simulation table at the bottom;
  - `Top Vertical`: WBS Table and 4D Simulation table side-by-side (50%/50% horizontal) at the top, with the Gantt Timeline across the bottom;
  - `Bottom Vertical`: Gantt Timeline across the top, with WBS Table and 4D Simulation table side-by-side (50%/50% horizontal) at the bottom;
  - `Left Horizontal`: WBS Table and 4D Simulation table stacked vertically (50%/50%) on the left, with the Gantt Timeline occupying the entire right side;
  - `Right Horizontal`: Gantt Timeline occupying the entire left side, with WBS Table and 4D Simulation table stacked vertically (50%/50%) on the right.
  - The selection displays an exclusive checkmark and is immediately saved to the `layout.json` configuration file (`C:\Users\<user>\AppData\Local\EisenPo\layout.json`) using safe atomic writes (`QSaveFile`). On every startup of the Gantt window or application, the preference is loaded and automatically restored without affecting other preferences.
- **Proportional 50% / 50% Space Division**: in the default layout and combined table arrangements, the WBS Table and Gantt Timeline divide the available width strictly fifty-fifty (50%/50%), ensuring generous room for both data columns and the timeline bar chart.
- **Parented Dialog Windows and Multi-Monitor Support**: when deleting schedule activities or confirming operations, all message boxes are parented and centered directly over the Gantt window, even on secondary monitors, without pulling the main EisenPo host to the foreground.
- **Bidirectional Column and Row Resizing**:
  - Drag the **right border** of any column header right to widen or left to narrow column width.
  - Drag the **left border** of any column header left to expand or right to shrink column width.
  - In the "4D Simulation" table, vertical header row dividers can be dragged up or down to comfortably adjust table row heights.
- **Precise Single and Multi-Selection**:
  - Single clicking any cell selects that exact item without jumping or shifting to adjacent cells.
  - Select contiguous cell blocks by dragging or using `Shift+Click`.
  - Select non-contiguous cells using `Ctrl+Click`.
  - Click any top horizontal column header to select the entire column (or multiple columns with `Ctrl`/`Shift`).
  - Click any vertical numbered row header to select the entire row (or multiple rows with `Ctrl`/`Shift`).
- **Windows 11 Theme-Adaptive Visual Highlighting**:
  - **In Dark Theme**: hovering over or selecting cells applies a gentle lighter gray tone relative to cell background, providing crisp contrast without harsh white glare.
  - **In Light Theme**: hovering over or selecting cells applies an elegant darker gray tone relative to cell background, avoiding pitch-black fills.
  - **Seamless Propagation**: the highlight propagates gracefully across isolated cells, multiple blocks, full rows, and full columns.
  - **Preserved Status Colors**: label colors and badges maintain full vibrancy and readability: `✅ Executed:`, `🔄 In Progress:`, `⚠️ Delayed:`, and `⏳ Future:`.

### 16.4. Timeline and Gantt Chart Navigation
- **Flexible Time Scales**: switch between Day, Week, and Month views to examine granular daily execution or macro multi-year construction programs.
- **Fluid Scrolling**: smooth horizontal panning and zooming across the project timeline.
- **Harmonized Chromatic Coloring on Gantt Boxes**: activity boxes and timeline nodes adopt the same status coloring as the WBS and 4D BIM status fonts (blue for in progress, red for delayed, and neutral gray for future), applying a gentle, refined green tone (#4caf50 with balanced opacity and contrast) specifically for completed activities, making finalized deliverables immediately distinguishable from ongoing or delayed work.
- **Detailed and Localized Dynamic Tooltips**: hovering the mouse cursor over any activity box or timeline node displays a rich, structured info card showing the WBS code, styled status badge, start and finish dates formatted according to the active language (`MM/dd/yyyy HH:mm` in English and `dd/MM/yyyy HH:mm` in Portuguese), duration in days, physical percentage complete, Critical Path status with colored emphasis (`YES` in red or `No` in neutral gray), and total float in hours. All tooltip labels, dates, and values re-translate dynamically and instantly at runtime whenever the interface language is switched.
- **Persistent Ghost Line Overlay**: when enabled in the Settings menu, ghost bars corresponding to the saved baseline are rendered directly on the timeline, facilitating immediate comparison between planned and actual dates. Rendering remains stable and continuous even when adding new tasks in EisenPo or creating new activities in the schedule.
- **Live Localized Headers**: months and weekdays on the timeline scale update instantaneously at runtime when alternating between English and Portuguese without reload.

### 16.5. Linking Eisenhower Matrix Tasks to Schedule Activities
- **How to Link**: right-click any task in the Eisenhower Matrix and select `🔗 Link to Gantt Schedule`.
- **Intuitive Selection**: choose the target schedule and WBS item in the linking dialog.
- **Visual Feedback**: the linked WBS item displays the `🔗 Linked` badge and associated WBS code.
- **Discrete Physical Progress Calculation by Pomodoros**:
  - The completion percentage of linked schedule activities is calculated strictly and proportionally based on completed Pomodoro blocks relative to the task's planned cycle target:
    $$\text{Physical Progress} = \left(\frac{\text{Completed Pomodoros}}{\text{Target Pomodoros}}\right) \times 100\%$$
  - **Predictable Discrete Increments**:
    - For a task with a target of 4 pomodoros: progress steps advance strictly across **0%**, **25%**, **50%**, **75%**, and **100%**;
    - For a target of 5 pomodoros: progress steps advance strictly across **0%**, **20%**, **40%**, **60%**, **80%**, and **100%**;
    - Tasks with any other integer cycle target (1 to 999 cycles) progress proportionally in exact mathematical steps.
  - **Removal of Arbitrary 10% Initial Progress**:
    - The arbitrary 10% progress increment previously assigned upon linking or starting a task has been completely eliminated;
    - The activity begins at strictly **0%** physical progress until Pomodoro work cycles are completed.
  - **Full Activity Completion**:
    - The task reaches 100% physical progress when all planned cycle pomodoros are finished or when the user marks the task as completed in the Eisenhower Matrix.
- **Instantaneous Real-Time Synchronization**: any task modification made on the main screen (description edits, dates, priority, quadrant moves, completion toggle, or pomodoro increments) propagates immediately and in real time to the open Gantt schedule, keeping the Gantt bar and completion percentages in perfect sync without manual action.
- **Instant Opening and Conditional Synchronization**: the Gantt schedule window opens instantaneously ($O(1)$) by evaluating if changes occurred in the matrix tasks since the last sync. If no tasks were modified, redundant database refreshes and recalculations are skipped.
- **High-Performance O(1) Memory Cache**: in-memory lookup cache eliminates disk I/O, allowing instant language switching and task reordering with zero UI freezes.

### 16.6. 4D BIM Construction Simulation (IFC Linkage)
- **Link Building Elements**: map universal IFC element identifiers (`IFC GUID / UniqueId`) to schedule activities.
- **3-Mode Adaptive Control Bar**: when dragging vertical splitters, the simulation bar dynamically reflows into:
  - `Single-Line` (width >= 1200px): playback buttons, Reference Date picker, timeline scrubber, and KPI badges in a single row;
  - `Two-Lines` (width 780px to 1200px): playback buttons and Reference Date picker on the top row, and KPI status badges ("Total:", "Executed:", "In Progress:", "Delayed:", "Future:") on the bottom row;
  - `Three-Lines` (width < 780px): Row 1 = playback buttons ("▶️ Start Simulation", "⏹️ Stop Simulation", "🔄 Reset Simulation"); Row 2 = Reference Date group and timeline scrubber; Row 3 = KPI status badges.
  - Automatic dynamic container height adjustments (~58px, ~84px, ~114px) prevent any clipping or table overlap.
- **Strict Button Minimum Width Sizing Based on Text Length**: automated minimum width enforcement for simulation buttons based on font metrics, text length, and styling, preventing truncated text when resizing splitters or switching languages.
- **Tight Coupling for "Reference Date:" and Calendar Picker**: direct 3px spacing between the "Reference Date:" label and the date picker widget, eliminating excessive blank margins and avoiding collisions with the reset button or scrubber slider.
- **Interactive Simulation Player**: use `▶️ Play`, `⏸️ Pause`, step controls, and temporal scrubber slider to inspect construction progress over time.
- **Playback Speed Control**: adjust simulation steps per second to match presentation needs.
- **Dynamic 4D Element Status Classification**: real-time element evaluation at the simulated date:
  - `Planned`: element work has not yet started;
  - `In Construction`: element actively being built;
  - `Constructed`: element completed;
  - `Delayed`: simulated date exceeded planned finish without completion;
  - `Ahead of Schedule`: element constructed before scheduled milestone.
- **Live Header Translation**: table columns (`4D Status` and `IFC GUID / Unique ID`) and status badges translate dynamically in real time.

### 16.7. CSV Import, Export, and Local SQLite Persistence
- **Interoperability**: export full schedules to `.csv` for Excel, LibreOffice, MS Project, or Primavera P6, and batch-import existing data.
- **Safe Local Storage**: schedules, items, dependencies, and BIM links persist in local transactional SQLite database (`eisenpo_gantt.db`) under `%LOCALAPPDATA%/EisenPo`.
- **Webhooks & REST API**: architectural readiness for network integrations, automated schedule notifications, and OpenAPI 3.0.3 compliant services.

### 16.8. Working Days and Work Shift Configuration
Access via `Settings -> Working Days` in the Gantt Schedule window:
- **Individual Weekday Selection**: The `Configure Working Days and Working Hours` dialog provides individual checkboxes for all 7 weekdays (Monday through Sunday). Check the days designated as working days and uncheck rest days (e.g., enabling Saturday operations or non-standard shift rotations).
- **Work Shift Hours**: Define the daily start and end working hours (default: 08:00 to 17:00).
- **Real-Time Working Hours Computation**: The interface calculates and displays the effective daily working duration in real time (e.g., `8 hours and 0 minutes`).
- **Dialog Controls**:
  - `Restore Defaults`: resets working days to Monday through Friday from 08:00 to 17:00.
  - `Cancel`: closes without modifying settings.
  - `OK`: saves settings into `WorkCalendar` and automatically triggers analytical recalculation of all schedule finish dates and CPM network boundaries.

### 16.9. Holiday Management and National Holiday Calendar
Access via `Settings -> Holidays` in the Gantt Schedule window:
- **Interactive Visual Calendar**: Browse months and years effortlessly. Click on any date to highlight it and load it into the date field. Days registered as holidays are visually highlighted on the calendar grid.
- **Manual Registration and Removal**:
  - To register a custom holiday or local day off: select the date on the calendar, enter its label in the `Description:` field, and click `➕ Add Holiday`.
  - To delete a holiday: select its row in the holiday table and click `❌ Remove Holiday`.
- **Automatic Holiday Loader for 18 Countries**:
  - Choose a nation from the `Holidays by Country:` dropdown. The built-in registry covers 18 countries:
    1. Brazil
    2. United States
    3. Portugal
    4. Argentina
    5. Canada
    6. United Kingdom
    7. France
    8. Germany
    9. Italy
    10. Spain
    11. Japan
    12. China
    13. Mexico
    14. Chile
    15. Colombia
    16. Uruguay
    17. Paraguay
    18. Australia
  - Click `Apply`: the software automatically registers all official national holidays for the selected country for the current and upcoming years.
  - The engine mathematically calculates Easter-dependent movable holidays (Carnival, Good Friday, Easter Sunday, Corpus Christi) alongside all fixed public and civil holidays.
- **WBS-Styled Table Interactivity**:
  - Organized columns for `Date` and `Description`.
  - Bidirectional resizing by dragging either the right or left header borders.
  - Subtle theme-adaptive visual highlight matching Windows 11 Light and Dark themes.
- **Schedule Impact**: Holidays are strictly accounted for as non-working days by the schedule calculation engine and CPM math, automatically skipping tasks over holidays and adjusting project completion milestones.

---

## 17. Productivity tools

Accessible in the `Tools` menu:

- **Instant Opening and In-Memory Performance**: All tools and dialog windows in the Tools menu operate with direct $O(1)$ in-memory data extraction, completely eliminating heavy disk read/write cycles and redundant signal emissions, ensuring immediate and smooth opening without freezes.
- **Global Search** (`Ctrl+Shift+G`): Search tasks by title, notes, tags, or file attachments across all modules.
- **Today / Next 7 days** (`Ctrl+Shift+H`): Quick execution panel for upcoming deadlines.
- **Filters** (`Ctrl+Alt+F`): Filter tasks by quadrant, priority, completion state, and dates.
- **Activity Registration**: Academic tracking, evaluation planning, and custom schedules with reliable persistence and fast loading.
- **Gantt & 4D BIM Schedule**: Full project scheduling, WBS tree, CPM analysis, and 4D constructive simulation with on-demand synchronization.
- **Generate Next Recurrences**: Batch generate recurring tasks (daily, weekly, monthly) with preview and duplicate prevention.
- **Enable Local Reminders**: In-app and system-tray notifications for scheduled deadlines with fast initialization.
- **Export Calendar .ics**: Export standard iCalendar files for integration with external calendar apps.
- **Productivity History**: Track completed tasks and focus session metrics.
- **Integrated Study Flow**: Convert study materials into Feynman concepts and review tasks.
- **Templates**: Predefined task templates for study, work, reading, and weekly planning.
- **Spaced Reviews**: Automated spaced repetition based on mastery levels (1, 3, and 7 days) calculated directly in memory.
- **Plugin Manager**: Inspect loaded plugin statuses, versions, and diagnostics.
- **Log Manager**: Real-time chronological reader for `file_eisenpo_*.log` files.

### 17.1. Batch Recurrence Generation
Accessible via `Tools -> Generate Next Recurrences`:
- **Prior Individual Configuration**: Right-click any task in the Eisenhower Matrix or Global Task List and choose `🔁 Recurrence` to set its frequency (daily, weekly, monthly, or disabled) and optional expiration date.
- **Batch Processing**:
  - Select source and destination methods (Eisenhower, Pomodoro, Feynman);
  - Choose the projection timeframe (e.g., next 30, 60, or 90 days);
  - Specify maximum occurrences per item;
  - Keep `Avoid existing duplicates` checked to prevent duplicate tasks on identical dates;
  - Use `Apply default frequency` to process tasks that lack individual frequency configurations;
  - For items without scheduled times, activate the default time option and set a reference time.
- **Dialog Controls & Actions**:
  - `Save definitions`: persists your batch recurrence settings;
  - `Generate occurrences`: executes creation of scheduled recurring tasks in the unified database;
  - `Restore defaults`: resets all parameters back to factory settings;
  - `Restore previous tasks`: reverts the most recent batch generation using the safety snapshot;
  - `Close`: exits the dialog.
- **Dynamic Runtime Translation & Localization**:
  - All action buttons (`"Restore defaults"`, `"Generate occurrences"`, `"Restore previous tasks"`, `"Save definitions"`, `"Close"`), checkboxes, the empty table placeholder notice (`"No tasks ready to generate occurrences with current settings."`), and localized date formatting update instantaneously when alternating the interface language without closing the window.

### 17.2. Enabling and Managing Local Reminders
Accessible via `Tools -> Enable Local Reminders`:
- **Alert Scheduling & Options**:
  - In the **"When to alert"** frame, select between:
    - `Use date and time from each task`: respects the specific date and time configured in each task;
    - `Set a single custom time`: consolidates all reminder notifications at a uniform user-defined time.
  - **Notification Channels**: check `Display in-app alert` for internal message popups, `Notify in system tray` for Windows system tray notifications, or both simultaneously.
  - **Handling Tasks Without Times**: check `Default time if missing` to automatically schedule tasks that only define a date.
- **Dialog Controls & Actions**:
  - `Test notification`: triggers an immediate demo notification to verify audio and visual feedback;
  - `Apply reminders`: activates the scheduled reminders in the local timer subsystem;
  - `Clear active`: cancels and clears all currently scheduled active reminders with one click;
  - `Save definitions`: saves your notification preferences and channel configurations;
  - `Restore defaults`: resets all options to recommended default values;
  - `Close`: closes the dialog.
- **Dynamic Runtime Translation**:
  - All buttons (`"Clear active"`, `"Restore defaults"`, `"Apply reminders"`, `"Save definitions"`, `"Close"`, `"Test notification"`), the frame title (`"When to alert"`), radio buttons (`"Use date and time from each task"` and `"Set a single custom time"`), and all option controls translate immediately at runtime when switching between English and Portuguese.

### 17.3. Integrated Study Flow, Templates, and Spaced Reviews
- **Integrated Study Flow** (`Tools -> Integrated Study Flow`):
  - Converts any study topic into structured active learning: creates a concept in the Feynman module and automatically schedules a consolidation review task in the Eisenhower Matrix 2 days later.
  - Labels, inputs, placeholders, and buttons update with live dynamic translation.
- **Task Templates** (`Tools -> Templates`):
  - Pre-packaged task bundles for standard workflows:
    - *Study Plan*: reading, note-taking, exercises, and practice exams;
    - *Project Development*: requirements gathering, architecture, implementation, testing, and deployment;
    - *Weekly Planning*: priority alignment, routine structure, and retrospective;
    - *Content Review*: spaced repetition blocks and self-explanation checkpoints.
  - Instant preview of template contents and one-click insertion into matrix quadrants. Titles, descriptions, and buttons support real-time runtime translation.
- **Create Spaced Reviews** (`Tools -> Create Spaced Reviews`):
  - Analyzes tasks in the active tab and calculates scientifically grounded spaced repetition intervals based on mastery level (`Beginner`: 1 day, `Intermediate`: 3 days, `Advanced`: 7 days).
  - Review and edit tasks before transferring them in batch to Eisenhower, Pomodoro, and Feynman, complete with full transfer history and undo capabilities.
  - Live localized columns and buttons upon runtime language change.

---

## 18. File, import, and export

- **New** (`Ctrl+N`): Start a new workspace session.
- **Open** (`Ctrl+O`): Import data from external files (JSON, CSV, XLSX, PDF).
- **Save** (`Ctrl+S`): Save the current state to the local database.
- **Save As** (`Ctrl+Shift+S`): Export tasks to JSON, CSV, XLSX, or formatted PDF reports.
- **Clear** (`Ctrl+Shift+Del`): Clear all workspace tasks with safety confirmation.
- **Exit** (`Ctrl+Q`): Close the application.

---

## 19. Languages and translations

Under `Settings -> Language`:
- **Português (Brasil)** (`pt_BR`)
- **English (United States)** (`en_US`)

Language switching occurs instantly at runtime across all windows, menus, dialogs, tables, tabs, and buttons without requiring a restart.

---

## 20. Persistence and privacy

- Local persistent folder: `%LOCALAPPDATA%\EisenPo` (with automatic migration from `%LOCALAPPDATA%\EisenhowerOrganizer`, if present).
- All files (`tarefas.json`, `layout.json`, `language.json`, `feynman_rotulos_config.json`, `listas.db`, `agenda_config.json`, `agendas/*.json`, `eisenpo_gantt.db`, font/recurrence/reminder configs, and `logs/file_eisenpo_*.log`) remain strictly on your local machine.
- Zero telemetry, tracking, or cloud uploads; **Microsoft does NOT cover this app under Microsoft's Privacy Statement** ([`docs/privacy-policy.md`](docs/privacy-policy.md#enus)).
- **Clean Uninstallation:** Uninstall **EisenPo** anytime via **Windows Settings → Apps → Installed Apps** (or Start Menu → Uninstall) without leaving background services.

---

## 21. Useful shortcuts

| Shortcut | Action |
| --- | --- |
| `Ctrl+N` | New task session |
| `Ctrl+O` | Open / Import file |
| `Ctrl+S` | Save tasks |
| `Ctrl+Shift+S` | Save As / Export (XLSX, PDF, CSV, JSON) |
| `Ctrl+Shift+Del` | Clear all workspace tasks |
| `Ctrl+Q` | Exit application |
| `Ctrl+M` | Toggle Calendar panel |
| `Ctrl+Shift+E` | Toggle Pomodoro In Progress panel |
| `Ctrl+Shift+B` | Open Gantt Schedule & 4D BIM |
| `Ctrl+Shift+G` | Global task search |
| `Ctrl+Shift+H` | Today / Next 7 days panel |
| `Ctrl+Alt+F` | Advanced filters dialog |
| `Ctrl+Shift+A` | About window |
| `Ctrl+Shift+M` | User Manual window |

---

## 22. Troubleshooting

- **Integrated and native modules**:
  - All four essential productivity modules (`Pomodoro`, `Feynman`, `Activity Registration / Agenda`, and `Gantt Schedule & 4D BIM`) are compiled and natively integrated into the primary EisenPo desktop executable.
  - There is no need to install external DLLs or maintain separate plugin folders. You can access any module directly from the `Tools` menu or via interface layout options under `Settings -> Task Layout`.
  - To inspect system components and active extensions, open `Tools -> Plugin Manager` or inspect runtime diagnostics in `Tools -> Log Manager`.
- **Linked file does not open**:
  - Ensure the file was not moved, renamed, or deleted and that your OS has a default handler for its file extension.
- **Local reminders do not fire**:
  - Keep EisenPo running (or minimized to system tray) and ensure scheduled times are in the future.
- **Gantt schedule fails to open or save**:
  - Verify that `%LOCALAPPDATA%\EisenPo` has proper read and write permissions to manage `eisenpo_gantt.db` (and `listas.db` for Agenda).

---

## 23. Author, Licensing & Support

- **Author:** Fernando Nillsson Cidade
- **Support Contact (best-effort, no Microsoft support):** `linceu_lighthouse@outlook.com`
- **Repository (GNU AGPLv3):** https://github.com/fernandoncidade/EisenPo_cpp
- **Microsoft Store (Official EULA):** [Get EisenPo on Microsoft Store](https://apps.microsoft.com/detail/9P289X0185C3) · [Licensing (`docs/licensing.md`)](docs/licensing.md#enus)
