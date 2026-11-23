#include "source/app/app_06_send_task_to_feynman.hpp"

namespace source {
using namespace app;

namespace {

QString canonical_sync_module(const QString &module)
{
    const QString normalized = module.trimmed().toCaseFolded();
    if (normalized == QStringLiteral("tempo")) {
        return QStringLiteral("pomodoro");
    }
    if (normalized == QStringLiteral("eisenhower") || normalized == QStringLiteral("pomodoro")
        || normalized == QStringLiteral("feynman")) {
        return normalized;
    }
    return {};
}

QStringList enabled_sync_modules(const EisenhowerMatrixApp *app)
{
    QStringList modules;
    if (app != nullptr && app->sync_eisenhower_enabled()) {
        modules.append(QStringLiteral("eisenhower"));
    }
    if (app != nullptr && app->sync_pomodoro_enabled() && app->has_pomodoro_plugin()) {
        modules.append(QStringLiteral("pomodoro"));
    }
    if (app != nullptr && app->sync_feynman_enabled() && app->has_feynman_plugin()) {
        modules.append(QStringLiteral("feynman"));
    }
    return modules;
}

QStringList active_sync_modules_for_origin(const EisenhowerMatrixApp *app, const QString &origin)
{
    QStringList modules = enabled_sync_modules(app);
    const QString origin_module = canonical_sync_module(origin);
    if (!origin_module.isEmpty() && !modules.contains(origin_module)) {
        return {};
    }
    return modules;
}

QString sync_module_display_name(const QString &module)
{
    const QString normalized = canonical_sync_module(module);
    if (normalized == QStringLiteral("eisenhower")) {
        return get_text(QStringLiteral("EisenPo"));
    }
    if (normalized == QStringLiteral("pomodoro")) {
        return get_text(QStringLiteral("Pomodoro"));
    }
    if (normalized == QStringLiteral("feynman")) {
        return get_text(QStringLiteral("Feynman"));
    }
    return module;
}

QString sync_module_display_list(const QStringList &modules)
{
    QStringList labels;
    for (const QString &module : modules) {
        labels.append(sync_module_display_name(module));
    }
    return labels.join(QStringLiteral(", "));
}

}  // namespace

void EisenhowerMatrixApp::refresh_module_task_counter()
{
    _attach_module_task_counter_to_menu_bar();

    if (module_task_counter_label == nullptr) {
        return;
    }

    QString module_name;
    QPair<int, int> counts = {0, 0};
    QWidget *current_page = eisenhower_module_page;
    if (current_page == eisenhower_module_page) {
        module_name = QStringLiteral("EisenPo");
        counts = _eisenhower_task_counts();
    } else if (current_page == feynman_module_page) {
        module_name = QStringLiteral("Feynman");
        counts = _feynman_task_counts();
    } else if (current_page == pomodoro_module_page) {
        module_name = QStringLiteral("Pomodoro");
        counts = _pomodoro_task_counts();
    } else if (current_page == mapamental_module_page) {
        module_name = mapamental_display_name();
        counts = _mapamental_task_counts();
    } else if (current_page == audiolivro_module_page) {
        module_name = audiolivro_display_name();
        counts = _audiolivro_task_counts();
    }

    module_task_counter_label->setText(module_name.isEmpty()
                                           ? QString()
                                           : module_task_counter_text(module_name, counts.first, counts.second));
}

bool EisenhowerMatrixApp::clear_pomodoro_tasks_if_available()
{
    if (!has_pomodoro_plugin() || pomodoro_module_page == nullptr) {
        return false;
    }

    bool cleared = false;
    const bool invoked = QMetaObject::invokeMethod(pomodoro_module_page,
                                                   "limpar_todas_as_tarefas",
                                                   Qt::DirectConnection,
                                                   Q_RETURN_ARG(bool, cleared));
    if (!invoked) {
        logger()->warning(QStringLiteral("Plugin Pomodoro carregado, mas não expôs limpeza total de tarefas."));
        return false;
    }

    return cleared;
}

bool EisenhowerMatrixApp::clear_feynman_tasks_if_available()
{
    if (!has_feynman_plugin() || feynman_module_page == nullptr) {
        return false;
    }

    bool cleared = false;
    const bool invoked = QMetaObject::invokeMethod(feynman_module_page,
                                                   "limpar_todas_as_tarefas",
                                                   Qt::DirectConnection,
                                                   Q_RETURN_ARG(bool, cleared));
    if (!invoked) {
        logger()->warning(QStringLiteral("Plugin Feynman carregado, mas não expôs limpeza total de tarefas."));
        return false;
    }

    return cleared;
}

void EisenhowerMatrixApp::reload_all_modules_from_shared_store()
{
    pending_runtime_sync_origins_.clear();
    _reload_eisenhower_from_shared_store();
    _reload_pomodoro_from_shared_store();
    _reload_feynman_from_shared_store();
    _refresh_plugin_task_views();
    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
        calendar_pane->calendar_panel->update_task_list();
    }
}

