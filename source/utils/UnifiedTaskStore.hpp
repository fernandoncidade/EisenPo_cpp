#pragma once

#include <QVariantList>
#include <QVariantMap>
#include <QString>
#include <QStringList>

#include <QtGlobal>

namespace source::utils {

inline double calcular_percentual_conclusao(int ciclos_concluidos, int ciclos_planejados)
{
    if (ciclos_planejados <= 0 || ciclos_concluidos <= 0) {
        return 0.0;
    }
    if (ciclos_concluidos >= ciclos_planejados) {
        return 100.0;
    }
    const double percent = (static_cast<double>(ciclos_concluidos) / static_cast<double>(ciclos_planejados)) * 100.0;
    return qBound(0.0, percent, 100.0);
}

namespace UnifiedTaskStore {

using source::utils::calcular_percentual_conclusao;

QString shared_tasks_path();
QVariantMap normalize_record(const QVariantMap &record);
QVariantList normalize_records(const QVariantList &records);
QVariantList load_records(const QString &path);
bool save_records(const QString &path, const QVariantList &records);
bool replace_module_records(const QString &path,
                            const QString &module,
                            const QVariantList &module_records);
QVariantList project_records_for_module(const QVariantList &records, const QString &module);
QVariantList filter_records_by_module(const QVariantList &records, const QString &module);
QVariantList merge_module_records(const QVariantList &records,
                                  const QString &module,
                                  const QVariantList &module_records);
QVariantList remove_linked_records(const QVariantList &records,
                                   const QVariantMap &task,
                                   const QStringList &modules);

}  // namespace UnifiedTaskStore
}  // namespace source::utils
