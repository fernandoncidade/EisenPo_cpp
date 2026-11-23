#pragma once

#include <QMap>
#include <QString>
#include <optional>

namespace source::utils {

enum class FeynmanLabelKey {
    Concept = 0,
    Explanation = 1,
    Gaps = 2,
    Review = 3,
    Description = 4
};

constexpr int FEYNMAN_LABELS_COUNT = 5;

QString feynman_label_config_file_path();

QString get_feynman_key_id(FeynmanLabelKey key);

std::optional<FeynmanLabelKey> feynman_key_from_id(const QString &id);

QString get_default_feynman_label(FeynmanLabelKey key, bool is_english = false);

QString get_feynman_placeholder(FeynmanLabelKey key, bool is_english = false);

QMap<QString, QString> load_feynman_custom_labels();

bool save_feynman_custom_labels(const QMap<QString, QString> &custom_labels);

bool restore_feynman_default_labels();

QString get_effective_feynman_label(FeynmanLabelKey key, bool is_english = false);

bool is_feynman_label_customized(FeynmanLabelKey key);

}  // namespace source::utils