bool EisenhowerMatrixApp::has_active_task_sync(const QString &origin) const
{
    return active_sync_modules_for_origin(this, origin).size() > 1;
}

void EisenhowerMatrixApp::show_task_sync_add_info(const QString &origin)
{
    const QStringList modules = active_sync_modules_for_origin(this, origin);
    if (modules.size() <= 1) {
        return;
    }

    QMessageBox box(this);
    box.setIcon(QMessageBox::Information);
    box.setWindowTitle(get_text(QStringLiteral("Sincronização de Tarefas")));
    box.setText(get_text(QStringLiteral("A tarefa será adicionada aos métodos sincronizados: %1."))
                    .arg(sync_module_display_list(modules)));
    box.setStandardButtons(QMessageBox::Ok);
    if (gerenciador_traducao != nullptr) {
        gerenciador_traducao->traduzir_botoes_padrao(&box);
    }
    box.exec();
}

void EisenhowerMatrixApp::show_task_sync_delete_info(const QString &origin, QWidget *parent)
{
    const QStringList modules = active_sync_modules_for_origin(this, origin);
    if (modules.size() <= 1) {
        return;
    }

    QWidget *dialog_parent = parent ? parent : this;
    QMessageBox box(dialog_parent);
    box.setWindowModality(Qt::WindowModal);
    box.setIcon(QMessageBox::Information);
    box.setWindowTitle(get_text(QStringLiteral("Sincronização de Tarefas")));
    box.setText(get_text(QStringLiteral("A tarefa será excluída dos métodos sincronizados: %1."))
                    .arg(sync_module_display_list(modules)));
    box.setStandardButtons(QMessageBox::Ok);
    if (gerenciador_traducao != nullptr) {
        gerenciador_traducao->traduzir_botoes_padrao(&box);
    }
    box.exec();
}

void EisenhowerMatrixApp::remove_task_from_synced_modules(const QVariantMap &task, const QString &origin)
{
    const QStringList modules = active_sync_modules_for_origin(this, origin);
    if (modules.size() <= 1 || tasks_path.trimmed().isEmpty()) {
        return;
    }

    const QVariantList records = source::utils::UnifiedTaskStore::load_records(tasks_path);
    const QVariantList updated = source::utils::UnifiedTaskStore::remove_linked_records(records, task, modules);
    if (updated.size() != records.size()) {
        source::utils::UnifiedTaskStore::save_records(tasks_path, updated);
    }
}

bool EisenhowerMatrixApp::_send_task_payload_to_feynman(const QVariantMap &task, bool activate_tab)
{
    if (!has_feynman_plugin() || feynman_plugin_ == nullptr) {
        return false;
    }

    QVariantMap payload = task;
    if (payload.value(QStringLiteral("origin")).toString().trimmed().isEmpty()) {
        payload.insert(QStringLiteral("origin"), QStringLiteral("eisenhower"));
    }

    feynman_plugin_->receive_task(payload);

    if (activate_tab) {
        open_eisenhower();
    }

    return true;
}

