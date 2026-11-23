#include "source/app/app_07_set_task_layout.hpp"

#include <QGridLayout>
#include <QScrollArea>
#include <QVBoxLayout>

namespace source {
using namespace app;

namespace {

constexpr int kSplitTaskPanelBottomInset = 0;
constexpr int kNativeTaskPanelBottomInset = 4;
constexpr int kMatrixTaskPanelBottomInset = 4;
constexpr int kMatrixStatusPageBottomInset = 0;
constexpr int kQuadrantColumnsGap = 6;

QString quadrant_title(source::EisenhowerMatrixApp *app, int index)
{
    const QList<QLabel *> labels = {
        app != nullptr ? app->quadrant1_label : nullptr,
        app != nullptr ? app->quadrant2_label : nullptr,
        app != nullptr ? app->quadrant3_label : nullptr,
        app != nullptr ? app->quadrant4_label : nullptr,
    };
    const QList<QString> fallback = {
        get_text(QStringLiteral("🔴 Importante e Urgente")),
        get_text(QStringLiteral("🟠 Importante, mas Não Urgente")),
        get_text(QStringLiteral("🟡 Não Importante, mas Urgente")),
        get_text(QStringLiteral("🟢 Não Importante e Não Urgente")),
    };

    if (index >= 0 && index < labels.size() && labels.at(index) != nullptr) {
        return labels.at(index)->text();
    }
    return fallback.value(index);
}

QString pending_title(source::EisenhowerMatrixApp *app, int index)
{
    Q_UNUSED(app);
    const QList<QString> fallback = {
        get_text(QStringLiteral("🔴 Pendentes")),
        get_text(QStringLiteral("🟠 Pendentes")),
        get_text(QStringLiteral("🟡 Pendentes")),
        get_text(QStringLiteral("🟢 Pendentes")),
    };
    return fallback.value(index);
}

QString completed_title(source::EisenhowerMatrixApp *app, int index)
{
    const QList<QLabel *> labels = {
        app != nullptr ? app->quadrant1_completed_label : nullptr,
        app != nullptr ? app->quadrant2_completed_label : nullptr,
        app != nullptr ? app->quadrant3_completed_label : nullptr,
        app != nullptr ? app->quadrant4_completed_label : nullptr,
    };
    const QList<QString> fallback = {
        get_text(QStringLiteral("🔴 Concluídas")),
        get_text(QStringLiteral("🟠 Concluídas")),
        get_text(QStringLiteral("🟡 Concluídas")),
        get_text(QStringLiteral("🟢 Concluídas")),
    };

    if (index >= 0 && index < labels.size() && labels.at(index) != nullptr) {
        return labels.at(index)->text();
    }
    return fallback.value(index);
}

void set_group_layout_bottom_inset(QGroupBox *group, int bottom_inset)
{
    if (group == nullptr || group->layout() == nullptr) {
        return;
    }

    QMargins margins = group->layout()->contentsMargins();
    margins.setBottom(bottom_inset);
    group->layout()->setContentsMargins(margins);
}

void prepare_split_group(QGroupBox *group,
                         QListWidget *list,
                         const QString &title,
                         bool use_native_frame,
                         bool horizontal_layout = false)
{
    if (group == nullptr) {
        return;
    }

    group->setTitle(title);
    group->setCheckable(true);
    group->setFlat(!use_native_frame);
    set_group_layout_bottom_inset(group, use_native_frame ? kNativeTaskPanelBottomInset : kSplitTaskPanelBottomInset);
    QSizePolicy policy = group->sizePolicy();
    if (horizontal_layout) {
        policy.setHorizontalPolicy(group->isChecked() ? QSizePolicy::Expanding : QSizePolicy::Fixed);
        policy.setVerticalPolicy(QSizePolicy::Expanding);
        if (!group->isChecked()) {
            const int collapsed_w = group->minimumSizeHint().width();
            group->setMinimumWidth(collapsed_w);
            group->setMaximumWidth(collapsed_w);
            group->setCursor(Qt::PointingHandCursor);
        } else {
            group->setMinimumWidth(0);
            group->setMaximumWidth(QWIDGETSIZE_MAX);
            group->unsetCursor();
        }
    } else {
        policy.setHorizontalPolicy(QSizePolicy::Expanding);
        policy.setVerticalPolicy(group->isChecked() ? QSizePolicy::Expanding : QSizePolicy::Fixed);
        group->setMinimumWidth(0);
        group->setMaximumWidth(QWIDGETSIZE_MAX);
        group->unsetCursor();
    }
    group->setSizePolicy(policy);
    group->show();
    if (list != nullptr) {
        list->setVisible(group->isChecked());
    }
    if (group->layout() != nullptr) {
        group->layout()->invalidate();
        group->layout()->activate();
    }
    group->updateGeometry();
    group->update();
}

void prepare_matrix_group(QGroupBox *group, QListWidget *list, const QString &title)
{
    if (group == nullptr) {
        return;
    }

    group->setTitle(title);
    group->setChecked(true);
    group->setCheckable(false);
    group->setFlat(false);
    set_group_layout_bottom_inset(group, kMatrixTaskPanelBottomInset);
    group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    group->show();
    if (list != nullptr) {
        list->setVisible(true);
        list->show();
    }
    if (group->layout() != nullptr) {
        group->layout()->invalidate();
        group->layout()->activate();
    }
    group->updateGeometry();
    group->update();
}

bool attach_split_panel(QWidget *panel,
                        QGroupBox *pending_group,
                        QListWidget *pending_list,
                        QGroupBox *in_progress_group,
                        QListWidget *in_progress_list,
                        QGroupBox *completed_group,
                        QListWidget *completed_list,
                        const QString &pending_title,
                        const QString &in_progress_title,
                        const QString &completed_title_text,
                        bool use_native_frame,
                        bool horizontal_layout = false)
{
    auto *layout = panel != nullptr ? dynamic_cast<QBoxLayout *>(panel->layout()) : nullptr;
    if (layout == nullptr || pending_group == nullptr || in_progress_group == nullptr || completed_group == nullptr) {
        return false;
    }

    clear_layout(layout);
    layout->setDirection(horizontal_layout ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
    layout->setSpacing(horizontal_layout ? 8 : 6);
    pending_group->setParent(panel);
    in_progress_group->setParent(panel);
    completed_group->setParent(panel);
    prepare_split_group(pending_group, pending_list, pending_title, use_native_frame, horizontal_layout);
    prepare_split_group(in_progress_group, in_progress_list, in_progress_title, use_native_frame, horizontal_layout);
    prepare_split_group(completed_group, completed_list, completed_title_text, use_native_frame, horizontal_layout);
    layout->addWidget(pending_group, 1);
    layout->addWidget(in_progress_group, 1);
    layout->addWidget(completed_group, 1);
    layout->addStretch(1);
    const bool pending_visible = pending_group->isChecked();
    const bool in_progress_visible = in_progress_group->isChecked();
    const bool completed_visible = completed_group->isChecked();
    const bool any_visible = pending_visible || in_progress_visible || completed_visible;
    layout->setStretch(0, pending_visible ? 1 : 0);
    layout->setStretch(1, in_progress_visible ? 1 : 0);
    layout->setStretch(2, completed_visible ? 1 : 0);
    layout->setStretch(3, any_visible ? 0 : 1);
    layout->invalidate();
    layout->activate();
    panel->setMinimumWidth(0);
    panel->setMaximumWidth(QWIDGETSIZE_MAX);
    panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    panel->show();
    panel->updateGeometry();
    panel->update();
    return true;
}

void clear_status_tabs(QTabWidget *tabs, QGroupBox *pending_group, QGroupBox *in_progress_group, QGroupBox *completed_group)
{
    if (tabs == nullptr) {
        return;
    }

    while (tabs->count() > 0) {
        QWidget *page = tabs->widget(0);
        tabs->removeTab(0);
        if (page == nullptr || page == pending_group || page == in_progress_group || page == completed_group) {
            continue;
        }
        if (pending_group != nullptr && pending_group->parentWidget() == page) {
            pending_group->setParent(nullptr);
        }
        if (in_progress_group != nullptr && in_progress_group->parentWidget() == page) {
            in_progress_group->setParent(nullptr);
        }
        if (completed_group != nullptr && completed_group->parentWidget() == page) {
            completed_group->setParent(nullptr);
        }
        page->deleteLater();
    }
}

void clear_matrix_status_tabs(source::EisenhowerMatrixApp *app)
{
    if (app == nullptr) {
        return;
    }

    clear_status_tabs(app->quadrant1_status_tabs, app->quadrant1_group, app->quadrant1_in_progress_group, app->quadrant1_completed_group);
    clear_status_tabs(app->quadrant2_status_tabs, app->quadrant2_group, app->quadrant2_in_progress_group, app->quadrant2_completed_group);
    clear_status_tabs(app->quadrant3_status_tabs, app->quadrant3_group, app->quadrant3_in_progress_group, app->quadrant3_completed_group);
    clear_status_tabs(app->quadrant4_status_tabs, app->quadrant4_group, app->quadrant4_in_progress_group, app->quadrant4_completed_group);
}

QWidget *matrix_status_page(QTabWidget *tabs, QGroupBox *group)
{
    if (tabs == nullptr || group == nullptr) {
        return group;
    }

    auto *page = new QWidget(tabs);
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, kMatrixStatusPageBottomInset);
    layout->setSpacing(0);
    group->setParent(page);
    layout->addWidget(group, 1);
    layout->activate();
    page->updateGeometry();
    page->update();
    return page;
}

bool attach_status_tabs(QTabWidget *tabs,
                        QGroupBox *pending_group,
                        QListWidget *pending_list,
                        QGroupBox *in_progress_group,
                        QListWidget *in_progress_list,
                        QGroupBox *completed_group,
                        QListWidget *completed_list,
                        const QString &quadrant_title_text)
{
    if (tabs == nullptr || pending_group == nullptr || in_progress_group == nullptr || completed_group == nullptr) {
        return false;
    }

    clear_status_tabs(tabs, pending_group, in_progress_group, completed_group);

    prepare_matrix_group(pending_group, pending_list, quadrant_title_text);
    prepare_matrix_group(in_progress_group, in_progress_list, quadrant_title_text);
    prepare_matrix_group(completed_group, completed_list, quadrant_title_text);
    tabs->addTab(matrix_status_page(tabs, pending_group), get_text(QStringLiteral("Pendente")));
    tabs->addTab(matrix_status_page(tabs, in_progress_group), get_text(QStringLiteral("Em Progresso")));
    tabs->addTab(matrix_status_page(tabs, completed_group), get_text(QStringLiteral("Concluídas")));
    tabs->setCurrentIndex(0);
    tabs->show();
    if (tabs->layout() != nullptr) {
        tabs->layout()->invalidate();
        tabs->layout()->activate();
    }
    tabs->updateGeometry();
    tabs->update();
    return true;
}

}  // namespace

bool EisenhowerMatrixApp::set_task_layout(TaskLayoutMode mode, bool persist)
{
    const TaskLayoutMode previous_mode = task_layout_mode_;
    task_layout_mode_ = mode;
    if (!_apply_task_layout()) {
        task_layout_mode_ = previous_mode;
        _apply_task_layout();
        return false;
    }

    if (persist) {
        _save_task_layout_preference();
    }

    return true;
}

EisenhowerMatrixApp::TaskLayoutMode EisenhowerMatrixApp::current_task_layout() const
{
    return task_layout_mode_;
}

bool EisenhowerMatrixApp::set_task_sort_mode(TaskSortMode mode, bool persist)
{
    if (mode == TaskSortMode::None) {
        task_sort_criteria_.clear();
    } else {
        const TaskSortCriterion criterion = task_sort_criterion_from_mode(mode);
        const Qt::SortOrder order = task_sort_order_from_mode(mode);
        const int existing_index = _task_sort_criterion_index(criterion);

        if (existing_index >= 0) {
            task_sort_criteria_[existing_index].order = order;
        } else {
            task_sort_criteria_.append({criterion, order});
        }
    }

    for (QListWidget *list : all_task_lists(this)) {
        _sort_task_list(list);
    }

    save_tasks();
    if (persist) {
        _save_task_layout_preference();
    }

    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
        calendar_pane->calendar_panel->update_task_list();
    }

