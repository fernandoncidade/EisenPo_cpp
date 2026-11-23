#pragma once

#include <functional>
#include <utility>

#include <QLocale>
#include <QString>
#include <QVariantMap>
#include <Qt>

class QObject;

namespace source::plugin {

class FeynmanHostBridge {
public:
    using TaskHandler = std::function<bool(const QVariantMap &)>;
    using AvailabilityChecker = std::function<bool()>;
    using ReservedSpaceChecker = std::function<bool()>;
    using WatermarkChecker = std::function<bool()>;
    using UiLayoutModeKeyChecker = std::function<QString()>;
    using TaskSyncEnabledChecker = std::function<bool()>;
    using StandardButtonsHandler = std::function<void(QObject *)>;
    using SharedStateChangedHandler = std::function<void(const QString &)>;
    using SyncOperationInfoHandler = std::function<void(const QString &, const QString &)>;
    using TaskDeletedHandler = std::function<void(const QVariantMap &, const QString &)>;
    using TaskSortStateChecker = std::function<bool()>;
    using TaskSortLessHandler = std::function<bool(const QVariantMap &, const QVariantMap &)>;

    void set_language_code(const QString &language_code)
    {
        language_code_ = language_code.trimmed().isEmpty() ? QLocale::system().name() : language_code.trimmed();
    }

    QString language_code() const
    {
        return language_code_;
    }

    bool is_english() const
    {
        return language_code_.startsWith(QStringLiteral("en"), Qt::CaseInsensitive);
    }

    void set_task_to_eisenhower_handler(TaskHandler handler)
    {
        task_to_eisenhower_handler_ = std::move(handler);
    }

    bool publish_task_to_eisenhower(const QVariantMap &task) const
    {
        return task_to_eisenhower_handler_ ? task_to_eisenhower_handler_(task) : false;
    }

    void set_eisenhower_available_checker(AvailabilityChecker checker)
    {
        eisenhower_available_checker_ = std::move(checker);
    }

    bool can_publish_task_to_eisenhower() const
    {
        if (!task_to_eisenhower_handler_) {
            return false;
        }

        return !eisenhower_available_checker_ || eisenhower_available_checker_();
    }

    void set_task_to_pomodoro_handler(TaskHandler handler)
    {
        task_to_pomodoro_handler_ = std::move(handler);
    }

    bool publish_task_to_pomodoro(const QVariantMap &task) const
    {
        return task_to_pomodoro_handler_ ? task_to_pomodoro_handler_(task) : false;
    }

    void set_pomodoro_available_checker(AvailabilityChecker checker)
    {
        pomodoro_available_checker_ = std::move(checker);
    }

    bool can_publish_task_to_pomodoro() const
    {
        if (!task_to_pomodoro_handler_) {
            return false;
        }

        return !pomodoro_available_checker_ || pomodoro_available_checker_();
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

    void set_ui_layout_mode_key_checker(UiLayoutModeKeyChecker checker)
    {
        ui_layout_mode_key_checker_ = std::move(checker);
    }

    QString ui_layout_mode_key() const
    {
        return ui_layout_mode_key_checker_ ? ui_layout_mode_key_checker_() : QStringLiteral("vertical");
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

private:
    QString language_code_ = QLocale::system().name();
    TaskHandler task_to_eisenhower_handler_;
    AvailabilityChecker eisenhower_available_checker_;
    TaskHandler task_to_pomodoro_handler_;
    AvailabilityChecker pomodoro_available_checker_;
    ReservedSpaceChecker reserved_space_checker_;
    WatermarkChecker watermark_checker_;
    UiLayoutModeKeyChecker ui_layout_mode_key_checker_;
    TaskSyncEnabledChecker task_sync_enabled_checker_;
    StandardButtonsHandler standard_buttons_handler_;
    SharedStateChangedHandler shared_state_changed_handler_;
    SyncOperationInfoHandler sync_operation_info_handler_;
    TaskDeletedHandler task_deleted_handler_;
    TaskSortStateChecker task_sort_state_checker_;
    TaskSortLessHandler task_sort_less_handler_;
};

}  // namespace source::plugin