bool EisenhowerMatrixApp::send_task_to_feynman(QListWidgetItem *item, QListWidget *source_list, bool activate_tab)
{
    if (!has_feynman_plugin() || item == nullptr || !(item->flags() & Qt::ItemIsSelectable)) {
        return false;
    }

    QVariantMap data = item->data(Qt::UserRole).toMap();
    QString task_id = data.value(QStringLiteral("id")).toString().trimmed();
    if (task_id.isEmpty()) {
        task_id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        data.insert(QStringLiteral("id"), task_id);
        item->setData(Qt::UserRole, data);
    }

    QVariantMap payload;
    payload.insert(QStringLiteral("integration_id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    payload.insert(QStringLiteral("origin"), QStringLiteral("eisenhower"));
    payload.insert(QStringLiteral("id"), task_id);
    payload.insert(QStringLiteral("titulo"), base_task_text(item));
    payload.insert(QStringLiteral("prioridade"), priority_label_without_emoji(data.value(QStringLiteral("priority")), this));
    if (data.contains(QStringLiteral("dominio"))) {
        payload.insert(QStringLiteral("dominio"), normalized_domain_value(data.value(QStringLiteral("dominio"))));
    }
    payload.insert(QStringLiteral("description"), data.value(QStringLiteral("description")).toString());
    payload.insert(QStringLiteral("file_path"), data.value(QStringLiteral("file_path")).toString());
    payload.insert(QStringLiteral("date"), data.value(QStringLiteral("date")).toString());
    payload.insert(QStringLiteral("time"), data.value(QStringLiteral("time")).toString());
    payload.insert(QStringLiteral("cor_texto"), data.value(QStringLiteral("cor_texto")).toString());
    payload.insert(QStringLiteral("cor_texto_original"), data.value(QStringLiteral("cor_texto_original")).toString());
    payload.insert(QStringLiteral("cor_texto_customizada"), data.value(QStringLiteral("cor_texto_customizada")).toString());
    payload.insert(QStringLiteral("completed"), item->checkState() == Qt::Checked);
    payload.insert(QStringLiteral("status"),
                   (source_list != nullptr ? is_completed_list(source_list) : item->checkState() == Qt::Checked)
                       ? QStringLiteral("Done")
                       : QStringLiteral("Todo"));

    const bool sent = _send_task_payload_to_feynman(payload, activate_tab);
    if (sent) {
        save_tasks();
    }

    return sent;
}

bool EisenhowerMatrixApp::send_task_to_pomodoro(QListWidgetItem *item, QListWidget *source_list, bool activate_tab)
{
    if (!has_pomodoro_plugin() || item == nullptr || !(item->flags() & Qt::ItemIsSelectable)) {
        return false;
    }

    QVariantMap data = item->data(Qt::UserRole).toMap();
    QString task_id = data.value(QStringLiteral("id")).toString().trimmed();
    if (task_id.isEmpty()) {
        task_id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        data.insert(QStringLiteral("id"), task_id);
        item->setData(Qt::UserRole, data);
    }

    QVariantMap payload;
    payload.insert(QStringLiteral("integration_id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    payload.insert(QStringLiteral("origin"), QStringLiteral("eisenhower"));
    payload.insert(QStringLiteral("id"), task_id);
    payload.insert(QStringLiteral("titulo"), base_task_text(item));
    payload.insert(QStringLiteral("prioridade"), priority_label_without_emoji(data.value(QStringLiteral("priority")), this));
    if (data.contains(QStringLiteral("dominio"))) {
        payload.insert(QStringLiteral("dominio"), normalized_domain_value(data.value(QStringLiteral("dominio"))));
    }
    payload.insert(QStringLiteral("description"), data.value(QStringLiteral("description")).toString());
    payload.insert(QStringLiteral("file_path"), data.value(QStringLiteral("file_path")).toString());
    payload.insert(QStringLiteral("date"), data.value(QStringLiteral("date")).toString());
    payload.insert(QStringLiteral("time"), data.value(QStringLiteral("time")).toString());
    payload.insert(QStringLiteral("cor_texto"), data.value(QStringLiteral("cor_texto")).toString());
    payload.insert(QStringLiteral("cor_texto_original"), data.value(QStringLiteral("cor_texto_original")).toString());
    payload.insert(QStringLiteral("cor_texto_customizada"), data.value(QStringLiteral("cor_texto_customizada")).toString());
    const int target_cycles_pomo = data.value(QStringLiteral("ciclos_totais"),
                                      data.value(QStringLiteral("ciclos_permitidos"),
                                          (pomodoro_host_bridge_ ? pomodoro_host_bridge_->configured_cycles() : 4))).toInt();
    payload.insert(QStringLiteral("ciclos_totais"), target_cycles_pomo > 0 ? target_cycles_pomo : 4);
    payload.insert(QStringLiteral("ciclos_permitidos"), target_cycles_pomo > 0 ? target_cycles_pomo : 4);
    payload.insert(QStringLiteral("completed"), item->checkState() == Qt::Checked);
    payload.insert(QStringLiteral("status"),
                   (source_list != nullptr ? is_completed_list(source_list) : item->checkState() == Qt::Checked)
                       ? QStringLiteral("Done")
                       : QStringLiteral("Todo"));

    pomodoro_plugin_->receive_eisenhower_task(payload);
    save_tasks();

    if (activate_tab) {
        open_pomodoro();
    }

    return true;
}

bool EisenhowerMatrixApp::_upsert_task_from_external(const QVariantMap &task)
{
    const QString origin = task.value(QStringLiteral("origin")).toString().trimmed().toCaseFolded();
    if (origin.isEmpty() || origin == QStringLiteral("eisenhower")) {
        return false;
    }

    const QString title = task.value(QStringLiteral("titulo")).toString().trimmed();
    if (title.isEmpty()) {
        return false;
    }

    QListWidget *existing_list = nullptr;
    QListWidgetItem *existing_item = nullptr;
    const bool sync_only = task.value(QStringLiteral("sync_only")).toBool();
    for (QListWidget *list : all_task_lists(this)) {
        if (list == nullptr) {
            continue;
        }

        for (int i = 0; i < list->count(); ++i) {
            QListWidgetItem *candidate = list->item(i);
            if (candidate == nullptr || !(candidate->flags() & Qt::ItemIsSelectable)) {
                continue;
            }

            const bool matched =
                sync_only ? task_matches_sync_payload(candidate, task) : task_matches_payload(candidate, task);
            if (matched) {
                existing_list = list;
                existing_item = candidate;
                break;
            }
        }

        if (existing_item != nullptr) {
            break;
        }
    }

    if (existing_item == nullptr && sync_only) {
        return false;
    }

    const int quadrant = quadrant_from_priority(task.value(QStringLiteral("prioridade")), this);
    const bool completed = payload_completed(task);
    QListWidget *target_list = quadrant_list(quadrant, completed);
    if (target_list == nullptr) {
        return false;
    }

    QVariant preserved_insertion_order;
    QVariant preserved_domain;
    if (existing_item != nullptr) {
        const QVariantMap existing_data = existing_item->data(Qt::UserRole).toMap();
        preserved_insertion_order = existing_data.value(QStringLiteral("insertion_order"));
        preserved_domain = existing_data.value(QStringLiteral("dominio"));
    }

    if (existing_list != nullptr && existing_item != nullptr) {
        const int row = existing_list->row(existing_item);
        if (row >= 0) {
            delete existing_list->takeItem(row);
            cleanup_time_groups(existing_list);
        }
    }

    if (target_list->count() == 1 && !(target_list->item(0)->flags() & Qt::ItemIsSelectable)) {
        target_list->clear();
    }

    QVariantMap normalized_task = task;
    if (!normalized_task.contains(QStringLiteral("dominio")) && preserved_domain.isValid()
        && !preserved_domain.toString().trimmed().isEmpty()) {
        normalized_task.insert(QStringLiteral("dominio"), preserved_domain);
    }

    const int pomodoros = task.value(QStringLiteral("pomodoros_completados"), task.value(QStringLiteral("pomodoros"), 0)).toInt();
    const auto [display_text, tooltip] = display_and_tooltip_for_task(this, normalized_task, quadrant);
    auto *new_item = new QListWidgetItem(display_text);
    new_item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    new_item->setFlags(new_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    new_item->setCheckState(completed ? Qt::Checked : Qt::Unchecked);

    QVariantMap data;
    data.insert(QStringLiteral("id"), task.value(QStringLiteral("id")).toString().trimmed());
    data.insert(QStringLiteral("text"), title);
    data.insert(QStringLiteral("date"), task.value(QStringLiteral("date")).toString().trimmed());
    data.insert(QStringLiteral("time"), task.value(QStringLiteral("time")).toString().trimmed());
    data.insert(QStringLiteral("priority"), quadrant);
    data.insert(QStringLiteral("integracao_origem"), origin);
    data.insert(QStringLiteral("pomodoros"), pomodoros);
    data.insert(QStringLiteral("pomodoros_completados"), pomodoros);
    const int target_cycles_upsert = task.value(QStringLiteral("ciclos_totais"),
                                         task.value(QStringLiteral("ciclos_permitidos"),
                                             (pomodoro_host_bridge_ ? pomodoro_host_bridge_->configured_cycles() : 4))).toInt();
    data.insert(QStringLiteral("ciclos_totais"), target_cycles_upsert > 0 ? target_cycles_upsert : 4);
    data.insert(QStringLiteral("ciclos_permitidos"), target_cycles_upsert > 0 ? target_cycles_upsert : 4);
    const double dur_val = task.contains(QStringLiteral("duracao"))
                               ? task.value(QStringLiteral("duracao")).toDouble()
                               : (task.contains(QStringLiteral("duration"))
                                      ? task.value(QStringLiteral("duration")).toDouble()
                                      : 1.0);
    data.insert(QStringLiteral("duracao"), dur_val > 0.0 ? dur_val : 1.0);
    if (normalized_task.contains(QStringLiteral("dominio"))) {
        data.insert(QStringLiteral("dominio"), normalized_domain_value(normalized_task.value(QStringLiteral("dominio"))));
    }
    if (preserved_insertion_order.isValid()) {
        data.insert(QStringLiteral("insertion_order"), preserved_insertion_order);
    }
    if (!task.value(QStringLiteral("file_path")).toString().trimmed().isEmpty()) {
        data.insert(QStringLiteral("file_path"), task.value(QStringLiteral("file_path")).toString().trimmed());
    }
    if (!task.value(QStringLiteral("description")).toString().trimmed().isEmpty()) {
        data.insert(QStringLiteral("description"), task.value(QStringLiteral("description")).toString().trimmed());
    }
    if (!task.value(QStringLiteral("cor_texto"), task.value(QStringLiteral("task_color"))).toString().trimmed().isEmpty()) {
        data.insert(QStringLiteral("cor_texto"),
                    task.value(QStringLiteral("cor_texto"), task.value(QStringLiteral("task_color"))).toString().trimmed());
    }
    if (!task.value(QStringLiteral("cor_texto_original"), task.value(QStringLiteral("agenda_cor_texto")))
             .toString()
             .trimmed()
             .isEmpty()) {
        data.insert(QStringLiteral("cor_texto_original"),
                    task.value(QStringLiteral("cor_texto_original"), task.value(QStringLiteral("agenda_cor_texto")))
                        .toString()
                        .trimmed());
    }
    if (!task.value(QStringLiteral("cor_texto_customizada")).toString().trimmed().isEmpty()) {
        data.insert(QStringLiteral("cor_texto_customizada"), task.value(QStringLiteral("cor_texto_customizada")).toString().trimmed());
    }
    new_item->setData(Qt::UserRole, data);
    new_item->setToolTip(tooltip);
    _apply_task_item_appearance(new_item);

    insert_task_into_quadrant_list(target_list, new_item);
    cleanup_time_groups(target_list);
    pending_shared_origin_ = origin;
    save_tasks();
    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
        calendar_pane->calendar_panel->update_task_list();
    }
    return true;
}

}  // namespace source