    atualizar_eisenhower_coluna_doing_timer();
    _refresh_plugin_task_views();

    return true;
}

bool EisenhowerMatrixApp::clear_task_sort_criterion(TaskSortCriterion criterion, bool persist)
{
    const int existing_index = _task_sort_criterion_index(criterion);
    if (existing_index < 0) {
        return true;
    }

    task_sort_criteria_.removeAt(existing_index);

    for (QListWidget *list : all_task_lists(this)) {
        _sort_task_list(list);
    }

    save_tasks();
    if (persist) {
        _save_task_layout_preference();
    }

    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
        calendar_pane->calendar_panel->update_task_list();
    }

    atualizar_eisenhower_coluna_doing_timer();
    _refresh_plugin_task_views();

    return true;
}

bool EisenhowerMatrixApp::has_task_sort_criterion(TaskSortCriterion criterion) const
{
    return _task_sort_criterion_index(criterion) >= 0;
}

Qt::SortOrder EisenhowerMatrixApp::task_sort_order(TaskSortCriterion criterion) const
{
    const int index = _task_sort_criterion_index(criterion);
    return index >= 0 ? task_sort_criteria_.at(index).order : Qt::AscendingOrder;
}

int EisenhowerMatrixApp::task_sort_priority(TaskSortCriterion criterion) const
{
    return _task_sort_criterion_index(criterion);
}

