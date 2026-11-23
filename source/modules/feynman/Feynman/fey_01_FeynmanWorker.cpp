#include "source/modules/feynman/Feynman/fey_01_FeynmanWorker.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include "source/modules/feynman/Feynman/feynman_support.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"
#include "source/utils/UnifiedTaskStore.hpp"

namespace source::plugin::Feynman {
namespace {

source::utils::LogManager *logger()
{
    return source::utils::LogManager::get_logger();
}

QString legacy_concepts_path()
{
    return QDir(source::utils::obter_caminho_persistente()).filePath(QStringLiteral("conceitos_feynman.json"));
}

bool is_done_status(const QString &status)
{
    return status.compare(QStringLiteral("Done"), Qt::CaseInsensitive) == 0
        || status.compare(QStringLiteral("Completed"), Qt::CaseInsensitive) == 0;
}

QVariantMap concept_from_unified_record(const QVariantMap &record)
{
    const QVariantMap normalized = source::utils::UnifiedTaskStore::normalize_record(record);
    QVariantMap conceito = {
        {QStringLiteral("id"), normalized.value(QStringLiteral("id"))},
        {QStringLiteral("titulo"), normalized.value(QStringLiteral("titulo")).toString()},
        {QStringLiteral("explicacao"), normalized.value(QStringLiteral("explicacao")).toString()},
        {QStringLiteral("lacunas"), normalized.value(QStringLiteral("lacunas")).toString()},
        {QStringLiteral("revisao"), normalized.value(QStringLiteral("revisao")).toString()},
        {QStringLiteral("prioridade"), normalized.value(QStringLiteral("prioridade")).toInt()},
        {QStringLiteral("dominio"), normalized.value(QStringLiteral("dominio")).toInt()},
        {QStringLiteral("arquivo_vinculado"), normalized.value(QStringLiteral("arquivo_vinculado")).toString()},
        {QStringLiteral("cor_texto"), normalized.value(QStringLiteral("cor_texto")).toString()},
        {QStringLiteral("integracao_origem"), normalized.value(QStringLiteral("integracao_origem")).toString()},
        {QStringLiteral("integracao_task_id"), normalized.value(QStringLiteral("integracao_task_id")).toString()},
        {QStringLiteral("integracao_status"), normalized.value(QStringLiteral("integracao_status")).toString()},
        {QStringLiteral("integracao_date"), normalized.value(QStringLiteral("integracao_date"),
                                                             normalized.value(QStringLiteral("data"))).toString()},
        {QStringLiteral("integracao_time"), normalized.value(QStringLiteral("integracao_time"),
                                                             normalized.value(QStringLiteral("hora"))).toString()},
        {QStringLiteral("integracao_completed"),
         normalized.value(QStringLiteral("integracao_completed"),
                          normalized.value(QStringLiteral("concluida"))).toBool()},
        {QStringLiteral("font_config"), normalized.value(QStringLiteral("font_config")).toMap()},
    };

    const bool descricao_customizada = normalized.value(QStringLiteral("descricao_customizada")).toBool();
    conceito.insert(QStringLiteral("descricao_customizada"), descricao_customizada);
    if (descricao_customizada || !normalized.value(QStringLiteral("descricao")).toString().trimmed().isEmpty()) {
        conceito.insert(QStringLiteral("descricao"), normalized.value(QStringLiteral("descricao")).toString());
    }

    if (conceito.value(QStringLiteral("arquivo_vinculado")).toString().trimmed().isEmpty()) {
        conceito.insert(QStringLiteral("arquivo_vinculado"), QVariant());
    }

    return conceito;
}

QVariantMap unified_record_from_concept(const QVariantMap &conceito)
{
    const QString status = conceito.value(QStringLiteral("integracao_status")).toString().trimmed();
    const bool completed = conceito.value(QStringLiteral("integracao_completed")).toBool() || is_done_status(status);
    const int prioridade = conceito.value(QStringLiteral("prioridade")).toInt();
    const QString integration_id = conceito.value(QStringLiteral("integracao_task_id")).toString().trimmed();

    QVariantMap record = {
        {QStringLiteral("id"),
         conceito.value(QStringLiteral("id")).toString().trimmed().isEmpty() ? integration_id
                                                                             : conceito.value(QStringLiteral("id")).toString().trimmed()},
        {QStringLiteral("modulo"), QStringLiteral("feynman")},
        {QStringLiteral("tipo"), QStringLiteral("conceito")},
        {QStringLiteral("titulo"), conceito.value(QStringLiteral("titulo")).toString()},
        {QStringLiteral("descricao"), conceito.value(QStringLiteral("descricao")).toString()},
        {QStringLiteral("explicacao"), conceito.value(QStringLiteral("explicacao")).toString()},
        {QStringLiteral("lacunas"), conceito.value(QStringLiteral("lacunas")).toString()},
        {QStringLiteral("revisao"), conceito.value(QStringLiteral("revisao")).toString()},
        {QStringLiteral("descricao_customizada"),
         conceito.value(QStringLiteral("descricao_customizada")).toBool() || conceito.contains(QStringLiteral("descricao"))},
        {QStringLiteral("prioridade"), prioridade},
        {QStringLiteral("quadrante"), prioridade},
        {QStringLiteral("categoria"), QStringLiteral("conceito")},
        {QStringLiteral("dominio"), conceito.value(QStringLiteral("dominio")).toInt()},
        {QStringLiteral("status"), status},
        {QStringLiteral("concluida"), completed},
        {QStringLiteral("data"), conceito.value(QStringLiteral("integracao_date")).toString()},
        {QStringLiteral("hora"), conceito.value(QStringLiteral("integracao_time")).toString()},
        {QStringLiteral("arquivo_vinculado"), conceito.value(QStringLiteral("arquivo_vinculado")).toString()},
        {QStringLiteral("cor_texto"), conceito.value(QStringLiteral("cor_texto")).toString()},
        {QStringLiteral("integracao_origem"), conceito.value(QStringLiteral("integracao_origem")).toString()},
        {QStringLiteral("integracao_task_id"), integration_id},
        {QStringLiteral("integracao_status"), status},
        {QStringLiteral("integracao_date"), conceito.value(QStringLiteral("integracao_date")).toString()},
        {QStringLiteral("integracao_time"), conceito.value(QStringLiteral("integracao_time")).toString()},
        {QStringLiteral("integracao_completed"), completed},
        {QStringLiteral("font_config"), conceito.value(QStringLiteral("font_config")).toMap()},
    };
    return source::utils::UnifiedTaskStore::normalize_record(record);
}

QVariantList concepts_from_records(const QVariantList &records)
{
    QVariantList concepts;
    concepts.reserve(records.size());
    for (const QVariant &value : records) {
        const QVariantMap normalized = source::utils::UnifiedTaskStore::normalize_record(value.toMap());
        if (normalized.value(QStringLiteral("modulo")).toString().compare(QStringLiteral("feynman"), Qt::CaseInsensitive)
            == 0) {
            concepts.append(concept_from_unified_record(normalized));
        }
    }
    return concepts;
}

QVariantList unified_records_from_concepts(const QVariantList &conceitos)
{
    QVariantList records;
    records.reserve(conceitos.size());
    for (const QVariant &value : conceitos) {
        records.append(unified_record_from_concept(value.toMap()));
    }
    return records;
}

}  // namespace

FeynmanWorker::FeynmanWorker(QObject *parent) : QObject(parent)
{
    connect(this, &FeynmanWorker::request_load, this, &FeynmanWorker::_load);
    connect(this, &FeynmanWorker::request_save, this, &FeynmanWorker::_save);
}

QVariantList FeynmanWorker::load_file(const QString &arquivo_conceitos)
{
    if (arquivo_conceitos.trimmed().isEmpty()) {
        return {};
    }

    const QVariantList loaded_records = source::utils::UnifiedTaskStore::load_records(arquivo_conceitos);
    const bool sync_enabled = compat::host_bridge() == nullptr || compat::host_bridge()->task_sync_enabled();
    QVariantList shared_records = sync_enabled
        ? source::utils::UnifiedTaskStore::project_records_for_module(loaded_records, QStringLiteral("feynman"))
        : source::utils::UnifiedTaskStore::filter_records_by_module(loaded_records, QStringLiteral("feynman"));
    if (shared_records.isEmpty() && sync_enabled) {
        shared_records = source::utils::UnifiedTaskStore::filter_records_by_module(loaded_records, QStringLiteral("feynman"));
    }
    if (!shared_records.isEmpty()) {
        return concepts_from_records(shared_records);
    }

    const QString legacy_path = legacy_concepts_path();
    if (QFileInfo(legacy_path).exists()
        && QFileInfo(legacy_path).absoluteFilePath() != QFileInfo(arquivo_conceitos).absoluteFilePath()) {
        const QVariantList legacy_loaded = source::utils::UnifiedTaskStore::load_records(legacy_path);
        QVariantList legacy_records = sync_enabled
            ? source::utils::UnifiedTaskStore::project_records_for_module(legacy_loaded, QStringLiteral("feynman"))
            : source::utils::UnifiedTaskStore::filter_records_by_module(legacy_loaded, QStringLiteral("feynman"));
        if (legacy_records.isEmpty() && sync_enabled) {
            legacy_records =
                source::utils::UnifiedTaskStore::filter_records_by_module(legacy_loaded, QStringLiteral("feynman"));
        }
        if (!legacy_records.isEmpty()) {
            source::utils::UnifiedTaskStore::replace_module_records(arquivo_conceitos,
                                                                    QStringLiteral("feynman"),
                                                                    legacy_records);
            return concepts_from_records(legacy_records);
        }
    }

    return {};
}

bool FeynmanWorker::save_file(const QString &arquivo_conceitos, const QVariantList &conceitos)
{
    if (arquivo_conceitos.trimmed().isEmpty()) {
        return false;
    }

    if (!source::utils::UnifiedTaskStore::replace_module_records(arquivo_conceitos,
                                                                QStringLiteral("feynman"),
                                                                unified_records_from_concepts(conceitos))) {
        logger()->error(QStringLiteral("Erro ao salvar conceitos do Método Feynman em: %1").arg(arquivo_conceitos));
        return false;
    }

    return true;
}

void FeynmanWorker::_load(const QString &arquivo_conceitos)
{
    try {
        emit conceitos_loaded(load_file(arquivo_conceitos));
    } catch (...) {
        const QString message = QStringLiteral("Erro no worker (Feynman) ao carregar conceitos.");
        logger()->error(message, true);
        emit error(message);
    }
}

void FeynmanWorker::_save(const QString &arquivo_conceitos, const QVariantList &conceitos)
{
    try {
        const bool ok = save_file(arquivo_conceitos, conceitos);
        if (!ok) {
            emit error(QStringLiteral("Erro no worker (Feynman) ao salvar conceitos."));
        }
        emit save_finished(ok);
    } catch (...) {
        const QString message = QStringLiteral("Erro no worker (Feynman) ao salvar conceitos.");
        logger()->error(message, true);
        emit error(message);
        emit save_finished(false);
    }
}

}  // namespace source::plugin::Feynman
