#include "source/services/RecursosProdutividade/recpro_03_recorrencias.hpp"
#include "source/services/RecursosProdutividade/recpro_01_utilitarios_compartilhados.hpp"

#include <QFontMetrics>
#include <QFrame>
#include <QListWidget>
#include <QSignalBlocker>
#include <QToolButton>

namespace source::InterfaceCore::ProductivityFeatures {

namespace {

enum class RecurrenceDialogAction {
    Apply,
    SaveOnly,
    Cancel
};

struct RecurrenceBaseDate {
    QDate date;
    bool include_base_date = false;
};

RecurrenceBaseDate recurrence_base_date_for_group(const TaskRecordGroup &group)
{
    const QDate date = first_group_date(group);
    if (date.isValid()) {
        return {date, false};
    }
    return {QDate::currentDate(), true};
}

QTime occurrence_time_for_group(const TaskRecordGroup &group)
{
    return first_group_time(group);
}

QStringList recurrence_date_options(const QDate &base_date,
                                    const QString &recurrence,
                                    const QDateTime &window_start,
                                    const QDateTime &window_end,
                                    const QTime &occurrence_time,
                                    bool include_base_date,
                                    int max_per_item)
{
    QStringList dates;
    if (!base_date.isValid() || recurrence.trimmed().isEmpty()) {
        return dates;
    }

    QDate next = include_base_date ? base_date : next_recurrence_date(base_date, recurrence);
    int guard = 0;
    while (next.isValid() && next <= window_end.date() && dates.size() < max_per_item && guard++ < 120) {
        if (recurrence_occurrence_in_window(next, occurrence_time, window_start, window_end)) {
            dates << (occurrence_time.isValid()
                          ? QStringLiteral("%1 %2").arg(next.toString(QStringLiteral("dd/MM/yyyy")),
                                                        occurrence_time.toString(QStringLiteral("HH:mm")))
                          : next.toString(QStringLiteral("dd/MM/yyyy")));
        }
        next = next_recurrence_date(next, recurrence);
    }
    return dates;
}

}  // namespace

void materialize_recurring_tasks(source::EisenhowerMatrixApp *app)
{
    if (app == nullptr) {
        return;
    }

    QVariantList unified_records = records_for_app(app);
    const QJsonObject map_backup;
    const QStringList available_modules = available_productivity_modules(app);
    const QSet<QString> all_modules = module_set_from_list(available_modules);
    QVector<TaskRecordGroup> groups =
        collect_productivity_task_groups(app, unified_records, map_backup, all_modules);
    if (groups.isEmpty()) {
        QMessageBox::information(app,
                                 get_text(QStringLiteral("Recorrências")),
                                 get_text(QStringLiteral("Nenhuma tarefa encontrada.")));
        return;
    }

    RuntimeTranslatedDialog dialog(app);
    dialog.setWindowTitle(get_text(QStringLiteral("Gerar próximas recorrências")));
    dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    auto *layout = new QVBoxLayout(&dialog);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto *summary = new QLabel(&dialog);
    summary->setWordWrap(true);
    layout->addWidget(summary);

    auto *selection_group = new QGroupBox(get_text(QStringLiteral("Itens")), &dialog);
    auto *selection_layout = new QVBoxLayout(selection_group);
    selection_layout->setContentsMargins(10, 8, 10, 10);
    selection_layout->setSpacing(7);
    auto *all_items_radio = new QRadioButton(get_text(QStringLiteral("Todos filtrados")), selection_group);
    auto *selected_items_radio = new QRadioButton(get_text(QStringLiteral("Somente selecionados")), selection_group);
    all_items_radio->setChecked(true);
    auto *selection_mode_layout = new QHBoxLayout();
    selection_mode_layout->addWidget(all_items_radio);
    selection_mode_layout->addWidget(selected_items_radio);
    selection_mode_layout->addStretch(1);
    selection_layout->addLayout(selection_mode_layout);

    auto *item_filter = new QLineEdit(selection_group);
    item_filter->setPlaceholderText(get_text(QStringLiteral("Pesquisar item")));
    selection_layout->addWidget(item_filter);

    auto *item_model = new source::models::TaskRecordListModel(selection_group);
    auto *item_proxy = new QSortFilterProxyModel(selection_group);
    item_proxy->setSourceModel(item_model);
    item_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    item_proxy->setFilterRole(source::models::TaskRecordListModel::SearchableRole);

    auto *item_view = new QListView(selection_group);
    item_view->setModel(item_proxy);
    item_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    item_view->setUniformItemSizes(false);
    item_view->setAlternatingRowColors(true);
    item_view->setMinimumHeight(118);
    selection_layout->addWidget(item_view, 1);

    auto *selection_buttons_widget = new QWidget(selection_group);
    selection_buttons_widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    auto *selection_buttons = new QHBoxLayout(selection_buttons_widget);
    selection_buttons->setContentsMargins(0, 8, 0, 0);
    selection_buttons->setSpacing(6);
    auto *select_visible_button = new QPushButton(get_text(QStringLiteral("Selecionar visíveis")), selection_buttons_widget);
    auto *clear_selection_button = new QPushButton(get_text(QStringLiteral("Limpar")), selection_buttons_widget);
    select_visible_button->setMaximumWidth(150);
    clear_selection_button->setMaximumWidth(90);
    selection_buttons->addWidget(select_visible_button);
    selection_buttons->addWidget(clear_selection_button);
    selection_buttons->addStretch(1);
    selection_layout->addWidget(selection_buttons_widget);
    selection_group->setMinimumHeight(200);
    selection_group->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    auto *settings_group = new QGroupBox(get_text(QStringLiteral("Configurações")), &dialog);
    auto *form = new QFormLayout(settings_group);
    auto *horizon_days = new QSpinBox(settings_group);
    horizon_days->setRange(1, 365);
    horizon_days->setValue(30);
    auto *max_per_item = new QSpinBox(settings_group);
    max_per_item->setRange(1, 60);
    max_per_item->setValue(8);
    auto *include_completed = new QCheckBox(get_text(QStringLiteral("Incluir itens concluídos")), settings_group);
    auto *avoid_duplicates = new QCheckBox(get_text(QStringLiteral("Evitar duplicadas existentes")), settings_group);
    avoid_duplicates->setChecked(true);
    auto *apply_default = new QCheckBox(
        get_text(QStringLiteral("Aplicar frequência padrão a itens sem recorrência definida")), settings_group);
    auto *default_recurrence = new QComboBox(settings_group);
    default_recurrence->addItem(get_text(QStringLiteral("Diária")), QStringLiteral("daily"));
    default_recurrence->addItem(get_text(QStringLiteral("Semanal")), QStringLiteral("weekly"));
    default_recurrence->addItem(get_text(QStringLiteral("Mensal")), QStringLiteral("monthly"));
    default_recurrence->setCurrentIndex(1);
    default_recurrence->setEnabled(false);
    QObject::connect(apply_default, &QCheckBox::toggled, default_recurrence, &QComboBox::setEnabled);

    auto *horizon_label = new QLabel(get_text(QStringLiteral("Alcance (dias)")), settings_group);
    auto *max_label = new QLabel(get_text(QStringLiteral("Máximo por item")), settings_group);
    auto *default_recurrence_label = new QLabel(get_text(QStringLiteral("Frequência padrão")), settings_group);

    form->addRow(horizon_label, horizon_days);
    form->addRow(max_label, max_per_item);
    form->addRow(include_completed);
    form->addRow(avoid_duplicates);
    form->addRow(apply_default);
    form->addRow(default_recurrence_label, default_recurrence);

    auto *when_group = new QGroupBox(get_text(QStringLiteral("Quando aplicar")), &dialog);
    auto *when_layout = new QHBoxLayout(when_group);
    when_layout->setContentsMargins(10, 8, 10, 8);
    when_layout->setSpacing(12);
    auto *when_form = new QFormLayout();
    when_form->setContentsMargins(0, 0, 0, 0);
    when_form->setSpacing(6);
    auto *application_start_date = new QDateEdit(QDate::currentDate(), when_group);
    application_start_date->setCalendarPopup(true);
    application_start_date->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    auto *application_end_date = new QDateEdit(QDate::currentDate().addDays(horizon_days->value()), when_group);
    application_end_date->setCalendarPopup(true);
    application_end_date->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    auto *application_start_time = new QTimeEdit(QTime(0, 0), when_group);
    application_start_time->setDisplayFormat(QStringLiteral("HH:mm"));
    auto *application_end_time = new QTimeEdit(QTime(23, 59), when_group);
    application_end_time->setDisplayFormat(QStringLiteral("HH:mm"));
    auto *missing_base_date = new QDateEdit(QDate::currentDate(), when_group);
    missing_base_date->setCalendarPopup(true);
    missing_base_date->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    auto *missing_base_time = new QComboBox(when_group);
    for (int minutes = 0; minutes < 24 * 60; minutes += 15) {
        const QTime time(minutes / 60, minutes % 60);
        missing_base_time->addItem(time.toString(QStringLiteral("HH:mm")), time.toString(QStringLiteral("HH:mm")));
    }
    missing_base_time->setEditable(true);
    missing_base_time->setMaxVisibleItems(12);
    const auto recurrence_field_width = [&]() {
        int width = 132;
        const QWidget *fields[] = {application_start_date,
                                   application_end_date,
                                   application_start_time,
                                   application_end_time,
                                   missing_base_date,
                                   missing_base_time};
        for (const QWidget *field : fields) {
            if (field != nullptr) {
                width = qMax(width, field->sizeHint().width() + 10);
            }
        }
        return width;
    }();
    for (QWidget *field : {static_cast<QWidget *>(application_start_date),
                           static_cast<QWidget *>(application_end_date),
                           static_cast<QWidget *>(application_start_time),
                           static_cast<QWidget *>(application_end_time),
                           static_cast<QWidget *>(missing_base_date),
                           static_cast<QWidget *>(missing_base_time)}) {
        field->setMinimumWidth(recurrence_field_width);
        field->setMaximumWidth(recurrence_field_width);
    }
    auto *use_start_time_when_missing =
        new QCheckBox(get_text(QStringLiteral("Usar hora inicial quando o item não tiver horário")), when_group);

    auto *start_date_label = new QLabel(get_text(QStringLiteral("Data inicial")), when_group);
    auto *start_time_label = new QLabel(get_text(QStringLiteral("Hora inicial")), when_group);
    auto *end_date_label = new QLabel(get_text(QStringLiteral("Data final")), when_group);
    auto *end_time_label = new QLabel(get_text(QStringLiteral("Hora final")), when_group);
    auto *missing_date_label = new QLabel(get_text(QStringLiteral("Data para itens sem data")), when_group);
    auto *missing_time_label = new QLabel(get_text(QStringLiteral("Hora para itens sem horário")), when_group);

    when_form->addRow(start_date_label, application_start_date);
    when_form->addRow(start_time_label, application_start_time);
    when_form->addRow(end_date_label, application_end_date);
    when_form->addRow(end_time_label, application_end_time);
    when_form->addRow(use_start_time_when_missing);
    when_form->addRow(missing_date_label, missing_base_date);
    when_form->addRow(missing_time_label, missing_base_time);
    when_layout->addLayout(when_form);

    auto *calendar_column = new QVBoxLayout();
    calendar_column->setContentsMargins(0, 0, 0, 0);
    calendar_column->setSpacing(4);
    auto *calendar_strip =
        create_multi_month_calendar_strip(when_group, application_start_date->date(), 180);
    auto *calendar_filter = new CalendarStripResizeFilter(calendar_strip, calendar_strip->container);
    calendar_strip->container->installEventFilter(calendar_filter);
    calendar_column->addWidget(calendar_strip->container, 1);
    auto *calendar_buttons = new QHBoxLayout();
    auto *set_start_button = new QPushButton(get_text(QStringLiteral("Início")), when_group);
    auto *set_end_button = new QPushButton(get_text(QStringLiteral("Fim")), when_group);
    set_start_button->setMaximumWidth(86);
    set_end_button->setMaximumWidth(86);
    calendar_buttons->addWidget(set_start_button);
    calendar_buttons->addWidget(set_end_button);
    calendar_buttons->addStretch(1);
    calendar_column->addLayout(calendar_buttons);
    when_layout->addLayout(calendar_column, 1);
    when_group->setMinimumSize(when_group->sizeHint());

    auto *preview_group = new QGroupBox(get_text(QStringLiteral("Prévia do que será criado")), &dialog);
    auto *preview_layout = new QVBoxLayout(preview_group);
    auto *preview_table = new QTableWidget(preview_group);
    preview_table->setColumnCount(3);
    preview_table->setHorizontalHeaderLabels({get_text(QStringLiteral("Tarefa")),
                                              get_text(QStringLiteral("Recorrência usada")),
                                              get_text(QStringLiteral("Próximas datas"))});
    preview_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    preview_table->setSelectionMode(QAbstractItemView::NoSelection);
    preview_table->setAlternatingRowColors(true);
    preview_table->verticalHeader()->setVisible(false);
    preview_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    preview_table->horizontalHeader()->setStretchLastSection(true);
    preview_table->horizontalHeader()->setSectionsMovable(true);
    preview_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    preview_table->setColumnWidth(0, 240);
    preview_table->setColumnWidth(1, 140);
    preview_table->setColumnWidth(2, 280);
    preview_table->setMinimumHeight(150);
    preview_layout->addWidget(preview_table);

    auto *left_column_widget = new QWidget(&dialog);
    auto *left_column = new QVBoxLayout(left_column_widget);
    left_column->setContentsMargins(0, 0, 0, 0);
    left_column->setSpacing(layout->spacing());
    left_column->addWidget(selection_group, 1);
    left_column->addWidget(settings_group);

    auto *right_column_widget = new QWidget(&dialog);
    auto *right_column = new QVBoxLayout(right_column_widget);
    right_column->setContentsMargins(0, 0, 0, 0);
    right_column->setSpacing(layout->spacing());
    right_column->addWidget(when_group);
    right_column->addWidget(preview_group, 1);

    left_column_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    right_column_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    const int left_column_min_width = left_column_widget->sizeHint().width();
    const int right_column_min_width = right_column_widget->sizeHint().width();
    left_column_widget->setMinimumWidth(left_column_min_width);
    right_column_widget->setMinimumWidth(right_column_min_width);
    when_group->setMinimumWidth(right_column_min_width);

    auto *columns_row = new QHBoxLayout();
    columns_row->setSpacing(layout->spacing());
    columns_row->addWidget(left_column_widget, 1);
    columns_row->addWidget(right_column_widget, 1);
    layout->addLayout(columns_row, 1);

    const auto set_combo_current_data = [](QComboBox *combo, const QString &data) {
        if (combo == nullptr) {
            return;
        }
        const int index = combo->findData(data);
        if (index >= 0) {
            combo->setCurrentIndex(index);
        }
    };
    const auto set_time_combo_current_time = [&](QComboBox *combo, const QTime &time) {
        if (combo == nullptr || !time.isValid()) {
            return;
        }
        const QString value = time.toString(QStringLiteral("HH:mm"));
        int index = combo->findData(value);
        if (index < 0) {
            combo->addItem(value, value);
            index = combo->findData(value);
        }
        if (index >= 0) {
            combo->setCurrentIndex(index);
        }
    };
    const auto time_combo_time = [](const QComboBox *combo) {
        if (combo == nullptr) {
            return QTime();
        }
        const QString value = combo->currentData().isValid() ? combo->currentData().toString() : combo->currentText();
        return parse_time(value);
    };

    QPointer<QFrame> preview_list_popup;
    const auto set_window_range = [&](QDate start, QDate end) {
        if (!start.isValid()) {
            start = QDate::currentDate();
        }
        if (!end.isValid() || end < start) {
            end = start;
        }
        const QSignalBlocker block_start(application_start_date);
        const QSignalBlocker block_end(application_end_date);
        application_start_date->setDate(start);
        application_end_date->setDate(end);
        update_calendar_strip_dates(calendar_strip, start, end);
    };
    const auto update_calendar_from_inputs = [&]() {
        update_calendar_strip_dates(calendar_strip, application_start_date->date(), application_end_date->date());
    };

    const auto selected_group_indices = [&]() {
        QSet<int> indices;
        if (item_view->selectionModel() == nullptr) {
            return indices;
        }
        const QModelIndexList selected = item_view->selectionModel()->selectedRows();
        for (const QModelIndex &proxy_index : selected) {
            const QModelIndex source_index = item_proxy->mapToSource(proxy_index);
            const QVariantMap entry = item_model->record_at(source_index.row());
            const int group_index = entry.value(QStringLiteral("group_index"), -1).toInt();
            if (group_index >= 0) {
                indices.insert(group_index);
            }
        }
        return indices;
    };

    const auto group_matches_current_filters = [&](int group_index) {
        if (group_index < 0 || group_index >= groups.size()) {
            return false;
        }
        const TaskRecordGroup &group = groups.at(group_index);
        return include_completed->isChecked() || group_has_open_record(group);
    };

    const auto group_is_considered = [&](int group_index, const QSet<int> &selected_indices) {
        if (!group_matches_current_filters(group_index)) {
            return false;
        }
        return !selected_items_radio->isChecked() || selected_indices.contains(group_index);
    };

    bool is_initializing = true;

    const auto refresh_preview = [&]() {
        if (is_initializing) {
            return;
        }

        if (!preview_list_popup.isNull()) {
            preview_list_popup->close();
            preview_list_popup->deleteLater();
            preview_list_popup.clear();
        }

        const QSet<int> selected_indices = selected_group_indices();
        int considered_count = 0;
        int recurring_count = 0;
        for (int i = 0; i < groups.size(); ++i) {
            if (!group_is_considered(i, selected_indices)) {
                continue;
            }
            ++considered_count;
            if (!group_recurrence(groups.at(i)).isEmpty()) {
                ++recurring_count;
            }
        }
        summary->setText(get_text(QStringLiteral(
                             "%1 tarefa(s) serão consideradas. %2 já têm recorrência definida."))
                             .arg(considered_count)
                             .arg(recurring_count));

        preview_table->setRowCount(0);
        int shown = 0;
        const QDateTime window_start =
            recurrence_window_start(application_start_date->date(), application_start_time->time());
        const QDateTime window_end =
            recurrence_window_end(application_end_date->date(), application_end_time->time());
        const QString fallback_recurrence =
            apply_default->isChecked() ? default_recurrence->currentData().toString() : QString();

        for (int i = 0; i < groups.size(); ++i) {
            if (!group_is_considered(i, selected_indices)) {
                continue;
            }
            const TaskRecordGroup &group = groups.at(i);

            QString recurrence = group_recurrence(group);
            QString recurrence_label = recurrence_user_label(recurrence);
            if (recurrence.isEmpty() && !fallback_recurrence.isEmpty()) {
                recurrence = fallback_recurrence;
                recurrence_label = get_text(QStringLiteral("Padrão: %1")).arg(recurrence_user_label(recurrence));
            }
            if (recurrence.isEmpty()) {
                continue;
            }

            const int row = preview_table->rowCount();
            preview_table->insertRow(row);
            preview_table->setItem(row, 0, new QTableWidgetItem(title_for_record(group.primary)));
            preview_table->setItem(row, 1, new QTableWidgetItem(recurrence_label));

            const QTime occurrence_time = occurrence_time_for_group(group);
            const RecurrenceBaseDate base_date = recurrence_base_date_for_group(group);
            const QStringList recurrence_dates = recurrence_date_options(base_date.date,
                                                                         recurrence,
                                                                         window_start,
                                                                         window_end,
                                                                         occurrence_time,
                                                                         base_date.include_base_date,
                                                                         max_per_item->value());
            preview_table->setItem(row, 2, new QTableWidgetItem(recurrence_dates.join(QStringLiteral(", "))));
            ++shown;
        }
        if (shown == 0) {
            preview_table->insertRow(0);
            preview_table->setSpan(0, 0, 1, preview_table->columnCount());
            preview_table->setItem(0,
                                   0,
                                   new QTableWidgetItem(get_text(QStringLiteral(
                                       "Nenhuma tarefa pronta para gerar ocorrências com as configurações atuais."))));
        }
    };

    const auto update_item_model = [&]() {
        const QSet<int> previously_selected = selected_group_indices();
        QVariantList entries;
        for (int i = 0; i < groups.size(); ++i) {
            if (!group_matches_current_filters(i)) {
                continue;
            }
            QVariantMap entry = display_entry_for_group(groups.at(i));
            entry.insert(QStringLiteral("group_index"), i);
            entries.append(entry);
        }

        item_model->set_records(entries);
        item_proxy->setFilterFixedString(item_filter->text());
        if (item_view->selectionModel() != nullptr && !previously_selected.isEmpty()) {
            for (int row = 0; row < item_model->rowCount(); ++row) {
                const QVariantMap entry = item_model->record_at(row);
                const int group_index = entry.value(QStringLiteral("group_index"), -1).toInt();
                if (!previously_selected.contains(group_index)) {
                    continue;
                }
                const QModelIndex source_index = item_model->index(row, 0);
                const QModelIndex proxy_index = item_proxy->mapFromSource(source_index);
                if (proxy_index.isValid()) {
                    item_view->selectionModel()->select(proxy_index,
                                                        QItemSelectionModel::Select | QItemSelectionModel::Rows);
                }
            }
        }
        refresh_preview();
    };

    const auto update_selection_controls = [&]() {
        const bool enabled = selected_items_radio->isChecked();
        item_filter->setEnabled(enabled);
        item_view->setEnabled(enabled);
        select_visible_button->setEnabled(enabled);
        clear_selection_button->setEnabled(enabled);
        refresh_preview();
    };

    QSet<QString> saved_selected_source_ids;
    const auto save_recurrence_dialog_settings = [&]() {
        QJsonObject settings;
        settings.insert(QStringLiteral("selection_mode"),
                        selected_items_radio->isChecked() ? QStringLiteral("selected") : QStringLiteral("all"));
        settings.insert(QStringLiteral("item_filter"), item_filter->text());

        QJsonArray selected_ids;
        const QSet<int> selected_indices = selected_group_indices();
        for (int index : selected_indices) {
            if (index >= 0 && index < groups.size()) {
                selected_ids.append(source_id_for_group(groups.at(index)));
            }
        }
        settings.insert(QStringLiteral("selected_source_ids"), selected_ids);

        settings.insert(QStringLiteral("horizon_days"), horizon_days->value());
        settings.insert(QStringLiteral("max_per_item"), max_per_item->value());
        settings.insert(QStringLiteral("include_completed"), include_completed->isChecked());
        settings.insert(QStringLiteral("avoid_duplicates"), avoid_duplicates->isChecked());
        settings.insert(QStringLiteral("apply_default"), apply_default->isChecked());
        settings.insert(QStringLiteral("default_recurrence"), default_recurrence->currentData().toString());
        settings.insert(QStringLiteral("application_start_date"), application_start_date->date().toString(Qt::ISODate));
        settings.insert(QStringLiteral("application_end_date"), application_end_date->date().toString(Qt::ISODate));
        settings.insert(QStringLiteral("application_start_time"),
                        application_start_time->time().toString(QStringLiteral("HH:mm")));
        settings.insert(QStringLiteral("application_end_time"),
                        application_end_time->time().toString(QStringLiteral("HH:mm")));
        settings.insert(QStringLiteral("use_start_time_when_missing"), use_start_time_when_missing->isChecked());
        settings.insert(QStringLiteral("missing_base_date"), missing_base_date->date().toString(Qt::ISODate));
        const QString missing_time = missing_base_time->currentData().isValid()
                                         ? missing_base_time->currentData().toString()
                                         : missing_base_time->currentText();
        settings.insert(QStringLiteral("missing_base_time"), missing_time);
        write_json_object(recurring_tasks_settings_path(), settings);
    };

    const auto load_recurrence_dialog_settings = [&]() {
        const QJsonObject settings = read_json_object(recurring_tasks_settings_path());
        if (settings.isEmpty()) {
            return;
        }

        selected_items_radio->setChecked(settings.value(QStringLiteral("selection_mode")).toString() == QStringLiteral("selected"));
        all_items_radio->setChecked(!selected_items_radio->isChecked());
        item_filter->setText(settings.value(QStringLiteral("item_filter")).toString());
        item_proxy->setFilterFixedString(item_filter->text());

        saved_selected_source_ids.clear();
        const QJsonArray selected_ids = settings.value(QStringLiteral("selected_source_ids")).toArray();
        for (const QJsonValue &value : selected_ids) {
            const QString id = value.toString().trimmed();
            if (!id.isEmpty()) {
                saved_selected_source_ids.insert(id);
            }
        }

        horizon_days->setValue(qBound(horizon_days->minimum(),
                                      settings.value(QStringLiteral("horizon_days")).toInt(horizon_days->value()),
                                      horizon_days->maximum()));
        max_per_item->setValue(qBound(max_per_item->minimum(),
                                      settings.value(QStringLiteral("max_per_item")).toInt(max_per_item->value()),
                                      max_per_item->maximum()));
        include_completed->setChecked(settings.value(QStringLiteral("include_completed")).toBool(include_completed->isChecked()));
        avoid_duplicates->setChecked(settings.value(QStringLiteral("avoid_duplicates")).toBool(avoid_duplicates->isChecked()));
        apply_default->setChecked(settings.value(QStringLiteral("apply_default")).toBool(apply_default->isChecked()));
        default_recurrence->setEnabled(apply_default->isChecked());
        set_combo_current_data(default_recurrence, settings.value(QStringLiteral("default_recurrence")).toString());

        const QDate loaded_start_date =
            QDate::fromString(settings.value(QStringLiteral("application_start_date")).toString(), Qt::ISODate);
        const QDate loaded_end_date =
            QDate::fromString(settings.value(QStringLiteral("application_end_date")).toString(), Qt::ISODate);
        if (loaded_start_date.isValid() && loaded_end_date.isValid()) {
            set_window_range(loaded_start_date, loaded_end_date);
        }

        const QTime loaded_start_time =
            QTime::fromString(settings.value(QStringLiteral("application_start_time")).toString(),
                              QStringLiteral("HH:mm"));
        if (loaded_start_time.isValid()) {
            application_start_time->setTime(loaded_start_time);
        }
        const QTime loaded_end_time =
            QTime::fromString(settings.value(QStringLiteral("application_end_time")).toString(), QStringLiteral("HH:mm"));
        if (loaded_end_time.isValid()) {
            application_end_time->setTime(loaded_end_time);
        }

        use_start_time_when_missing->setChecked(settings.value(QStringLiteral("use_start_time_when_missing"))
                                                    .toBool(use_start_time_when_missing->isChecked()));
        const QDate loaded_missing_date =
            QDate::fromString(settings.value(QStringLiteral("missing_base_date")).toString(), Qt::ISODate);
        if (loaded_missing_date.isValid()) {
            missing_base_date->setDate(loaded_missing_date);
        }
        const QString loaded_missing_time = settings.value(QStringLiteral("missing_base_time")).toString();
        if (!loaded_missing_time.isEmpty()) {
            const QTime parsed = parse_time(loaded_missing_time);
            if (parsed.isValid()) {
                set_time_combo_current_time(missing_base_time, parsed);
            }
        }
    };

    auto *buttons = new QDialogButtonBox(&dialog);
    auto *apply_button = buttons->addButton(get_text(QStringLiteral("Gerar ocorrências")), QDialogButtonBox::AcceptRole);
    auto *save_only_button =
        buttons->addButton(get_text(QStringLiteral("Salvar definições")), QDialogButtonBox::ApplyRole);
    auto *restore_original_button =
        buttons->addButton(get_text(QStringLiteral("Restaurar tarefas anteriores")), QDialogButtonBox::ActionRole);
    auto *restore_defaults_button =
        buttons->addButton(get_text(QStringLiteral("Restaurar padrão")), QDialogButtonBox::ResetRole);
    auto *cancel_button = buttons->addButton(QDialogButtonBox::Cancel);
    translate_standard_buttons(app, buttons);
    layout->addWidget(buttons);

    const auto restore_item_selection = [&]() {
        if (saved_selected_source_ids.isEmpty() || item_view->selectionModel() == nullptr) {
            return;
        }
        for (int row = 0; row < item_model->rowCount(); ++row) {
            const QVariantMap entry = item_model->record_at(row);
            const int group_index = entry.value(QStringLiteral("group_index"), -1).toInt();
            if (group_index < 0 || group_index >= groups.size()) {
                continue;
            }
            const QString source_id = source_id_for_group(groups.at(group_index));
            if (!saved_selected_source_ids.contains(source_id)) {
                continue;
            }
            const QModelIndex source_index = item_model->index(row, 0);
            const QModelIndex proxy_index = item_proxy->mapFromSource(source_index);
            if (proxy_index.isValid()) {
                item_view->selectionModel()->select(proxy_index,
                                                    QItemSelectionModel::Select | QItemSelectionModel::Rows);
            }
        }
    };

    const auto do_apply_recurrences = [&]() {
        save_recurrence_dialog_settings();

        // Sincroniza a recorrência nos itens de tarefas considerados caso "Aplicar frequência padrão" esteja ativado
        if (apply_default->isChecked()) {
            const QString fallback = default_recurrence->currentData().toString();
            if (!fallback.isEmpty() && app != nullptr) {
                const QSet<int> sel_indices = selected_group_indices();
                bool any_updated = false;
                const QList<QListWidget *> lists = {
                    app->quadrant1_list, app->quadrant1_in_progress_list, app->quadrant1_completed_list,
                    app->quadrant2_list, app->quadrant2_in_progress_list, app->quadrant2_completed_list,
                    app->quadrant3_list, app->quadrant3_in_progress_list, app->quadrant3_completed_list,
                    app->quadrant4_list, app->quadrant4_in_progress_list, app->quadrant4_completed_list,
                    app->eisenhower_doing_list
                };
                for (int i = 0; i < groups.size(); ++i) {
                    if (!group_is_considered(i, sel_indices)) {
                        continue;
                    }
                    const QString source_id = source_id_for_group(groups.at(i));
                    for (QListWidget *lw : lists) {
                        if (!lw) continue;
                        for (int r = 0; r < lw->count(); ++r) {
                            QListWidgetItem *it = lw->item(r);
                            if (!it) continue;
                            QVariantMap d = it->data(Qt::UserRole).toMap();
                            if (d.value(QStringLiteral("id")).toString() == source_id) {
                                d.insert(QStringLiteral("recurrence"), fallback);
                                it->setData(Qt::UserRole, d);
                                any_updated = true;
                            }
                        }
                    }
                }
                if (any_updated) {
                    app->save_tasks_now();
                }
            }
        }

        const QSet<int> selected_indices_for_accept = selected_group_indices();
        if (selected_items_radio->isChecked() && selected_indices_for_accept.isEmpty()) {
            QMessageBox::warning(&dialog,
                                 get_text(QStringLiteral("Recorrências")),
                                 get_text(QStringLiteral("Selecione pelo menos uma tarefa.")));
            return;
        }

        QSet<QString> existing = existing_recurrence_keys(unified_records, map_backup);
        const QDateTime selected_window_start =
            recurrence_window_start(application_start_date->date(), application_start_time->time());
        const QDateTime selected_window_end =
            recurrence_window_end(application_end_date->date(), application_end_time->time());
        if (!selected_window_start.isValid() || !selected_window_end.isValid() || selected_window_end < selected_window_start) {
            QMessageBox::warning(&dialog,
                                 get_text(QStringLiteral("Recorrências")),
                                 get_text(QStringLiteral("O período de aplicação é inválido.")));
            return;
        }
        const QString fallback_recurrence =
            apply_default->isChecked() ? default_recurrence->currentData().toString() : QString();
        const bool use_start_time_for_missing_occurrences = use_start_time_when_missing->isChecked();

        QVariantList additions;
        int total_created = 0;

        for (int i = 0; i < groups.size(); ++i) {
            if (!group_is_considered(i, selected_indices_for_accept)) {
                continue;
            }
            const TaskRecordGroup &group = groups.at(i);

            QString recurrence = group_recurrence(group);
            if (recurrence.isEmpty()) {
                recurrence = fallback_recurrence;
            }
            if (recurrence.isEmpty()) {
                continue;
            }

            const RecurrenceBaseDate base_date = recurrence_base_date_for_group(group);
            if (!base_date.date.isValid()) {
                continue;
            }

            const QDate until = group_recurrence_until(group);
            const QString source_id = source_id_for_group(group);
            QDate next = base_date.include_base_date ? base_date.date : next_recurrence_date(base_date.date, recurrence);
            int generated_for_item = 0;
            int guard = 0;
            QTime occurrence_time = occurrence_time_for_group(group);
            if (!occurrence_time.isValid() && use_start_time_for_missing_occurrences) {
                occurrence_time = time_combo_time(missing_base_time);
            }
            while (next.isValid() && next <= selected_window_end.date() && generated_for_item < max_per_item->value()
                   && guard++ < 120) {
                if (until.isValid() && next > until) {
                    break;
                }

                if (recurrence_occurrence_in_window(next, occurrence_time, selected_window_start, selected_window_end)) {
                    const QString occurrence_id = QUuid::createUuid().toString(QUuid::WithoutBraces);
                    const QString module = QStringLiteral("eisenhower");
                    const QVariantMap source_record = record_for_group_module(group, module);
                    const QString key = recurrence_duplicate_key(module,
                                                                 source_id,
                                                                 title_for_record(source_record),
                                                                 next.toString(Qt::ISODate));
                    if (!avoid_duplicates->isChecked() || !existing.contains(key)) {
                        QString occurrence_time_override;
                        if (!parse_time(time_for_record(source_record)).isValid() && use_start_time_for_missing_occurrences) {
                            const QTime fallback_time = time_combo_time(missing_base_time);
                            if (fallback_time.isValid()) {
                                occurrence_time_override = fallback_time.toString(QStringLiteral("HH:mm"));
                            }
                        }
                        additions.append(recurring_record_copy(source_record,
                                                              module,
                                                              next,
                                                              occurrence_id,
                                                              source_id,
                                                              occurrence_time_override));
                        existing.insert(key);
                        ++total_created;
                    }
                    ++generated_for_item;
                }

                next = next_recurrence_date(next, recurrence);
            }
        }

        if (additions.isEmpty()) {
            QMessageBox::information(&dialog,
                                     get_text(QStringLiteral("Recorrências")),
                                     get_text(QStringLiteral("Nenhuma ocorrência nova foi gerada.")));
            return;
        }

        RecurringTasksUndoSnapshot undo_snapshot;
        undo_snapshot.records_before = unified_records;
        undo_snapshot.mapamental_backup_before = map_backup;
        undo_snapshot.affected_modules = {QStringLiteral("eisenhower")};
        undo_snapshot.created_at = QDateTime::currentDateTime();
        g_recurring_tasks_undo = undo_snapshot;
        save_recurring_tasks_undo_snapshot(undo_snapshot);

        bool saved_records = true;
        QVariantList merged = unified_records;
        for (const QVariant &record : additions) {
            merged.append(record);
        }
        saved_records = source::utils::UnifiedTaskStore::save_records(app->tasks_path, merged);

        if (saved_records) {
            app->reload_all_modules_from_shared_store();
        }
        if (app->calendar_pane != nullptr && app->calendar_pane->calendar_panel != nullptr) {
            app->calendar_pane->calendar_panel->update_task_list();
        }

        unified_records = records_for_app(app);
        groups = collect_productivity_task_groups(app, unified_records, map_backup, all_modules);
        update_item_model();
        refresh_preview();

        const QString header = saved_records
                                   ? get_text(QStringLiteral("%1 ocorrência(s) gerada(s).")).arg(total_created)
                                   : get_text(QStringLiteral("Ocorrências geradas com avisos."));
        QMessageBox result_box(&dialog);
        result_box.setIcon(QMessageBox::Information);
        result_box.setWindowTitle(get_text(QStringLiteral("Recorrências")));
        result_box.setText(header);
        QPushButton *restore_button = result_box.addButton(get_text(QStringLiteral("Restaurar original")),
                                                           QMessageBox::ActionRole);
        result_box.addButton(QMessageBox::Ok);
        translate_standard_buttons(app, &result_box);
        result_box.exec();
        if (result_box.clickedButton() == restore_button) {
            if (show_recurring_tasks_restore_dialog(app)) {
                unified_records = records_for_app(app);
                groups = collect_productivity_task_groups(app, unified_records, map_backup, all_modules);
                update_item_model();
                refresh_preview();
            }
        }
    };

    QObject::connect(apply_button, &QPushButton::clicked, &dialog, do_apply_recurrences);
    QObject::connect(save_only_button, &QPushButton::clicked, &dialog, [&]() {
        save_recurrence_dialog_settings();
        QMessageBox::information(&dialog,
                                 get_text(QStringLiteral("Recorrências")),
                                 get_text(QStringLiteral("Definições registradas.")));
    });
    QObject::connect(restore_original_button, &QPushButton::clicked, &dialog, [&]() {
        if (show_recurring_tasks_restore_dialog(app)) {
            unified_records = records_for_app(app);
            groups = collect_productivity_task_groups(app, unified_records, map_backup, all_modules);
            update_item_model();
            refresh_preview();
        }
    });
    QObject::connect(cancel_button, &QPushButton::clicked, &dialog, &QDialog::reject);
    load_recurrence_dialog_settings();

    QObject::connect(item_filter, &QLineEdit::textChanged, item_proxy, [item_proxy, refresh_preview](const QString &text) {
        item_proxy->setFilterFixedString(text);
        refresh_preview();
    });
    QObject::connect(item_view->selectionModel(),
                     &QItemSelectionModel::selectionChanged,
                     &dialog,
                     [&](const QItemSelection &, const QItemSelection &) { refresh_preview(); });
    QObject::connect(all_items_radio, &QRadioButton::toggled, &dialog, [&](bool) { update_selection_controls(); });
    QObject::connect(selected_items_radio, &QRadioButton::toggled, &dialog, [&](bool) { update_selection_controls(); });
    QObject::connect(select_visible_button, &QPushButton::clicked, &dialog, [&]() {
        selected_items_radio->setChecked(true);
        if (item_view->selectionModel() == nullptr) {
            return;
        }
        for (int row = 0; row < item_proxy->rowCount(); ++row) {
            const QModelIndex index = item_proxy->index(row, 0);
            item_view->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
        refresh_preview();
    });
    QObject::connect(clear_selection_button, &QPushButton::clicked, &dialog, [&]() {
        if (item_view->selectionModel() != nullptr) {
            item_view->selectionModel()->clearSelection();
        }
        refresh_preview();
    });
    QObject::connect(restore_defaults_button, &QPushButton::clicked, &dialog, [&]() {
        all_items_radio->setChecked(true);
        item_filter->clear();
        if (item_view->selectionModel() != nullptr) {
            item_view->selectionModel()->clearSelection();
        }
        horizon_days->setValue(30);
        max_per_item->setValue(8);
        include_completed->setChecked(false);
        avoid_duplicates->setChecked(true);
        apply_default->setChecked(false);
        default_recurrence->setEnabled(false);
        default_recurrence->setCurrentIndex(1);
        set_window_range(QDate::currentDate(), QDate::currentDate().addDays(30));
        application_start_time->setTime(QTime(0, 0));
        application_end_time->setTime(QTime(23, 59));
        use_start_time_when_missing->setChecked(false);
        missing_base_date->setDate(QDate::currentDate());
        missing_base_time->setCurrentIndex(0);
        update_item_model();
        update_selection_controls();
    });

    QObject::connect(horizon_days, &QSpinBox::valueChanged, &dialog, [&](int value) {
        if (application_start_date->date().isValid()) {
            set_window_range(application_start_date->date(), application_start_date->date().addDays(value));
        }
        refresh_preview();
    });
    QObject::connect(max_per_item, &QSpinBox::valueChanged, &dialog, [&](int) { refresh_preview(); });
    QObject::connect(include_completed, &QCheckBox::toggled, &dialog, [&](bool) { update_item_model(); });
    QObject::connect(avoid_duplicates, &QCheckBox::toggled, &dialog, [&](bool) { refresh_preview(); });
    QObject::connect(apply_default, &QCheckBox::toggled, &dialog, [&](bool) { refresh_preview(); });
    QObject::connect(default_recurrence, &QComboBox::currentIndexChanged, &dialog, [&](int) { refresh_preview(); });
    QObject::connect(application_start_date, &QDateEdit::dateChanged, &dialog, [update_calendar_from_inputs, refresh_preview](const QDate &) {
        update_calendar_from_inputs();
        refresh_preview();
    });
    QObject::connect(application_end_date, &QDateEdit::dateChanged, &dialog, [update_calendar_from_inputs, refresh_preview](const QDate &) {
        update_calendar_from_inputs();
        refresh_preview();
    });
    QObject::connect(set_start_button, &QPushButton::clicked, &dialog, [&]() {
        if (calendar_strip->selected_date.isValid()) {
            set_window_range(calendar_strip->selected_date, application_end_date->date());
            refresh_preview();
        }
    });
    QObject::connect(set_end_button, &QPushButton::clicked, &dialog, [&]() {
        if (calendar_strip->selected_date.isValid()) {
            set_window_range(application_start_date->date(), calendar_strip->selected_date);
            refresh_preview();
        }
    });
    QObject::connect(application_start_time, &QTimeEdit::timeChanged, &dialog, [&](const QTime &) { refresh_preview(); });
    QObject::connect(application_end_time, &QTimeEdit::timeChanged, &dialog, [&](const QTime &) { refresh_preview(); });
    QObject::connect(use_start_time_when_missing, &QCheckBox::toggled, &dialog, [&](bool) { refresh_preview(); });
    QObject::connect(missing_base_date, &QDateEdit::dateChanged, &dialog, [&](const QDate &) { refresh_preview(); });
    QObject::connect(missing_base_time, &QComboBox::currentTextChanged, &dialog, [&](const QString &) { refresh_preview(); });

    const auto retranslate_ui = [&]() {
        const QLocale loc = effective_locale(app);

        dialog.setWindowTitle(get_text(QStringLiteral("Gerar próximas recorrências")));
        summary->setText(get_text(QStringLiteral("Gera ocorrências futuras para itens com padrão de repetição cadastrado.")));

        // Left column - items selection
        selection_group->setTitle(get_text(QStringLiteral("Itens")));
        all_items_radio->setText(get_text(QStringLiteral("Todos filtrados")));
        selected_items_radio->setText(get_text(QStringLiteral("Somente selecionados")));
        item_filter->setPlaceholderText(get_text(QStringLiteral("Pesquisar item")));
        select_visible_button->setText(get_text(QStringLiteral("Selecionar visíveis")));
        clear_selection_button->setText(get_text(QStringLiteral("Limpar")));

        // Left column - settings
        settings_group->setTitle(get_text(QStringLiteral("Configurações")));
        horizon_label->setText(get_text(QStringLiteral("Alcance (dias)")));
        max_label->setText(get_text(QStringLiteral("Máximo por item")));
        include_completed->setText(get_text(QStringLiteral("Incluir itens concluídos")));
        avoid_duplicates->setText(get_text(QStringLiteral("Evitar duplicadas existentes")));
        apply_default->setText(get_text(QStringLiteral("Aplicar frequência padrão a itens sem recorrência definida")));
        default_recurrence_label->setText(get_text(QStringLiteral("Frequência padrão")));
        default_recurrence->setItemText(0, get_text(QStringLiteral("Diária")));
        default_recurrence->setItemText(1, get_text(QStringLiteral("Semanal")));
        default_recurrence->setItemText(2, get_text(QStringLiteral("Mensal")));

        // Right column - when
        when_group->setTitle(get_text(QStringLiteral("Quando aplicar")));
        start_date_label->setText(get_text(QStringLiteral("Data inicial")));
        start_time_label->setText(get_text(QStringLiteral("Hora inicial")));
        end_date_label->setText(get_text(QStringLiteral("Data final")));
        end_time_label->setText(get_text(QStringLiteral("Hora final")));
        use_start_time_when_missing->setText(get_text(QStringLiteral("Usar hora inicial quando o item não tiver horário")));
        missing_date_label->setText(get_text(QStringLiteral("Data para itens sem data")));
        missing_time_label->setText(get_text(QStringLiteral("Hora para itens sem horário")));
        set_start_button->setText(get_text(QStringLiteral("Início")));
        set_end_button->setText(get_text(QStringLiteral("Fim")));

        // Localized DateFormats and locales for incremental DateEdit controls
        application_start_date->setLocale(loc);
        application_start_date->setDisplayFormat(loc.dateFormat(QLocale::ShortFormat));
        application_end_date->setLocale(loc);
        application_end_date->setDisplayFormat(loc.dateFormat(QLocale::ShortFormat));
        missing_base_date->setLocale(loc);
        missing_base_date->setDisplayFormat(loc.dateFormat(QLocale::ShortFormat));
        if (calendar_strip != nullptr && calendar_strip->container != nullptr) {
            calendar_strip->container->setLocale(loc);
            update_calendar_strip(calendar_strip);
        }

        // Right column - preview
        preview_group->setTitle(get_text(QStringLiteral("Prévia do que será criado")));
        preview_table->setHorizontalHeaderLabels({get_text(QStringLiteral("Tarefa")),
                                                  get_text(QStringLiteral("Recorrência usada")),
                                                  get_text(QStringLiteral("Próximas datas"))});

        // Dialog buttons
        translate_standard_buttons(app, buttons);
        apply_button->setText(get_text(QStringLiteral("Gerar ocorrências")));
        save_only_button->setText(get_text(QStringLiteral("Salvar definições")));
        restore_original_button->setText(get_text(QStringLiteral("Restaurar tarefas anteriores")));
        restore_defaults_button->setText(get_text(QStringLiteral("Restaurar padrão")));
        cancel_button->setText(get_text(QStringLiteral("Cancelar")));

        refresh_preview();
    };

    dialog.set_retranslate_handler([&]() {
        retranslate_ui();
    });

    if (app != nullptr && app->gerenciador_traducao != nullptr) {
        QObject::connect(app->gerenciador_traducao, &GerenciadorTraducao::idioma_alterado, &dialog, [&]() {
            retranslate_ui();
        });
    }

    retranslate_ui();
    update_item_model();
    restore_item_selection();
    update_selection_controls();

    is_initializing = false;
    refresh_preview();

    dialog.exec();
}

}  // namespace source::InterfaceCore::ProductivityFeatures