bool EisenhowerMatrixApp::has_active_task_sort() const
{
    return !task_sort_criteria_.isEmpty();
}

bool EisenhowerMatrixApp::task_sort_less_for_payloads(const QVariantMap &left, const QVariantMap &right) const
{
    return _task_sort_less(normalized_task_sort_payload(left), normalized_task_sort_payload(right));
}

int EisenhowerMatrixApp::_task_sort_criterion_index(TaskSortCriterion criterion) const
{
    for (int i = 0; i < task_sort_criteria_.size(); ++i) {
        if (task_sort_criteria_.at(i).criterion == criterion) {
            return i;
        }
    }
    return -1;
}

void EisenhowerMatrixApp::_ensure_task_metadata(QVariantMap &data)
{
    QString task_id = data.value(QStringLiteral("id")).toString().trimmed();
    if (task_id.isEmpty()) {
        task_id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        data.insert(QStringLiteral("id"), task_id);
    }

    bool ok = false;
    const qint64 insertion_order = data.value(QStringLiteral("insertion_order")).toLongLong(&ok);
    if (!ok || insertion_order <= 0) {
        data.insert(QStringLiteral("insertion_order"), ++task_insertion_counter_);
    } else if (insertion_order > task_insertion_counter_) {
        task_insertion_counter_ = insertion_order;
    }

    data.insert(QStringLiteral("integracao_origem"), QStringLiteral("eisenhower"));
}

qint64 EisenhowerMatrixApp::_task_insertion_order(const QVariantMap &data) const
{
    bool ok = false;
    const qint64 insertion_order = data.value(QStringLiteral("insertion_order")).toLongLong(&ok);
    return (ok && insertion_order > 0) ? insertion_order : std::numeric_limits<qint64>::max();
}

