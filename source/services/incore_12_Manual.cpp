#include "source/services/incore_12_Manual.hpp"

#include "source/services/incore_16_manual_pt-BR.hpp"
#include "source/services/incore_17_manual_en-US.hpp"
#include "source/utils/BarraTitulosUtils.hpp"

namespace source::InterfaceCore::incore_12_Manual {
namespace {

QString manual_intro_text(const QString &normalized_lang)
{
    return normalized_lang == QStringLiteral("en_US")
               ? incore_17_manual_en_US::manual_intro_text()
               : incore_16_manual_pt_BR::manual_intro_text();
}

QString manual_toc_title(const QString &normalized_lang)
{
    return normalized_lang == QStringLiteral("en_US")
               ? incore_17_manual_en_US::manual_toc_title()
               : incore_16_manual_pt_BR::manual_toc_title();
}

}  // namespace

QString normalize_language(const QString &lang)
{
    if (lang.trimmed().isEmpty()) {
        return QStringLiteral("pt_BR");
    }

    const QString value = lang.trimmed().replace(QLatin1Char('-'), QLatin1Char('_')).toLower();
    if (value == QStringLiteral("pt") || value == QStringLiteral("pt_br")) {
        return QStringLiteral("pt_BR");
    }
    if (value == QStringLiteral("en") || value == QStringLiteral("en_us")) {
        return QStringLiteral("en_US");
    }
    return QStringLiteral("pt_BR");
}

QString get_manual_title(const QString &lang)
{
    Q_UNUSED(lang);
    return source::utils::titulo_janela_manual();
}

QString to_unicode_bold(const QString &text)
{
    QString output;
    output.reserve(text.size());
    for (QChar ch : text) {
        const char32_t u = ch.unicode();
        if (u >= 'A' && u <= 'Z') {
            const char32_t codepoint = 0x1D400 + (u - 'A');
            output.append(QString::fromUcs4(&codepoint, 1));
        } else if (u >= 'a' && u <= 'z') {
            const char32_t codepoint = 0x1D41A + (u - 'a');
            output.append(QString::fromUcs4(&codepoint, 1));
        } else if (u >= '0' && u <= '9') {
            const char32_t codepoint = 0x1D7CE + (u - '0');
            output.append(QString::fromUcs4(&codepoint, 1));
        } else {
            output.append(ch);
        }
    }
    return output;
}

// Monta o documento unificado do manual consultando o módulo de idioma correspondente.
// Inclui instruções completas do host e dos plugins opcionais (Pomodoro, Feynman, Agenda e Gantt & BIM 4D),
// contemplando o novo layout em 3 linhas do quadro Nova Tarefa (com listas auto-ajustáveis e duração em dias),
// a barra de menus do Gantt, redimensionamento bidirecional de colunas e linhas, seleção de células/linhas/colunas,
// realce adaptativo a temas Claro e Escuro, Baseline, Linha Fantasma, padronização cromática unificada de status na Tabela WBS e nas caixas do Gantt (Executado, Em Andamento, Atrasado e Futuro), tooltips dinâmicos detalhados retraduzidos em tempo de execução, configuração de dias úteis e jornada diária (Configurações -> Dias Úteis),
// gerenciamento de feriados e dias não úteis com calendário interativo e biblioteca de 18 nações (Configurações -> Feriados),
// as 5 disposições de layout de tabelas (Configurações -> Layout -> Tabelas: Original 50%/50%, Vertical Topo, Vertical Inferior, Horizontal Esquerda e Horizontal Direita) com persistência atômica no arquivo layout.json e restauração automática a cada reinicialização,
// centralização de diálogos sobre a janela do Gantt com suporte multi-telas, barra de Simulação 4D adaptativa em 3 modos com botões dimensionados por texto e data acoplada,
// configuração de Meta de Ciclos Pomodoro individual por tarefa (1 a 999), janela de Aviso de Metas de Ciclos com alinhamento rápido,
// confirmação de sucesso instantânea com fechamento imediato e persistência assíncrona em segundo plano na edição de tarefas,
// exibição do ciclo de vida completo de tarefas no Calendário e Lista Global ([Em Progresso] / [In Progress], além de [Concluída] e [Pendente]),
// sincronização bidirecional de calendários extras no painel lateral, layout integrado PoCa (Pomodoro + Calendário Duplo) com empilhamento vertical sincronizado,
// alinhamento simétrico das molduras dos quatro quadros (Temporizador, Configurações, Calendário 1 e Calendário 2),
// sequenciamento matricial dinâmico 2xK de meses (Linha 1 com meses M a M+K-1 e Linha 2 com meses M+K a M+2K-1) ao redimensionar o divisor vertical,
// espaçamento rente entre calendários, proteção preventiva contra sobreposição da barra de rolagem, inicialização instantânea sem congelamento,
// otimização de arraste e solte de tarefas, abertura instantânea condicional do Gantt, cálculo físico discreto de progresso no Gantt por blocos Pomodoro (0%, 25%, 50%, 75%, 100% para meta 4) sem avanço arbitrário inicial de 10%, sincronização instantânea em tempo real com o cronograma,
// abertura imediata e sem travamentos das ferramentas do menu Ferramentas (Registro de Atividades, Cronograma de Gantt BIM 4D, Gerar próximas recorrências, Ativar lembretes locais e Criar revisões espaçadas) por meio de extração em memória O(1) e supressão de I/O em disco,
// unificação e integração nativa permanente dos módulos Pomodoro, Feynman, Registro de Atividades e Gantt & BIM 4D diretamente no executável principal EisenPo sem dependência de DLLs externas,
// centralização unificada dos catálogos de tradução (.ts e .qm) na árvore oficial com deploy direto para translations/ e retradução dinâmica em tempo real,
// persistência atômica assíncrona não-bloqueante no UnifiedTaskStore com sincronização contínua com o cronograma,
// retradução dinâmica ao vivo de caixas de diálogo de ferramentas (recorrências, lembretes, fluxos, templates e revisões),
// instruções de consulta ao duplo licenciamento, privacidade offline e notas de versão em Opções -> Sobre e uso da busca rápida em Opções -> Manual.
QList<ManualSection> get_manual_document(const QString &lang,
                                         bool include_pomodoro_plugin,
                                         bool include_feynman_plugin,
                                         bool include_mapamental_plugin,
                                         bool include_audiolivro_plugin,
                                         bool include_agenda_plugin,
                                         bool include_gantt_plugin)
{
    const QString normalized = normalize_language(lang);
    if (normalized == QStringLiteral("en_US")) {
        return incore_17_manual_en_US::get_manual_document(include_pomodoro_plugin,
                                                           include_feynman_plugin,
                                                           include_mapamental_plugin,
                                                           include_audiolivro_plugin,
                                                           include_agenda_plugin,
                                                           include_gantt_plugin);
    }

    return incore_16_manual_pt_BR::get_manual_document(include_pomodoro_plugin,
                                                       include_feynman_plugin,
                                                       include_mapamental_plugin,
                                                       include_audiolivro_plugin,
                                                       include_agenda_plugin,
                                                       include_gantt_plugin);
}

QPair<QList<ManualBlock>, QStringList> get_manual_blocks(const QString &lang,
                                                         bool include_pomodoro_plugin,
                                                         bool include_feynman_plugin,
                                                         bool include_mapamental_plugin,
                                                         bool include_audiolivro_plugin,
                                                         bool include_agenda_plugin,
                                                         bool include_gantt_plugin)
{
    const QString normalized = normalize_language(lang);
    const QList<ManualSection> sections =
        get_manual_document(normalized,
                            include_pomodoro_plugin,
                            include_feynman_plugin,
                            include_mapamental_plugin,
                            include_audiolivro_plugin,
                            include_agenda_plugin,
                            include_gantt_plugin);
    QList<ManualBlock> blocks;
    QStringList order;

    const QString title = get_manual_title(normalized);
    blocks.append({QStringLiteral("line"), title, QString()});
    blocks.append({QStringLiteral("line"), QString(title.size(), QLatin1Char('=')), QString()});
    blocks.append({QStringLiteral("blank"), QString(), QString()});

    blocks.append({QStringLiteral("paragraph"),
                   manual_intro_text(normalized),
                   QString()});
    blocks.append({QStringLiteral("blank"), QString(), QString()});
    blocks.append({QStringLiteral("toc_title"),
                   manual_toc_title(normalized),
                   QString()});

    for (int i = 0; i < sections.size(); ++i) {
        blocks.append({QStringLiteral("toc_item"), QStringLiteral("%1. %2").arg(i + 1).arg(sections.at(i).title), sections.at(i).id});
    }

    blocks.append({QStringLiteral("blank"), QString(), QString()});
    blocks.append({QStringLiteral("divider"), QString(60, QLatin1Char('-')), QString()});
    blocks.append({QStringLiteral("blank"), QString(), QString()});

    for (const ManualSection &section : sections) {
        order << section.id;
        blocks.append({QStringLiteral("section_title"), section.title, section.id});
        blocks.append({QStringLiteral("blank"), QString(), QString()});
        for (const QString &paragraph : section.paragraphs) {
            blocks.append({QStringLiteral("paragraph"), paragraph, QString()});
            blocks.append({QStringLiteral("blank"), QString(), QString()});
        }
        for (const QString &bullet : section.bullets) {
            blocks.append({QStringLiteral("bullet"), bullet, QString()});
        }
        if (!section.bullets.isEmpty()) {
            blocks.append({QStringLiteral("blank"), QString(), QString()});
        }
        for (const ManualDetails &detail : section.details) {
            blocks.append({QStringLiteral("detail_title"), detail.summary, QString()});
            blocks.append({QStringLiteral("blank"), QString(), QString()});
            for (const QString &paragraph : detail.paragraphs) {
                blocks.append({QStringLiteral("paragraph"), paragraph, QString()});
                blocks.append({QStringLiteral("blank"), QString(), QString()});
            }
            for (const QString &bullet : detail.bullets) {
                blocks.append({QStringLiteral("bullet"), bullet, QString()});
            }
            if (!detail.bullets.isEmpty()) {
                blocks.append({QStringLiteral("blank"), QString(), QString()});
            }
        }
        blocks.append({QStringLiteral("divider"), QString(60, QLatin1Char('-')), QString()});
        blocks.append({QStringLiteral("blank"), QString(), QString()});
    }

    return {blocks, order};
}

QString get_manual_text(const QString &lang,
                        bool include_pomodoro_plugin,
                        bool include_feynman_plugin,
                        bool include_mapamental_plugin,
                        bool include_audiolivro_plugin,
                        bool include_agenda_plugin,
                        bool include_gantt_plugin)
{
    return get_manual_text_with_positions(lang,
                                          include_pomodoro_plugin,
                                          include_feynman_plugin,
                                          include_mapamental_plugin,
                                          include_audiolivro_plugin,
                                          include_agenda_plugin,
                                          include_gantt_plugin)
        .text;
}

ManualTextWithPositions get_manual_text_with_positions(const QString &lang,
                                                       bool include_pomodoro_plugin,
                                                       bool include_feynman_plugin,
                                                       bool include_mapamental_plugin,
                                                       bool include_audiolivro_plugin,
                                                       bool include_agenda_plugin,
                                                       bool include_gantt_plugin)
{
    const QString normalized = normalize_language(lang);
    const QList<ManualSection> sections =
        get_manual_document(normalized,
                            include_pomodoro_plugin,
                            include_feynman_plugin,
                            include_mapamental_plugin,
                            include_audiolivro_plugin,
                            include_agenda_plugin,
                            include_gantt_plugin);

    QStringList lines;
    QMap<QString, int> positions;
    QStringList order;

    auto add_line = [&lines](const QString &line = QString()) { lines << line; };
    auto current_offset = [&lines]() {
        int total = 0;
        for (const QString &line : lines) {
            total += line.size() + 1;
        }
        return total;
    };

    const QString title = get_manual_title(normalized);
    add_line(title);
    add_line(QString(title.size(), QLatin1Char('=')));
    add_line();
    add_line(manual_intro_text(normalized));
    add_line();
    add_line(manual_toc_title(normalized));
    add_line(QStringLiteral("----------"));
    for (int i = 0; i < sections.size(); ++i) {
        add_line(QStringLiteral("%1. %2").arg(i + 1).arg(sections.at(i).title));
    }
    add_line();
    add_line(QString(60, QLatin1Char('-')));
    add_line();

    for (const ManualSection &section : sections) {
        positions.insert(section.id, current_offset());
        order << section.id;
        add_line(section.title);
        add_line(QString(section.title.size(), QLatin1Char('-')));
        add_line();
        for (const QString &paragraph : section.paragraphs) {
            add_line(paragraph);
            add_line();
        }
        for (const QString &bullet : section.bullets) {
            add_line(QStringLiteral("- %1").arg(bullet));
        }
        if (!section.bullets.isEmpty()) {
            add_line();
        }
        for (const ManualDetails &detail : section.details) {
            add_line(detail.summary);
            add_line(QString(detail.summary.size(), QLatin1Char('.')));
            add_line();
            for (const QString &paragraph : detail.paragraphs) {
                add_line(paragraph);
                add_line();
            }
            for (const QString &bullet : detail.bullets) {
                add_line(QStringLiteral("- %1").arg(bullet));
            }
            if (!detail.bullets.isEmpty()) {
                add_line();
            }
        }
        add_line(QString(60, QLatin1Char('-')));
        add_line();
    }

    return {lines.join(QLatin1Char('\n')), positions, order};
}

}  // namespace source::InterfaceCore::incore_12_Manual
