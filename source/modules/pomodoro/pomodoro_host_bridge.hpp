#pragma once

#include <functional>
#include <utility>

#include <QDialog>
#include <QLocale>
#include <QString>
#include <QVariantMap>

class QObject;
class QWidget;

namespace source::plugin {

class PomodoroHostBridge {
public:
    using TaskHandler = std::function<void(const QVariantMap &)>;
    using FeynmanTaskHandler = std::function<bool(const QVariantMap &)>;
    using AvailabilityChecker = std::function<bool()>;
    using ReservedSpaceChecker = std::function<bool()>;
    using WatermarkChecker = std::function<bool()>;
    using TaskSyncEnabledChecker = std::function<bool()>;
    using StandardButtonsHandler = std::function<void(QObject *)>;
    using SharedStateChangedHandler = std::function<void(const QString &)>;
    using SyncOperationInfoHandler = std::function<void(const QString &, const QString &)>;
    using TaskDeletedHandler = std::function<void(const QVariantMap &, const QString &)>;
    using TaskSortStateChecker = std::function<bool()>;
    using TaskSortLessHandler = std::function<bool(const QVariantMap &, const QVariantMap &)>;
    using ReportPomodorosChecker = std::function<bool()>;
    using ClearExternalSelectionsHandler = std::function<void()>;
    using ConfiguredCyclesChangedHandler = std::function<void(int)>;
    using RedefinirCiclosEmProgressoHandler = std::function<void(int)>;
    using CalendarWidgetFactory = std::function<QWidget *(QWidget *parent)>;

    void set_clear_external_selections_handler(ClearExternalSelectionsHandler handler)
    {
        clear_external_selections_handler_ = std::move(handler);
    }

    void clear_external_selections() const
    {
        if (clear_external_selections_handler_) {
            clear_external_selections_handler_();
        }
    }

    void set_report_pomodoros_checker(ReportPomodorosChecker checker)
    {
        report_pomodoros_checker_ = std::move(checker);
    }

    bool report_pomodoros_enabled() const
    {
        return report_pomodoros_checker_ ? report_pomodoros_checker_() : true;
    }

    void set_language_code(const QString &language_code)
    {
        language_code_ = language_code.trimmed().isEmpty() ? QLocale::system().name() : language_code.trimmed();
    }

    QString language_code() const
    {
        return language_code_;
    }

    QLocale locale() const
    {
        if (language_code_.startsWith(QStringLiteral("pt"), Qt::CaseInsensitive)) {
            return QLocale(QLocale::Portuguese, QLocale::Brazil);
        }
        if (language_code_.startsWith(QStringLiteral("en"), Qt::CaseInsensitive)) {
            return QLocale(QLocale::English, QLocale::UnitedStates);
        }
        return QLocale::system();
    }

    QString date_format() const
    {
        return language_code_.startsWith(QStringLiteral("en"), Qt::CaseInsensitive) ? QStringLiteral("MM/dd/yyyy")
                                                                                     : QStringLiteral("dd/MM/yyyy");
    }

    void set_task_to_eisenhower_handler(TaskHandler handler)
    {
        task_to_eisenhower_handler_ = std::move(handler);
    }

    void publish_task_to_eisenhower(const QVariantMap &task) const
    {
        if (task_to_eisenhower_handler_) {
            task_to_eisenhower_handler_(task);
        }
    }

    void set_task_to_feynman_handler(FeynmanTaskHandler handler)
    {
        task_to_feynman_handler_ = std::move(handler);
    }

    bool publish_task_to_feynman(const QVariantMap &task) const
    {
        return task_to_feynman_handler_ ? task_to_feynman_handler_(task) : false;
    }

    void set_feynman_available_checker(AvailabilityChecker checker)
    {
        feynman_available_checker_ = std::move(checker);
    }

    bool can_publish_task_to_feynman() const
    {
        if (!task_to_feynman_handler_) {
            return false;
        }

        return !feynman_available_checker_ || feynman_available_checker_();
    }

    void set_reserved_space_checker(ReservedSpaceChecker checker)
    {
        reserved_space_checker_ = std::move(checker);
    }

    bool reserved_space_enabled() const
    {
        return reserved_space_checker_ ? reserved_space_checker_() : false;
    }

    void set_watermark_checker(WatermarkChecker checker)
    {
        watermark_checker_ = std::move(checker);
    }

    bool watermark_enabled() const
    {
        return watermark_checker_ ? watermark_checker_() : false;
    }

    void set_task_sync_enabled_checker(TaskSyncEnabledChecker checker)
    {
        task_sync_enabled_checker_ = std::move(checker);
    }