void EisenhowerMatrixApp::_load_task_layout_preference()
{
    task_layout_mode_ = TaskLayoutMode::Tabs;
    task_sort_criteria_.clear();
    calendar_start_expanded = false;
    feynman_task_panel_start_expanded = false;
    pomodoro_in_progress_panel_start_expanded = false;
    eisenhower_pomodoro_expanded = true;
    sync_eisenhower_enabled_ = true;
    sync_pomodoro_enabled_ = true;
    sync_feynman_enabled_ = true;
    reserved_space_eisenhower_enabled_ = true;
    reserved_space_pomodoro_enabled_ = false;
    reserved_space_feynman_enabled_ = false;
    reserved_watermark_eisenhower_enabled_ = true;
    reserved_watermark_pomodoro_enabled_ = false;
    reserved_watermark_feynman_enabled_ = false;
    feynman_ui_layout_mode_key_ = QStringLiteral("tabs");
    pomodoro_ui_layout_mode_key_ = QStringLiteral("simple:tabs");


    QFile file(layout_path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return;
    }

    const QString layout_value = doc.object().value(QStringLiteral("layout")).toString().trimmed().toLower();
    if (layout_value == QStringLiteral("quadrants")) {
        task_layout_mode_ = TaskLayoutMode::Quadrants;
    } else if (layout_value == QStringLiteral("matrix_2x2")) {
        task_layout_mode_ = TaskLayoutMode::Matrix2x2;
    } else if (layout_value == QStringLiteral("tabs_horizontal") || layout_value == QStringLiteral("horizontal")) {
        task_layout_mode_ = TaskLayoutMode::TabsHorizontal;
    }
    pomodoro_ui_layout_mode_key_ =
        normalized_pomodoro_ui_layout_key(doc.object().value(QStringLiteral("pomodoro_ui_layout")).toString(pomodoro_ui_layout_mode_key_));
    feynman_ui_layout_mode_key_ =
        normalized_feynman_ui_layout_key(doc.object().value(QStringLiteral("feynman_ui_layout")).toString());

    pomodoro_report_pomodoros_eisenhower_enabled_ =
        doc.object().value(QStringLiteral("report_pomodoros_eisenhower")).toBool(pomodoro_report_pomodoros_eisenhower_enabled_);
    pomodoro_report_pomodoros_pomodoro_enabled_ =
        doc.object().value(QStringLiteral("report_pomodoros_pomodoro")).toBool(pomodoro_report_pomodoros_pomodoro_enabled_);
    pomodoro_report_pomodoros_calendario_enabled_ =
        doc.object().value(QStringLiteral("report_pomodoros_calendario")).toBool(pomodoro_report_pomodoros_calendario_enabled_);
    pomodoro_report_pomodoros_enabled_ = pomodoro_report_pomodoros_eisenhower_enabled_;

    pomodoro_automatic_mode_enabled_ =
        doc.object().value(QStringLiteral("pomodoro_automatic_mode_enabled")).toBool(pomodoro_automatic_mode_enabled_);

    const QJsonValue criteria_value = doc.object().value(QStringLiteral("task_sort_criteria"));
    if (criteria_value.isArray()) {
        const QJsonArray criteria_array = criteria_value.toArray();
        for (const QJsonValue &value : criteria_array) {
            if (!value.isObject()) {
                continue;
            }

            const QJsonObject entry = value.toObject();
            const QString criterion_key = entry.value(QStringLiteral("criterion")).toString().trimmed().toLower();
            const QString order_key = entry.value(QStringLiteral("order")).toString().trimmed().toLower();

            TaskSortCriterion criterion;
            if (criterion_key == QStringLiteral("date")) {
                criterion = TaskSortCriterion::Date;
            } else if (criterion_key == QStringLiteral("alphabetical")) {
                criterion = TaskSortCriterion::Alphabetical;
            } else if (criterion_key == QStringLiteral("time")) {
                criterion = TaskSortCriterion::Time;
            } else {
                continue;
            }

            const Qt::SortOrder order = order_key == QStringLiteral("desc") ? Qt::DescendingOrder
                                                                            : Qt::AscendingOrder;
            const int existing_index = _task_sort_criterion_index(criterion);
            if (existing_index >= 0) {
                task_sort_criteria_[existing_index].order = order;
            } else {
                task_sort_criteria_.append({criterion, order});
            }
        }
    } else {
        const QString sort_value = doc.object().value(QStringLiteral("task_sort")).toString().trimmed().toLower();
        if (sort_value == QStringLiteral("date_asc")) {
            task_sort_criteria_.append({TaskSortCriterion::Date, Qt::AscendingOrder});
        } else if (sort_value == QStringLiteral("date_desc")) {
            task_sort_criteria_.append({TaskSortCriterion::Date, Qt::DescendingOrder});
        } else if (sort_value == QStringLiteral("time_asc")) {
            task_sort_criteria_.append({TaskSortCriterion::Time, Qt::AscendingOrder});
        } else if (sort_value == QStringLiteral("time_desc")) {
            task_sort_criteria_.append({TaskSortCriterion::Time, Qt::DescendingOrder});
        } else if (sort_value == QStringLiteral("alphabetical_asc")) {
            task_sort_criteria_.append({TaskSortCriterion::Alphabetical, Qt::AscendingOrder});
        } else if (sort_value == QStringLiteral("alphabetical_desc")) {
            task_sort_criteria_.append({TaskSortCriterion::Alphabetical, Qt::DescendingOrder});
        }
    }

    sync_eisenhower_enabled_ =
        doc.object().value(QStringLiteral("sync_eisenhower")).toBool(sync_eisenhower_enabled_);
    sync_pomodoro_enabled_ = doc.object().value(QStringLiteral("sync_pomodoro")).toBool(sync_pomodoro_enabled_);
    sync_feynman_enabled_ = doc.object().value(QStringLiteral("sync_feynman")).toBool(sync_feynman_enabled_);
    reserved_space_eisenhower_enabled_ =
        doc.object().value(QStringLiteral("reserved_space_eisenhower")).toBool(reserved_space_eisenhower_enabled_);
    reserved_space_pomodoro_enabled_ =
        doc.object().value(QStringLiteral("reserved_space_pomodoro")).toBool(reserved_space_pomodoro_enabled_);
    reserved_space_feynman_enabled_ =
        doc.object().value(QStringLiteral("reserved_space_feynman")).toBool(reserved_space_feynman_enabled_);
    reserved_watermark_eisenhower_enabled_ = doc.object()
                                                 .value(QStringLiteral("reserved_watermark_eisenhower"))
                                                 .toBool(reserved_watermark_eisenhower_enabled_);
    reserved_watermark_pomodoro_enabled_ =
        doc.object().value(QStringLiteral("reserved_watermark_pomodoro")).toBool(reserved_watermark_pomodoro_enabled_);
    reserved_watermark_feynman_enabled_ =
        doc.object().value(QStringLiteral("reserved_watermark_feynman")).toBool(reserved_watermark_feynman_enabled_);
    calendar_start_expanded = doc.object().value(QStringLiteral("show_calendar_panel")).toBool(calendar_start_expanded);
    feynman_task_panel_start_expanded =
        doc.object().value(QStringLiteral("show_feynman_task_panel")).toBool(feynman_task_panel_start_expanded);
    if (doc.object().contains(QStringLiteral("show_pomodoro_panel"))) {
        eisenhower_pomodoro_expanded =
            doc.object().value(QStringLiteral("show_pomodoro_panel")).toBool(eisenhower_pomodoro_expanded);
    }
    pomodoro_in_progress_panel_start_expanded =
        doc.object().value(QStringLiteral("show_pomodoro_in_progress_panel")).toBool(pomodoro_in_progress_panel_start_expanded);
    poca_layout_active_ = doc.object().value(QStringLiteral("poca_layout_active")).toBool(poca_layout_active_);
}

