#include "source/ui/ui_14_CalendarPanel.hpp"

#include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QBrush>
#include <QCheckBox>
#include <QColor>
#include <QComboBox>
#include <QCoreApplication>
#include <QDateEdit>
#include <QDesktopServices>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFont>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QPalette>
#include <QResizeEvent>
#include <QShowEvent>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QTextCharFormat>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>
#include <algorithm>
#include <utility>

#include "source/services/incore_13_prioridade_display.hpp"
#include "source/ui/agregador_01_ui.hpp"
#include "source/ui/ui_10_BadgeCalendarWidget.hpp"
#include "source/ui/ui_11_NativeLineEditFrame.hpp"
#include "source/ui/ui_12_CalendarDialog.hpp"
#include "source/ui/ui_16_YearCalendarWidget.hpp"
#include "source/eisenhower_app.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"
#include "source/app/app_02_get_text.hpp"
#include "source/utils/LinkedTitleUnderlineDelegate.hpp"
#include "source/utils/LogManager.hpp"

namespace source::GerenciamentoUI {
namespace {

constexpr int kTasksPanelHorizontalInset = 2;
constexpr int kTasksPanelTopInset = 12;
constexpr int kTasksPanelBottomInset = 4;

source::utils::LogManager *logger()
{
    return source::utils::LogManager::get_logger();
}

QString get_text(const QString &text)
{
    return QCoreApplication::translate("InterfaceGrafica", text.toUtf8().constData());
}

void apply_task_title_appearance(QListWidgetItem *item, const QVariantMap &task)
{
    if (item == nullptr) {
        return;
    }

    const QVariantMap font_config = task.value(QStringLiteral("font_config")).toMap();
    if (!font_config.isEmpty() && item->data(source::utils::kCustomFontSettingsRole).toMap().isEmpty()) {
        item->setData(source::utils::kCustomFontSettingsRole, font_config);
    }

    const bool has_file = !task.value(QStringLiteral("file_path")).toString().trimmed().isEmpty();
    source::utils::apply_task_title_font(item, task.value(QStringLiteral("text")).toString(), has_file);

    const QColor text_color(task.value(QStringLiteral("cor_texto")).toString().trimmed());
    if (text_color.isValid()) {
        item->setForeground(QBrush(text_color));
    } else {
        item->setForeground(QBrush());
    }
}

QString tooltip_for_task(const QVariantMap &task, source::EisenhowerMatrixApp *app)
{
    QStringList lines;
    const QString file_path = task.value(QStringLiteral("file_path")).toString();
    if (!file_path.isEmpty()) {
        lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Arquivo")), file_path);
    }
    const QString date_str = task.value(QStringLiteral("date")).toString();
    if (!date_str.isEmpty()) {
        const QDate qd = QDate::fromString(date_str, Qt::ISODate);
        const QString formatted =
            qd.isValid() && app->date_input != nullptr ? qd.toString(app->date_input->displayFormat()) : date_str;
        lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Data")), formatted);
    }
    const QString time_str = task.value(QStringLiteral("time")).toString();
    if (!time_str.isEmpty()) {
        lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Horário")), time_str);
    }
    const QString desc = task.value(QStringLiteral("description")).toString();
    if (!desc.isEmpty()) {
        lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Descrição")), desc);
    }
    const QVariant prio = task.value(QStringLiteral("priority"));
    if (prio.isValid() && !prio.toString().isEmpty()) {
        lines << QStringLiteral("%1: %2")
                     .arg(get_text(QStringLiteral("Prioridade")),
                          source::InterfaceCore::prioridade_para_texto(prio, app));
    }
    const QVariant domain = task.value(QStringLiteral("dominio"));
    if (domain.isValid() && !domain.toString().trimmed().isEmpty()) {
        lines << QStringLiteral("%1: %2")
                     .arg(get_text(QStringLiteral("Nível de Domínio")),
                          source::InterfaceCore::dominio_para_texto(domain, false));
    }
    const double duration_val = task.contains(QStringLiteral("duracao"))
                                    ? task.value(QStringLiteral("duracao")).toDouble()
                                    : (task.contains(QStringLiteral("duration"))
                                           ? task.value(QStringLiteral("duration")).toDouble()
                                           : 1.0);
    lines << source::app::format_duration_tooltip_line(duration_val > 0.0 ? duration_val : 1.0);
    const int pomodoros = qMax(0, task.value(QStringLiteral("pomodoros"),
                                             task.value(QStringLiteral("pomodoros_completados"), 0)).toInt());
    if (app != nullptr && app->pomodoro_report_pomodoros_calendario_enabled()) {
        lines << QCoreApplication::translate("PomodoroPlugin", "🍅 {n} pomodoros")
                     .replace(QStringLiteral("{n}"), QString::number(pomodoros));
    }
    return lines.join(QLatin1Char('\n'));
}

QString visible_global_task_text(source::EisenhowerMatrixApp *app,
                                 const QString &emoji,
                                 const QString &decorated_title,
                                 const QString &status_text,
                                 const QString &date_text,
                                 const QString &time_str)
{
    if (app == nullptr || !app->reserved_space_eisenhower_enabled() || date_text.trimmed().isEmpty()) {
        return QStringLiteral("%1 %2 — [%3]").arg(emoji, decorated_title, status_text).trimmed();
    }

    const QString dt_str =
        time_str.trimmed().isEmpty() ? date_text.trimmed() : QStringLiteral("%1 %2").arg(date_text.trimmed(), time_str.trimmed());
    return QStringLiteral("%1 %2 — %3 — [%4]").arg(emoji, dt_str, decorated_title, status_text).trimmed();
}

QString global_task_group_label(source::EisenhowerMatrixApp *app, const QString &date_text, const QString &time_str)
{
    if (app == nullptr || !app->reserved_watermark_eisenhower_enabled()) {
        return {};
    }

    QStringList parts;
    if (!date_text.trimmed().isEmpty()) {
        parts << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Data")), date_text.trimmed());
    }
    if (!time_str.trimmed().isEmpty()) {
        parts << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Horário")), time_str.trimmed());
    }
    return parts.join(QStringLiteral("  |  "));
}

QString global_task_group_key(source::EisenhowerMatrixApp *app, const QVariantMap &task)
{
    if (app == nullptr || !app->reserved_watermark_eisenhower_enabled()) {
        return {};
    }

    const QString time_str = task.value(QStringLiteral("time")).toString().trimmed();
    if (time_str.isEmpty()) {
        return {};
    }

    const QString date_str = task.value(QStringLiteral("date")).toString().trimmed();
    const QString date_key = date_str.isEmpty() ? QStringLiteral("9999-99-99") : date_str;
    return QStringLiteral("%1|%2").arg(date_key, time_str);
}

}  // namespace

CalendarPanel::CalendarPanel(source::EisenhowerMatrixApp *app_in, QWidget *parent)
    : QWidget(parent), app(app_in), _date_format(app_in->date_input != nullptr ? app_in->date_input->displayFormat()
                                                                              : QStringLiteral("dd/MM/yyyy")),
      _calendar_view_mode(QStringLiteral("month"))
{
    _year_resize_timer = new QTimer(this);
    _year_resize_timer->setSingleShot(true);
    connect(_year_resize_timer, &QTimer::timeout, this, [this]() { _update_year_calendars_for_width(); });

    auto *main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(5, 4, 0, 4);
    main_layout->setSpacing(6);

    calendar = new BadgeCalendarWidget(this);
    const QDate initial_date = app->date_input != nullptr ? app->date_input->date() : QDate::currentDate();
    _selected_reference_date = initial_date;
    if (initial_date.isValid()) {
        _selected_reference_dates = {initial_date.toString(Qt::ISODate)};
    }
    calendar->setSelectedDate(initial_date);
    calendar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(calendar, &QWidget::customContextMenuRequested, this, [this](const QPoint &pos) {
        _on_any_calendar_context_menu(calendar, pos);
    });

    const QSize hint = calendar->minimumSizeHint();
    const QSize fallback(240, 240);
    const QSize cal_size = hint.expandedTo(fallback);
    _calendar_content_width = qMax(220, cal_size.width());
    calendar->setMinimumSize(cal_size);
    calendar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _apply_locale_to_calendar();

    calendar_group = new QGroupBox(get_text(QStringLiteral("Calendário")), this);
    calendar_group->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    calendar_group->setFixedWidth(_calendar_content_width);
    auto *calendar_group_layout = new QVBoxLayout(calendar_group);
    calendar_group_layout->setContentsMargins(4, 12, 4, 4);
    calendar_group_layout->setSpacing(0);
    calendar_group_layout->addWidget(calendar, 1);

    year_calendar = new YearCalendarWidget(this);
    year_calendar->app = app;
    year_calendar->owner = this;
    year_calendar->setSelectedDate(initial_date);
    connect(year_calendar, &YearCalendarWidget::monthSelected, this, &CalendarPanel::_on_year_month_selected);
    connect(year_calendar, &YearCalendarWidget::monthDoubleClicked, this, &CalendarPanel::_on_year_month_double_clicked);
    _configure_year_calendar_widget(year_calendar);

    year_calendar_group = new QGroupBox(get_text(QStringLiteral("Calendário")), this);
    year_calendar_group->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    year_calendar_group->setFixedWidth(_calendar_content_width);
    auto *year_group_layout = new QVBoxLayout(year_calendar_group);
    year_group_layout->setContentsMargins(4, 12, 4, 4);
    year_group_layout->setSpacing(0);
    year_group_layout->addWidget(year_calendar, 1);

    months_container = new QWidget(this);
    months_layout = new QHBoxLayout(months_container);
    months_layout->setContentsMargins(0, 0, 0, 0);
    months_layout->setSpacing(6);
    months_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    months_layout->addWidget(calendar_group, 0, Qt::AlignLeft | Qt::AlignTop);

    years_container = new QWidget(this);
    years_layout = new QHBoxLayout(years_container);
    years_layout->setContentsMargins(0, 0, 0, 0);
    years_layout->setSpacing(6);
    years_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    years_layout->addWidget(year_calendar_group, 0, Qt::AlignLeft | Qt::AlignTop);

    calendar_stack = new QStackedWidget(this);
    calendar_stack->addWidget(months_container);
    calendar_stack->addWidget(years_container);
    calendar_stack->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    main_layout->addWidget(calendar_stack);

    controls_container = new QWidget(this);
    auto *controls_layout = new QHBoxLayout(controls_container);
    controls_layout->setContentsMargins(0, 0, 0, 0);
    controls_layout->setSpacing(6);
    filter_label = new QLabel(get_text(QStringLiteral("Exibir por")), this);
    controls_layout->addWidget(filter_label);

    filter_combo = new QComboBox(this);
    filter_combo->addItem(get_text(QStringLiteral("Dia")), QStringLiteral("day"));
    filter_combo->addItem(get_text(QStringLiteral("Semana")), QStringLiteral("week"));
    filter_combo->addItem(get_text(QStringLiteral("Mês")), QStringLiteral("month"));
    filter_combo->addItem(get_text(QStringLiteral("Ano")), QStringLiteral("year"));
    controls_layout->addWidget(filter_combo);
    controls_layout->addStretch();

    show_label = new QLabel(get_text(QStringLiteral("Mostrar:")), this);
    controls_layout->addWidget(show_label);

    show_priority_checkbox = new QCheckBox(get_text(QStringLiteral("Prioridade")), this);
    show_priority_checkbox->setChecked(true);
    connect(show_priority_checkbox, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState) { update_task_list(); });
    controls_layout->addWidget(show_priority_checkbox);

