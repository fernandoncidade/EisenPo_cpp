#include "source/utils/FeynmanLabelsConfig.hpp"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "source/utils/CaminhoPersistenteUtils.hpp"

namespace source::utils {

namespace {

struct FeynmanLabelMeta {
    FeynmanLabelKey key;
    const char *id;
    const char *default_pt;
    const char *default_en;
    const char *placeholder_pt;
    const char *placeholder_en;
};

const FeynmanLabelMeta METAS[FEYNMAN_LABELS_COUNT] = {
    {FeynmanLabelKey::Concept, "concept", "1️⃣ Conceito", "1️⃣ Concept", "1️⃣ Novo Rótulo", "1️⃣ New Label"},
    {FeynmanLabelKey::Explanation, "explanation", "2️⃣ Explique com Simplicidade", "2️⃣ Explain Simply", "2️⃣ Novo Rótulo", "2️⃣ New Label"},
    {FeynmanLabelKey::Gaps, "gaps", "3️⃣ Lacunas Identificadas", "3️⃣ Identified Gaps", "3️⃣ Novo Rótulo", "3️⃣ New Label"},
    {FeynmanLabelKey::Review, "review", "4️⃣ Revisão e Simplificação Final", "4️⃣ Final Review and Simplification", "4️⃣ Novo Rótulo", "4️⃣ New Label"},
    {FeynmanLabelKey::Description, "description", "5️⃣ Descrição", "5️⃣ Description", "5️⃣ Novo Rótulo", "5️⃣ New Label"}
};

const FeynmanLabelMeta *find_meta(FeynmanLabelKey key)
{
    const int index = static_cast<int>(key);
    if (index >= 0 && index < FEYNMAN_LABELS_COUNT) {
        return &METAS[index];
    }
    return nullptr;
}

}  // namespace

QString feynman_label_config_file_path()
{
    return QDir(obter_caminho_persistente()).filePath(QStringLiteral("feynman_rotulos_config.json"));
}

QString get_feynman_key_id(FeynmanLabelKey key)
{
    const auto *meta = find_meta(key);
    return meta != nullptr ? QString::fromUtf8(meta->id) : QString();
}

std::optional<FeynmanLabelKey> feynman_key_from_id(const QString &id)
{
    for (const auto &meta : METAS) {
        if (id.compare(QString::fromUtf8(meta.id), Qt::CaseInsensitive) == 0) {
            return meta.key;
        }
    }
    return std::nullopt;
}

QString get_default_feynman_label(FeynmanLabelKey key, bool is_english)
{
    const auto *meta = find_meta(key);
    if (meta == nullptr) {
        return {};
    }
    return is_english ? QString::fromUtf8(meta->default_en) : QString::fromUtf8(meta->default_pt);
}

QString get_feynman_placeholder(FeynmanLabelKey key, bool is_english)
{
    const auto *meta = find_meta(key);
    if (meta == nullptr) {
        return {};
    }
    return is_english ? QString::fromUtf8(meta->placeholder_en) : QString::fromUtf8(meta->placeholder_pt);
}

QMap<QString, QString> load_feynman_custom_labels()
{
    QMap<QString, QString> result;
    QFile file(feynman_label_config_file_path());
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return result;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return result;
    }

    const QJsonObject obj = doc.object();
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        const QString value = it.value().toString().trimmed();
        if (!value.isEmpty()) {
            result.insert(it.key(), value);
        }
    }

    return result;
}

bool save_feynman_custom_labels(const QMap<QString, QString> &custom_labels)
{
    QJsonObject obj;
    for (auto it = custom_labels.begin(); it != custom_labels.end(); ++it) {
        const QString value = it.value().trimmed();
        if (!value.isEmpty()) {
            obj.insert(it.key(), value);
        }
    }

    const QString path = feynman_label_config_file_path();
    const QDir dir = QFileInfo(path).dir();
    if (!dir.exists()) {
        dir.mkpath(QStringLiteral("."));
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    return true;
}

bool restore_feynman_default_labels()
{
    const QString path = feynman_label_config_file_path();
    if (QFile::exists(path)) {
        return QFile::remove(path);
    }
    return true;
}

QString get_effective_feynman_label(FeynmanLabelKey key, bool is_english)
{
    const QString id = get_feynman_key_id(key);
    if (!id.isEmpty()) {
        const QMap<QString, QString> custom = load_feynman_custom_labels();
        if (custom.contains(id)) {
            const QString custom_val = custom.value(id).trimmed();
            if (!custom_val.isEmpty()) {
                return custom_val;
            }
        }
    }
    return get_default_feynman_label(key, is_english);
}

bool is_feynman_label_customized(FeynmanLabelKey key)
{
    const QString id = get_feynman_key_id(key);
    if (id.isEmpty()) {
        return false;
    }
    const QMap<QString, QString> custom = load_feynman_custom_labels();
    return custom.contains(id) && !custom.value(id).trimmed().isEmpty();
}

}  // namespace source::utils