void EisenhowerMatrixApp::_save_task_layout_preference() const
{
    if (layout_path.isEmpty()) {
        return;
    }

    const QFileInfo info(layout_path);
    QDir().mkpath(info.absolutePath());

    QJsonObject config;
    QFile existing_file(layout_path);
    if (existing_file.open(QIODevice::ReadOnly)) {
        const QJsonDocument existing_doc = QJsonDocument::fromJson(existing_file.readAll());
        if (existing_doc.isObject()) {
            config = existing_doc.object();
        }
        existing_file.close();
    }

    QSaveFile file(layout_path);
    if (!file.open(QIODevice::WriteOnly)) {
        logger()->error(QStringLiteral("Erro ao salvar configuração de layout: %1").arg(layout_path));
        return;
    }

    config.insert(QStringLiteral("layout"), task_layout_key(task_layout_mode_));
    config.insert(QStringLiteral("pomodoro_ui_layout"), normalized_pomodoro_ui_layout_key(pomodoro_ui_layout_mode_key_));
    config.insert(QStringLiteral("feynman_ui_layout"), normalized_feynman_ui_layout_key(feynman_ui_layout_mode_key_));

    QJsonArray sort_criteria;
    for (const TaskSortCriterionState &entry : task_sort_criteria_) {
        QJsonObject criterion_entry;
        criterion_entry.insert(QStringLiteral("criterion"), task_sort_criterion_key(entry.criterion));
        criterion_entry.insert(QStringLiteral("order"), task_sort_order_key(entry.order));
        sort_criteria.append(criterion_entry);
    }
    config.insert(QStringLiteral("task_sort_criteria"), sort_criteria);
    config.insert(QStringLiteral("sync_eisenhower"), sync_eisenhower_enabled_);
    config.insert(QStringLiteral("sync_pomodoro"), sync_pomodoro_enabled_);
    config.insert(QStringLiteral("sync_feynman"), sync_feynman_enabled_);
    config.insert(QStringLiteral("reserved_space_eisenhower"), reserved_space_eisenhower_enabled_);
    config.insert(QStringLiteral("reserved_space_pomodoro"), reserved_space_pomodoro_enabled_);
    config.insert(QStringLiteral("reserved_space_feynman"), reserved_space_feynman_enabled_);
    config.insert(QStringLiteral("reserved_watermark_eisenhower"), reserved_watermark_eisenhower_enabled_);
    config.insert(QStringLiteral("reserved_watermark_pomodoro"), reserved_watermark_pomodoro_enabled_);
    config.insert(QStringLiteral("reserved_watermark_feynman"), reserved_watermark_feynman_enabled_);
    config.insert(QStringLiteral("show_calendar_panel"), calendar_panel_visible());
    config.insert(QStringLiteral("show_feynman_task_panel"), feynman_task_panel_visible());
    config.insert(QStringLiteral("show_pomodoro_panel"), pomodoro_panel_visible());
    config.insert(QStringLiteral("show_pomodoro_in_progress_panel"), pomodoro_in_progress_panel_visible());

    config.insert(QStringLiteral("report_pomodoros_eisenhower"), pomodoro_report_pomodoros_eisenhower_enabled_);
    config.insert(QStringLiteral("report_pomodoros_pomodoro"), pomodoro_report_pomodoros_pomodoro_enabled_);
    config.insert(QStringLiteral("report_pomodoros_calendario"), pomodoro_report_pomodoros_calendario_enabled_);
    config.insert(QStringLiteral("pomodoro_automatic_mode_enabled"), pomodoro_automatic_mode_enabled_);
    config.insert(QStringLiteral("pomodoro_ui_layout"), normalized_pomodoro_ui_layout_key(pomodoro_ui_layout_mode_key_));
    config.insert(QStringLiteral("poca_layout_active"), poca_layout_active());
    const QByteArray payload = QJsonDocument(config).toJson(QJsonDocument::Indented);
    if (file.write(payload) != payload.size() || !file.commit()) {
        logger()->error(QStringLiteral("Erro ao confirmar configuração de layout: %1").arg(layout_path));
    }
}

bool EisenhowerMatrixApp::_attach_quadrant_panels_to_tabs(bool horizontal_layout)
{
    if (quadrant_tabs == nullptr || quadrant1_tab == nullptr || quadrant2_tab == nullptr || quadrant3_tab == nullptr
        || quadrant4_tab == nullptr) {
        return false;
    }

    clear_layout(quadrants_layout);
    clear_layout(matrix2x2_layout);
    clear_matrix_status_tabs(this);

    if (!attach_split_panel(quadrant1_tab,
                            quadrant1_group,
                            quadrant1_list,
                            quadrant1_in_progress_group,
                            quadrant1_in_progress_list,
                            quadrant1_completed_group,
                            quadrant1_completed_list,
                            pending_title(this, 0),
                            quadrant1_in_progress_label->text(),
                            completed_title(this, 0),
                            true,
                            horizontal_layout)
        || !attach_split_panel(quadrant2_tab,
                               quadrant2_group,
                               quadrant2_list,
                               quadrant2_in_progress_group,
                               quadrant2_in_progress_list,
                               quadrant2_completed_group,
                               quadrant2_completed_list,
                               pending_title(this, 1),
                               quadrant2_in_progress_label->text(),
                               completed_title(this, 1),
                               true,
                               horizontal_layout)
        || !attach_split_panel(quadrant3_tab,
                               quadrant3_group,
                               quadrant3_list,
                               quadrant3_in_progress_group,
                               quadrant3_in_progress_list,
                               quadrant3_completed_group,
                               quadrant3_completed_list,
                               pending_title(this, 2),
                               quadrant3_in_progress_label->text(),
                               completed_title(this, 2),
                               true,
                               horizontal_layout)
        || !attach_split_panel(quadrant4_tab,
                               quadrant4_group,
                               quadrant4_list,
                               quadrant4_in_progress_group,
                               quadrant4_in_progress_list,
                               quadrant4_completed_group,
                               quadrant4_completed_list,
                               pending_title(this, 3),
                               quadrant4_in_progress_label->text(),
                               completed_title(this, 3),
                               true,
                               horizontal_layout)) {
        return false;
    }

    while (quadrant_tabs->count() > 0) {
        quadrant_tabs->removeTab(0);
    }

    const QList<QPair<QWidget *, QString>> panels = {
        {quadrant1_tab, quadrant1_label != nullptr ? quadrant1_label->text()
                                                   : get_text(QStringLiteral("🔴 Importante e Urgente"))},
        {quadrant2_tab, quadrant2_label != nullptr ? quadrant2_label->text()
                                                   : get_text(QStringLiteral("🟠 Importante, mas Não Urgente"))},
        {quadrant3_tab, quadrant3_label != nullptr ? quadrant3_label->text()
                                                   : get_text(QStringLiteral("🟡 Não Importante, mas Urgente"))},
        {quadrant4_tab, quadrant4_label != nullptr ? quadrant4_label->text()
                                                   : get_text(QStringLiteral("🟢 Não Importante e Não Urgente"))},
    };

    for (const auto &panel : panels) {
        if (panel.first != nullptr) {
            panel.first->setMinimumWidth(0);
            panel.first->setMaximumWidth(QWIDGETSIZE_MAX);
            panel.first->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            quadrant_tabs->addTab(panel.first, panel.second);
        }
    }

    if (quadrant_tabs->count() > 0) {
        quadrant_tabs->setCurrentIndex(qBound(0, last_quadrant_tab_index_, quadrant_tabs->count() - 1));
    }

    return true;
}

