#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_14_mostrar_menu_contexto.hpp"

#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace GerenciadorTarefasInternal;

void GerenciadorTarefas::mostrar_menu_contexto(const QPoint &pos, QListWidget *lista)
{
    try {
        if (lista == nullptr) {
            return;
        }

        auto *item = lista->itemAt(pos);
        if (item == nullptr || is_group_header(item) || !(item->flags() & Qt::ItemIsSelectable)) {
            return;
        }

        QMenu menu(lista);

        const QVector<QListWidgetItem *> marked = collect_marked_items();
        const QVector<QListWidgetItem *> selected = marked.isEmpty() ? QVector<QListWidgetItem *>{item} : marked;

        auto *action_edit = menu.addAction(QCoreApplication::translate("PomodoroPlugin", "✏️ Editar Tarefa"));
        connect(action_edit, &QAction::triggered, this, [this, item]() {
            Tarefa *task = find_task(item);
            if (task == nullptr) {
                return;
            }

            const int configured_cycles_default = (compat::host_bridge() != nullptr)
                                                      ? compat::host_bridge()->configured_cycles()
                                                      : 4;
            const bool is_in_prog = (task->status.compare(QStringLiteral("Doing"), Qt::CaseInsensitive) == 0);
            const int default_goal = is_in_prog ? configured_cycles_default : 4;

            EditTaskDialog dialog(
                this,
                task->titulo,
                task->file_path,
                task->descricao,
                task->date,
                task->time,
                task->pomodoros_completados,
                task->prioridade,
                task->dominio,
                task->duracao,
                task->ciclos_totais > 0 ? task->ciclos_totais : default_goal);

            if (dialog.exec() != QDialog::Accepted) {
                return;
            }

            const auto values = dialog.values();
            if (values.title.isEmpty()) {
                return;
            }

            task->titulo = values.title;
            task->descricao = values.description;
            task->prioridade = Tarefa::normalizar_prioridade(values.prioridade);
            task->dominio = Tarefa::normalizar_dominio(values.dominio);
            task->pomodoros_completados = values.pomodoros;
            task->ciclos_totais = values.ciclos_totais > 0 ? values.ciclos_totais : default_goal;
            task->duracao = values.duracao;
            task->file_path = values.file_path;
            task->date = values.date_iso;
            task->time = values.time_str;

            salvar_tarefas();
            atualizar_listas();
            compat::MessageBox::tempo_edit_task_success(this);
        });

        auto *action_font = menu.addAction(QCoreApplication::translate("PomodoroPlugin", "🔤 Configurações de Fonte"));
        connect(action_font, &QAction::triggered, this, [this, selected]() {
            QList<QListWidgetItem *> target_items;
            for (QListWidgetItem *sel_item : selected) {
                if (sel_item != nullptr && !target_items.contains(sel_item)) {
                    target_items.append(sel_item);
                }
            }
            if (source::utils::open_task_font_settings_dialog(this, target_items, source::utils::PluginFontContext::Global)) {
                if (QWidget *top = this->window()) {
                    for (QListWidget *list : top->findChildren<QListWidget *>()) {
                        if (list == nullptr) continue;
                        for (int i = 0; i < list->count(); ++i) {
                            if (QListWidgetItem *sel_item = list->item(i)) {
                                if (Tarefa *task = find_task(sel_item)) {
                                    task->font_config = sel_item->data(source::utils::kCustomFontSettingsRole).toMap();
                                }
                            }
                        }
                    }
                }
                salvar_tarefas();
                atualizar_listas();
            }
        });

        auto *action_color = menu.addAction(QCoreApplication::translate("PomodoroPlugin", "🎨 Colorir tarefas"));
        connect(action_color, &QAction::triggered, this, [this, selected]() {
            QColor initial_color;
            for (QListWidgetItem *sel_item : selected) {
                if (Tarefa *task = find_task(sel_item)) {
                    if (QColor col(task->cor_texto.trimmed()); col.isValid()) {
                        initial_color = col;
                        break;
                    }
                }
            }
            const QColor selected_color = source::utils::choose_task_color(this, initial_color, source::utils::PluginFontContext::Pomodoro);
            if (!selected_color.isValid()) {
                return;
            }
            const QString color_name = selected_color.name(QColor::HexRgb);
            for (QListWidgetItem *sel_item : selected) {
                if (Tarefa *task = find_task(sel_item)) {
                    task->cor_texto = color_name;
                }
            }
            salvar_tarefas();
            atualizar_listas();
        });

        auto *share_action =
            menu.addAction(QCoreApplication::translate("PomodoroPlugin", "🗂️ Compartilhar com Eisenhower"));
        connect(share_action, &QAction::triggered, this, [this, selected]() {
            int sent = 0;
            QSet<QString> seen;

            for (QListWidgetItem *selected_item : selected) {
                if (Tarefa *task = find_task(selected_item)) {
                    const QString signature = compat::task_identity_signature(*task);
                    if (seen.contains(signature)) {
                        continue;
                    }

                    seen.insert(signature);
                    QVariantMap payload = task_payload(*task);
                    payload.insert(QStringLiteral("integration_id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
                    payload.insert(QStringLiteral("origin"), QStringLiteral("tempo"));
                    compat::get_event_bus()->send_tarefa(payload);
                    task->sincronizada_com_eisenhower = true;
                    ++sent;
                }
            }

            if (sent > 0) {
                salvar_tarefas();
            }

            if (sent > 0) {
                compat::MessageBox::tempo_share_eisenhower_success(this, sent);
            }
        });

        if (compat::host_bridge() != nullptr && compat::host_bridge()->can_publish_task_to_feynman()) {
            auto *share_feynman_action =
                menu.addAction(QCoreApplication::translate("PomodoroPlugin", "🪢 Compartilhar com Feynman"));
            connect(share_feynman_action, &QAction::triggered, this, [this, selected]() {
                int sent = 0;
                QStringList failed_names;
                QSet<QString> seen;

                for (QListWidgetItem *selected_item : selected) {
                    Tarefa *task = find_task(selected_item);
                    if (task == nullptr) {
                        failed_names << selected_item->text().split(QLatin1Char('\n')).value(0).trimmed();
                        continue;
                    }

                    const QString signature = compat::task_identity_signature(*task);
                    if (seen.contains(signature)) {
                        continue;
                    }

                    seen.insert(signature);
                    QVariantMap payload = task_payload(*task);
                    payload.insert(QStringLiteral("integration_id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
                    payload.insert(QStringLiteral("origin"), QStringLiteral("pomodoro"));

                    if (compat::host_bridge() == nullptr || !compat::host_bridge()->publish_task_to_feynman(payload)) {
                        failed_names << task->titulo;
                        continue;
                    }

                    ++sent;
                }

                if (sent > 0) {
                    compat::MessageBox::tempo_share_feynman_success(this, sent);
                }

                if (!failed_names.isEmpty()) {
                    compat::MessageBox::warning(
                        this,
                        QCoreApplication::translate("PomodoroPlugin", "Erro"),
                        QCoreApplication::translate("PomodoroPlugin",
                                                    "Nao foi possivel compartilhar a tarefa \"%1\" com Feynman.")
                            .arg(failed_names.join(QStringLiteral("\", \""))));
                }
            });
        }

        if (selected.size() > 1) {
            auto *move_selected =
                menu.addAction(QCoreApplication::translate("PomodoroPlugin", "🔀 Mover Tarefa(s) Marcadas"));
            connect(move_selected, &QAction::triggered, this, &GerenciadorTarefas::mover_tarefas_marcadas);

            auto *remove_selected =
                menu.addAction(QCoreApplication::translate("PomodoroPlugin", "🗑️ Remover Tarefa(s) Marcadas"));
            connect(remove_selected, &QAction::triggered, this, &GerenciadorTarefas::remover_tarefas_marcadas);
        } else {
            auto *move_single = menu.addAction(QCoreApplication::translate("PomodoroPlugin", "🔀 Mover Tarefa"));
            connect(move_single, &QAction::triggered, this, [this, item]() { mover_tarefa_por_item(item); });

            auto *remove_single = menu.addAction(QCoreApplication::translate("PomodoroPlugin", "🗑️ Remover Tarefa"));
            connect(remove_single, &QAction::triggered, this, [this, item]() { remover_tarefa_por_item(item); });
        }

        menu.exec(lista->mapToGlobal(pos));
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao exibir menu de contexto: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao exibir menu de contexto"), true);
    }
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::GerenciadorTarefasFns { void mostrar_menu_contexto(GerenciadorTarefas &self, const QPoint &pos, QListWidget *lista) { self.mostrar_menu_contexto(pos, lista); } }