    show_domain_checkbox = new QCheckBox(get_text(QStringLiteral("Domínio")), this);
    show_domain_checkbox->setChecked(true);
    connect(show_domain_checkbox, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState) { update_task_list(); });
    controls_layout->addWidget(show_domain_checkbox);

    controls_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    main_layout->addWidget(controls_container);

    tasks_group = new QGroupBox(get_text(QStringLiteral("Lista Global de Tarefas:")), this);
    tasks_group->setFlat(false);
    tasks_group->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    auto *tasks_group_layout = new QVBoxLayout(tasks_group);
    tasks_group_layout->setContentsMargins(kTasksPanelHorizontalInset,
                                           kTasksPanelTopInset,
                                           kTasksPanelHorizontalInset,
                                           kTasksPanelBottomInset);
    tasks_group_layout->setSpacing(6);

    tasks_list = new QListWidget(tasks_group);
    tasks_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tasks_list->setSelectionBehavior(QAbstractItemView::SelectItems);
    tasks_list->setSelectionRectVisible(true);
    tasks_list->setMouseTracking(true);
    tasks_list->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    tasks_list->setMinimumHeight(0);
    tasks_list->setWordWrap(true);
    tasks_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tasks_list->setContextMenuPolicy(Qt::CustomContextMenu);
    source::utils::install_linked_title_underline_delegate(tasks_list);
    tasks_list->installEventFilter(this);
    if (tasks_list->viewport() != nullptr) {
        tasks_list->viewport()->installEventFilter(this);
    }
    connect(tasks_list, &QWidget::customContextMenuRequested, this, [this](const QPoint &pt) {
        if (tasks_list == nullptr) {
            return;
        }
        QListWidgetItem *item = tasks_list->itemAt(pt);
        if (item == nullptr || !(item->flags() & Qt::ItemIsSelectable)) {
            return;
        }
        app->show_context_menu(pt, tasks_list);
    });
    connect(tasks_list, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
        const QVariantMap data = item != nullptr ? item->data(Qt::UserRole).toMap() : QVariantMap();
        const QString path = data.value(QStringLiteral("file_path")).toString();
        if (!path.isEmpty()) {
            const auto source_entry = source::GerenciamentoUI::task_utils::find_source_item_for_calendar(app, data);
            if (source_entry.second != nullptr) {
                app->handle_linked_file_open_request(source_entry.second, this);
            } else {
                QMessageBox::warning(this, get_text(QStringLiteral("Erro")), get_text(QStringLiteral("Tarefa não encontrada.")));
            }
        }
    });
    tasks_group_layout->addWidget(tasks_list, 1);
    main_layout->addWidget(tasks_group, 1);

    connect(calendar, &QCalendarWidget::selectionChanged, this, &CalendarPanel::_on_primary_calendar_selection_changed);
    connect(calendar, &QCalendarWidget::clicked, this, [this](const QDate &qd) { _on_any_calendar_date_clicked(calendar, qd); });
    connect(calendar, &QCalendarWidget::currentPageChanged, this, &CalendarPanel::_on_primary_calendar_page_changed);
    connect(filter_combo, &QComboBox::currentIndexChanged, this, [this]() { _on_filter_mode_changed(); });

    if (app->gerenciador_traducao != nullptr) {
        connect(app->gerenciador_traducao,
                &source::language::GerenciadorTraducao::idioma_alterado,
                this,
                [this](const QString &) { _on_language_changed(); });
    }

    _apply_locale_to_calendar();
    set_calendar_content_width(_calendar_content_width);
    _update_month_calendars_for_width();
    _update_year_calendars_for_width();
    _on_filter_mode_changed();
    update_task_list();
}

void CalendarPanel::set_calendar_content_width(int width)
{
    _calendar_content_width = qMax(220, width);

    if (controls_container != nullptr) {
        controls_container->setMinimumWidth(_calendar_content_width);
        controls_container->setMaximumWidth(QWIDGETSIZE_MAX);
    }
    if (calendar_stack != nullptr) {
        calendar_stack->setMinimumWidth(_calendar_content_width);
    }

    if (_is_month_view_active()) {
        _update_month_calendars_for_width();
        _sync_month_calendar_pages_from_primary();
    }
    if (_is_year_view_active()) {
        _update_year_calendars_for_width();
        _sync_year_calendar_pages_from_primary();
    }
    updateGeometry();
}

void CalendarPanel::set_month_step(int step)
{
    const int new_step = qMax(1, step);
    if (_month_step == new_step) {
        return;
    }
    _month_step = new_step;
    if (_is_month_view_active()) {
        _sync_month_calendar_pages_from_primary();
        _sync_month_calendars_visual_data();
    }
}

int CalendarPanel::month_step() const
{
    return _month_step;
}

void CalendarPanel::update_calendars_for_width()
{
    if (_is_month_view_active()) {
        _update_month_calendars_for_width();
    }
    if (_is_year_view_active()) {
        _update_year_calendars_for_width();
    }
}