bool EisenhowerMatrixApp::_attach_quadrant_panels_to_columns()
{
    if (quadrants_layout == nullptr || quadrant1_tab == nullptr || quadrant2_tab == nullptr || quadrant3_tab == nullptr
        || quadrant4_tab == nullptr || quadrants_page == nullptr) {
        return false;
    }

    if (quadrant_tabs != nullptr && quadrant_tabs->count() > 0) {
        last_quadrant_tab_index_ = qMax(0, quadrant_tabs->currentIndex());
    }

    clear_layout(quadrants_layout);
    clear_layout(matrix2x2_layout);
    if (quadrant_tabs != nullptr) {
        while (quadrant_tabs->count() > 0) {
            quadrant_tabs->removeTab(0);
        }
    }
    clear_matrix_status_tabs(this);

    for (QWidget *panel : {quadrant1_tab, quadrant2_tab, quadrant3_tab, quadrant4_tab}) {
        if (panel != nullptr) {
            panel->setMinimumWidth(0);
            panel->setMaximumWidth(QWIDGETSIZE_MAX);
            panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }
    }

    if (!attach_split_panel(quadrant1_tab,
                            quadrant1_group,
                            quadrant1_list,
                            quadrant1_in_progress_group,
                            quadrant1_in_progress_list,
                            quadrant1_completed_group,
                            quadrant1_completed_list,
                            quadrant_title(this, 0),
                            quadrant1_in_progress_label->text(),
                            completed_title(this, 0),
                            true)
        || !attach_split_panel(quadrant2_tab,
                               quadrant2_group,
                               quadrant2_list,
                               quadrant2_in_progress_group,
                               quadrant2_in_progress_list,
                               quadrant2_completed_group,
                               quadrant2_completed_list,
                               quadrant_title(this, 1),
                               quadrant2_in_progress_label->text(),
                               completed_title(this, 1),
                               true)
        || !attach_split_panel(quadrant3_tab,
                               quadrant3_group,
                               quadrant3_list,
                               quadrant3_in_progress_group,
                               quadrant3_in_progress_list,
                               quadrant3_completed_group,
                               quadrant3_completed_list,
                               quadrant_title(this, 2),
                               quadrant3_in_progress_label->text(),
                               completed_title(this, 2),
                               true)
        || !attach_split_panel(quadrant4_tab,
                               quadrant4_group,
                               quadrant4_list,
                               quadrant4_in_progress_group,
                               quadrant4_in_progress_list,
                               quadrant4_completed_group,
                               quadrant4_completed_list,
                               quadrant_title(this, 3),
                               quadrant4_in_progress_label->text(),
                               completed_title(this, 3),
                               true)) {
        return false;
    }

    for (QWidget *panel : {quadrant1_tab, quadrant2_tab, quadrant3_tab, quadrant4_tab}) {
        if (panel != nullptr) {
            panel->setMinimumWidth(0);
            panel->setMaximumWidth(QWIDGETSIZE_MAX);
            panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            quadrants_layout->addWidget(panel, 1);
            panel->show();
        }
    }

    quadrants_layout->invalidate();
    quadrants_layout->activate();
    quadrants_page->updateGeometry();
    quadrants_page->update();
    return true;
}

bool EisenhowerMatrixApp::_attach_quadrant_panels_to_matrix_2x2()
{
    if (matrix2x2_layout == nullptr || matrix2x2_page == nullptr || quadrant1_status_tabs == nullptr
        || quadrant2_status_tabs == nullptr || quadrant3_status_tabs == nullptr || quadrant4_status_tabs == nullptr) {
        return false;
    }

    if (quadrant_tabs != nullptr && quadrant_tabs->count() > 0) {
        last_quadrant_tab_index_ = qMax(0, quadrant_tabs->currentIndex());
    }

    clear_layout(quadrants_layout);
    clear_layout(matrix2x2_layout);
    if (quadrant_tabs != nullptr) {
        while (quadrant_tabs->count() > 0) {
            quadrant_tabs->removeTab(0);
        }
    }

    for (QWidget *panel : {quadrant1_tab, quadrant2_tab, quadrant3_tab, quadrant4_tab}) {
        if (panel != nullptr) {
            panel->setMinimumWidth(0);
            panel->setMaximumWidth(QWIDGETSIZE_MAX);
            panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }
    }

    if (!attach_status_tabs(quadrant1_status_tabs,
                            quadrant1_group,
                            quadrant1_list,
                            quadrant1_in_progress_group,
                            quadrant1_in_progress_list,
                            quadrant1_completed_group,
                            quadrant1_completed_list,
                            quadrant_title(this, 0))
        || !attach_status_tabs(quadrant2_status_tabs,
                               quadrant2_group,
                               quadrant2_list,
                               quadrant2_in_progress_group,
                               quadrant2_in_progress_list,
                               quadrant2_completed_group,
                               quadrant2_completed_list,
                               quadrant_title(this, 1))
        || !attach_status_tabs(quadrant3_status_tabs,
                               quadrant3_group,
                               quadrant3_list,
                               quadrant3_in_progress_group,
                               quadrant3_in_progress_list,
                               quadrant3_completed_group,
                               quadrant3_completed_list,
                               quadrant_title(this, 2))
        || !attach_status_tabs(quadrant4_status_tabs,
                               quadrant4_group,
                               quadrant4_list,
                               quadrant4_in_progress_group,
                               quadrant4_in_progress_list,
                               quadrant4_completed_group,
                               quadrant4_completed_list,
                               quadrant_title(this, 3))) {
        return false;
    }

    matrix2x2_layout->addWidget(quadrant1_status_tabs, 0, 0);
    matrix2x2_layout->addWidget(quadrant2_status_tabs, 0, 1);
    matrix2x2_layout->addWidget(quadrant3_status_tabs, 1, 0);
    matrix2x2_layout->addWidget(quadrant4_status_tabs, 1, 1);
    matrix2x2_layout->invalidate();
    matrix2x2_layout->activate();
    matrix2x2_page->updateGeometry();
    matrix2x2_page->update();
    return true;
}

