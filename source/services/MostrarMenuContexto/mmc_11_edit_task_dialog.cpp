#include "source/services/MostrarMenuContexto/mmc_11_edit_task_dialog.hpp"

#include <QBrush>
#include <QColor>
#include <QCoreApplication>
#include <QFont>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QVariantMap>

#include "source/ui/ui_09_Calendar.hpp"
#include "source/ui/ui_14_CalendarPanel.hpp"
#include "source/services/MostrarMenuContexto/mmc_05_is_completed_list.hpp"
#include "source/services/MostrarMenuContexto/mmc_07_target_list_for_quadrant.hpp"
#include "source/services/MostrarMenuContexto/mmc_08_find_source_item_for_calendar.hpp"
#include "source/services/MostrarMenuContexto/mmc_09_base_text_from_item.hpp"
#include "source/services/MostrarMenuContexto/mmc_10_build_display_and_tooltip.hpp"
#include "source/services/incore_13_prioridade_display.hpp"
#include "source/services/incore_14_EditTaskDialog.hpp"
#include "source/eisenhower_app.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"
#include "source/app/app_02_get_text.hpp"
#include "source/utils/LinkedTitleUnderlineDelegate.hpp"
#include "source/utils/UnifiedTaskStore.hpp"
#include "source/modules/gantt/gantt_plugin_interface.hpp"
#include "source/modules/pomodoro/Pomodoro/tmp_02_PomodoroTimer.hpp"

