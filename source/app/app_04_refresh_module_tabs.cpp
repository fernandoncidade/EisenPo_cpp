#include "source/app/app_04_refresh_module_tabs.hpp"
#include "source/app/EisenhowerGanttHostAdapter.hpp"

namespace source {
using namespace app;

void EisenhowerMatrixApp::_sync_pomodoro_host_bridge()
{
    if (pomodoro_host_bridge_ == nullptr || gerenciador_traducao == nullptr) {
        return;
    }

    pomodoro_host_bridge_->set_language_code(gerenciador_traducao->obter_idioma_atual());
    pomodoro_host_bridge_->set_clear_external_selections_handler([this]() {
        this->clear_all_task_selections_except(nullptr);
    });
}

void EisenhowerMatrixApp::_sync_feynman_host_bridge()
{
    if (feynman_host_bridge_ == nullptr || gerenciador_traducao == nullptr) {
        return;
    }

    feynman_host_bridge_->set_language_code(gerenciador_traducao->obter_idioma_atual());
}

void EisenhowerMatrixApp::_sync_agenda_host_bridge()
{
    if (agenda_host_bridge_ == nullptr || gerenciador_traducao == nullptr) {
        return;
    }

    agenda_host_bridge_->set_language_code(gerenciador_traducao->obter_idioma_atual());
    agenda_host_bridge_->set_persistent_dir(source::utils::obter_caminho_persistente());
}

void EisenhowerMatrixApp::_sync_gantt_host_bridge()
{
    if (gantt_host_bridge_ == nullptr) {
        gantt_host_bridge_ = new source::plugin::GanttHostBridge();
    }
    if (gantt_host_adapter_) {
        gantt_host_bridge_->set_eisenhower_host(gantt_host_adapter_.get());
    }
    if (gerenciador_traducao != nullptr) {
        gantt_host_bridge_->set_language_code(gerenciador_traducao->obter_idioma_atual());
    }
    gantt_host_bridge_->set_persistent_dir(source::utils::obter_caminho_persistente());
}



void EisenhowerMatrixApp::_connect_task_counter_list(QListWidget *list)
{
    if (list == nullptr) {
        return;
    }

    QObject::connect(list, &QListWidget::itemSelectionChanged, this, &EisenhowerMatrixApp::refresh_module_task_counter);
    if (QAbstractItemModel *model = list->model(); model != nullptr) {
        QObject::connect(model, &QAbstractItemModel::modelReset, this, &EisenhowerMatrixApp::refresh_module_task_counter);
        QObject::connect(model,
                         &QAbstractItemModel::rowsInserted,
                         this,
                         [this](const QModelIndex &, int, int) { refresh_module_task_counter(); });
        QObject::connect(model,
                         &QAbstractItemModel::rowsRemoved,
                         this,
                         [this](const QModelIndex &, int, int) { refresh_module_task_counter(); });
    }
}

void EisenhowerMatrixApp::_connect_feynman_task_counter_sources()
{
    if (feynman_module_page == nullptr) {
        return;
    }

    QObject::connect(feynman_module_page, SIGNAL(task_counter_changed()), this, SLOT(refresh_module_task_counter()));
}

void EisenhowerMatrixApp::_connect_pomodoro_task_counter_sources()
{
    if (pomodoro_module_page == nullptr) {
        return;
    }

    QObject::connect(pomodoro_module_page, SIGNAL(task_counter_changed()), this, SLOT(refresh_module_task_counter()));
}

void EisenhowerMatrixApp::_connect_mapamental_task_counter_sources()
{
    if (mapamental_module_page == nullptr) {
        return;
    }

    QObject::connect(mapamental_module_page, SIGNAL(task_counter_changed()), this, SLOT(refresh_module_task_counter()));
}

void EisenhowerMatrixApp::_connect_audiolivro_task_counter_sources()
{
    if (audiolivro_module_page == nullptr) {
        return;
    }

    QObject::connect(audiolivro_module_page, SIGNAL(task_counter_changed()), this, SLOT(refresh_module_task_counter()));
}

void EisenhowerMatrixApp::clear_pomodoro_selections()
{
    if (pomodoro_module_page != nullptr) {
        QMetaObject::invokeMethod(pomodoro_module_page, "clear_all_selections", Qt::DirectConnection);
    }
}

void EisenhowerMatrixApp::clear_all_task_selections_except(const QListWidget *except_list)
{
    const QList<QListWidget *> task_lists = {quadrant1_list,
                                             quadrant2_list,
                                             quadrant3_list,
                                             quadrant4_list,
                                             quadrant1_in_progress_list,
                                             quadrant2_in_progress_list,
                                             quadrant3_in_progress_list,
                                             quadrant4_in_progress_list,
                                             quadrant1_completed_list,
                                             quadrant2_completed_list,
                                             quadrant3_completed_list,
                                             quadrant4_completed_list,
                                             eisenhower_doing_list,
                                             calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr
                                                 ? calendar_pane->calendar_panel->tasks_list
                                                 : nullptr};

    for (QListWidget *list : task_lists) {
        if (list == nullptr || list == except_list) {
            continue;
        }

        if (!list->selectedItems().isEmpty() || list->currentItem() != nullptr) {
            if (QItemSelectionModel *selection_model = list->selectionModel(); selection_model != nullptr) {
                selection_model->clear();
            } else {
                list->clearSelection();
            }
            list->setCurrentItem(nullptr);
        }
    }

    if (except_list != nullptr) {
        clear_pomodoro_selections();
    }
}

QPair<int, int> EisenhowerMatrixApp::_eisenhower_task_counts() const
{
    QSet<QString> selected_keys;
    int total_count = 0;

    for (const QListWidget *list : {static_cast<const QListWidget *>(quadrant1_list),
                                    static_cast<const QListWidget *>(quadrant2_list),
                                    static_cast<const QListWidget *>(quadrant3_list),
                                    static_cast<const QListWidget *>(quadrant4_list),
                                    static_cast<const QListWidget *>(quadrant1_in_progress_list),
                                    static_cast<const QListWidget *>(quadrant2_in_progress_list),
                                    static_cast<const QListWidget *>(quadrant3_in_progress_list),
                                    static_cast<const QListWidget *>(quadrant4_in_progress_list),
                                    static_cast<const QListWidget *>(quadrant1_completed_list),
                                    static_cast<const QListWidget *>(quadrant2_completed_list),
                                    static_cast<const QListWidget *>(quadrant3_completed_list),
                                    static_cast<const QListWidget *>(quadrant4_completed_list)}) {
        collect_selected_task_keys(list, selected_keys);
        total_count += count_selectable_items(list);
    }

    if (eisenhower_doing_list != nullptr) {
        collect_selected_task_keys(eisenhower_doing_list, selected_keys);
    }

    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
        collect_selected_task_keys(calendar_pane->calendar_panel->tasks_list, selected_keys);
    }