void CalendarPanel::resizeEvent(QResizeEvent *event)
{
    if (_is_month_view_active()) {
        _update_month_calendars_for_width();
    }
    if (_is_year_view_active()) {
        const int target_count = _desired_year_calendar_count();
        _reconcile_year_calendar_count(target_count, false);
        _apply_year_calendar_width_distribution();
        _schedule_year_calendars_update(80);
    }
    QWidget::resizeEvent(event);
}

void CalendarPanel::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (_is_month_view_active()) {
        QTimer::singleShot(0, this, [this]() { _update_month_calendars_for_width(); });
    }
    if (_is_year_view_active()) {
        QTimer::singleShot(0, this, [this]() { _update_year_calendars_for_width(); });
    }
}

bool CalendarPanel::_is_month_view_active() const
{
    return calendar_stack != nullptr && calendar_stack->currentWidget() == months_container;
}

bool CalendarPanel::_is_year_view_active() const
{
    return calendar_stack != nullptr && calendar_stack->currentWidget() == years_container;
}

void CalendarPanel::_schedule_year_calendars_update(int delay_ms)
{
    if (_year_resize_timer != nullptr) {
        _year_resize_timer->stop();
        _year_resize_timer->start(qMax(0, delay_ms));
    } else {
        _update_year_calendars_for_width();
    }
}

int CalendarPanel::_available_calendar_width() const
{
    const QMargins margins = layout() != nullptr ? layout()->contentsMargins() : QMargins();
    return qMax(0, width() - margins.left() - margins.right());
}

int CalendarPanel::_available_months_inner_width() const
{
    const int avail = _available_calendar_width();
    if (avail <= 0) {
        return qMax(1, _calendar_content_width);
    }
    return avail;
}

int CalendarPanel::_available_years_inner_width() const
{
    const int avail = _available_calendar_width();
    if (avail <= 0) {
        return qMax(1, _calendar_content_width);
    }
    return avail;
}

int CalendarPanel::_desired_month_calendar_count() const
{
    const int spacing = months_layout != nullptr ? qMax(0, months_layout->spacing()) : 0;
    return qMax(1, (_available_months_inner_width() + spacing) / (_calendar_content_width + spacing));
}

void CalendarPanel::_apply_month_calendar_width_distribution()
{
    QList<QGroupBox *> groups = {calendar_group};
    groups.append(_extra_month_groups);
    groups.erase(std::remove(groups.begin(), groups.end(), nullptr), groups.end());
    if (groups.isEmpty()) {
        return;
    }

    const int spacing = months_layout != nullptr ? qMax(0, months_layout->spacing()) : 0;
    const int available = _available_months_inner_width();
    const int total_spacing = spacing * qMax(0, groups.size() - 1);
    const int distributable = qMax(0, available - total_spacing);
    if (distributable <= 0) {
        return;
    }

    const int base_width = distributable / groups.size();
    const int remainder = distributable % groups.size();
    for (int index = 0; index < groups.size(); ++index) {
        groups.at(index)->setFixedWidth(qMax(1, base_width + (index < remainder ? 1 : 0)));
    }
}

void CalendarPanel::_create_extra_month_calendar()
{
    auto *extra_group = new QGroupBox(get_text(QStringLiteral("Calendário")), months_container);
    extra_group->setSizePolicy(_calendar_only_mode ? QSizePolicy::Expanding : QSizePolicy::Preferred,
                               _calendar_only_mode ? QSizePolicy::Maximum : QSizePolicy::Preferred);
    extra_group->setFixedWidth(_calendar_content_width);
    auto *extra_layout = new QVBoxLayout(extra_group);
    extra_layout->setContentsMargins(4, 12, 4, 4);
    extra_layout->setSpacing(0);

    auto *extra_calendar = new BadgeCalendarWidget(extra_group);
    const QSize hint = extra_calendar->minimumSizeHint();
    const QSize fallback(240, 240);
    const QSize cal_size = hint.expandedTo(fallback);
    extra_calendar->setMinimumSize(cal_size);
    extra_calendar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    extra_calendar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(extra_calendar, &QWidget::customContextMenuRequested, this, [this, extra_calendar](const QPoint &pos) {
        _on_any_calendar_context_menu(extra_calendar, pos);
    });
    extra_calendar->setLocale(calendar->locale());
    extra_calendar->setFirstDayOfWeek(calendar->firstDayOfWeek());
    connect(extra_calendar, &QCalendarWidget::selectionChanged, this, [this, extra_calendar]() {
        _on_extra_calendar_selection_changed(extra_calendar);
    });
    connect(extra_calendar, &QCalendarWidget::clicked, this, [this, extra_calendar](const QDate &qd) {
        _on_any_calendar_date_clicked(extra_calendar, qd);
    });
    connect(extra_calendar, &QCalendarWidget::currentPageChanged, this, [this, extra_calendar](int year, int month) {
        _on_extra_calendar_page_changed(extra_calendar, year, month);
    });

    if (calendar != nullptr) {
        extra_calendar->_task_counts_by_date = calendar->_task_counts_by_date;
        extra_calendar->_task_counts_by_julian_day = calendar->_task_counts_by_julian_day;
        extra_calendar->_task_counts = calendar->_task_counts;
        extra_calendar->_task_counts_by_week = calendar->_task_counts_by_week;
        extra_calendar->_week_counts = calendar->_week_counts;
        extra_calendar->_task_counts_by_month = calendar->_task_counts_by_month;
        extra_calendar->_month_counts = calendar->_month_counts;
        extra_calendar->set_task_tooltip_map(calendar->_tasks_by_date);
        extra_calendar->set_tasks_by_date_data(calendar->_tasks_by_date_data);
    }

    extra_layout->addWidget(extra_calendar, 1);
    months_layout->addWidget(extra_group, 0, Qt::AlignLeft | Qt::AlignTop);
    _extra_month_calendars.append(extra_calendar);
    _extra_month_groups.append(extra_group);
}

void CalendarPanel::_remove_last_extra_month_calendar()
{
    if (_extra_month_calendars.isEmpty()) {
        return;
    }
    BadgeCalendarWidget *extra_calendar = _extra_month_calendars.takeLast();
    QGroupBox *extra_group = !_extra_month_groups.isEmpty() ? _extra_month_groups.takeLast() : nullptr;
    if (months_layout != nullptr && extra_group != nullptr) {
        months_layout->removeWidget(extra_group);
    }
    if (extra_group != nullptr) {
        extra_group->deleteLater();
    } else if (extra_calendar != nullptr) {
        extra_calendar->deleteLater();
    }
}

void CalendarPanel::_ensure_month_calendar_count(int target_count)
{
    target_count = qMax(1, target_count);
    int current_count = 1 + _extra_month_calendars.size();
    while (current_count < target_count) {
        _create_extra_month_calendar();
        ++current_count;
    }
    while (current_count > target_count) {
        _remove_last_extra_month_calendar();
        --current_count;
    }
}

void CalendarPanel::_update_month_calendars_for_width()
{
    const int old_count = calendar_count();
    _ensure_month_calendar_count(_desired_month_calendar_count());
    _apply_month_calendar_width_distribution();
    _sync_month_calendar_pages_from_primary();
    _sync_month_calendars_visual_data();
    const int new_count = calendar_count();
    if (new_count != old_count) {
        emit calendar_count_changed(new_count);
    }
}

int CalendarPanel::_desired_year_calendar_count() const
{
    return qMax(1, _available_years_inner_width() / qMax(1, _calendar_content_width));
}

void CalendarPanel::_apply_year_calendar_width_distribution()
{
    QList<QGroupBox *> groups = {year_calendar_group};
    groups.append(_extra_year_groups);
    groups.erase(std::remove(groups.begin(), groups.end(), nullptr), groups.end());
    if (groups.isEmpty()) {
        return;
    }

    const int spacing = years_layout != nullptr ? qMax(0, years_layout->spacing()) : 0;
    const int available = _available_years_inner_width();
    const int total_spacing = spacing * qMax(0, groups.size() - 1);
    const int distributable = qMax(0, available - total_spacing);
    if (distributable <= 0) {
        return;
    }

    const int base_width = distributable / groups.size();
    const int remainder = distributable % groups.size();
    for (int index = 0; index < groups.size(); ++index) {
        groups.at(index)->setFixedWidth(qMax(1, base_width + (index < remainder ? 1 : 0)));
    }
}