    bool task_sync_enabled() const
    {
        return task_sync_enabled_checker_ ? task_sync_enabled_checker_() : true;
    }

    void set_standard_buttons_handler(StandardButtonsHandler handler)
    {
        standard_buttons_handler_ = std::move(handler);
    }

    void translate_standard_buttons(QObject *dialog) const
    {
        if (standard_buttons_handler_) {
            standard_buttons_handler_(dialog);
        }
    }

    void set_shared_state_changed_handler(SharedStateChangedHandler handler)
    {
        shared_state_changed_handler_ = std::move(handler);
    }

    void notify_shared_state_changed(const QString &origin) const
    {
        if (shared_state_changed_handler_) {
            shared_state_changed_handler_(origin);
        }
    }

    void set_sync_operation_info_handler(SyncOperationInfoHandler handler)
    {
        sync_operation_info_handler_ = std::move(handler);
    }

    void show_sync_operation_info(const QString &operation, const QString &origin) const
    {
        if (sync_operation_info_handler_) {
            sync_operation_info_handler_(operation, origin);
        }
    }

    void set_task_deleted_handler(TaskDeletedHandler handler)
    {
        task_deleted_handler_ = std::move(handler);
    }

    void publish_task_deleted(const QVariantMap &task, const QString &origin) const
    {
        if (task_deleted_handler_) {
            task_deleted_handler_(task, origin);
        }
    }

    void set_task_sort_state_checker(TaskSortStateChecker checker)
    {
        task_sort_state_checker_ = std::move(checker);
    }

    bool has_active_task_sort() const
    {
        return task_sort_state_checker_ ? task_sort_state_checker_() : false;
    }

    void set_task_sort_less_handler(TaskSortLessHandler handler)
    {
        task_sort_less_handler_ = std::move(handler);
    }

    bool task_sort_less(const QVariantMap &left, const QVariantMap &right) const
    {
        return task_sort_less_handler_ ? task_sort_less_handler_(left, right) : false;
    }

    int configured_cycles() const
    {
        return configured_cycles_;
    }

    void set_configured_cycles(int cycles)
    {
        configured_cycles_ = qMax(1, cycles);
    }

    void set_configured_cycles_changed_handler(ConfiguredCyclesChangedHandler handler)
    {
        configured_cycles_changed_handler_ = std::move(handler);
    }

    void notify_configured_cycles_changed(int cycles) const
    {
        if (configured_cycles_changed_handler_) {
            configured_cycles_changed_handler_(cycles);
        }
    }

    void set_redefinir_ciclos_em_progresso_handler(RedefinirCiclosEmProgressoHandler handler)
    {
        redefinir_ciclos_em_progresso_handler_ = std::move(handler);
    }

    void notify_redefinir_ciclos_em_progresso(int cycles) const
    {
        if (redefinir_ciclos_em_progresso_handler_) {
            redefinir_ciclos_em_progresso_handler_(cycles);
        }
    }

    void set_calendar_widget_factory(CalendarWidgetFactory factory)
    {
        calendar_widget_factory_ = std::move(factory);
    }

    QWidget *create_calendar_widget(QWidget *parent = nullptr) const
    {
        return calendar_widget_factory_ ? calendar_widget_factory_(parent) : nullptr;
    }

    bool has_calendar_widget_factory() const
    {
        return static_cast<bool>(calendar_widget_factory_);
    }

private:
    QString language_code_ = QLocale::system().name();
    int configured_cycles_ = 4;
    TaskHandler task_to_eisenhower_handler_;
    FeynmanTaskHandler task_to_feynman_handler_;
    AvailabilityChecker feynman_available_checker_;
    ReservedSpaceChecker reserved_space_checker_;
    WatermarkChecker watermark_checker_;
    TaskSyncEnabledChecker task_sync_enabled_checker_;
    StandardButtonsHandler standard_buttons_handler_;
    SharedStateChangedHandler shared_state_changed_handler_;
    SyncOperationInfoHandler sync_operation_info_handler_;
    TaskDeletedHandler task_deleted_handler_;
    TaskSortStateChecker task_sort_state_checker_;
    TaskSortLessHandler task_sort_less_handler_;
    ReportPomodorosChecker report_pomodoros_checker_;
    ClearExternalSelectionsHandler clear_external_selections_handler_;
    ConfiguredCyclesChangedHandler configured_cycles_changed_handler_;
    RedefinirCiclosEmProgressoHandler redefinir_ciclos_em_progresso_handler_;
    CalendarWidgetFactory calendar_widget_factory_;
};

}  // namespace source::plugin

