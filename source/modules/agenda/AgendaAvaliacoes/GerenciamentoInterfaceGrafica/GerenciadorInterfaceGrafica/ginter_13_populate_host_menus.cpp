#include "source/modules/agenda/AgendaAvaliacoes/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"

#include "source/modules/agenda/AgendaAvaliacoes/GerenciamentoInterfaceGrafica/GerenciadorInterfaceGrafica/ginter_12_excluir_item_lista_personalizada.hpp"
#include "source/modules/agenda/AgendaAvaliacoes/utils/DialogTranslationUtils.hpp"

#include <QAction>
#include <QActionGroup>
#include <QCoreApplication>
#include <QMenu>
#include <QMessageBox>

namespace {
    QString titulo_linha_customizada(const InterfaceGerenciadorAtividades::CustomAgendaRow &linha) {
        if (linha.title_label != nullptr && !linha.title_label->text().trimmed().isEmpty()) {
            return linha.title_label->text().trimmed();
        }
        return QString();
    }

}

void InterfaceGerenciadorAtividades::populate_delete_item_menu(QMenu *menu) {
    popular_menu_excluir_item_agenda(menu);
}

void InterfaceGerenciadorAtividades::popular_menu_excluir_item_agenda(QMenu *menu) {
    if (menu == nullptr) {
        return;
    }

    auto tr = [](const char *context, const char *source) { return QCoreApplication::translate(context, source); };
    menu->clear();

    if (custom_agenda_mode) {
        bool adicionou_titulo = false;
        for (int i = 0; i < custom_agenda_rows.size(); ++i) {
            const CustomAgendaRow &linha = custom_agenda_rows.at(i);
            const QString titulo = titulo_linha_customizada(linha);
            if (titulo.isEmpty()) {
                continue;
            }

            adicionou_titulo = true;
            QMenu *submenu = menu->addMenu(titulo);
            connect(submenu, &QMenu::aboutToShow, this, [this, submenu, i] {
                auto tr = [](const char *context, const char *source) {
                    return QCoreApplication::translate(context, source);
                };
                submenu->clear();
                if (i < 0 || i >= custom_agenda_rows.size()) {
                    return;
                }

                const QStringList itens = itens_linha_agenda_customizada(i);
                if (itens.isEmpty()) {
                    QAction *empty = submenu->addAction(
                        tr("InterfaceGerenciadorAtividades", "Nenhum item personalizado encontrado."));
                    empty->setEnabled(false);
                    return;
                }

                for (const QString &texto_item_bruto : itens) {
                    const QString texto_item = texto_item_bruto.trimmed();
                    if (texto_item.isEmpty()) {
                        continue;
                    }
                    QAction *action = submenu->addAction(texto_item);
                    connect(action, &QAction::triggered, this, [this, submenu, i, texto_item] {
                        if (i < 0 || i >= custom_agenda_rows.size()) {
                            return;
                        }
                        const auto resposta = perguntar_sim_nao(
                            this,
                            QCoreApplication::translate("InterfaceGerenciadorAtividades", "Confirmar exclusão"),
                            QCoreApplication::translate("InterfaceGerenciadorAtividades", "Deseja remover o item selecionado?"));
                        if (resposta != QMessageBox::Yes) {
                            return;
                        }

                        remover_item_linha_agenda_customizada(i, texto_item);
                        submenu->clear();
                    });
                }

                if (submenu->actions().isEmpty()) {
                    QAction *empty = submenu->addAction(
                        tr("InterfaceGerenciadorAtividades", "Nenhum item personalizado encontrado."));
                    empty->setEnabled(false);
                }
            });
        }

        if (!adicionou_titulo) {
            QAction *empty =
                menu->addAction(tr("InterfaceGerenciadorAtividades", "Nenhum título personalizado encontrado."));
            empty->setEnabled(false);
        }
        return;
    }

    for (const auto &categoria : CATEGORIAS_EXCLUSAO_LISTAS) {
        QMenu *submenu = menu->addMenu(tr("InterfaceGerenciadorAtividades", categoria.second.toUtf8().constData()));
        connect(submenu, &QMenu::aboutToShow, this, [this, submenu, codigo = categoria.first] {
            popular_menu_itens_personalizados(this, submenu, codigo);
        });
    }
}

void InterfaceGerenciadorAtividades::populate_color_menu(QMenu *menu) {
    if (menu == nullptr) {
        return;
    }

    auto tr = [](const char *context, const char *source) { return QCoreApplication::translate(context, source); };
    menu->clear();

    const bool is_own_menu = (menu == menu_cores);
    if (is_own_menu) {
        color_actions.clear();
        menu_preto = nullptr;
        menu_coloridas = nullptr;
    }

    const QString modo_atual = gerenciamento_atividades.obter_modo_cores();

    auto *group = new QActionGroup(menu);
    group->setExclusive(true);

    QAction *action_preto = menu->addAction(tr("InterfaceGerenciadorAtividades", "Preto"));
    action_preto->setCheckable(true);
    action_preto->setActionGroup(group);
    action_preto->setData(QStringLiteral("preto"));
    action_preto->setChecked(modo_atual == QStringLiteral("preto"));

    QAction *action_coloridas = menu->addAction(tr("InterfaceGerenciadorAtividades", "Coloridas"));
    action_coloridas->setCheckable(true);
    action_coloridas->setActionGroup(group);
    action_coloridas->setData(QStringLiteral("coloridas"));
    action_coloridas->setChecked(modo_atual == QStringLiteral("coloridas"));

    if (is_own_menu) {
        color_actions.insert(QStringLiteral("preto"), action_preto);
        color_actions.insert(QStringLiteral("coloridas"), action_coloridas);
    }

    connect(action_preto, &QAction::triggered, this, [this]() {
        change_color_mode(QStringLiteral("preto"));
    });

    connect(action_coloridas, &QAction::triggered, this, [this]() {
        change_color_mode(QStringLiteral("coloridas"));
    });
}