void CalendarPanel::_configure_year_calendar_widget(YearCalendarWidget *year_calendar_widget)
{
    if (year_calendar_widget == nullptr) {
        return;
    }
    const QSize hint = year_calendar_widget->minimumSizeHint();
    const QSize fallback(240, qMax(240, hint.height()));
    year_calendar_widget->setMinimumSize(hint.expandedTo(fallback));
    year_calendar_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void CalendarPanel::_create_extra_year_calendar()
{
    YearCalendarWidget *extra_year_calendar = nullptr;
    QGroupBox *extra_year_group = nullptr;
    while (!_year_calendar_pool.isEmpty()) {
        extra_year_calendar = _year_calendar_pool.takeLast();
        extra_year_group = !_year_group_pool.isEmpty() ? _year_group_pool.takeLast() : nullptr;
        if (extra_year_calendar != nullptr) {
            break;
        }
    }

    if (extra_year_calendar == nullptr) {
        extra_year_group = new QGroupBox(get_text(QStringLiteral("Calendário")), years_container);
        extra_year_group->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        auto *extra_layout = new QVBoxLayout(extra_year_group);
        extra_layout->setContentsMargins(4, 12, 4, 4);
        extra_layout->setSpacing(0);

        extra_year_calendar = new YearCalendarWidget(extra_year_group);
        extra_layout->addWidget(extra_year_calendar, 1);

        connect(extra_year_calendar, &YearCalendarWidget::monthSelected, this, &CalendarPanel::_on_year_month_selected);
        connect(extra_year_calendar,
                &YearCalendarWidget::monthDoubleClicked,
                this,
                &CalendarPanel::_on_year_month_double_clicked);
    } else {
        if (extra_year_group != nullptr) {
            extra_year_group->setParent(years_container);
            extra_year_group->setVisible(true);
        }
        extra_year_calendar->setVisible(true);
    }

    extra_year_calendar->app = app;
    extra_year_calendar->owner = this;
    _configure_year_calendar_widget(extra_year_calendar);
    if (extra_year_group != nullptr) {
        extra_year_group->setFixedWidth(_calendar_content_width);
    }
    extra_year_calendar->setLocale(calendar->locale());
    if (extra_year_group != nullptr) {
        years_layout->addWidget(extra_year_group, 0, Qt::AlignLeft | Qt::AlignTop);
        _extra_year_groups.append(extra_year_group);
    }
    _extra_year_calendars.append(extra_year_calendar);
}

void CalendarPanel::_remove_last_extra_year_calendar()
{
    if (_extra_year_calendars.isEmpty()) {
        return;
    }
    YearCalendarWidget *extra = _extra_year_calendars.takeLast();
    QGroupBox *extra_grp = !_extra_year_groups.isEmpty() ? _extra_year_groups.takeLast() : nullptr;
    if (years_layout != nullptr && extra_grp != nullptr) {
        years_layout->removeWidget(extra_grp);
    }
    if (extra != nullptr) {
        extra->setVisible(false);
        extra->setParent(this);
        _year_calendar_pool.append(extra);
    }
    if (extra_grp != nullptr) {
        extra_grp->setVisible(false);
        extra_grp->setParent(this);
        _year_group_pool.append(extra_grp);
    }
}

bool CalendarPanel::_ensure_year_calendar_count(int target_count)
{
    target_count = qMax(1, target_count);
    int current_count = 1 + _extra_year_calendars.size();
    bool changed = false;
    while (current_count < target_count) {
        _create_extra_year_calendar();
        ++current_count;
        changed = true;
    }
    while (current_count > target_count) {
        _remove_last_extra_year_calendar();
        --current_count;
        changed = true;
    }
    return changed;
}

bool CalendarPanel::_reconcile_year_calendar_count(int target_count, bool sync_after_change)
{
    const int current_count = 1 + _extra_year_calendars.size();
    bool count_changed = false;
    if (target_count != current_count) {
        years_container->setUpdatesEnabled(false);
        count_changed = _ensure_year_calendar_count(target_count);
        years_container->setUpdatesEnabled(true);
    }

    if (count_changed && !sync_after_change) {
        _year_sync_pending = true;
        return true;
    }

    const bool should_sync = sync_after_change && (count_changed || _year_sync_pending);
    if (should_sync) {
        _sync_year_calendar_pages_from_primary();
        _sync_year_calendars_visual_data();
        _year_sync_pending = false;
    }
    return count_changed;
}

void CalendarPanel::_update_year_calendars_for_width()
{
    if (!_is_year_view_active()) {
        return;
    }
    _reconcile_year_calendar_count(_desired_year_calendar_count(), true);
    _apply_year_calendar_width_distribution();
}

bool CalendarPanel::_ctrl_multi_select_active() const
{
    return bool(QApplication::keyboardModifiers() & Qt::ControlModifier);
}

void CalendarPanel::_update_reference_selection(const QDate &qdate, bool allow_multi_on_ctrl)
{
    if (!qdate.isValid()) {
        return;
    }

    const QString selected_iso = qdate.toString(Qt::ISODate);
    QStringList current = _selected_reference_dates;
    const bool ctrl_pressed = allow_multi_on_ctrl && _ctrl_multi_select_active();

    if (ctrl_pressed) {
        if (!current.contains(selected_iso)) {
            current.append(selected_iso);
        }
    } else {
        current = {selected_iso};
    }

    _selected_reference_dates = current;
    _selected_reference_date = qdate;
}

void CalendarPanel::_on_any_calendar_date_clicked(BadgeCalendarWidget *calendar_widget, const QDate &qdate)
{
    if (_syncing_month_calendars || !qdate.isValid()) {
        return;
    }

    _update_reference_selection(qdate, true);
    if (year_calendar != nullptr) {
        year_calendar->setSelectedDate(qdate);
    }
    if (calendar_widget == calendar) {
        _sync_month_calendar_pages_from_primary();
    }
    update_task_list();
}

void CalendarPanel::_on_primary_calendar_selection_changed()
{
    if (_syncing_month_calendars) {
        return;
    }
    const QDate selected = calendar->selectedDate();
    if (selected.isValid()) {
        _update_reference_selection(selected, true);
    }
    _sync_month_calendar_pages_from_primary();
    update_task_list();
}

void CalendarPanel::_on_primary_calendar_page_changed(int year, int month)
{
    if (_syncing_month_calendars) {
        return;
    }

    const QDate selected = calendar->selectedDate();
    const int day = selected.isValid() ? selected.day() : 1;
    const int max_day = QDate(year, month, 1).daysInMonth();
    const QDate target_date(year, month, qMin(qMax(day, 1), max_day));

    if (selected != target_date) {
        calendar->setSelectedDate(target_date);
        return;
    }

    if (target_date.isValid()) {
        _update_reference_selection(target_date, false);
    }
    _sync_month_calendar_pages_from_primary();
    update_task_list();
}

void CalendarPanel::_on_extra_calendar_selection_changed(BadgeCalendarWidget *calendar_widget)
{
    if (_syncing_month_calendars || calendar_widget == nullptr) {
        return;
    }
    const QDate selected = calendar_widget->selectedDate();
    if (!selected.isValid()) {
        return;
    }
    _update_reference_selection(selected, true);
    if (year_calendar != nullptr) {
        year_calendar->setSelectedDate(selected);
    }
    update_task_list();
}

void CalendarPanel::_on_extra_calendar_page_changed(BadgeCalendarWidget *calendar_widget, int year, int month)
{
    if (_syncing_month_calendars || calendar_widget == nullptr) {
        return;
    }

    // Find the index of this extra calendar in the list
    const int idx = _extra_month_calendars.indexOf(calendar_widget);
    if (idx < 0) {
        return;
    }

    // Extra calendar at index idx displays base_month + (idx + 1) * _month_step.
    // Derive the primary calendar's target month from that relationship.
    const int offset = (idx + 1) * _month_step;
    const QDate extra_anchor(year, month, 1);
    const QDate primary_anchor = extra_anchor.addMonths(-offset);

    // Update the primary calendar to the derived month
    const QDate primary_selected = calendar->selectedDate();
    const int day = primary_selected.isValid() ? primary_selected.day() : 1;
    const int max_day = QDate(primary_anchor.year(), primary_anchor.month(), 1).daysInMonth();
    const QDate new_primary_date(primary_anchor.year(), primary_anchor.month(), qMin(qMax(day, 1), max_day));

    _syncing_month_calendars = true;
    calendar->setCurrentPage(primary_anchor.year(), primary_anchor.month());
    if (new_primary_date.isValid() && calendar->selectedDate() != new_primary_date) {
        calendar->setSelectedDate(new_primary_date);
    }
    _syncing_month_calendars = false;

    // Now sync all extra calendars from the (updated) primary
    _sync_month_calendar_pages_from_primary();

    // Update reference selection and year calendar
    const QDate selected = calendar_widget->selectedDate();
    if (selected.isValid()) {
        _update_reference_selection(selected, false);
        if (year_calendar != nullptr) {
            year_calendar->setSelectedDate(selected);
        }
    }
    update_task_list();
}

void CalendarPanel::_sync_month_calendar_pages_from_primary()
{
    if (_syncing_month_calendars || _extra_month_calendars.isEmpty()) {
        return;
    }

    int year = calendar != nullptr ? calendar->yearShown() : 0;
    int month = calendar != nullptr ? calendar->monthShown() : 0;
    if (year <= 0 || month <= 0) {
        QDate base_date = calendar != nullptr ? calendar->selectedDate() : QDate();
        if (!base_date.isValid()) {
            base_date = QDate::currentDate();
        }
        year = base_date.year();
        month = base_date.month();
    }
    const QDate base_anchor(year, month, 1);
    QMap<BadgeCalendarWidget *, int> preserved_days;
    for (BadgeCalendarWidget *extra : _extra_month_calendars) {
        const QDate selected = extra->selectedDate();
        if (selected.isValid()) {
            preserved_days.insert(extra, selected.day());
        }
    }

    _syncing_month_calendars = true;
    for (int idx = 0; idx < _extra_month_calendars.size(); ++idx) {
        BadgeCalendarWidget *extra = _extra_month_calendars.at(idx);
        const QDate month_anchor = base_anchor.addMonths((idx + 1) * _month_step);
        extra->setCurrentPage(month_anchor.year(), month_anchor.month());
        const int preferred_day = preserved_days.value(extra, 1);
        const int max_day = month_anchor.daysInMonth();
        const QDate target_date(month_anchor.year(), month_anchor.month(), qMin(qMax(preferred_day, 1), max_day));
        const QDate current_selected = extra->selectedDate();
        if (target_date.isValid() && current_selected != target_date) {
            extra->setSelectedDate(target_date);
        }
    }
    _syncing_month_calendars = false;
}

void CalendarPanel::_sync_month_calendars_visual_data()
{
    if (_extra_month_calendars.isEmpty()) {
        _extra_highlighted_dates.clear();
        return;
    }

    const auto task_counts = calendar->_task_counts_by_date;
    const auto task_counts_jd = calendar->_task_counts_by_julian_day;
    const auto week_counts = calendar->_task_counts_by_week;
    const auto month_counts = calendar->_task_counts_by_month;
    const auto tooltip_map = calendar->_tasks_by_date;
    const auto tasks_by_date_data = calendar->_tasks_by_date_data;

    for (BadgeCalendarWidget *extra : _extra_month_calendars) {
        extra->_task_counts_by_date = task_counts;
        extra->_task_counts_by_julian_day = task_counts_jd;
        extra->_task_counts = task_counts;
        extra->_task_counts_by_week = week_counts;
        extra->_week_counts = week_counts;
        extra->_task_counts_by_month = month_counts;
        extra->_month_counts = month_counts;
        extra->set_task_tooltip_map(tooltip_map);
        extra->set_tasks_by_date_data(tasks_by_date_data);
        extra->refresh_all_cells();
    }

    const QSet<QString> new_dates = _highlighted_dates;
    const QSet<QString> to_clear = _extra_highlighted_dates - new_dates;
    const QTextCharFormat blank;
    for (const QString &ds : to_clear) {
        const QDate qd = QDate::fromString(ds, Qt::ISODate);
        if (qd.isValid()) {
            for (BadgeCalendarWidget *extra : _extra_month_calendars) {
                extra->setDateTextFormat(qd, blank);
            }
        }
    }

    QTextCharFormat fmt;
    const QColor base = palette().highlight().color();
    fmt.setBackground(QBrush(QColor(base.red(), base.green(), base.blue(), 60)));
    fmt.setFontWeight(QFont::Bold);

    for (const QString &ds : new_dates) {
        const QDate qd = QDate::fromString(ds, Qt::ISODate);
        if (qd.isValid()) {
            for (BadgeCalendarWidget *extra : _extra_month_calendars) {
                extra->setDateTextFormat(qd, fmt);
            }
        }
    }

    _extra_highlighted_dates = new_dates;
}

void CalendarPanel::_on_any_calendar_context_menu(BadgeCalendarWidget *calendar_widget, const QPoint &pos)
{
    if (calendar_widget == nullptr) {
        return;
    }
    calendar_widget->setProperty("_context_pos", pos);
    _on_calendar_context_menu(pos);
}

QList<BadgeCalendarWidget *> CalendarPanel::_iter_calendars() const
{
    QList<BadgeCalendarWidget *> calendars = {calendar};
    calendars.append(_extra_month_calendars);
    return calendars;
}

QList<QVariantMap> CalendarPanel::_collect_tasks() const
{
    return task_utils::collect_tasks_from_app(app);
}

void CalendarPanel::_rebuild_task_date_index()
{
    if (_task_index_dirty || _task_index_records.isEmpty()) {
        _task_index_records = _collect_tasks();
        _task_index_dirty = false;
    }
    _task_index_by_date.clear();
    _task_index_dates.clear();
    for (const QVariantMap &task : std::as_const(_task_index_records)) {
        const QString date_str = task.value(QStringLiteral("date")).toString();
        if (date_str.isEmpty()) {
            continue;
        }
        _task_index_dates.insert(date_str);
        _task_index_by_date[date_str].append(task);
    }
}

void CalendarPanel::update_task_list()
{
    try {
        tasks_list->clear();
        _rebuild_task_date_index();
        const bool show_priority = show_priority_checkbox != nullptr && show_priority_checkbox->isChecked();
        const bool show_domain = show_domain_checkbox != nullptr && show_domain_checkbox->isChecked();
        setProperty("show_priority_checked", show_priority);
        setProperty("show_domain_checked", show_domain);
        _apply_highlighted_dates();
        _update_badge_counts();

        QDate selected_date = get_filter_reference_date();
        QStringList selected_dates = get_filter_reference_dates();
        const QSet<QString> selected_dates_set(selected_dates.begin(), selected_dates.end());
        const QString filter_mode = filter_combo->currentData().toString();
        const QList<QVariantMap> tasks_all = _task_index_records;

        QMap<QString, QStringList> tooltip_map;
        QMap<QString, QList<QVariantMap>> tasks_by_date_data;
        QMap<QPair<int, int>, QStringList> month_tooltip_map;

        for (const QVariantMap &task : tasks_all) {
            const QString ds = task.value(QStringLiteral("date")).toString();
            if (ds.isEmpty()) {
                continue;
            }
            const QString text = task.value(QStringLiteral("text")).toString().trimmed();
            if (text.isEmpty()) {
                continue;
            }

            const QString time_str = task.value(QStringLiteral("time")).toString();
            const QString emoji = show_priority ? task_utils::priority_emoji(task.value(QStringLiteral("priority")), app)
                                                : QString();
            const QString domain_badge =
                show_domain ? source::InterfaceCore::dominio_insignia(task.value(QStringLiteral("dominio"))) : QString();
            const QString display_title = text;
            const QString decorated_text =
                domain_badge.isEmpty() ? display_title : QStringLiteral("%1 %2").arg(domain_badge, display_title);
            const QString status_text = task_utils::task_status_text(task);
            const QString line =
                time_str.isEmpty()
                    ? QStringLiteral("%1 %2 (%3)").arg(emoji, decorated_text, status_text).trimmed()
                    : QStringLiteral("%1 %2 — %3 (%4)").arg(emoji, time_str, decorated_text, status_text).trimmed();
            tooltip_map[ds].append(line);
            tasks_by_date_data[ds].append(task);

            const QDate qd = QDate::fromString(ds, Qt::ISODate);
            if (qd.isValid()) {
                const QString date_for_month = qd.toString(_date_format);
                const QString month_line =
                    time_str.isEmpty()
                        ? QStringLiteral("%1 %2 — %3 (%4)").arg(emoji, date_for_month, decorated_text, status_text).trimmed()
                        : QStringLiteral("%1 %2 %3 — %4 (%5)")
                              .arg(emoji, date_for_month, time_str, decorated_text, status_text)
                              .trimmed();
                month_tooltip_map[qMakePair(qd.year(), qd.month())].append(month_line);
            }
        }

        calendar->set_task_tooltip_map(tooltip_map);
        calendar->set_tasks_by_date_data(tasks_by_date_data);
        if (filter_mode == QStringLiteral("month") || filter_mode == QStringLiteral("year")) {
            year_calendar->set_month_tooltip_map(month_tooltip_map);
        } else {
            year_calendar->set_month_tooltip_map({});
        }

        QList<QPair<QDate, QVariantMap>> filtered;
        for (const QVariantMap &task : tasks_all) {
            const QString date_str = task.value(QStringLiteral("date")).toString();
            const QDate qdate = QDate::fromString(date_str, Qt::ISODate);
            if (!qdate.isValid()) {
                continue;
            }

            if (filter_mode == QStringLiteral("day")) {
                if (!selected_dates_set.isEmpty()) {
                    if (!selected_dates_set.contains(date_str)) {
                        continue;
                    }
                } else if (qdate != selected_date) {
                    continue;
                }
            }

            if (filter_mode == QStringLiteral("week")) {
                const QDate selected_start = selected_date.addDays(-(selected_date.dayOfWeek() % 7));
                const QDate task_start = qdate.addDays(-(qdate.dayOfWeek() % 7));
                if (selected_start != task_start) {
                    continue;
                }
            }

            if (filter_mode == QStringLiteral("month")
                && (qdate.month() != selected_date.month() || qdate.year() != selected_date.year())) {
                continue;
            }

            if (filter_mode == QStringLiteral("year") && qdate.year() != selected_date.year()) {
                continue;
            }

            filtered.append({qdate, task});
        }

        if (filtered.isEmpty()) {
            auto *placeholder = new QListWidgetItem(get_text(QStringLiteral("Nenhuma tarefa para o período selecionado.")));
            placeholder->setFlags((placeholder->flags() & ~Qt::ItemIsSelectable) & ~Qt::ItemIsEnabled);
            tasks_list->addItem(placeholder);
            _sync_year_calendar_counts();
            _sync_month_calendars_visual_data();
            return;
        }

        const bool use_app_sort = app != nullptr && app->has_active_task_sort();
        std::stable_sort(filtered.begin(), filtered.end(), [this, use_app_sort](const auto &left, const auto &right) {
            if (use_app_sort && app != nullptr) {
                return app->task_sort_less_for_payloads(left.second, right.second);
            }

            if (left.first != right.first) {
                return left.first < right.first;
            }

            const QString left_time = left.second.value(QStringLiteral("time")).toString();
            const QString right_time = right.second.value(QStringLiteral("time")).toString();
            if (left_time != right_time) {
                return left_time < right_time;
            }

            return left.second.value(QStringLiteral("text")).toString()
                   < right.second.value(QStringLiteral("text")).toString();
        });

        const bool should_insert_month_headers =
            filter_mode == QStringLiteral("month") || filter_mode == QStringLiteral("year");
        QPair<int, int> last_month_key(-1, -1);
        QString last_group_key;
        QLocale list_locale = calendar->locale();

        for (const auto &entry : filtered) {
            const QDate qdate = entry.first;
            const QVariantMap task = entry.second;
            if (should_insert_month_headers) {
                const QPair<int, int> month_key(qdate.year(), qdate.month());
                if (month_key != last_month_key) {
                    QString month_name = list_locale.standaloneMonthName(qdate.month(), QLocale::LongFormat);
                    if (month_name.isEmpty()) {
                        month_name = list_locale.monthName(qdate.month(), QLocale::LongFormat);
                    }
                    if (month_name.isEmpty()) {
                        month_name = qdate.toString(QStringLiteral("MMMM"));
                    }
                    auto *header_item = new QListWidgetItem(
                        QStringLiteral("%1 %2")
                            .arg(month_name.left(1).toUpper() + month_name.mid(1))
                            .arg(qdate.year()));
                    header_item->setFlags((header_item->flags() & ~Qt::ItemIsSelectable) & ~Qt::ItemIsEnabled);
                    QFont header_font = header_item->font();
                    header_font.setBold(true);
                    header_item->setFont(header_font);
                    tasks_list->addItem(header_item);
                    last_month_key = month_key;
                    last_group_key.clear();
                }
            }

            const QString status_text = task_utils::task_status_text(task);
            const QString date_text = qdate.toString(_date_format);
            const QString time_str = task.value(QStringLiteral("time")).toString();
            const QString emoji = show_priority ? task_utils::priority_emoji(task.value(QStringLiteral("priority")), app)
                                                : QString();
            const QString domain_badge =
                show_domain ? source::InterfaceCore::dominio_insignia(task.value(QStringLiteral("dominio"))) : QString();
            const QString plain_title = task.value(QStringLiteral("text")).toString();
            const QString display_title = plain_title;
            const QString decorated_title = domain_badge.isEmpty()
                                                ? display_title
                                                : QStringLiteral("%1 %2").arg(domain_badge, display_title);
            const QString group_key = global_task_group_key(app, task);
            if (!group_key.isEmpty() && group_key != last_group_key) {
                auto *header_item = new QListWidgetItem(global_task_group_label(app, date_text, time_str));
                header_item->setFlags((header_item->flags() & ~Qt::ItemIsSelectable) & ~Qt::ItemIsEnabled);
                header_item->setData(Qt::UserRole + 1, QStringLiteral("group_header"));
                header_item->setData(Qt::UserRole + 2, group_key);
                tasks_list->addItem(header_item);
                last_group_key = group_key;
            } else if (group_key.isEmpty()) {
                last_group_key.clear();
            }

            QString title_line =
                visible_global_task_text(app, emoji, decorated_title, status_text, date_text, time_str);
            const int pomodoros = qMax(0, task.value(QStringLiteral("pomodoros"),
                                                     task.value(QStringLiteral("pomodoros_completados"), 0)).toInt());
            if (app != nullptr && app->pomodoro_report_pomodoros_calendario_enabled()) {
                title_line += QStringLiteral("\n") + QCoreApplication::translate("PomodoroPlugin", "🍅 {n} pomodoros")
                                                         .replace(QStringLiteral("{n}"), QString::number(pomodoros));
            }
            auto *item = new QListWidgetItem(title_line);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setData(Qt::UserRole, task);
            item->setToolTip(tooltip_for_task(task, app));
            apply_task_title_appearance(item, task);
            tasks_list->addItem(item);
        }

        _sync_year_calendar_counts();
        _sync_month_calendars_visual_data();
    } catch (...) {
        logger()->error(QStringLiteral("Erro ao atualizar lista de tarefas do painel de calendário."));
    }
}

void CalendarPanel::_update_badge_counts()
{
    QStringList task_dates;
    if (_task_index_records.isEmpty() && _task_index_dates.isEmpty()) {
        _rebuild_task_date_index();
    }
    for (const QVariantMap &task : std::as_const(_task_index_records)) {
        const QString date_str = task.value(QStringLiteral("date")).toString().trimmed();
        const QDate qd = QDate::fromString(date_str, Qt::ISODate);
        if (qd.isValid()) {
            task_dates.append(qd.toString(Qt::ISODate));
        }
    }

    for (BadgeCalendarWidget *cal : _iter_calendars()) {
        if (cal != nullptr) {
            cal->set_task_counts(task_dates);
        }
    }
}

QSet<QString> CalendarPanel::_get_task_dates() const
{
    QSet<QString> dates;
    for (const QString &ds : _task_index_dates) {
        const QDate qd = QDate::fromString(ds, Qt::ISODate);
        if (qd.isValid()) {
            dates.insert(qd.toString(Qt::ISODate));
        }
    }
    return dates;
}

void CalendarPanel::_apply_highlighted_dates()
{
    const QSet<QString> new_dates = _get_task_dates();
    const QSet<QString> to_clear = _highlighted_dates - new_dates;
    const QTextCharFormat blank;

    for (const QString &ds : to_clear) {
        const QDate qd = QDate::fromString(ds, Qt::ISODate);
        if (qd.isValid()) {
            for (BadgeCalendarWidget *cal : _iter_calendars()) {
                cal->setDateTextFormat(qd, blank);
            }
        }
    }

    QTextCharFormat fmt;
    const QColor base = palette().highlight().color();
    fmt.setBackground(QBrush(QColor(base.red(), base.green(), base.blue(), 60)));
    fmt.setFontWeight(QFont::Bold);
    for (const QString &ds : new_dates) {
        const QDate qd = QDate::fromString(ds, Qt::ISODate);
        if (qd.isValid()) {
            for (BadgeCalendarWidget *cal : _iter_calendars()) {
                cal->setDateTextFormat(qd, fmt);
            }
        }
    }
    _highlighted_dates = new_dates;
}

void CalendarPanel::_on_language_changed()
{
    setUpdatesEnabled(false);
    filter_label->setText(get_text(QStringLiteral("Exibir por")));
    filter_combo->setItemText(0, get_text(QStringLiteral("Dia")));
    filter_combo->setItemText(1, get_text(QStringLiteral("Semana")));
    filter_combo->setItemText(2, get_text(QStringLiteral("Mês")));
    filter_combo->setItemText(3, get_text(QStringLiteral("Ano")));
    if (calendar_group != nullptr) {
        calendar_group->setTitle(get_text(QStringLiteral("Calendário")));
    }
    for (QGroupBox *grp : _extra_month_groups) {
        if (grp != nullptr) {
            grp->setTitle(get_text(QStringLiteral("Calendário")));
        }
    }
    if (year_calendar_group != nullptr) {
        year_calendar_group->setTitle(get_text(QStringLiteral("Calendário")));
    }
    for (QGroupBox *grp : _extra_year_groups) {
        if (grp != nullptr) {
            grp->setTitle(get_text(QStringLiteral("Calendário")));
        }
    }
    if (tasks_group != nullptr) {
        tasks_group->setTitle(get_text(QStringLiteral("Lista Global de Tarefas:")));
    }
    if (tasks_label != nullptr) {
        tasks_label->setText(get_text(QStringLiteral("Lista Global de Tarefas:")));
    }
    if (show_label != nullptr) {
        show_label->setText(get_text(QStringLiteral("Mostrar:")));
    }
    if (show_priority_checkbox != nullptr) {
        show_priority_checkbox->setText(get_text(QStringLiteral("Prioridade")));
    }
    if (show_domain_checkbox != nullptr) {
        show_domain_checkbox->setText(get_text(QStringLiteral("Domínio")));
    }
    _date_format = app->date_input != nullptr ? app->date_input->displayFormat() : _date_format;
    _apply_locale_to_calendar();
    update_task_list();
    setUpdatesEnabled(true);
}

void CalendarPanel::_apply_locale_to_calendar()
{
    QString idioma;
    if (app->gerenciador_traducao != nullptr) {
        idioma = app->gerenciador_traducao->obter_idioma_atual();
    }
    QLocale locale = QLocale::system();
    if (idioma.startsWith(QStringLiteral("pt"))) {
        locale = QLocale(QLocale::Portuguese, QLocale::Brazil);
    } else if (idioma.startsWith(QStringLiteral("en"))) {
        locale = QLocale(QLocale::English, QLocale::UnitedStates);
    }
    for (BadgeCalendarWidget *cal : _iter_calendars()) {
        cal->setLocale(locale);
        cal->setFirstDayOfWeek(Qt::Sunday);
    }
    if (year_calendar != nullptr) {
        year_calendar->setLocale(locale);
    }
    for (YearCalendarWidget *extra : _extra_year_calendars) {
        if (extra != nullptr) {
            extra->setLocale(locale);
        }
    }
}

void CalendarPanel::_on_calendar_context_menu(const QPoint &pos)
{
    BadgeCalendarWidget *context_calendar = qobject_cast<BadgeCalendarWidget *>(sender());
    if (context_calendar == nullptr) {
        context_calendar = calendar;
    }
    const QDate qd = context_calendar->_date_at_pos(pos);
    if (!qd.isValid()) {
        return;
    }

    QList<QVariantMap> tasks;
    for (const QVariantMap &task : _collect_tasks()) {
        if (task.value(QStringLiteral("date")).toString() == qd.toString(Qt::ISODate)) {
            tasks.append(task);
        }
    }

    QMenu menu(context_calendar);
    QMenu *edit_menu = menu.addMenu(get_text(QStringLiteral("✏️ Editar Tarefa")));
    QMenu *remove_menu = menu.addMenu(get_text(QStringLiteral("🗑️ Remover Tarefa")));
    if (tasks.isEmpty()) {
        edit_menu->setEnabled(false);
        remove_menu->setEnabled(false);
    } else {
        const bool show_priority = show_priority_checkbox != nullptr && show_priority_checkbox->isChecked();
        const bool show_domain = show_domain_checkbox != nullptr && show_domain_checkbox->isChecked();
        for (const QVariantMap &task : tasks) {
            QAction *act = edit_menu->addAction(task_utils::task_label_for_day(task, show_priority, show_domain, app));
            connect(act, &QAction::triggered, this, [this, task]() {
                const auto found = task_utils::find_source_item_for_calendar(app, task);
                if (found.first != nullptr && found.second != nullptr) {
                    app->edit_task_datetime(found.second, found.first);
                    update_task_list();
                }
            });
        }
        for (const QVariantMap &task : tasks) {
            QAction *act = remove_menu->addAction(task_utils::task_label_for_day(task, show_priority, show_domain, app));
            connect(act, &QAction::triggered, this, [this, task]() {
                const auto found = task_utils::find_source_item_for_calendar(app, task);
                if (found.first != nullptr && found.second != nullptr && app->remove_task(found.second, found.first, true)) {
                    update_task_list();
                }
            });
        }
    }
    menu.exec(context_calendar->mapToGlobal(pos));
}

void CalendarPanel::_prompt_edit_task_for_date(const QDate &qdate)
{
    CalendarDialog dialog(app);
    dialog._prompt_edit_task_for_date(qdate);
}

void CalendarPanel::_prompt_remove_task_for_date(const QDate &qdate)
{
    CalendarDialog dialog(app);
    dialog._prompt_remove_task_for_date(qdate);
}

void CalendarPanel::_on_filter_mode_changed()
{
    const QString mode = filter_combo->currentData().toString();
    if (mode == QStringLiteral("month") || mode == QStringLiteral("year")) {
        set_calendar_view_mode(QStringLiteral("year"));
    } else {
        set_calendar_view_mode(QStringLiteral("month"));
    }
    emit month_view_toggle_visibility_changed(mode == QStringLiteral("month"));
    update_task_list();
}

void CalendarPanel::_on_year_month_selected(const QDate &qdate)
{
    if (_syncing_year_calendars) {
        return;
    }
    if (qdate.isValid()) {
        const int current_day = calendar->selectedDate().day();
        const int max_day = qdate.daysInMonth();
        const QDate selected(qdate.year(), qdate.month(), qMin(current_day, max_day));
        _update_reference_selection(selected, false);
        if (sender() == year_calendar) {
            calendar->setSelectedDate(selected);
            year_calendar->setSelectedDate(selected);
            _sync_year_calendar_pages_from_primary();
        }
    }
    if (filter_combo->currentData().toString() == QStringLiteral("year")
        || filter_combo->currentData().toString() == QStringLiteral("month")) {
        update_task_list();
    }
}

void CalendarPanel::_on_year_month_double_clicked(const QDate &qdate)
{
    if (qdate.isValid()) {
        _selected_reference_date = qdate;
    }
    if (filter_combo->currentData().toString() == QStringLiteral("month")) {
        set_calendar_view_mode(QStringLiteral("month"));
    }
}

void CalendarPanel::_sync_year_calendar_counts()
{
    year_calendar->set_month_counts(calendar->_task_counts_by_month);
    const QDate selected = calendar->selectedDate();
    if (selected.isValid()) {
        year_calendar->setSelectedDate(selected);
    }
    _sync_year_calendar_pages_from_primary();
    _sync_year_calendars_visual_data();
}

void CalendarPanel::_sync_year_calendar_pages_from_primary()
{
    if (_syncing_year_calendars || _extra_year_calendars.isEmpty()) {
        return;
    }

    QDate base_date = year_calendar->selectedDate();
    if (!base_date.isValid()) {
        base_date = calendar->selectedDate();
    }
    if (!base_date.isValid()) {
        base_date = QDate::currentDate();
    }

    _syncing_year_calendars = true;
    for (int idx = 0; idx < _extra_year_calendars.size(); ++idx) {
        YearCalendarWidget *extra = _extra_year_calendars.at(idx);
        int target_year = base_date.year() + idx + 1;
        int target_month = base_date.month();
        const QDate extra_selected = extra->selectedDate();
        if (extra_selected.isValid()) {
            target_month = extra_selected.month();
        }
        extra->setSelectedDate(QDate(target_year, qBound(1, target_month, 12), 1));
    }
    _syncing_year_calendars = false;
}

void CalendarPanel::_sync_year_calendars_visual_data()
{
    for (YearCalendarWidget *extra : _extra_year_calendars) {
        extra->set_month_data(year_calendar->month_counts(), year_calendar->month_tooltips());
    }
}

QDate CalendarPanel::get_filter_reference_date() const
{
    const QString mode = filter_combo->currentData().toString();
    if ((mode == QStringLiteral("year")) || (mode == QStringLiteral("month") && _calendar_view_mode == QStringLiteral("year"))) {
        if (_selected_reference_date.isValid()) {
            return _selected_reference_date;
        }
        return year_calendar->selectedDate();
    }
    if (_selected_reference_date.isValid()) {
        return _selected_reference_date;
    }
    return calendar->selectedDate();
}

QStringList CalendarPanel::get_filter_reference_dates() const
{
    QStringList normalized;
    QSet<QString> seen;
    for (const QString &ds : _selected_reference_dates) {
        const QString trimmed = ds.trimmed();
        const QDate qd = QDate::fromString(trimmed, Qt::ISODate);
        if (qd.isValid() && !seen.contains(trimmed)) {
            seen.insert(trimmed);
            normalized.append(trimmed);
        }
    }
    if (!normalized.isEmpty()) {
        return normalized;
    }
    const QDate ref = get_filter_reference_date();
    if (ref.isValid()) {
        return {ref.toString(Qt::ISODate)};
    }
    return {};
}

void CalendarPanel::set_calendar_view_mode(const QString &mode)
{
    const QString target_mode = mode == QStringLiteral("year") ? QStringLiteral("year") : QStringLiteral("month");
    if (target_mode == QStringLiteral("year")) {
        year_calendar->setSelectedDate(calendar->selectedDate());
        calendar_stack->setCurrentWidget(years_container);
        _update_year_calendars_for_width();
    } else {
        if (_year_resize_timer != nullptr) {
            _year_resize_timer->stop();
        }
        QDate target_date = year_calendar->selectedDate();
        if (filter_combo->currentData().toString() == QStringLiteral("month") && _selected_reference_date.isValid()) {
            target_date = _selected_reference_date;
        }
        calendar->setSelectedDate(target_date);
        calendar_stack->setCurrentWidget(months_container);
        _update_month_calendars_for_width();
    }
    _calendar_view_mode = target_mode;
    emit month_view_mode_changed(target_mode);
}

void CalendarPanel::toggle_month_filter_view_mode()
{
    if (filter_combo->currentData().toString() != QStringLiteral("month")) {
        return;
    }
    set_calendar_view_mode(_calendar_view_mode == QStringLiteral("year") ? QStringLiteral("month")
                                                                          : QStringLiteral("year"));
    update_task_list();
}

bool CalendarPanel::is_month_filter_active() const
{
    return filter_combo->currentData().toString() == QStringLiteral("month");
}

QString CalendarPanel::calendar_view_mode() const
{
    return _calendar_view_mode;
}

void CalendarPanel::set_calendar_only_mode(bool enable)
{
    _calendar_only_mode = enable;
    if (controls_container != nullptr) {
        controls_container->setVisible(!enable);
    }
    if (tasks_group != nullptr) {
        tasks_group->setVisible(!enable);
    }
    if (calendar_stack != nullptr) {
        calendar_stack->setSizePolicy(enable ? QSizePolicy::Expanding : QSizePolicy::Preferred,
                                      enable ? QSizePolicy::Maximum : QSizePolicy::Maximum);
    }
    if (layout() != nullptr) {
        if (enable) {
            layout()->setContentsMargins(0, 0, 0, 0);
            layout()->setSpacing(0);
        } else {
            layout()->setContentsMargins(5, 4, 0, 4);
            layout()->setSpacing(6);
        }
    }
    if (calendar_group != nullptr) {
        calendar_group->setSizePolicy(enable ? QSizePolicy::Expanding : QSizePolicy::Preferred,
                                      enable ? QSizePolicy::Maximum : QSizePolicy::Preferred);
    }
    for (QGroupBox *extra_group : _extra_month_groups) {
        if (extra_group != nullptr) {
            extra_group->setSizePolicy(enable ? QSizePolicy::Expanding : QSizePolicy::Preferred,
                                       enable ? QSizePolicy::Maximum : QSizePolicy::Preferred);
        }
    }
    if (year_calendar_group != nullptr) {
        year_calendar_group->setSizePolicy(enable ? QSizePolicy::Expanding : QSizePolicy::Preferred,
                                           enable ? QSizePolicy::Maximum : QSizePolicy::Preferred);
    }
    for (QGroupBox *extra_year : _extra_year_groups) {
        if (extra_year != nullptr) {
            extra_year->setSizePolicy(enable ? QSizePolicy::Expanding : QSizePolicy::Preferred,
                                      enable ? QSizePolicy::Maximum : QSizePolicy::Preferred);
        }
    }
    setSizePolicy(enable ? QSizePolicy::Expanding : QSizePolicy::Preferred,
                  enable ? QSizePolicy::Maximum : QSizePolicy::Preferred);
    update_calendars_for_width();
    updateGeometry();
}

bool CalendarPanel::calendar_only_mode() const
{
    return _calendar_only_mode;
}

int CalendarPanel::calendar_count() const
{
    if (_is_year_view_active()) {
        return 1 + _extra_year_calendars.size();
    }
    return 1 + _extra_month_calendars.size();
}

QSize CalendarPanel::sizeHint() const
{
    if (_calendar_only_mode && calendar_stack != nullptr) {
        const QSize stack_hint = calendar_stack->sizeHint();
        return QSize(qMax(stack_hint.width(), _calendar_content_width), stack_hint.height());
    }
    return QWidget::sizeHint();
}

QSize CalendarPanel::minimumSizeHint() const
{
    if (_calendar_only_mode && calendar_stack != nullptr) {
        const QSize stack_hint = calendar_stack->minimumSizeHint();
        return QSize(qMax(stack_hint.width(), _calendar_content_width), stack_hint.height());
    }
    return QWidget::minimumSizeHint();
}

bool CalendarPanel::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == tasks_list && event != nullptr && event->type() == QEvent::Resize) {
        tasks_list->doItemsLayout();
    }
    if ((watched == tasks_list || (tasks_list != nullptr && watched == tasks_list->viewport()))
        && event != nullptr && event->type() == QEvent::MouseButtonPress) {
        auto *mouse_event = static_cast<QMouseEvent *>(event);
        if (mouse_event->button() == Qt::LeftButton && tasks_list != nullptr) {
            const QPoint vp_pos = (watched == tasks_list->viewport())
                                      ? mouse_event->position().toPoint()
                                      : tasks_list->viewport()->mapFrom(tasks_list, mouse_event->position().toPoint());
            QListWidgetItem *item = tasks_list->itemAt(vp_pos);
            const bool is_selectable = item != nullptr && (item->flags() & Qt::ItemIsSelectable);
            if (!is_selectable) {
                if (!(mouse_event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier))) {
                    tasks_list->clearSelection();
                    tasks_list->setCurrentItem(nullptr);
                }
            }
            if (app != nullptr) {
                app->clear_all_task_selections_except(tasks_list);
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

}  // namespace source::GerenciamentoUI