    return {selected_keys.size(), total_count};
}

QPair<int, int> EisenhowerMatrixApp::_feynman_task_counts() const
{
    if (feynman_module_page == nullptr) {
        return {0, 0};
    }

    int selected_count = 0;
    int total_count = 0;
    QMetaObject::invokeMethod(feynman_module_page,
                              "selected_task_count",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(int, selected_count));
    QMetaObject::invokeMethod(feynman_module_page, "total_task_count", Qt::DirectConnection, Q_RETURN_ARG(int, total_count));
    return {selected_count, total_count};
}

QPair<int, int> EisenhowerMatrixApp::_pomodoro_task_counts() const
{
    if (pomodoro_module_page == nullptr) {
        return {0, 0};
    }

    int selected_count = 0;
    int total_count = 0;
    QMetaObject::invokeMethod(pomodoro_module_page,
                              "selected_task_count",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(int, selected_count));
    QMetaObject::invokeMethod(pomodoro_module_page, "total_task_count", Qt::DirectConnection, Q_RETURN_ARG(int, total_count));
    return {selected_count, total_count};
}

QPair<int, int> EisenhowerMatrixApp::_mapamental_task_counts() const
{
    if (mapamental_module_page == nullptr) {
        return {0, 0};
    }

    int selected_count = 0;
    int total_count = 0;
    QMetaObject::invokeMethod(mapamental_module_page,
                              "selected_task_count",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(int, selected_count));
    QMetaObject::invokeMethod(mapamental_module_page,
                              "total_task_count",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(int, total_count));
    return {selected_count, total_count};
}

QPair<int, int> EisenhowerMatrixApp::_audiolivro_task_counts() const
{
    if (audiolivro_module_page == nullptr) {
        return {0, 0};
    }

    int selected_count = 0;
    int total_count = 0;
    QMetaObject::invokeMethod(audiolivro_module_page,
                              "selected_task_count",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(int, selected_count));
    QMetaObject::invokeMethod(audiolivro_module_page,
                              "total_task_count",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(int, total_count));
    return {selected_count, total_count};
}

}  // namespace source