namespace source::InterfaceCore::MostrarMenuContextoModules {
namespace {

QString get_text(const QString &text)
{
    return QCoreApplication::translate("InterfaceGrafica", text.toUtf8().constData());
}

int priority_for_list(source::EisenhowerMatrixApp *app, QListWidget *list)
{
    if (app == nullptr) {
        return 1;
    }
    if (list == app->quadrant1_list || list == app->quadrant1_completed_list || list == app->quadrant1_in_progress_list) {
        return 1;
    }
    if (list == app->quadrant2_list || list == app->quadrant2_completed_list || list == app->quadrant2_in_progress_list) {
        return 2;
    }
    if (list == app->quadrant3_list || list == app->quadrant3_completed_list || list == app->quadrant3_in_progress_list) {
        return 3;
    }
    if (list == app->quadrant4_list || list == app->quadrant4_completed_list || list == app->quadrant4_in_progress_list) {
        return 4;
    }
    return 1;
}

QString placeholder_for_list(source::EisenhowerMatrixApp *app, QListWidget *list)
{
    if (app == nullptr) {
        return get_text(QStringLiteral("Nenhuma Tarefa Concluída"));
    }
    if (list == app->quadrant1_list) {
        return get_text(QStringLiteral("1º Quadrante"));
    }
    if (list == app->quadrant2_list) {
        return get_text(QStringLiteral("2º Quadrante"));
    }
    if (list == app->quadrant3_list) {
        return get_text(QStringLiteral("3º Quadrante"));
    }
    if (list == app->quadrant4_list) {
        return get_text(QStringLiteral("4º Quadrante"));
    }
    return get_text(QStringLiteral("Nenhuma Tarefa Concluída"));
}

bool list_has_selectable_items(QListWidget *list)
{
    if (list == nullptr) {
        return false;
    }
    for (int i = 0; i < list->count(); ++i) {
        QListWidgetItem *item = list->item(i);
        if (item != nullptr && (item->flags() & Qt::ItemIsSelectable)) {
            return true;
        }
    }
    return false;
}

}  // namespace

void edit_task_dialog(source::EisenhowerMatrixApp *app, QListWidgetItem *item)
{
    if (app == nullptr || item == nullptr) {
        return;
    }

    QVariantMap data = item->data(Qt::UserRole).toMap();
    QListWidget *src_list = item->listWidget();
    QListWidgetItem *src_item = item;

    if (data.contains(QStringLiteral("category"))) {
        const auto resolved = find_source_item_for_calendar(app, data);
        if (resolved.first != nullptr && resolved.second != nullptr) {
            src_list = resolved.first;
            src_item = resolved.second;
            data = src_item->data(Qt::UserRole).toMap();
        }
    } else if (src_list == app->eisenhower_doing_list) {
        const QString item_id = data.value(QStringLiteral("id")).toString().trimmed();
        QString item_text = data.value(QStringLiteral("text"), data.value(QStringLiteral("titulo"))).toString().trimmed();
        if (item_text.isEmpty()) {
            item_text = base_text_from_item(item).trimmed();
        }
        item_text.remove(QRegularExpression(QStringLiteral("^[🔴🟠🟡🟢🥇🥈🥉\\s]+")));

        QListWidgetItem *quad_item = nullptr;
        QListWidget *quad_list = nullptr;

        const QList<QListWidget *> search_lists = {
            app->quadrant1_in_progress_list,
            app->quadrant2_in_progress_list,
            app->quadrant3_in_progress_list,
            app->quadrant4_in_progress_list,
            app->quadrant1_list,
            app->quadrant2_list,
            app->quadrant3_list,
            app->quadrant4_list,
        };

        for (QListWidget *l : search_lists) {
            if (l == nullptr) {
                continue;
            }
            for (int i = 0; i < l->count(); ++i) {
                QListWidgetItem *cand = l->item(i);
                if (cand == nullptr || !(cand->flags() & Qt::ItemIsSelectable)) {
                    continue;
                }
                const QVariantMap cdata = cand->data(Qt::UserRole).toMap();
                const QString cid = cdata.value(QStringLiteral("id")).toString().trimmed();
                QString ctitle = cdata.value(QStringLiteral("text"), cdata.value(QStringLiteral("titulo"))).toString().trimmed();
                ctitle.remove(QRegularExpression(QStringLiteral("^[🔴🟠🟡🟢🥇🥈🥉\\s]+")));

                const bool match_id = !item_id.isEmpty() && !cid.isEmpty() && (cid == item_id);
                const bool match_title = !item_text.isEmpty() && !ctitle.isEmpty() &&
                                         (ctitle == item_text || ctitle.contains(item_text) || item_text.contains(ctitle));

                if (match_id || match_title) {
                    quad_item = cand;
                    quad_list = l;
                    break;
                }
            }
            if (quad_item != nullptr) {
                break;
            }
        }

        if (quad_item != nullptr && quad_list != nullptr) {
            src_item = quad_item;
            src_list = quad_list;
            data = src_item->data(Qt::UserRole).toMap();
        }
    }

    const QString initial_text = data.value(QStringLiteral("text")).toString().isEmpty() ? base_text_from_item(src_item)
                                                                                          : data.value(QStringLiteral("text")).toString();
    const QString initial_file = data.value(QStringLiteral("file_path")).toString();
    const QString initial_desc = data.value(QStringLiteral("description")).toString();
    const QString initial_date_iso = data.value(QStringLiteral("date")).toString();
    const QString initial_time_str = data.value(QStringLiteral("time")).toString();
    const int initial_pomodoros = data.value(QStringLiteral("pomodoros_completados"), data.value(QStringLiteral("pomodoros"), 0)).toInt();

    QVariant initial_priority = data.value(QStringLiteral("priority"));
    if (!initial_priority.isValid() || initial_priority.toString().trimmed().isEmpty() || initial_priority.toInt() <= 0 || initial_priority.toInt() > 4) {
        initial_priority = data.value(QStringLiteral("prioridade"), data.value(QStringLiteral("quadrante")));
    }
    bool prio_ok = false;
    const int prio_val = initial_priority.toInt(&prio_ok);
    if (!prio_ok || prio_val < 1 || prio_val > 4) {
        initial_priority = priority_for_list(app, src_list);
    }

    QVariant initial_domain = data.value(QStringLiteral("dominio"));
    if (!initial_domain.isValid() || initial_domain.toString().trimmed().isEmpty()) {
        initial_domain = data.value(QStringLiteral("domain"), data.value(QStringLiteral("titulo_dominio")));
    }

    double initial_duration = 1.0;
    if (data.contains(QStringLiteral("duracao")) && data.value(QStringLiteral("duracao")).toDouble() > 0.0) {
        initial_duration = data.value(QStringLiteral("duracao")).toDouble();
    } else if (data.contains(QStringLiteral("duration")) && data.value(QStringLiteral("duration")).toDouble() > 0.0) {
        initial_duration = data.value(QStringLiteral("duration")).toDouble();
    }

    const int configured_cycles_default = (app != nullptr && app->pomodoro_host_bridge() != nullptr)
                                              ? app->pomodoro_host_bridge()->configured_cycles()
                                              : (app != nullptr && app->eisenhower_pomodoro_timer != nullptr
                                                     ? app->eisenhower_pomodoro_timer->ciclos_permitidos
                                                     : 4);
    const bool is_in_progress = (app != nullptr && (src_list == app->eisenhower_doing_list || app->is_in_progress_list(src_list)));
    const int fallback_cycles = is_in_progress ? configured_cycles_default : 4;
    const int initial_ciclos_totais = qMax(1, data.value(QStringLiteral("ciclos_totais"),
                                              data.value(QStringLiteral("ciclos_permitidos"),
                                              data.value(QStringLiteral("total_ciclos"), fallback_cycles))).toInt());

    source::InterfaceCore::incore_14_EditTaskDialog::EditTaskDialog dialog(app,
                                                                           app,
                                                                           initial_text,
                                                                           initial_file,
                                                                           initial_desc,
                                                                           initial_date_iso,
                                                                           initial_time_str,
                                                                           initial_priority,
                                                                           initial_domain,
                                                                           initial_pomodoros,
                                                                           initial_duration,
                                                                           initial_ciclos_totais);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const auto values = dialog.get_values();
    if (values.text.isEmpty()) {
        return;
    }

    QVariantMap new_data = data;
    new_data.insert(QStringLiteral("text"), values.text);
    new_data.insert(QStringLiteral("date"), values.date_iso);
    new_data.insert(QStringLiteral("time"), values.time_str);
    new_data.insert(QStringLiteral("priority"), values.priority);
    new_data.insert(QStringLiteral("dominio"), values.domain);
    new_data.insert(QStringLiteral("pomodoros"), values.pomodoros);
    new_data.insert(QStringLiteral("pomodoros_completados"), values.pomodoros);
    new_data.insert(QStringLiteral("ciclos_completados"), values.pomodoros);
    const int target_cycles_val = values.ciclos_totais > 0 ? values.ciclos_totais : 4;
    new_data.insert(QStringLiteral("ciclos_totais"), target_cycles_val);
    new_data.insert(QStringLiteral("ciclos_permitidos"), target_cycles_val);
    new_data.insert(QStringLiteral("total_ciclos"), target_cycles_val);
    new_data.insert(QStringLiteral("meta_pomodoros"), target_cycles_val);
    new_data.insert(QStringLiteral("duracao"), values.duration);
    new_data.insert(QStringLiteral("duration"), values.duration);
    const double calculated_pct = source::utils::calcular_percentual_conclusao(values.pomodoros, target_cycles_val);
    new_data.insert(QStringLiteral("gantt_progress"), calculated_pct);
    new_data.insert(QStringLiteral("concluida"), values.pomodoros >= target_cycles_val);
    new_data.insert(QStringLiteral("completed"), values.pomodoros >= target_cycles_val);
    if (!values.file_path.isEmpty()) {
        new_data.insert(QStringLiteral("file_path"), values.file_path);
    } else {
        new_data.remove(QStringLiteral("file_path"));
    }
    if (!values.description.isEmpty()) {
        new_data.insert(QStringLiteral("description"), values.description);
    } else {
        new_data.remove(QStringLiteral("description"));
    }

    const QString domain_badge = source::InterfaceCore::dominio_insignia(values.domain);
    const QString decorated_text =
        domain_badge.isEmpty() ? values.text : QStringLiteral("%1 %2").arg(domain_badge, values.text);
    const auto display_and_tooltip =
        build_display_and_tooltip(app, decorated_text, values.date_iso, values.time_str, values.pomodoros);
    QStringList tooltip_lines;
    if (!display_and_tooltip.second.isEmpty()) {
        tooltip_lines << display_and_tooltip.second;
    }
    if (!values.file_path.isEmpty()) {
        tooltip_lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Arquivo")), values.file_path);
    }
    if (!values.description.isEmpty()) {
        QStringList lines;
        for (const QString &line : values.description.split(QLatin1Char('\n'))) {
            if (!line.trimmed().isEmpty()) {
                lines << line.trimmed();
            }
            if (lines.size() == 3) {
                break;
            }
        }
        if (!lines.isEmpty()) {
            tooltip_lines << QStringLiteral("%1:").arg(get_text(QStringLiteral("Descrição")));
            tooltip_lines << lines.join(QLatin1Char('\n'));
        }
    }
    tooltip_lines << QStringLiteral("%1: %2")
                         .arg(get_text(QStringLiteral("Prioridade")),
                              source::InterfaceCore::prioridade_para_texto(values.priority, app));
    tooltip_lines << QStringLiteral("%1: %2")
                         .arg(get_text(QStringLiteral("Nível de Domínio")),
                              source::InterfaceCore::dominio_para_texto(values.domain, false));
    tooltip_lines << source::app::format_duration_tooltip_line(values.duration);
    const int target_cycles = values.ciclos_totais > 0 ? values.ciclos_totais : fallback_cycles;
    const QString meta_lbl = get_text(QStringLiteral("Meta de Ciclos Pomodoro (🎯):"));
    const QString unit = (target_cycles == 1 ? get_text(QStringLiteral("ciclo")) : get_text(QStringLiteral("ciclos")));
    tooltip_lines << (meta_lbl.endsWith(QLatin1Char(':'))
                          ? QStringLiteral("%1 %2 %3").arg(meta_lbl, QString::number(target_cycles), unit)
                          : QStringLiteral("%1: %2 %3").arg(meta_lbl, QString::number(target_cycles), unit));


    QListWidget *target_list = src_list;
    if (values.priority >= 1 && values.priority <= 4) {
        if (app->is_in_progress_list(src_list)) {
            const QList<QListWidget *> in_progress = {app->quadrant1_in_progress_list,
                                                      app->quadrant2_in_progress_list,
                                                      app->quadrant3_in_progress_list,
                                                      app->quadrant4_in_progress_list};
            target_list = in_progress.at(values.priority - 1);
        } else {
            target_list = target_list_for_quadrant(app, values.priority - 1, is_completed_list(app, src_list), false);
        }
        if (target_list == nullptr) {
            target_list = src_list;
        }
    }

    const int row = src_list != nullptr && src_item != nullptr ? src_list->row(src_item) : -1;
    const Qt::CheckState check_state = src_item != nullptr ? src_item->checkState() : Qt::Unchecked;
    if (row >= 0) {
        delete src_list->takeItem(row);
    }
    app->cleanup_time_groups(src_list);
    if (!list_has_selectable_items(src_list)) {
        src_list->clear();
        app->add_placeholder(src_list, placeholder_for_list(app, src_list));
    }

    auto *new_item = new QListWidgetItem(display_and_tooltip.first);
    new_item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    new_item->setFlags(new_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    new_item->setCheckState(check_state);
    new_item->setData(Qt::UserRole, new_data);
    source::utils::apply_task_title_font(new_item, values.text, !values.file_path.isEmpty());
    if (!tooltip_lines.isEmpty()) {
        new_item->setToolTip(tooltip_lines.join(QLatin1Char('\n')));
    }
    if (!values.file_path.isEmpty()) {
        const QColor text_color(new_data.value(QStringLiteral("cor_texto")).toString().trimmed());
        if (text_color.isValid()) {
            new_item->setForeground(QBrush(text_color));
        } else {
            new_item->setForeground(QBrush());
        }
    } else {
        const QColor text_color(new_data.value(QStringLiteral("cor_texto")).toString().trimmed());
        if (text_color.isValid()) {
            new_item->setForeground(QBrush(text_color));
        }
    }

    if (target_list->count() == 1 && !(target_list->item(0)->flags() & Qt::ItemIsSelectable)) {
        target_list->clear();
    }
    app->insert_task_into_quadrant_list(target_list, new_item);
    app->cleanup_time_groups(target_list);
    if (app->calendar_pane != nullptr && app->calendar_pane->calendar_panel != nullptr) {
        app->calendar_pane->calendar_panel->update_task_list();
    }
    app->atualizar_eisenhower_coluna_doing_timer();
    app->save_tasks_now();

    QMessageBox message_box(app);
    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(get_text(QStringLiteral("✏️ Editar Tarefa")));
    message_box.setText(get_text(QStringLiteral("Alterações aplicadas com sucesso.")));
    message_box.setStandardButtons(QMessageBox::Ok);
    message_box.setDefaultButton(QMessageBox::Ok);
    if (app->gerenciador_traducao != nullptr) {
        app->gerenciador_traducao->traduzir_botoes_padrao(&message_box);
    }
    message_box.exec();
}

}  // namespace source::InterfaceCore::MostrarMenuContextoModules
