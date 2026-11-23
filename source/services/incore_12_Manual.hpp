#pragma once

#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

namespace source::InterfaceCore::incore_12_Manual {

struct ManualDetails {
    QString summary;
    QStringList paragraphs;
    QStringList bullets;
};

struct ManualSection {
    QString id;
    QString title;
    QStringList paragraphs;
    QStringList bullets;
    QList<ManualDetails> details;
};

struct ManualBlock {
    QString kind;
    QString text;
    QString section_id;
};

struct ManualTextWithPositions {
    QString text;
    QMap<QString, int> positions;
    QStringList order;
};

QString normalize_language(const QString &lang = QString());
QString get_manual_title(const QString &lang = QString());
QString to_unicode_bold(const QString &text);
// O documento gerado inclui instruções de uso para Configurações -> Layout de Tarefas ->
// Mostrar Abas de Interface, para o Layout Matriz 2x2 do Eisenhower, para as opções Simples/Robusta do Pomodoro,
// para ferramentas de produtividade como recorrências e lembretes locais, para a lista global de tarefas,
// para remoção de tarefas pelo menu de contexto no Eisenhower e no Feynman, para colorir tarefas selecionadas,
// para configurações de fonte de tarefas em geral e tarefas com arquivo vinculado (globais e independentes por plugin),
// para confirmação de sucesso ao editar tarefas, para o redimensionamento inteligente, maximização e minimização da janela Editar Tarefa,
// para a personalização de rótulos do Método Feynman (Configurações -> Editar Rótulos),
// para o alinhamento simétrico de bordas e espessura padronizada de divisores verticais,
// para a sequência otimizada do menu de contexto de tarefas,
// para a reorganização ergonômica em 3 linhas do quadro Nova Tarefa, com dimensionamento automático da largura das listas suspensas (Prioridade e Nível de Domínio),
// definição de duração da tarefa em dias inteiros e proteção rígida contra sobreposição durante o ajuste por splitters verticais,
// para o Gerenciador de plugins, para o Gerenciador de logs,
// para as agendas personalizadas com listas aninhadas, vínculo de arquivos e cores dinâmicas do Registro de Atividades,
// para a gestão de cronograma, EAP/WBS, caminho crítico CPM, linha do tempo Gantt, simulação 4D BIM com elementos IFC,
// para a padronização cromática unificada de status na Tabela WBS e nas caixas do Diagrama de Gantt com tonalidade suave para atividades concluídas (em conformidade com o quadro Simulação Temporal - 4D BIM: Executado, Em Andamento, Atrasado e Futuro),
// para os tooltips informativos dinâmicos e retraduzidos em tempo de execução nas caixas e nós da linha do tempo do Gantt com formatação regional de data e detalhes de caminho crítico e folga,
// para a barra de menus do Gantt (Arquivo e Configurações), redimensionamento bidirecional de colunas e linhas,
// seleção de células/linhas/colunas, realce visual adaptativo aos temas Claro e Escuro do Windows 11 com propagação,
// gerenciamento de Baseline, Linha Fantasma e sincronização/importação com o EisenPo,
// para a configuração de dias úteis da semana e jornada diária de trabalho (Configurações -> Dias Úteis),
// para o gerenciamento de feriados e dias não úteis com calendário interativo, tabela no padrão WBS e biblioteca integrada de 18 nações com cálculo analítico de datas móveis (Configurações -> Feriados),
// para as 5 opções de configuração de layout de tabelas e diagrama (Configurações -> Layout -> Tabelas: Original 50%/50%, Vertical Topo, Vertical Inferior, Horizontal Esquerda e Horizontal Direita) com persistência atômica no arquivo layout.json e carregamento automático a cada reinicialização,
// para o direcionamento de janelas de diálogo de confirmação e exclusão sobre a janela do Gantt com suporte nativo a múltiplos monitores e persistência da Linha Fantasma,
// para a barra de controle da Simulação 4D adaptativa em 3 modos (SingleLine, TwoLines e ThreeLines) com largura protegida de botões e acoplamento estrito de Data de Referência,
// para a configuração de Meta de Ciclos Pomodoro individual por tarefa (1 a 999) na janela Editar Tarefa, com cálculo estrito de percentual e conclusão por meta própria,
// para a janela interativa '⚠️ Aviso de Metas de Ciclos' no Pomodoro com tabela detalhada de discrepâncias e 3 opções de ação (Redefinir todas, Iniciar mesmo assim e Cancelar),
// para a confirmação de sucesso instantânea e fechamento imediato com salvamento assíncrono em segundo plano na janela Editar Tarefa,
// para a exibição abrangente do ciclo de vida de tarefas no Calendário e Lista Global ([Em Progresso] / [In Progress], além de [Concluída] e [Pendente]) com contadores de esforço e interação ágil,
// para a sincronização bidirecional encadeada de calendários extras no painel lateral,
// para o layout integrado PoCa (Pomodoro + Calendário Duplo) com empilhamento vertical sincronizado,
// alinhamento milimétrico simétrico das molduras dos quatro quadros (Temporizador, Configurações, Calendário 1 e Calendário 2),
// sequenciamento matricial dinâmico 2xK de meses na replicação horizontal por arrasto do splitter (Linha 1 com meses M a M+K-1 e Linha 2 com meses M+K a M+2K-1),
// espaçamento rente sem folgas verticais entre calendários e proteção preventiva contra sobreposição da barra de rolagem,
// para a inicialização instantânea sem congelamentos de interface por inicialização sob demanda de subsistemas multimídia,
// para a otimização de desempenho e fluidez no arraste e solte (drag-and-drop) de tarefas sem escritas redundantes no disco,
// para a abertura instantânea e sincronização sob demanda condicional da janela de Cronograma Gantt & BIM 4D,
// para o cálculo físico discreto de progresso no Gantt por blocos Pomodoro (ex.: 0%, 25%, 50%, 75%, 100% para meta de 4 pomodoros) com eliminação do progresso arbitrário inicial de 10%,
// para a sincronização instantânea em tempo real entre as edições da interface principal e a atualização do cronograma Gantt,
// para a abertura imediata e sem travamentos das ferramentas do menu Ferramentas (Registro de Atividades, Cronograma de Gantt BIM 4D, Gerar próximas recorrências, Ativar lembretes locais e Criar revisões espaçadas) por meio de extração em memória O(1), eliminação de I/O forçado em disco e supressão de tempestade de sinais na inicialização,
// para a unificação e integração nativa definitiva dos quatro módulos essenciais de produtividade (Pomodoro, Feynman, Registro de Atividades e Cronograma de Gantt & BIM 4D) como alvos de compilação direta dentro do executável principal EisenPo, eliminando dependências de DLLs dinâmicas externas,
// para a centralização unificada de todos os catálogos de tradução (.ts e .qm) na árvore oficial com compilação direta para o diretório translations/ do executável,
// persistência assíncrona concorrente no repositório unificado de tarefas com sincronização contínua com o cronograma sem travamento de interface,
// retradução dinâmica em tempo de execução de botões, tooltips de tarefas, retradução ao vivo de diálogos abertos, janelas auxiliares, amostragem de cores, contadores e caixas de diálogo de ferramentas (Gerar próximas recorrências, Ativar lembretes locais, Fluxo integrado de estudo, Templates e Criar revisões espaçadas),
// consulta completa de termos legais, duplo licenciamento (GNU AGPLv3 e EULA Oficial + CLC), Política de Privacidade 100% offline em %LOCALAPPDATA%\EisenPo e notas de versão em Opções -> Sobre,
// além de navegação por índice clicável e busca rápida em Opções -> Manual, respeitando os módulos integrados na aplicação.
QList<ManualSection> get_manual_document(const QString &lang = QString(),
                                         bool include_pomodoro_plugin = false,
                                         bool include_feynman_plugin = false,
                                         bool include_mapamental_plugin = false,
                                         bool include_audiolivro_plugin = false,
                                         bool include_agenda_plugin = false,
                                         bool include_gantt_plugin = false);
QPair<QList<ManualBlock>, QStringList> get_manual_blocks(const QString &lang = QString(),
                                                         bool include_pomodoro_plugin = false,
                                                         bool include_feynman_plugin = false,
                                                         bool include_mapamental_plugin = false,
                                                         bool include_audiolivro_plugin = false,
                                                         bool include_agenda_plugin = false,
                                                         bool include_gantt_plugin = false);
QString get_manual_text(const QString &lang = QString(),
                        bool include_pomodoro_plugin = false,
                        bool include_feynman_plugin = false,
                        bool include_mapamental_plugin = false,
                        bool include_audiolivro_plugin = false,
                        bool include_agenda_plugin = false,
                        bool include_gantt_plugin = false);
ManualTextWithPositions get_manual_text_with_positions(const QString &lang = QString(),
                                                       bool include_pomodoro_plugin = false,
                                                       bool include_feynman_plugin = false,
                                                       bool include_mapamental_plugin = false,
                                                       bool include_audiolivro_plugin = false,
                                                       bool include_agenda_plugin = false,
                                                       bool include_gantt_plugin = false);

}  // namespace source::InterfaceCore::incore_12_Manual
