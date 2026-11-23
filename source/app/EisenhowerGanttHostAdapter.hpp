#pragma once

#include <QHash>
#include <QPointer>
#include <QVariantList>
#include <QVariantMap>
#include "source/modules/gantt/Gantt/application/services/IEisenhowerHost.hpp"

namespace source {
class EisenhowerMatrixApp;

class EisenhowerGanttHostAdapter : public gantt::IEisenhowerHost {
public:
    explicit EisenhowerGanttHostAdapter(EisenhowerMatrixApp *app);
    ~EisenhowerGanttHostAdapter() override = default;

    QVariantList get_available_tasks() const override;
    QVariantMap get_task_by_id(const QString &task_id) const override;
    void sync_task_progress(const QString &task_id, double percent, bool is_done) override;
    void sync_task_duration(const QString &task_id, double duration_days) override;
    bool remove_task_by_id(const QString &task_id) override;
    void show_task_sync_delete_info(const QString &origin, QWidget *parent = nullptr) override;
    QWidget *as_widget() override;

    void invalidate_cache() const;

private:
    void ensure_cache_valid() const;

    QPointer<EisenhowerMatrixApp> app_;
    mutable QHash<QString, QVariantMap> task_cache_;
    mutable QVariantList cached_tasks_list_;
    mutable bool cache_valid_ = false;
};

} // namespace source
