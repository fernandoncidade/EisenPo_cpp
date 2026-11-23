#pragma once

#include <QList>
#include <QString>

#include "source/services/incore_12_Manual.hpp"

namespace source::InterfaceCore::incore_16_manual_pt_BR {

using source::InterfaceCore::incore_12_Manual::ManualSection;

QString manual_intro_text();
QString manual_toc_title();

// Retorna as seções do manual em português para o Eisenhower Core, incluindo
// a integração nativa dos quatro módulos essenciais (Pomodoro, Feynman, Registro de Atividades e Gantt & BIM 4D),
// o catálogo unificado de idiomas com retradução dinâmica completa, gravação assíncrona não-bloqueante de tarefas,
// o layout PoCa (Pomodoro + Calendário Duplo), alinhamento simétrico das 4 molduras,
// dimensionamento dinâmico sem sobreposição de barras de rolagem, inicialização instantânea,
// abertura imediata e sem travamentos das ferramentas do menu Ferramentas por extração em memória O(1),
// retradução dinâmica ao vivo das janelas de ferramentas de produtividade
// e instruções de consulta ao duplo licenciamento, privacidade 100% offline e notas de versão em Opções -> Sobre e busca interativa em Opções -> Manual.
QList<ManualSection> pt_document();
QList<ManualSection> feynman_pt_document();
// Retorna as seções do manual dedicadas ao módulo Pomodoro Timer e Gerenciador de Tempo,
// incluindo a integração com os calendários duplos no layout PoCa.
QList<ManualSection> pomodoro_pt_document();
QList<ManualSection> mapamental_pt_document();
QList<ManualSection> audiolivro_pt_document();
QList<ManualSection> agenda_pt_document();
// Retorna as seções do manual dedicadas ao módulo Gantt & BIM 4D, incluindo:
// visão geral, barra de menus (Arquivo e Configurações), EAP/WBS e análise CPM com coloração cromática unificada de status,
// interatividade avançada das tabelas (redimensionamento bidirecional, seleção múltipla e realce adaptativo),
// configurações de layout de tabelas (Original 50%/50%, Vertical Topo, Vertical Inferior, Horizontal Esquerda, Horizontal Direita) com persistência atômica no arquivo layout.json e restauração automática a cada reinicialização,
// centralização de janelas de diálogo de exclusão na janela do Gantt com suporte multi-monitor e estabilidade da Linha Fantasma,
// linha do tempo e Gantt interativo com caixas cromáticas e tooltips dinâmicos detalhados, integração com Eisenhower com cálculo físico discreto de progresso por pomodoros e sincronização instantânea em tempo real, simulação construtiva 4D com barra de controles adaptativa em 3 modos,
// configuração de dias úteis e jornada de trabalho, e gerenciamento de feriados com calendário e biblioteca de 18 países.
QList<ManualSection> gantt_pt_document();
QList<ManualSection> final_sections_pt_document();

QList<ManualSection> get_manual_document(bool include_pomodoro_plugin = false,
                                         bool include_feynman_plugin = false,
                                         bool include_mapamental_plugin = false,
                                         bool include_audiolivro_plugin = false,
                                         bool include_agenda_plugin = false,
                                         bool include_gantt_plugin = false);

}  // namespace source::InterfaceCore::incore_16_manual_pt_BR
