#include "EisenhowerGanttHostAdapter.hpp"

#include <QFileInfo>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSet>
#include "app_01_eisenhower_matrix_app.hpp"
#include "source/services/incore_06_atualizar_placeholders.hpp"
#include "source/services/incore_11_atualizar_itens_tarefas.hpp"
#include "source/ui/agregador_01_ui.hpp"
#include "source/ui/ui_09_Calendar.hpp"
#include "source/ui/ui_14_CalendarPanel.hpp"
#include "source/utils/UnifiedTaskStore.hpp"

namespace source {

namespace {

QVariantMap harmonize_task_for_gantt(const QVariantMap &in)
{
    QVariantMap out = in;

    QString title = in.value(QStringLiteral("text")).toString().trimmed();
    if (title.isEmpty()) title = in.value(QStringLiteral("titulo")).toString().trimmed();
    if (title.isEmpty()) title = in.value(QStringLiteral("title")).toString().trimmed();
    out.insert(QStringLiteral("text"), title);
    out.insert(QStringLiteral("titulo"), title);

    QString date = in.value(QStringLiteral("date")).toString().trimmed();
    if (date.isEmpty()) date = in.value(QStringLiteral("data")).toString().trimmed();
    out.insert(QStringLiteral("date"), date);
    out.insert(QStringLiteral("data"), date);

    QString time = in.value(QStringLiteral("time")).toString().trimmed();
    if (time.isEmpty()) time = in.value(QStringLiteral("hora")).toString().trimmed();
    if (time.isEmpty()) time = in.value(QStringLiteral("horario")).toString().trimmed();
    out.insert(QStringLiteral("time"), time);
    out.insert(QStringLiteral("hora"), time);
    out.insert(QStringLiteral("horario"), time);

    int priority = 0;
    if (in.contains(QStringLiteral("priority")) && in.value(QStringLiteral("priority")).toInt() > 0) {
        priority = in.value(QStringLiteral("priority")).toInt();
    } else if (in.contains(QStringLiteral("prioridade")) && in.value(QStringLiteral("prioridade")).toInt() > 0) {
        priority = in.value(QStringLiteral("prioridade")).toInt();
    } else if (in.contains(QStringLiteral("quadrante")) && in.value(QStringLiteral("quadrante")).toInt() > 0) {
        priority = in.value(QStringLiteral("quadrante")).toInt();
    }
    if (priority < 1 || priority > 4) priority = 1;
    out.insert(QStringLiteral("priority"), priority);
    out.insert(QStringLiteral("prioridade"), priority);
    out.insert(QStringLiteral("quadrante"), priority);

    bool completed = false;
    if (in.contains(QStringLiteral("concluida"))) {
        completed = in.value(QStringLiteral("concluida")).toBool();
    } else if (in.contains(QStringLiteral("completed"))) {
        completed = in.value(QStringLiteral("completed")).toBool();
    } else {
        const QString st = in.value(QStringLiteral("status")).toString().toLower();
        completed = (st == QStringLiteral("done") || st == QStringLiteral("concluida"));
    }
    out.insert(QStringLiteral("completed"), completed);
    out.insert(QStringLiteral("concluida"), completed);

    QString id = in.value(QStringLiteral("id")).toString().trimmed();
    if (id.isEmpty()) {
        id = in.value(QStringLiteral("task_id")).toString().trimmed();
    }
    if (id.isEmpty()) {
        const QString sig = QStringLiteral("%1|%2|%3|%4").arg(title, date, time).arg(priority);
        id = QStringLiteral("eisen_") + QString::number(qHash(sig), 16);
    }
    out.insert(QStringLiteral("id"), id);

    QString desc = in.value(QStringLiteral("description")).toString();
    if (desc.isEmpty()) desc = in.value(QStringLiteral("descricao")).toString();
    out.insert(QStringLiteral("description"), desc);
    out.insert(QStringLiteral("descricao"), desc);

    double duration = 1.0;
    if (in.contains(QStringLiteral("duracao")) && in.value(QStringLiteral("duracao")).toDouble() > 0.0) {
        duration = in.value(QStringLiteral("duracao")).toDouble();
    } else if (in.contains(QStringLiteral("duration")) && in.value(QStringLiteral("duration")).toDouble() > 0.0) {
        duration = in.value(QStringLiteral("duration")).toDouble();
    }
    out.insert(QStringLiteral("duracao"), duration);
    out.insert(QStringLiteral("duration"), duration);

    int ciclos = 4;
    if (in.contains(QStringLiteral("ciclos_totais")) && in.value(QStringLiteral("ciclos_totais")).toInt() > 0) {
        ciclos = in.value(QStringLiteral("ciclos_totais")).toInt();
    } else if (in.contains(QStringLiteral("ciclos_permitidos")) && in.value(QStringLiteral("ciclos_permitidos")).toInt() > 0) {
        ciclos = in.value(QStringLiteral("ciclos_permitidos")).toInt();
    } else if (in.contains(QStringLiteral("total_ciclos")) && in.value(QStringLiteral("total_ciclos")).toInt() > 0) {
        ciclos = in.value(QStringLiteral("total_ciclos")).toInt();
    } else if (in.contains(QStringLiteral("meta_pomodoros")) && in.value(QStringLiteral("meta_pomodoros")).toInt() > 0) {
        ciclos = in.value(QStringLiteral("meta_pomodoros")).toInt();
    } else if (in.contains(QStringLiteral("meta")) && in.value(QStringLiteral("meta")).toInt() > 0) {
        ciclos = in.value(QStringLiteral("meta")).toInt();
    }
    out.insert(QStringLiteral("ciclos_totais"), ciclos);
    out.insert(QStringLiteral("ciclos_permitidos"), ciclos);
    out.insert(QStringLiteral("total_ciclos"), ciclos);
    out.insert(QStringLiteral("meta_pomodoros"), ciclos);

    const int pomodoros = qMax(0, in.value(QStringLiteral("pomodoros_completados"),
                                  in.value(QStringLiteral("pomodoros"),
                                  in.value(QStringLiteral("ciclos_completados"), 0))).toInt());
    out.insert(QStringLiteral("pomodoros"), pomodoros);
    out.insert(QStringLiteral("pomodoros_completados"), pomodoros);
    out.insert(QStringLiteral("ciclos_completados"), pomodoros);

    return out;
}

} // namespace

EisenhowerGanttHostAdapter::EisenhowerGanttHostAdapter(EisenhowerMatrixApp *app)
    : app_(app)
{
}

void EisenhowerGanttHostAdapter::invalidate_cache() const
{
    cache_valid_ = false;
    task_cache_.clear();
    cached_tasks_list_.clear();
}

void EisenhowerGanttHostAdapter::ensure_cache_valid() const
{
    if (cache_valid_) {
        return;
    }

    cached_tasks_list_.clear();
    task_cache_.clear();
    QSet<QString> seen_ids;

    // 1. Collect from live UI first (reflects current state on screen)
    if (app_) {
        const QList<QVariantMap> from_ui = source::GerenciamentoUI::task_utils::collect_tasks_from_app(app_.data());
        for (const auto &m : from_ui) {
            const QVariantMap harmonized = harmonize_task_for_gantt(m);
            const QString id = harmonized.value(QStringLiteral("id")).toString().trimmed();
            if (!id.isEmpty()) {
                seen_ids.insert(id);
                task_cache_.insert(id, harmonized);
            }
            cached_tasks_list_.append(harmonized);
        }
    }

    // 2. Supplement from persistent store (tarefas.json)
    const QString path = source::utils::UnifiedTaskStore::shared_tasks_path();
    if (!path.isEmpty() && QFileInfo::exists(path)) {
        const QVariantList from_store = source::utils::UnifiedTaskStore::load_records(path);
        for (const auto &var : from_store) {
            const QVariantMap map = var.toMap();
            const QString mod = map.value(QStringLiteral("modulo")).toString().trimmed().toLower();
            // If the live Eisenhower app is running, it is the sole authority for Eisenhower tasks.
            // Records from tarefas.json with modulo "eisenhower" (or unspecified) that are not in seen_ids were deleted.
            if (app_ && (mod.isEmpty() || mod == QStringLiteral("eisenhower"))) {
                continue;
            }
            const QVariantMap harmonized = harmonize_task_for_gantt(map);
            const QString id = harmonized.value(QStringLiteral("id")).toString().trimmed();
            if (id.isEmpty() || !seen_ids.contains(id)) {
                if (!id.isEmpty()) {
                    seen_ids.insert(id);
                    task_cache_.insert(id, harmonized);
                }
                cached_tasks_list_.append(harmonized);
            }
        }
    }

    cache_valid_ = true;
}

QVariantList EisenhowerGanttHostAdapter::get_available_tasks() const
{
    ensure_cache_valid();
    return cached_tasks_list_;
}

QVariantMap EisenhowerGanttHostAdapter::get_task_by_id(const QString &task_id) const
{
    const QString trimmed_id = task_id.trimmed();
    if (trimmed_id.isEmpty()) {
        return {};
    }
    ensure_cache_valid();
    return task_cache_.value(trimmed_id);
}

void EisenhowerGanttHostAdapter::sync_task_progress(const QString &task_id, double percent, bool is_done)
{
    if (task_id.trimmed().isEmpty()) {
        return;
    }

    // 1. Update in active GUI app if available
    if (app_) {
        const QList<QListWidget *> lists = {
            app_->quadrant1_list,
            app_->quadrant2_list,
            app_->quadrant3_list,
            app_->quadrant4_list,
            app_->quadrant1_in_progress_list,
            app_->quadrant2_in_progress_list,
            app_->quadrant3_in_progress_list,
            app_->quadrant4_in_progress_list,
            app_->quadrant1_completed_list,
            app_->quadrant2_completed_list,
            app_->quadrant3_completed_list,
            app_->quadrant4_completed_list,
        };

        for (QListWidget *list : lists) {
            if (!list) continue;
            for (int i = 0; i < list->count(); ++i) {
                QListWidgetItem *item = list->item(i);
                if (!item || !(item->flags() & Qt::ItemIsSelectable)) continue;

                QVariantMap data = item->data(Qt::UserRole).toMap();
                if (data.value(QStringLiteral("id")).toString() == task_id) {
                    const int target_cycles = qMax(1, data.value(QStringLiteral("ciclos_totais"),
                                                  data.value(QStringLiteral("ciclos_permitidos"),
                                                  data.value(QStringLiteral("total_ciclos"),
                                                  data.value(QStringLiteral("meta_pomodoros"), 4)))).toInt());
                    const int completed_pomodoros = qBound(0, qRound((percent / 100.0) * target_cycles), target_cycles);

                    data.insert(QStringLiteral("concluida"), is_done || percent >= 100.0);
                    data.insert(QStringLiteral("completed"), is_done || percent >= 100.0);
                    data.insert(QStringLiteral("gantt_progress"), percent);
                    data.insert(QStringLiteral("pomodoros"), completed_pomodoros);
                    data.insert(QStringLiteral("pomodoros_completados"), completed_pomodoros);
                    data.insert(QStringLiteral("ciclos_completados"), completed_pomodoros);
                    item->setData(Qt::UserRole, data);

                    int q = data.value(QStringLiteral("priority")).toInt();
                    if (q < 1 || q > 4) q = 1;

                    if (is_done || percent >= 100.0) {
                        QListWidget *target = app_->quadrant_list(q, true);
                        if (target && list != target) {
                            app_->move_item_between_lists(item, list, target, Qt::Checked);
                            app_->save_tasks();
                        }
                    } else if (percent > 0.0) {
                        QListWidget *target = app_->quadrant_in_progress_list(q);
                        if (target && list != target && !app_->is_completed_list(list)) {
                            app_->move_item_between_lists(item, list, target, Qt::Unchecked);
                            app_->save_tasks();
                        }
                    }
                    source::InterfaceCore::incore_11_atualizar_itens_tarefas::atualizar_itens_tarefas(app_.data());
                    break;
                }
            }
        }
    }

    // 2. Update persistent task store
    const QString path = source::utils::UnifiedTaskStore::shared_tasks_path();
    QVariantList records = source::utils::UnifiedTaskStore::load_records(path);
    bool store_updated = false;
    for (auto &rec : records) {
        QVariantMap map = rec.toMap();
        if (map.value(QStringLiteral("id")).toString() == task_id) {
            const int target_cycles = qMax(1, map.value(QStringLiteral("ciclos_totais"),
                                          map.value(QStringLiteral("ciclos_permitidos"),
                                          map.value(QStringLiteral("total_ciclos"),
                                          map.value(QStringLiteral("meta_pomodoros"), 4)))).toInt());
            const int completed_pomodoros = qBound(0, qRound((percent / 100.0) * target_cycles), target_cycles);

            map.insert(QStringLiteral("concluida"), is_done || percent >= 100.0);
            map.insert(QStringLiteral("completed"), is_done || percent >= 100.0);
            map.insert(QStringLiteral("gantt_progress"), percent);
            map.insert(QStringLiteral("pomodoros"), completed_pomodoros);
            map.insert(QStringLiteral("pomodoros_completados"), completed_pomodoros);
            map.insert(QStringLiteral("ciclos_completados"), completed_pomodoros);
            rec = map;
            store_updated = true;
            break;
        }
    }
    if (store_updated) {
        source::utils::UnifiedTaskStore::save_records(path, records);
    }
    invalidate_cache();
}

void EisenhowerGanttHostAdapter::sync_task_duration(const QString &task_id, double duration_days)
{
    if (task_id.trimmed().isEmpty()) {
        return;
    }

    if (duration_days <= 0.0) {
        duration_days = 1.0;
    }

    // 1. Update in active GUI app if available
    if (app_) {
        const QList<QListWidget *> lists = {
            app_->quadrant1_list,
            app_->quadrant2_list,
            app_->quadrant3_list,
            app_->quadrant4_list,
            app_->quadrant1_in_progress_list,
            app_->quadrant2_in_progress_list,
            app_->quadrant3_in_progress_list,
            app_->quadrant4_in_progress_list,
            app_->quadrant1_completed_list,
            app_->quadrant2_completed_list,
            app_->quadrant3_completed_list,
            app_->quadrant4_completed_list,
            app_->eisenhower_doing_list,
        };

        for (QListWidget *list : lists) {
            if (!list) continue;
            for (int i = 0; i < list->count(); ++i) {
                QListWidgetItem *item = list->item(i);
                if (!item || !(item->flags() & Qt::ItemIsSelectable)) continue;

                QVariantMap data = item->data(Qt::UserRole).toMap();
                if (data.value(QStringLiteral("id")).toString() == task_id) {
                    data.insert(QStringLiteral("duracao"), duration_days);
                    data.insert(QStringLiteral("duration"), duration_days);
                    item->setData(Qt::UserRole, data);
                    app_->_apply_task_item_appearance(item);
                    break;
                }
            }
        }

        app_->save_tasks();

        if (app_->calendar_pane != nullptr && app_->calendar_pane->calendar_panel != nullptr) {
            app_->calendar_pane->calendar_panel->update_task_list();
        }
    }

    // 2. Update persistent task store
    const QString path = source::utils::UnifiedTaskStore::shared_tasks_path();
    QVariantList records = source::utils::UnifiedTaskStore::load_records(path);
    bool store_updated = false;
    for (auto &rec : records) {
        QVariantMap map = rec.toMap();
        if (map.value(QStringLiteral("id")).toString() == task_id) {
            map.insert(QStringLiteral("duracao"), duration_days);
            map.insert(QStringLiteral("duration"), duration_days);
            rec = map;
            store_updated = true;
            break;
        }
    }
    if (store_updated) {
        source::utils::UnifiedTaskStore::save_records(path, records);
    }
    invalidate_cache();
}

bool EisenhowerGanttHostAdapter::remove_task_by_id(const QString &task_id)
{
    const QString target_id = task_id.trimmed();
    if (target_id.isEmpty()) {
        return false;
    }

    bool any_removed = false;
    QVariantMap deleted_task_data;

    // 1. Remove from all active EisenPo lists (pending, in-progress, completed across all 4 quadrants, plus doing list)
    if (app_) {
        const QList<QListWidget *> lists = {
            app_->quadrant1_list,
            app_->quadrant2_list,
            app_->quadrant3_list,
            app_->quadrant4_list,
            app_->quadrant1_in_progress_list,
            app_->quadrant2_in_progress_list,
            app_->quadrant3_in_progress_list,
            app_->quadrant4_in_progress_list,
            app_->quadrant1_completed_list,
            app_->quadrant2_completed_list,
            app_->quadrant3_completed_list,
            app_->quadrant4_completed_list,
            app_->eisenhower_doing_list,
        };

        for (QListWidget *list : lists) {
            if (!list) continue;
            for (int i = list->count() - 1; i >= 0; --i) {
                QListWidgetItem *item = list->item(i);
                if (!item || !(item->flags() & Qt::ItemIsSelectable)) continue;

                const QVariantMap data = item->data(Qt::UserRole).toMap();
                QString id = data.value(QStringLiteral("id")).toString().trimmed();
                if (id.isEmpty()) {
                    id = data.value(QStringLiteral("task_id")).toString().trimmed();
                }
                if (id.isEmpty()) {
                    const QString title = data.value(QStringLiteral("text"), data.value(QStringLiteral("titulo"))).toString().trimmed();
                    const QString date = data.value(QStringLiteral("date"), data.value(QStringLiteral("data"))).toString().trimmed();
                    const QString time = data.value(QStringLiteral("time"), data.value(QStringLiteral("hora"))).toString().trimmed();
                    const int priority = data.value(QStringLiteral("priority"), data.value(QStringLiteral("prioridade"), 1)).toInt();
                    const QString sig = QStringLiteral("%1|%2|%3|%4").arg(title, date, time).arg(priority);
                    id = QStringLiteral("eisen_") + QString::number(qHash(sig), 16);
                }

                if (id == target_id) {
                    if (deleted_task_data.isEmpty()) {
                        deleted_task_data = data;
                    }
                    delete list->takeItem(i);
                    any_removed = true;
                    app_->cleanup_time_groups(list);
                }
            }
        }

        if (any_removed) {
            source::InterfaceCore::incore_06_atualizar_placeholders::atualizar_placeholders(app_.data());
            if (!deleted_task_data.isEmpty()) {
                app_->remove_task_from_synced_modules(deleted_task_data, QStringLiteral("eisenhower"));
            }
            app_->save_tasks_now();

            if (app_->calendar_pane != nullptr && app_->calendar_pane->calendar_panel != nullptr) {
                app_->calendar_pane->calendar_panel->update_task_list();
            }
        }
    }

    // 2. Remove from persistent task store (tarefas.json)
    const QString path = source::utils::UnifiedTaskStore::shared_tasks_path();
    QVariantList records = source::utils::UnifiedTaskStore::load_records(path);
    bool store_updated = false;
    for (int i = records.size() - 1; i >= 0; --i) {
        const QVariantMap map = records.at(i).toMap();
        QString id = map.value(QStringLiteral("id")).toString().trimmed();
        if (id.isEmpty()) {
            id = map.value(QStringLiteral("task_id")).toString().trimmed();
        }
        if (id.isEmpty()) {
            const QString title = map.value(QStringLiteral("text"), map.value(QStringLiteral("titulo"))).toString().trimmed();
            const QString date = map.value(QStringLiteral("date"), map.value(QStringLiteral("data"))).toString().trimmed();
            const QString time = map.value(QStringLiteral("time"), map.value(QStringLiteral("hora"))).toString().trimmed();
            const int priority = map.value(QStringLiteral("priority"), map.value(QStringLiteral("prioridade"), 1)).toInt();
            const QString sig = QStringLiteral("%1|%2|%3|%4").arg(title, date, time).arg(priority);
            id = QStringLiteral("eisen_") + QString::number(qHash(sig), 16);
        }

        if (id == target_id) {
            if (deleted_task_data.isEmpty()) {
                deleted_task_data = map;
            }
            records.removeAt(i);
            store_updated = true;
            any_removed = true;
        }
    }

    if (store_updated) {
        source::utils::UnifiedTaskStore::save_records(path, records);
        if (app_ && !deleted_task_data.isEmpty()) {
            app_->remove_task_from_synced_modules(deleted_task_data, QStringLiteral("eisenhower"));
        }
    }

    if (any_removed) {
        invalidate_cache();
    }

    return any_removed;
}

void EisenhowerGanttHostAdapter::show_task_sync_delete_info(const QString &origin, QWidget *parent)
{
    if (app_) {
        app_->show_task_sync_delete_info(origin.trimmed().isEmpty() ? QStringLiteral("eisenhower") : origin, parent);
    }
}

QWidget *EisenhowerGanttHostAdapter::as_widget()
{
    return app_.data();
}

} // namespace source