bool EisenhowerMatrixApp::_apply_task_layout()
{
    if (task_layout_stack == nullptr || quadrant_tabs_page == nullptr || quadrants_page == nullptr || matrix2x2_page == nullptr) {
        return false;
    }

    bool applied = false;
    if (task_layout_mode_ == TaskLayoutMode::Quadrants) {
        applied = _attach_quadrant_panels_to_columns();
        if (applied) {
            task_layout_stack->setCurrentWidget(quadrants_page);
        }
    } else if (task_layout_mode_ == TaskLayoutMode::Matrix2x2) {
        applied = _attach_quadrant_panels_to_matrix_2x2();
        if (applied) {
            task_layout_stack->setCurrentWidget(matrix2x2_page);
        }
    } else if (task_layout_mode_ == TaskLayoutMode::TabsHorizontal) {
        applied = _attach_quadrant_panels_to_tabs(true);
        if (applied) {
            task_layout_stack->setCurrentWidget(quadrant_tabs_page);
        }
    } else {
        applied = _attach_quadrant_panels_to_tabs(false);
        if (applied) {
            task_layout_stack->setCurrentWidget(quadrant_tabs_page);
        }
    }

    if (!applied) {
        return false;
    }

    // Prevent hidden stacked pages from inflating the container's minimum size.
    // QStackedLayout reports minimumSize as the max of ALL children;
    // setting Ignored on hidden pages removes their influence.
    QWidget *current_page = task_layout_stack->currentWidget();
    for (int i = 0; i < task_layout_stack->count(); ++i) {
        QWidget *page = task_layout_stack->widget(i);
        if (page == nullptr) {
            continue;
        }
        if (page == current_page) {
            page->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        } else {
            page->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        }
    }
    task_layout_stack->invalidate();
    task_layout_stack->activate();
    if (QWidget *host = task_layout_stack->parentWidget()) {
        host->updateGeometry();
    }

    _apply_theme_palette();
    _atualizar_largura_minima_eisenhower();
    return true;
}

int EisenhowerMatrixApp::_calcular_largura_minima_layout_tarefas() const
{
    const auto mode = current_task_layout();
    const QFontMetrics fm = (quadrant1_label != nullptr) ? quadrant1_label->fontMetrics() : QFontMetrics(QApplication::font());
    const QFontMetrics ifm = (eisenhower_input_group != nullptr) ? eisenhower_input_group->fontMetrics() : fm;

    // Linha 2: Prioridade:, seletor, Nível de Domínio:, seletor
    int l2_w = 0;
    l2_w += ifm.horizontalAdvance(get_text(QStringLiteral("Prioridade:"))) + 12;
    l2_w += ifm.horizontalAdvance(get_text(QStringLiteral("🟠 Importante, mas Não Urgente"))) + 48;
    l2_w += ifm.horizontalAdvance(get_text(QStringLiteral("Nível de Domínio:"))) + 12;
    l2_w += ifm.horizontalAdvance(get_text(QStringLiteral("Intermediário"))) + 48;
    l2_w += 30;

    // Linha 3: Vincular data, data, Vincular horário, horário, Duração:, spinbox, Adicionar Tarefa
    int l3_w = 0;
    l3_w += ifm.horizontalAdvance(get_text(QStringLiteral("Vincular data"))) + 30;
    l3_w += 110;
    l3_w += ifm.horizontalAdvance(get_text(QStringLiteral("Vincular horário"))) + 30;
    l3_w += 80;
    l3_w += ifm.horizontalAdvance(get_text(QStringLiteral("Duração:"))) + 12;
    l3_w += 95;
    l3_w += ifm.horizontalAdvance(get_text(QStringLiteral("Adicionar Tarefa"))) + 48;
    l3_w += 40;

    const int input_min_w = qMax(l2_w, l3_w);

    if (mode == TaskLayoutMode::Tabs || mode == TaskLayoutMode::TabsHorizontal) {
        int tabs_text_width_sum = 0;
        const QFontMetrics tfm = (quadrant_tabs != nullptr) ? quadrant_tabs->fontMetrics() : fm;

        const QStringList tab_texts = {
            quadrant1_label != nullptr ? quadrant1_label->text() : get_text(QStringLiteral("🔴 Importante e Urgente")),
            quadrant2_label != nullptr ? quadrant2_label->text() : get_text(QStringLiteral("🟠 Importante, mas Não Urgente")),
            quadrant3_label != nullptr ? quadrant3_label->text() : get_text(QStringLiteral("🟡 Não Importante, mas Urgente")),
            quadrant4_label != nullptr ? quadrant4_label->text() : get_text(QStringLiteral("🟢 Não Importante e Não Urgente")),
        };

        for (const QString &txt : tab_texts) {
            tabs_text_width_sum += tfm.horizontalAdvance(txt) + 32;
        }

        if (mode == TaskLayoutMode::TabsHorizontal) {
            int h_sum = 0;
            const QStringList col_headers = {
                get_text(QStringLiteral("🔴 Pendentes")),
                get_text(QStringLiteral("Em Progresso")),
                get_text(QStringLiteral("🔴 Concluídas"))
            };
            for (const QString &h : col_headers) {
                h_sum += qMax(160, tfm.horizontalAdvance(h) + 48);
            }
            return qMax(tabs_text_width_sum, qMax(h_sum, input_min_w));
        }

        return qMax(tabs_text_width_sum, input_min_w);
    } else if (mode == TaskLayoutMode::Matrix2x2) {
        const QFontMetrics mfm = (quadrant1_status_tabs != nullptr) ? quadrant1_status_tabs->fontMetrics() : fm;

        auto calc_quadrant_min_width = [&](QTabWidget *tabs) -> int {
            int q_sum = 0;
            const QStringList status_texts = {
                get_text(QStringLiteral("Pendente")),
                get_text(QStringLiteral("Em Progresso")),
                get_text(QStringLiteral("Concluídas"))
            };
            for (int i = 0; i < 3; ++i) {
                QString t = (tabs != nullptr && tabs->count() > i) ? tabs->tabText(i) : status_texts.value(i);
                q_sum += mfm.horizontalAdvance(t) + 26;
            }
            return q_sum + 16;
        };

        const int q1_w = calc_quadrant_min_width(quadrant1_status_tabs);
        const int q2_w = calc_quadrant_min_width(quadrant2_status_tabs);
        const int q3_w = calc_quadrant_min_width(quadrant3_status_tabs);
        const int q4_w = calc_quadrant_min_width(quadrant4_status_tabs);

        const int col0_w = qMax(q1_w, q3_w);
        const int col1_w = qMax(q2_w, q4_w);

        const int matrix_w = col0_w + col1_w + kQuadrantColumnsGap;
        return qMax(matrix_w, input_min_w);
    } else {
        int max_col_w = 0;
        const QStringList labels = {
            quadrant1_label != nullptr ? quadrant1_label->text() : get_text(QStringLiteral("1º Quadrante")),
            quadrant2_label != nullptr ? quadrant2_label->text() : get_text(QStringLiteral("2º Quadrante")),
            quadrant3_label != nullptr ? quadrant3_label->text() : get_text(QStringLiteral("3º Quadrante")),
            quadrant4_label != nullptr ? quadrant4_label->text() : get_text(QStringLiteral("4º Quadrante")),
        };
        for (const QString &lbl : labels) {
            max_col_w = qMax(max_col_w, fm.horizontalAdvance(lbl) + 40);
        }
        max_col_w = qMax(max_col_w, 200);

        const int quad_w = (max_col_w * 4) + (3 * kQuadrantColumnsGap);
        return qMax(quad_w, input_min_w);
    }
}

