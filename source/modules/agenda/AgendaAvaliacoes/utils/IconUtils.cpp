#include "IconUtils.hpp"

#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFile>

QString get_icon_app_base_path() {
    const QString app_dir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        app_dir,
        QDir(app_dir).absoluteFilePath("agenda"),
        QDir(app_dir).absoluteFilePath("plugins/agenda"),
        QDir::currentPath(),
        QDir(QDir::currentPath()).absoluteFilePath("source/modules/agenda/AgendaAvaliacoes"),
    };

    for (const QString &candidate : candidates) {
        const QString clean = QDir::cleanPath(candidate);
        if (QDir(clean + "/assets/icones").exists() || QDir(clean + "/icones").exists()) {
            return QDir(clean).absolutePath();
        }
    }

    return QDir(app_dir).absolutePath();
}

QString get_icon_path(const QString &icon_name) {
    const QString basePath = get_icon_app_base_path();
    const QStringList possiblePaths = {basePath + "/assets/icones/" + icon_name, basePath + "/icones/" + icon_name,
                                       basePath + "/_internal/icones/" + icon_name,
                                       QDir(basePath).absoluteFilePath("../assets/icones/" + icon_name),
                                       QDir(basePath).absoluteFilePath("../icones/" + icon_name)};
    for (const QString &path : possiblePaths) {
        const QString abs = QDir::cleanPath(path);
        if (QFile::exists(abs)) {
            return abs;
        }
    }
    return possiblePaths.isEmpty() ? QString() : QDir::cleanPath(possiblePaths.first());
}