void EisenhowerMatrixApp::_atualizar_largura_minima_eisenhower()
{
    const int content_min_w = _calcular_largura_minima_layout_tarefas();
    quadrants_min_width = content_min_w;

    if (task_layout_host != nullptr) {
        task_layout_host->setMinimumWidth(content_min_w);
    }

    const auto mode = current_task_layout();
    if (mode == TaskLayoutMode::Matrix2x2) {
        const QFontMetrics fm = (quadrant1_status_tabs != nullptr) ? quadrant1_status_tabs->fontMetrics() : QFontMetrics(QApplication::font());
        auto calc_quadrant_min_width = [&](QTabWidget *tabs) -> int {
            int q_sum = 0;
            const QStringList status_texts = {
                get_text(QStringLiteral("Pendente")),
                get_text(QStringLiteral("Em Progresso")),
                get_text(QStringLiteral("Concluídas"))
            };
            for (int i = 0; i < 3; ++i) {
                QString t = (tabs != nullptr && tabs->count() > i) ? tabs->tabText(i) : status_texts.value(i);
                q_sum += fm.horizontalAdvance(t) + 26;
            }
            return q_sum + 16;
        };

        const int q1_w = calc_quadrant_min_width(quadrant1_status_tabs);
        const int q2_w = calc_quadrant_min_width(quadrant2_status_tabs);
        const int q3_w = calc_quadrant_min_width(quadrant3_status_tabs);
        const int q4_w = calc_quadrant_min_width(quadrant4_status_tabs);

        int col0_w = qMax(q1_w, q3_w);
        int col1_w = qMax(q2_w, q4_w);

        const int matrix_w = col0_w + col1_w + kQuadrantColumnsGap;
        if (content_min_w > matrix_w) {
            const int extra = (content_min_w - matrix_w) / 2;
            col0_w += extra;
            col1_w += extra;
        }

        if (quadrant1_status_tabs != nullptr) quadrant1_status_tabs->setMinimumWidth(col0_w);
        if (quadrant3_status_tabs != nullptr) quadrant3_status_tabs->setMinimumWidth(col0_w);
        if (quadrant2_status_tabs != nullptr) quadrant2_status_tabs->setMinimumWidth(col1_w);
        if (quadrant4_status_tabs != nullptr) quadrant4_status_tabs->setMinimumWidth(col1_w);
        if (quadrant_tabs != nullptr) quadrant_tabs->setMinimumWidth(0);
        for (QWidget *panel : {quadrant1_tab, quadrant2_tab, quadrant3_tab, quadrant4_tab}) {
            if (panel != nullptr) {
                panel->setMinimumWidth(0);
            }
        }
    } else if (mode == TaskLayoutMode::Tabs || mode == TaskLayoutMode::TabsHorizontal) {
        if (quadrant_tabs != nullptr) {
            quadrant_tabs->setMinimumWidth(content_min_w);
        }
        for (QTabWidget *tabs : {quadrant1_status_tabs, quadrant2_status_tabs, quadrant3_status_tabs, quadrant4_status_tabs}) {
            if (tabs != nullptr) {
                tabs->setMinimumWidth(0);
            }
        }
        for (QWidget *panel : {quadrant1_tab, quadrant2_tab, quadrant3_tab, quadrant4_tab}) {
            if (panel != nullptr) {
                panel->setMinimumWidth(0);
            }
        }
    } else {
        const QFontMetrics fm = (quadrant1_label != nullptr) ? quadrant1_label->fontMetrics() : QFontMetrics(QApplication::font());
        int max_col_w = 0;
        const QStringList labels = {
            quadrant1_label != nullptr ? quadrant1_label->text() : get_text(QStringLiteral("1º Quadrante")),
            quadrant2_label != nullptr ? quadrant2_label->text() : get_text(QStringLiteral("2º Quadrante")),
            quadrant3_label != nullptr ? quadrant3_label->text() : get_text(QStringLiteral("3º Quadrante")),
            quadrant4_label != nullptr ? quadrant4_label->text() : get_text(QStringLiteral("4º Quadrante")),
        };
        for (const QString &lbl : labels) {
            max_col_w = qMax(max_col_w, fm.horizontalAdvance(lbl) + 40);
        }
        max_col_w = qMax(max_col_w, 200);

        const int quad_w = (max_col_w * 4) + (3 * kQuadrantColumnsGap);
        if (content_min_w > quad_w) {
            const int extra_col = (content_min_w - (3 * kQuadrantColumnsGap)) / 4;
            max_col_w = qMax(max_col_w, extra_col);
        }

        if (quadrant1_tab != nullptr) quadrant1_tab->setMinimumWidth(max_col_w);
        if (quadrant2_tab != nullptr) quadrant2_tab->setMinimumWidth(max_col_w);
        if (quadrant3_tab != nullptr) quadrant3_tab->setMinimumWidth(max_col_w);
        if (quadrant4_tab != nullptr) quadrant4_tab->setMinimumWidth(max_col_w);

        if (quadrant_tabs != nullptr) {
            quadrant_tabs->setMinimumWidth(0);
        }
        for (QTabWidget *tabs : {quadrant1_status_tabs, quadrant2_status_tabs, quadrant3_status_tabs, quadrant4_status_tabs}) {
            if (tabs != nullptr) {
                tabs->setMinimumWidth(0);
            }
        }
    }

    if (quadrants_scroll_area != nullptr) {
        quadrants_scroll_area->setMinimumWidth(120);
        quadrants_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
}

}  // namespace source
