#include "find_VersionEditor.hpp"

#include <QApplication>
#include <QComboBox>
#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMap>
#include <QPair>
#include <QPushButton>
#include <QRegularExpression>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QtGlobal>

#include <optional>

#if defined(Q_OS_WIN)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace mocks::version_editor {
namespace {

const QStringList kPtMonths = {
    QStringLiteral("Janeiro"),
    QStringLiteral("Fevereiro"),
    QStringLiteral("Março"),
    QStringLiteral("Abril"),
    QStringLiteral("Maio"),
    QStringLiteral("Junho"),
    QStringLiteral("Julho"),
    QStringLiteral("Agosto"),
    QStringLiteral("Setembro"),
    QStringLiteral("Outubro"),
    QStringLiteral("Novembro"),
    QStringLiteral("Dezembro"),
};

const QStringList kPtMonthsLower = {
    QStringLiteral("janeiro"),
    QStringLiteral("fevereiro"),
    QStringLiteral("março"),
    QStringLiteral("abril"),
    QStringLiteral("maio"),
    QStringLiteral("junho"),
    QStringLiteral("julho"),
    QStringLiteral("agosto"),
    QStringLiteral("setembro"),
    QStringLiteral("outubro"),
    QStringLiteral("novembro"),
    QStringLiteral("dezembro"),
};

const QStringList kEnMonths = {
    QStringLiteral("January"),
    QStringLiteral("February"),
    QStringLiteral("March"),
    QStringLiteral("April"),
    QStringLiteral("May"),
    QStringLiteral("June"),
    QStringLiteral("July"),
    QStringLiteral("August"),
    QStringLiteral("September"),
    QStringLiteral("October"),
    QStringLiteral("November"),
    QStringLiteral("December"),
};

const QString &group_about()
{
    static const QString value = QStringLiteral("Sobre");
    return value;
}

const QString &group_clc()
{
    static const QString value = QStringLiteral("CLC");
    return value;
}

const QString &group_copyright()
{
    static const QString value = QStringLiteral("Direitos Autorais");
    return value;
}

const QString &group_eula()
{
    static const QString value = QStringLiteral("EULA");
    return value;
}

const QString &group_notices()
{
    static const QString value = QStringLiteral("Notices");
    return value;
}

const QString &group_privacy()
{
    static const QString value = QStringLiteral("Política de Privacidade");
    return value;
}

const QString &group_modules()
{
    static const QString value = QStringLiteral("Módulos");
    return value;
}

const QString &group_plugins()
{
    return group_modules();
}

const QString &group_docs()
{
    static const QString value = QStringLiteral("Documentação");
    return value;
}

const QString &group_misc()
{
    static const QString value = QStringLiteral("Miscelânia");
    return value;
}

const QStringList &group_order()
{
    static const QStringList order = {
        group_about(),
        group_clc(),
        group_copyright(),
        group_eula(),
        group_notices(),
        group_privacy(),
        group_modules(),
        group_docs(),
        group_misc(),
    };
    return order;
}

QString build_version_from_date(const QDate &date = QDate::currentDate())
{
    const QDate safe_date = date.isValid() ? date : QDate::currentDate();
    if (!safe_date.isValid()) {
        return QStringLiteral("2025.11.26.0");
    }

    return QStringLiteral("%1.%2.%3.0")
        .arg(safe_date.year())
        .arg(safe_date.month())
        .arg(safe_date.day());
}

QString readme_version_token(const QString &version)
{
    const QString trimmed = version.trimmed();
    if (trimmed.isEmpty()) {
        return QStringLiteral("v0.0.0.0");
    }

    return trimmed.startsWith(QLatin1Char('v'), Qt::CaseInsensitive) ? trimmed : QStringLiteral("v%1").arg(trimmed);
}

QString spec_content_key(const FileSpec &spec)
{
    return spec.content_key.isEmpty() ? spec.key : spec.content_key;
}

const QMap<QString, QStringList> &patterns()
{
    static const QMap<QString, QStringList> data = {
        {QStringLiteral("readme_pt"),
         {QStringLiteral(R"(^(\[!\[Latest Release\]\(https://img\.shields\.io/badge/Release-v[\d\.]+-[^)]+\)\]\(docs/release-v[\d\.]+\.md\))\s*$)"),
          QStringLiteral(R"(^(> \*\*Observa(?:ção|cao):\*\*.*(?:EISENPO|EisenPo).*)$)"),
          QStringLiteral(R"(^(Vers(?:ã|a)o:\s*v[\d\.]+<br>)\s*$)"),
          QStringLiteral(R"(^(Data t(?:é|e)cnica desta revis(?:ã|a)o:\s*.*<br>)\s*$)"),
          QStringLiteral(R"(^(-\s+.*\[Notas de Lan(?:ç|c)amento da Vers(?:ã|a)o\s+[\d\.]+\]\(docs/release-v[\d\.]+\.md#ptbr\).*)$)")}},
        {QStringLiteral("readme_en"),
         {QStringLiteral(R"(^(> \*\*Note:\*\*.*(?:EISENPO|EisenPo).*)$)"),
          QStringLiteral(R"(^(Version:\s*v[\d\.]+<br>)\s*$)"),
          QStringLiteral(R"(^(Technical revision date:\s*.*<br>)\s*$)"),
          QStringLiteral(R"(^(-\s+.*\[Release Notes v[\d\.]+\]\(docs/release-v[\d\.]+\.md#enus\).*)$)")}},
        {QStringLiteral("observacao_pt"),
         {QStringLiteral(R"(^(Vers(?:ã|a)o documentada:\s*`[\d\.]+`)\s*$)"),
          QStringLiteral(R"(^(Data do snapshot t(?:é|e)cnico:\s*`.*`)\s*$)")}},
        {QStringLiteral("privacy_pt"),
         {QStringLiteral(R"(^(Versão:\s*.*)$)"), QStringLiteral(R"(^(Última atualização:\s*.*)$)")}},
        {QStringLiteral("privacy_en"),
         {QStringLiteral(R"(^(Version:\s*.*)$)"), QStringLiteral(R"(^(Last updated:\s*.*)$)")}},
        {QStringLiteral("notice_pt"),
         {QStringLiteral(R"(^(Versão.*)$)"), QStringLiteral(R"(^(?:Data:\s*.*|Versão\s+[\d\.]+,?\s+.*)$)")}},
        {QStringLiteral("notice_en"),
         {QStringLiteral(R"(^(Version.*)$)"), QStringLiteral(R"(^(?:Date:\s*.*|Version\s+[\d\.]+,?\s+.*)$)")}},
        {QStringLiteral("eula_pt"),
         {QStringLiteral(R"(^(Versão.*)$)"), QStringLiteral(R"(^(?:Data:\s*.*|Versão\s+[\d\.]+,?\s+.*)$)")}},
        {QStringLiteral("eula_en"),
         {QStringLiteral(R"(^(Version.*)$)"), QStringLiteral(R"(^(?:Date:\s*.*|Version\s+[\d\.]+,?\s+.*)$)")}},
        {QStringLiteral("copyright_pt"),
         {QStringLiteral(R"(^(Versão.*)$)"),
          QStringLiteral(R"(^(?:Data:\s*.*|Versão\s+[\d\.]+,?\s+.*)$)"),
          QStringLiteral(R"(^(Copyright.*)$)")}},
        {QStringLiteral("copyright_en"),
         {QStringLiteral(R"(^(Version.*)$)"),
          QStringLiteral(R"(^(?:Date:\s*.*|Version\s+[\d\.]+,?\s+.*)$)"),
          QStringLiteral(R"(^(Copyright.*)$)")}},
        {QStringLiteral("clc_pt"),
         {QStringLiteral(R"(^(Versão:\s*.*)$)"), QStringLiteral(R"(^(Data:\s*.*)$)")}},
        {QStringLiteral("clc_en"),
         {QStringLiteral(R"(^(Version:\s*.*)$)"), QStringLiteral(R"(^(Date:\s*.*)$)")}},
        {QStringLiteral("about_pt"), {QStringLiteral(R"(^(Versão:\s*.*)$)")}},
        {QStringLiteral("about_en"), {QStringLiteral(R"(^(Version:\s*.*)$)")}},
        {QStringLiteral("about_dialog"), {QStringLiteral(R"(^(\s*"<p><b>%2:</b>\s*[\d\.]+</p>".*)$)")}},
        {QStringLiteral("installer_unified"), {QStringLiteral(R"(^(#define\s+MyAppVersion\s+"[\d\.]+";?)\r?$)")}},
        {QStringLiteral("installer_mingw"),
         {QStringLiteral(R"(^(#define\s+MyAppVersion\s+"[\d\.]+";?)\r?$)"),
          QStringLiteral(R"(^(OutputBaseFilename=EisenPo_mingw_v[\d\.]+)\r?$)")}},
        {QStringLiteral("installer_msvc"),
         {QStringLiteral(R"(^(#define\s+MyAppVersion\s+"[\d\.]+";?)\r?$)"),
          QStringLiteral(R"(^(\s*OutputBaseFilename=EisenPo_msvc_v[\d\.]+)\r?$)")}},
        {QStringLiteral("plugin_manifest"), {QStringLiteral(R"(^(\s*"version"\s*:\s*"[^"]+"\s*,?\s*)$)")}},
        {QStringLiteral("module_manifest"), {QStringLiteral(R"(^(\s*"version"\s*:\s*"[^"]+"\s*,?\s*)$)")}},
        {QStringLiteral("plugin_interface_iid"),
         {QStringLiteral(R"(^(\s*#define\s+SOURCE_[A-Z_]+_PLUGIN_INTERFACE_IID\s+"source\.plugin\.[^/"]+/[\d\.]+"\s*)$)")}},
        {QStringLiteral("module_interface_iid"),
         {QStringLiteral(R"(^(\s*#define\s+SOURCE_[A-Z_]+_PLUGIN_INTERFACE_IID\s+"source\.plugin\.[^/"]+/[\d\.]+"\s*)$)")}},
        {QStringLiteral("app_plugin_status_fallback"),
         {QStringLiteral(R"(^(\s*return\s+QStringLiteral\("[\d\.]+"\);\s*)$)"),
          QStringLiteral(R"(^(\s*return\s+document\.isObject\(\)\s*\?\s*document\.object\(\)\.value\(QStringLiteral\("version"\)\)\.toString\(QStringLiteral\("[\d\.]+"\)\)\s*)$)"),
          QStringLiteral(R"(^(\s*:\s*QStringLiteral\("[\d\.]+"\);\s*)$)")}},
        {QStringLiteral("doc_bilingual_md"),
         {QStringLiteral(R"(^(\*\*Vers(?:ã|a)o:\*\*\s*`[\d\.]+`)\s*$)"),
          QStringLiteral(R"(^(\*\*Data:\*\*\s*`.*`)\s*$)"),
          QStringLiteral(R"(^(\*\*Version:\*\*\s*`[\d\.]+`)\s*$)"),
          QStringLiteral(R"(^(\*\*Date:\*\*\s*`.*`)\s*$)")}},
        {QStringLiteral("manual_md"),
         {QStringLiteral(R"(^(\*\*Vers(?:ã|a)o documentada\s*/\s*Documented version:\*\*\s*`[\d\.]+`)\s*$)"),
          QStringLiteral(R"(^(\*\*Data\s*/\s*Date:\*\*\s*`[^`]+`\s*/\s*`[^`]+`)\s*$)"),
          QStringLiteral(R"(^(### 1\.1\. NOTAS DE LAN(?:Ç|C)AMENTO:\s*Vers(?:ã|a)o\s+[\d\.]+)\s*$)"),
          QStringLiteral(R"(^(### 1\.1\. RELEASE NOTES:\s*Version\s+[\d\.]+)\s*$)")}},
        {QStringLiteral("release_doc"),
         {QStringLiteral(R"(^(# Release Notes — EisenPo v[\d\.]+)\s*$)"),
          QStringLiteral(R"(^(# EisenPo — Vers(?:ã|a)o [\d\.]+ .*)\s*$)"),
          QStringLiteral(R"(^(\*\*Data de Lan(?:ç|c)amento:\*\* .*)\s*$)"),
          QStringLiteral(R"(^(## Principais Recursos e Consolida(?:çã|ca)o da Vers(?:ã|a)o [\d\.]+)\s*$)"),
          QStringLiteral(R"(^(# EisenPo — Version [\d\.]+ .*)\s*$)"),
          QStringLiteral(R"(^(\*\*Release Date:\*\* .*)\s*$)"),
          QStringLiteral(R"(^(## Highlights & Consolidation of Version [\d\.]+)\s*$)")}},
        {QStringLiteral("eisenpo_notes_txt"),
         {QStringLiteral(R"(^(NOTAS DE LAN(?:Ç|C)AMENTO:\s*Vers(?:ã|a)o\s+v?[\d\.]+)\r?$)"),
          QStringLiteral(R"(^(Data:\s*[^\r\n]*\S)\r?$)"),
          QStringLiteral(R"(^(RELEASE NOTES:\s*Version\s+v?[\d\.]+)\r?$)"),
          QStringLiteral(R"(^(Date:\s*[^\r\n]*\S)\r?$)")}},
        {QStringLiteral("appx_manifest"),
         {QStringLiteral(R"(^(\s*<Identity\b[^>]*\bVersion="[\d\.]+"[^>]*/>)\r?$)")}},
    };

    return data;
}

QString resolve_file_path(const QString &base_dir, const QString &relative_path)
{
    const QDir dir(base_dir);
    const QString primary = QFileInfo(dir.filePath(relative_path)).absoluteFilePath();
    if (QFileInfo::exists(primary)) {
        return primary;
    }

    const QStringList rel_parts = QDir::fromNativeSeparators(relative_path).split(QLatin1Char('/'), Qt::SkipEmptyParts);
    if (!rel_parts.isEmpty() && rel_parts.first() == QStringLiteral("source")) {
        QString stripped;
        for (int i = 1; i < rel_parts.size(); ++i) {
            if (!stripped.isEmpty()) {
                stripped += QLatin1Char('/');
            }
            stripped += rel_parts.at(i);
        }

        const QString alternate = QFileInfo(dir.filePath(stripped)).absoluteFilePath();
        if (QFileInfo::exists(alternate)) {
            return alternate;
        }
    }

    return primary;
}

std::optional<QString> read_file_text(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return std::nullopt;
    }

    QByteArray data = file.readAll();
    if (data.startsWith("\xEF\xBB\xBF")) {
        data = data.mid(3);
    }

    QString utf8 = QString::fromUtf8(data);
    if (!utf8.contains(QChar::ReplacementCharacter)) {
        return utf8;
    }

    QString latin1 = QString::fromLatin1(data);
    return latin1;
}

bool write_file_text(const QString &path, const QString &text, QString *error_message)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        if (error_message != nullptr) {
            *error_message = file.errorString();
        }
        return false;
    }

    const qint64 written = file.write(text.toUtf8());
    if (written < 0) {
        if (error_message != nullptr) {
            *error_message = file.errorString();
        }
        return false;
    }

    return true;
}

QString detect_line_ending(const QString &text)
{
    return text.contains(QStringLiteral("\r\n")) ? QStringLiteral("\r\n") : QStringLiteral("\n");
}

QStringList split_lines_preserving_empty(const QString &text)
{
    return text.split(QRegularExpression(QStringLiteral(R"(\r\n|\n|\r)")), Qt::KeepEmptyParts);
}

QString join_lines(const QStringList &lines, const QString &separator)
{
    return lines.join(separator);
}

QStringList first_lines(const QString &text, int limit)
{
    QStringList lines = split_lines_preserving_empty(text);
    if (lines.size() > limit) {
        lines = lines.mid(0, limit);
    }
    return lines;
}

QString replace_readme_version_token(const QString &line, const QString &version)
{
    const QRegularExpression rx(QStringLiteral(R"(^(.*\*\*)v[\d\.]+(\*\*.*)$)"));
    const QRegularExpressionMatch match = rx.match(line);
    if (!match.hasMatch()) {
        return line;
    }

    return QStringLiteral("%1%2%3").arg(match.captured(1), version, match.captured(2));
}

QString mock_windows_icon_name()
{
#if defined(MOCKS_WINDOWS_ICON_NAME)
    return QStringLiteral(MOCKS_WINDOWS_ICON_NAME);
#else
    return QStringLiteral("eisenpo.ico");
#endif
}

QString find_icon_path(const QString &project_root, const QString &preferred_name = QString())
{
    const QString primary_icon_name = preferred_name.trimmed().isEmpty() ? mock_windows_icon_name() : preferred_name.trimmed();
    const QStringList candidate_names = {
        primary_icon_name,
        QStringLiteral("eisenpo.ico"),
        QStringLiteral("eisenpo.png"),
        QStringLiteral("eisenpo1.png"),
        QStringLiteral("calendar.ico"),
        QStringLiteral("calendar.png")
    };

    const QDir root(project_root);
    const QDir app_dir(QCoreApplication::applicationDirPath());
    const QDir current_dir(QDir::currentPath());
    const QDir source_dir(QFileInfo(QString::fromUtf8(__FILE__)).absolutePath());

    for (const QString &icon_name : candidate_names) {
        if (icon_name.isEmpty()) {
            continue;
        }
        QStringList candidates = {
            app_dir.filePath(QStringLiteral("icones/%1").arg(icon_name)),
            current_dir.filePath(QStringLiteral("icones/%1").arg(icon_name)),
            current_dir.filePath(QStringLiteral("mocks/icones/%1").arg(icon_name)),
            source_dir.filePath(QStringLiteral("icones/%1").arg(icon_name)),
            source_dir.filePath(QStringLiteral("../source/assets/eisenhower/icones/%1").arg(icon_name)),
        };

        if (!project_root.isEmpty()) {
            candidates.prepend(root.filePath(QStringLiteral("source/assets/eisenhower/icones/%1").arg(icon_name)));
            candidates.prepend(root.filePath(QStringLiteral("mocks/icones/%1").arg(icon_name)));
            candidates.prepend(root.filePath(QStringLiteral("icones/%1").arg(icon_name)));
        }

        for (const QString &candidate : candidates) {
            if (QFileInfo::exists(candidate)) {
                return QFileInfo(candidate).absoluteFilePath();
            }
        }
    }

    return QString();
}

}  // namespace

QString normalize_dir(const QString &path)
{
    if (path.trimmed().isEmpty()) {
        return QString();
    }

    return QDir::cleanPath(QFileInfo(path).absoluteFilePath());
}

QString detect_project_root()
{
    const QString app_dir = QCoreApplication::applicationDirPath();
    const QString current_dir = QDir::currentPath();
    const QString source_file_dir = QFileInfo(QString::fromUtf8(__FILE__)).absolutePath();

    QStringList seeds = {app_dir, current_dir, source_file_dir};
    QStringList checked;

    auto is_project_root = [](const QString &candidate) {
        const QDir dir(candidate);
        return dir.exists(QStringLiteral("mocks")) && dir.exists(QStringLiteral("source"))
               && QFileInfo(dir.filePath(QStringLiteral("README.md"))).exists();
    };

    for (const QString &seed : seeds) {
        QDir dir(seed);
        for (int depth = 0; depth < 8; ++depth) {
            const QString candidate = normalize_dir(dir.absolutePath());
            if (!checked.contains(candidate)) {
                checked.append(candidate);
                if (is_project_root(candidate)) {
                    return candidate;
                }
            }

            if (!dir.cdUp()) {
                break;
            }
        }
    }

    return normalize_dir(QDir(source_file_dir).filePath(QStringLiteral("..")));
}

QString find_release_doc_relative_path(const QString &base_dir)
{
    const QDir docs_dir(QDir(base_dir).filePath(QStringLiteral("docs")));
    if (docs_dir.exists()) {
        const QStringList matches =
            docs_dir.entryList(QStringList{QStringLiteral("release-v*.md")}, QDir::Files, QDir::Name);
        if (!matches.isEmpty()) {
            return QStringLiteral("docs/%1").arg(matches.last());
        }
    }

    return QStringLiteral("docs/release-v2026.9.28.0.md");
}

QList<FileSpec> file_specs(const QString &base_dir)
{
    QList<FileSpec> data;

    auto make_key = [](const QString &relative_path) {
        QString key;
        bool last_was_separator = false;
        for (const QChar ch : relative_path.toCaseFolded()) {
            if (ch.isLetterOrNumber()) {
                key.append(ch);
                last_was_separator = false;
            } else if (!last_was_separator) {
                key.append(QLatin1Char('_'));
                last_was_separator = true;
            }
        }

        while (key.endsWith(QLatin1Char('_'))) {
            key.chop(1);
        }
        return key;
    };

    auto language_suffix = [](const QString &filename) {
        if (filename.contains(QStringLiteral("_pt_BR")) ||
            filename.startsWith(QStringLiteral("AVISO DE COPYRIGHT"), Qt::CaseInsensitive)) {
            return QStringLiteral("_pt");
        }
        return QStringLiteral("_en");
    };

    auto append = [&](const QString &group,
                      const QString &relative_path,
                      const QString &content_key) {
        data.append({make_key(relative_path), relative_path, content_key, group});
    };

    auto append_asset = [&](const QString &group,
                            const QString &product,
                            const QString &folder,
                            const QStringList &filenames,
                            const QString &content_key_prefix) {
        for (const QString &filename : filenames) {
            const QString relative_path =
                QStringLiteral("source/assets/%1/%2/%3").arg(product, folder, filename);
            append(group, relative_path, content_key_prefix + language_suffix(filename));
        }
    };

    const QStringList products = {QStringLiteral("eisenhower")};

    const QStringList about_files = {
        QStringLiteral("ABOUT_pt_BR.txt"),
        QStringLiteral("ABOUT_en_US.txt"),
        QStringLiteral("ABOUT_EisenPo_pt_BR.txt"),
        QStringLiteral("ABOUT_EisenPo_en_US.txt"),
        QStringLiteral("ABOUT_plugin_pomodoro_pt_BR.txt"),
        QStringLiteral("ABOUT_plugin_pomodoro_en_US.txt"),
        QStringLiteral("ABOUT_plugin_feynman_pt_BR.txt"),
        QStringLiteral("ABOUT_plugin_feynman_en_US.txt"),
        QStringLiteral("ABOUT_plugin_agenda_pt_BR.txt"),
        QStringLiteral("ABOUT_plugin_agenda_en_US.txt"),
        QStringLiteral("ABOUT_plugin_gantt_pt_BR.txt"),
        QStringLiteral("ABOUT_plugin_gantt_en_US.txt"),
    };

    const QStringList clc_files = {
        QStringLiteral("CLC_pt_BR.txt"),
        QStringLiteral("CLC_en_US.txt"),
        QStringLiteral("CLC_EisenPo_pt_BR.txt"),
        QStringLiteral("CLC_EisenPo_en_US.txt"),
        QStringLiteral("CLC_plugin_pomodoro_pt_BR.txt"),
        QStringLiteral("CLC_plugin_pomodoro_en_US.txt"),
        QStringLiteral("CLC_plugin_feynman_pt_BR.txt"),
        QStringLiteral("CLC_plugin_feynman_en_US.txt"),
        QStringLiteral("CLC_plugin_agenda_pt_BR.txt"),
        QStringLiteral("CLC_plugin_agenda_en_US.txt"),
        QStringLiteral("CLC_plugin_gantt_pt_BR.txt"),
        QStringLiteral("CLC_plugin_gantt_en_US.txt"),
    };

    const QStringList copyright_files = {
        QStringLiteral("AVISO DE COPYRIGHT E MARCA REGISTRA_pt_BR.txt"),
        QStringLiteral("COPYRIGHT AND TRADEMARK NOTICE_en_US.txt"),
        QStringLiteral("Copyright_Notice_EisenPo_pt_BR.txt"),
        QStringLiteral("Copyright_Notice_EisenPo_en_US.txt"),
        QStringLiteral("Copyright_Notice_plugin_pomodoro_pt_BR.txt"),
        QStringLiteral("Copyright_Notice_plugin_pomodoro_en_US.txt"),
        QStringLiteral("Copyright_Notice_plugin_feynman_pt_BR.txt"),
        QStringLiteral("Copyright_Notice_plugin_feynman_en_US.txt"),
        QStringLiteral("Copyright_Notice_plugin_agenda_pt_BR.txt"),
        QStringLiteral("Copyright_Notice_plugin_agenda_en_US.txt"),
        QStringLiteral("Copyright_Notice_plugin_gantt_pt_BR.txt"),
        QStringLiteral("Copyright_Notice_plugin_gantt_en_US.txt"),
    };

    const QStringList eula_files = {
        QStringLiteral("EULA_pt_BR.txt"),
        QStringLiteral("EULA_en_US.txt"),
        QStringLiteral("EULA_EisenPo_pt_BR.txt"),
        QStringLiteral("EULA_EisenPo_en_US.txt"),
        QStringLiteral("EULA_plugin_pomodoro_pt_BR.txt"),
        QStringLiteral("EULA_plugin_pomodoro_en_US.txt"),
        QStringLiteral("EULA_plugin_feynman_pt_BR.txt"),
        QStringLiteral("EULA_plugin_feynman_en_US.txt"),
        QStringLiteral("EULA_plugin_agenda_pt_BR.txt"),
        QStringLiteral("EULA_plugin_agenda_en_US.txt"),
        QStringLiteral("EULA_plugin_gantt_pt_BR.txt"),
        QStringLiteral("EULA_plugin_gantt_en_US.txt"),
    };

    const QStringList notice_files = {
        QStringLiteral("NOTICE_pt_BR.txt"),
        QStringLiteral("NOTICE_en_US.txt"),
        QStringLiteral("NOTICE_EisenPo_pt_BR.txt"),
        QStringLiteral("NOTICE_EisenPo_en_US.txt"),
        QStringLiteral("NOTICE_plugin_pomodoro_pt_BR.txt"),
        QStringLiteral("NOTICE_plugin_pomodoro_en_US.txt"),
        QStringLiteral("NOTICE_plugin_feynman_pt_BR.txt"),
        QStringLiteral("NOTICE_plugin_feynman_en_US.txt"),
        QStringLiteral("NOTICE_plugin_agenda_pt_BR.txt"),
        QStringLiteral("NOTICE_plugin_agenda_en_US.txt"),
        QStringLiteral("NOTICE_plugin_gantt_pt_BR.txt"),
        QStringLiteral("NOTICE_plugin_gantt_en_US.txt"),
    };

    const QStringList privacy_files = {
        QStringLiteral("Privacy_Policy_pt_BR.txt"),
        QStringLiteral("Privacy_Policy_en_US.txt"),
        QStringLiteral("Privacy_Policy_EisenPo_pt_BR.txt"),
        QStringLiteral("Privacy_Policy_EisenPo_en_US.txt"),
        QStringLiteral("Privacy_Policy_plugin_pomodoro_pt_BR.txt"),
        QStringLiteral("Privacy_Policy_plugin_pomodoro_en_US.txt"),
        QStringLiteral("Privacy_Policy_plugin_feynman_pt_BR.txt"),
        QStringLiteral("Privacy_Policy_plugin_feynman_en_US.txt"),
        QStringLiteral("Privacy_Policy_plugin_agenda_pt_BR.txt"),
        QStringLiteral("Privacy_Policy_plugin_agenda_en_US.txt"),
        QStringLiteral("Privacy_Policy_plugin_gantt_pt_BR.txt"),
        QStringLiteral("Privacy_Policy_plugin_gantt_en_US.txt"),
    };

    for (const QString &product : products) {
        append_asset(group_about(), product, QStringLiteral("ABOUT"), about_files, QStringLiteral("about"));
        append_asset(group_clc(), product, QStringLiteral("CLC"), clc_files, QStringLiteral("clc"));
        append_asset(group_copyright(),
                     product,
                     QStringLiteral("COPYRIGHT"),
                     copyright_files,
                     QStringLiteral("copyright"));
        append_asset(group_eula(), product, QStringLiteral("EULA"), eula_files, QStringLiteral("eula"));
        append_asset(group_notices(), product, QStringLiteral("NOTICES"), notice_files, QStringLiteral("notice"));
        append_asset(group_privacy(),
                     product,
                     QStringLiteral("PRIVACY_POLICY"),
                     privacy_files,
                     QStringLiteral("privacy"));
    }

    append(group_docs(), QStringLiteral("README.md"), QStringLiteral("readme_pt"));
    append(group_docs(), QStringLiteral("README.md"), QStringLiteral("readme_en"));
    append(group_docs(), QStringLiteral("OBSERVACAO.md"), QStringLiteral("observacao_pt"));
    append(group_docs(), QStringLiteral("MANUAL.md"), QStringLiteral("manual_md"));
    append(group_docs(), QStringLiteral("SECURITY.md"), QStringLiteral("doc_bilingual_md"));
    append(group_docs(), QStringLiteral("CODE_OF_CONDUCT.md"), QStringLiteral("doc_bilingual_md"));
    append(group_docs(), QStringLiteral("CONTRIBUTING.md"), QStringLiteral("doc_bilingual_md"));

    const QString release_rel = find_release_doc_relative_path(base_dir);
    append(group_docs(), release_rel, QStringLiteral("release_doc"));

    const QStringList docs_static = {
        QStringLiteral("docs/architecture.md"),
        QStringLiteral("docs/compliance.md"),
        QStringLiteral("docs/licensing.md"),
        QStringLiteral("docs/privacy-policy.md"),
        QStringLiteral("docs/security-model.md"),
        QStringLiteral("docs/suggested-issues.md"),
        QStringLiteral("docs/technical-audit-and-roadmap.md"),
        QStringLiteral("docs/user-guide.md"),
    };
    for (const QString &doc_rel : docs_static) {
        append(group_docs(), doc_rel, QStringLiteral("doc_bilingual_md"));
    }

    const QDir docs_dir(QDir(base_dir).filePath(QStringLiteral("docs")));
    if (docs_dir.exists()) {
        const QStringList extra_docs = docs_dir.entryList(QStringList{QStringLiteral("*.md")}, QDir::Files, QDir::Name);
        for (const QString &filename : extra_docs) {
            if (filename.startsWith(QStringLiteral("release-v"), Qt::CaseInsensitive)) {
                continue;
            }
            const QString doc_rel = QStringLiteral("docs/%1").arg(filename);
            if (!docs_static.contains(doc_rel) && doc_rel != release_rel) {
                append(group_docs(), doc_rel, QStringLiteral("doc_bilingual_md"));
            }
        }
    }

    append(group_misc(), QStringLiteral("source/services/incore_08_exibir_sobre.cpp"), QStringLiteral("about_dialog"));
    append(group_misc(), QStringLiteral("library/EisenPo.iss"), QStringLiteral("installer_unified"));
    append(group_misc(), QStringLiteral("library/EisenPo_MinGW.iss"), QStringLiteral("installer_mingw"));
    append(group_misc(), QStringLiteral("library/EisenPo_MSVC.iss"), QStringLiteral("installer_msvc"));
    append(group_misc(), QStringLiteral("library/EisenPo_cpp.txt"), QStringLiteral("eisenpo_notes_txt"));
    append(group_misc(), QStringLiteral("library/AppxManifest.xml"), QStringLiteral("appx_manifest"));

    append(group_modules(), QStringLiteral("source/modules/pomodoro/pomodoro_plugin.json"), QStringLiteral("module_manifest"));
    append(group_modules(), QStringLiteral("source/modules/feynman/feynman_plugin.json"), QStringLiteral("module_manifest"));
    append(group_modules(), QStringLiteral("source/modules/agenda/agenda_plugin.json"), QStringLiteral("module_manifest"));
    append(group_modules(), QStringLiteral("source/modules/gantt/gantt_plugin.json"), QStringLiteral("module_manifest"));
    append(group_modules(),
           QStringLiteral("source/modules/pomodoro/pomodoro_plugin_interface.hpp"),
           QStringLiteral("module_interface_iid"));
    append(group_modules(),
           QStringLiteral("source/modules/feynman/feynman_plugin_interface.hpp"),
           QStringLiteral("module_interface_iid"));
    append(group_modules(),
           QStringLiteral("source/modules/agenda/agenda_plugin_interface.hpp"),
           QStringLiteral("module_interface_iid"));
    append(group_modules(),
           QStringLiteral("source/modules/gantt/gantt_plugin_interface.hpp"),
           QStringLiteral("module_interface_iid"));
    append(group_modules(),
           QStringLiteral("source/app/app_05_load_feynman_plugin.cpp"),
           QStringLiteral("app_plugin_status_fallback"));

    return data;
}

const QList<FileSpec> &file_specs()
{
    static const QList<FileSpec> specs = file_specs(detect_project_root());
    return specs;
}

QList<FileStatus> check_expected_lines(const QString &base_dir)
{
    QList<FileStatus> statuses;

    for (const FileSpec &spec : file_specs(base_dir)) {
        FileStatus status;
        status.key = spec.key;
        status.group = spec.group.isEmpty() ? group_misc() : spec.group;
        status.relative_path = spec.relative_path;
        status.path = resolve_file_path(base_dir, spec.relative_path);

        const std::optional<QString> text = read_file_text(status.path);
        if (!text.has_value()) {
            status.exists = false;
            statuses.append(status);
            continue;
        }

        status.exists = true;
        const QStringList pattern_list = patterns().value(spec_content_key(spec));
        if (pattern_list.isEmpty()) {
            status.found = first_lines(text.value(), 3);
            statuses.append(status);
            continue;
        }

        for (const QString &pattern : pattern_list) {
            const QRegularExpression rx(pattern, QRegularExpression::MultilineOption);
            const QRegularExpressionMatch match = rx.match(text.value());
            if (match.hasMatch()) {
                status.found.append(match.captured(1).trimmed());
            } else {
                status.missing.append(pattern);
            }
        }

        statuses.append(status);
    }

    return statuses;
}

bool apply_line_updates(const QString &key, QStringList *lines, const UpdateContext &context)
{
    if (lines == nullptr) {
        return false;
    }

    auto replace_first_middle = [](QString &target, const QRegularExpression &rx, const QString &middle) {
        const QRegularExpressionMatch match = rx.match(target);
        if (!match.hasMatch()) {
            return;
        }
        target = target.left(match.capturedStart(0)) + match.captured(1) + middle + match.captured(2)
                 + target.mid(match.capturedEnd(0));
    };

    bool in_manual_pt_1_1 = false;
    bool in_manual_en_1_1 = false;
    bool seen_notes_pt = false;
    bool seen_notes_en = false;
    bool in_notes_pt = false;
    bool in_notes_en = false;

    bool changed = false;
    for (QString &line : *lines) {
        const QString original = line;

        if (key == QStringLiteral("readme_pt")) {
            if (line.contains(QStringLiteral("img.shields.io/badge/Release-"))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((Release-)v[\d\.]+(-[^)]+\)\]\(docs/))")),
                                     context.pt_readme_version);
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((\(docs/release-)v[\d\.]+(\.md\)))")),
                                     context.pt_readme_version);
            } else if (line.startsWith(QStringLiteral("> **Observação:**"))
                       || line.startsWith(QStringLiteral("> **Observacao:**"))) {
                line = replace_readme_version_token(line, context.pt_readme_version);
            } else if (line.startsWith(QStringLiteral("Versão:"))) {
                line = QStringLiteral("Versão: %1<br>").arg(context.pt_readme_version);
            } else if (line.startsWith(QStringLiteral("Versao:"))) {
                line = QStringLiteral("Versao: %1<br>").arg(context.pt_readme_version);
            } else if (line.startsWith(QStringLiteral("Data técnica desta revisão:"))) {
                line = QStringLiteral("Data técnica desta revisão: %1<br>").arg(context.pt_date);
            } else if (line.startsWith(QStringLiteral("Data tecnica desta revisao:"))) {
                line = QStringLiteral("Data tecnica desta revisao: %1<br>").arg(context.pt_date);
            } else if (line.contains(QStringLiteral("(Versão `")) || line.contains(QStringLiteral("(Versao `"))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((Vers(?:ã|a)o `)[\d\.]+(`))")),
                                     context.pt_version);
            } else if (line.contains(QStringLiteral("(docs/release-v")) && line.contains(QStringLiteral("#ptbr)"))) {
                replace_first_middle(
                    line,
                    QRegularExpression(QStringLiteral(R"((Notas de Lan(?:ç|c)amento da Vers(?:ã|a)o\s+)[\d\.]+(\]\())")),
                    context.pt_version);
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((\(docs/release-)v[\d\.]+(\.md#ptbr\)))")),
                                     context.pt_readme_version);
            }
        } else if (key == QStringLiteral("readme_en")) {
            if (line.startsWith(QStringLiteral("> **Note:**"))) {
                line = replace_readme_version_token(line, context.en_readme_version);
            } else if (line.startsWith(QStringLiteral("Version:"))) {
                line = QStringLiteral("Version: %1<br>").arg(context.en_readme_version);
            } else if (line.startsWith(QStringLiteral("Technical revision date:"))) {
                line = QStringLiteral("Technical revision date: %1<br>").arg(context.en_date);
            } else if (line.contains(QStringLiteral("(Version `"))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((Version `)[\d\.]+(`))")),
                                     context.en_version);
            } else if (line.contains(QStringLiteral("(docs/release-v")) && line.contains(QStringLiteral("#enus)"))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((Release Notes )v[\d\.]+(\]\())")),
                                     context.en_readme_version);
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((\(docs/release-)v[\d\.]+(\.md#enus\)))")),
                                     context.en_readme_version);
            }
        } else if (key == QStringLiteral("observacao_pt")) {
            if (line.startsWith(QStringLiteral("Versão documentada:"))) {
                line = QStringLiteral("Versão documentada: `%1`").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("Versao documentada:"))) {
                line = QStringLiteral("Versao documentada: `%1`").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("Data do snapshot técnico:"))) {
                line = QStringLiteral("Data do snapshot técnico: `%1`").arg(context.pt_date);
            } else if (line.startsWith(QStringLiteral("Data do snapshot tecnico:"))) {
                line = QStringLiteral("Data do snapshot tecnico: `%1`").arg(context.pt_date);
            } else if (line.contains(QStringLiteral("até a versão **`"))
                       || line.contains(QStringLiteral("ate a versao **`"))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((at(?:é|e) a vers(?:ã|a)o \*\*`)[\d\.]+(`\*\*))")),
                                     context.pt_version);
            } else if (line.startsWith(QStringLiteral("## 2. Linha técnica da versão "))
                       || line.startsWith(QStringLiteral("## 2. Linha tecnica da versao "))) {
                line = QStringLiteral("## 2. Linha técnica da versão %1").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("A versão ")) || line.startsWith(QStringLiteral("A versao "))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((^A vers(?:ã|a)o )[\d\.]+(\s+consolida))")),
                                     context.pt_version);
            } else if (line.startsWith(QStringLiteral("Principais mudanças consolidadas na versão "))
                       || line.startsWith(QStringLiteral("Principais mudancas consolidadas na versao "))) {
                line = QStringLiteral("Principais mudanças consolidadas na versão %1:").arg(context.pt_version);
            } else if (line.contains(QStringLiteral("na linha **`"))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((na linha \*\*`)[\d\.]+(`\*\*))")),
                                     context.pt_version);
            } else if (line.startsWith(QStringLiteral("# EISENPO TECHNICAL SNAPSHOT (`"))) {
                line = QStringLiteral("# EISENPO TECHNICAL SNAPSHOT (`%1`)").arg(context.en_version);
            } else if (line.startsWith(QStringLiteral("- **Documented Version:** `"))) {
                line = QStringLiteral("- **Documented Version:** `%1` (`%2`)").arg(context.en_version, context.en_date);
            }
        } else if (key == QStringLiteral("privacy_pt")) {
            if (line.startsWith(QStringLiteral("Versão:"))) {
                line = QStringLiteral("Versão: %1").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("Última atualização:"))) {
                line = QStringLiteral("Última atualização: %1").arg(context.pt_date);
            }
        } else if (key == QStringLiteral("privacy_en")) {
            if (line.startsWith(QStringLiteral("Version:"))) {
                line = QStringLiteral("Version: %1").arg(context.en_version);
            } else if (line.startsWith(QStringLiteral("Last updated:"))) {
                line = QStringLiteral("Last updated: %1").arg(context.en_date);
            }
        } else if (key == QStringLiteral("notice_pt") || key == QStringLiteral("eula_pt")
                   || key == QStringLiteral("copyright_pt")) {
            if (line.startsWith(QStringLiteral("Versão:"))) {
                line = QStringLiteral("Versão: %1").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("Data:"))) {
                line = QStringLiteral("Data: %1").arg(context.pt_date);
            } else if (line.startsWith(QStringLiteral("Versão "))) {
                line = QStringLiteral("Versão %1, %2").arg(context.pt_version, context.pt_date);
            }
        } else if (key == QStringLiteral("notice_en") || key == QStringLiteral("eula_en")
                   || key == QStringLiteral("copyright_en")) {
            if (line.startsWith(QStringLiteral("Version:"))) {
                line = QStringLiteral("Version: %1").arg(context.en_version);
            } else if (line.startsWith(QStringLiteral("Date:"))) {
                line = QStringLiteral("Date: %1").arg(context.en_date);
            } else if (line.startsWith(QStringLiteral("Version "))) {
                line = QStringLiteral("Version %1, %2").arg(context.en_version, context.en_date);
            }
        } else if (key == QStringLiteral("clc_pt")) {
            if (line.startsWith(QStringLiteral("Versão:"))) {
                line = QStringLiteral("Versão: %1").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("Data:"))) {
                line = QStringLiteral("Data: %1").arg(context.pt_date);
            }
        } else if (key == QStringLiteral("clc_en")) {
            if (line.startsWith(QStringLiteral("Version:"))) {
                line = QStringLiteral("Version: %1").arg(context.en_version);
            } else if (line.startsWith(QStringLiteral("Date:"))) {
                line = QStringLiteral("Date: %1").arg(context.en_date);
            }
        } else if (key == QStringLiteral("about_pt")) {
            if (line.startsWith(QStringLiteral("Versão:"))) {
                line = QStringLiteral("Versão: %1").arg(context.pt_version);
            }
        } else if (key == QStringLiteral("about_en")) {
            if (line.startsWith(QStringLiteral("Version:"))) {
                line = QStringLiteral("Version: %1").arg(context.en_version);
            }
        } else if (key == QStringLiteral("about_dialog")) {
            const QRegularExpression rx(QStringLiteral(R"((<p><b>%2:</b>\s*)[\d\.]+(</p>))"));
            const QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch()) {
                line = line.left(match.capturedStart(0)) + match.captured(1) + context.pt_version
                       + match.captured(2) + line.mid(match.capturedEnd(0));
            }
        } else if (key == QStringLiteral("installer_unified") || key == QStringLiteral("installer_mingw")
                   || key == QStringLiteral("installer_msvc")) {
            if (line.startsWith(QStringLiteral("#define MyAppVersion"))) {
                const QRegularExpression rx(QStringLiteral(R"((#define\s+MyAppVersion\s+")[\d\.]+(";?))"));
                const QRegularExpressionMatch match = rx.match(line);
                if (match.hasMatch()) {
                    line = match.captured(1) + context.pt_version + match.captured(2);
                }
            } else if (key == QStringLiteral("installer_mingw")
                       && line.startsWith(QStringLiteral("OutputBaseFilename=EisenPo_mingw_v"))) {
                line = QStringLiteral("OutputBaseFilename=EisenPo_mingw_v%1").arg(context.pt_version);
            } else if (key == QStringLiteral("installer_msvc")
                       && line.trimmed().startsWith(QStringLiteral("OutputBaseFilename=EisenPo_msvc_v"))) {
                const QString indentation = line.left(line.size() - line.trimmed().size());
                line = QStringLiteral("%1OutputBaseFilename=EisenPo_msvc_v%2")
                           .arg(indentation, context.pt_version);
            }
        } else if (key == QStringLiteral("plugin_manifest") || key == QStringLiteral("module_manifest")) {
            const QRegularExpression rx(QStringLiteral(R"(^(\s*"version"\s*:\s*")[^"]+("\s*,?\s*)$)"));
            const QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch()) {
                line = match.captured(1) + context.pt_version + match.captured(2);
            }
        } else if (key == QStringLiteral("plugin_interface_iid") || key == QStringLiteral("module_interface_iid")) {
            const QRegularExpression rx(QStringLiteral(
                R"((#define\s+SOURCE_[A-Z_]+_PLUGIN_INTERFACE_IID\s+"source\.plugin\.[^/"]+/)[\d\.]+("))"));
            const QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch()) {
                line = line.left(match.capturedStart(0)) + match.captured(1) + context.pt_version
                       + match.captured(2) + line.mid(match.capturedEnd(0));
            }
        } else if (key == QStringLiteral("app_plugin_status_fallback")) {
            const QRegularExpression rx(QStringLiteral(R"(QStringLiteral\("\d+(?:\.\d+){1,3}"\))"));
            line.replace(rx, QStringLiteral("QStringLiteral(\"%1\")").arg(context.pt_version));
        } else if (key == QStringLiteral("doc_bilingual_md")) {
            if (line.startsWith(QStringLiteral("**Versão:**")) || line.startsWith(QStringLiteral("**Versao:**"))) {
                line = QStringLiteral("**Versão:** `%1`  ").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("**Data:**"))) {
                line = QStringLiteral("**Data:** `%1`  ").arg(context.pt_date);
            } else if (line.startsWith(QStringLiteral("**Version:**"))) {
                line = QStringLiteral("**Version:** `%1`  ").arg(context.en_version);
            } else if (line.startsWith(QStringLiteral("**Date:**"))) {
                line = QStringLiteral("**Date:** `%1`  ").arg(context.en_date);
            } else {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((\bVersion=")[\d\.]+("))")),
                                     context.pt_version);
                if (line.contains(QStringLiteral("EisenPo_msvc_v"))) {
                    replace_first_middle(line,
                                         QRegularExpression(QStringLiteral(R"((EisenPo_msvc_v)[\d\.]+(\.exe))")),
                                         context.pt_version);
                }
                if (line.contains(QStringLiteral("EisenPo_mingw_v"))) {
                    replace_first_middle(line,
                                         QRegularExpression(QStringLiteral(R"((EisenPo_mingw_v)[\d\.]+(\.exe))")),
                                         context.pt_version);
                }
            }
        } else if (key == QStringLiteral("manual_md")) {
            if (line.startsWith(QStringLiteral("**Versão documentada / Documented version:**"))
                || line.startsWith(QStringLiteral("**Versao documentada / Documented version:**"))) {
                line = QStringLiteral("**Versão documentada / Documented version:** `%1`  ").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("**Data / Date:**"))) {
                line = QStringLiteral("**Data / Date:** `%1` / `%2`  ").arg(context.pt_date, context.en_date);
            } else if (line.startsWith(QStringLiteral("### 1.1. NOTAS DE LANÇAMENTO:"))
                       || line.startsWith(QStringLiteral("### 1.1. NOTAS DE LANCAMENTO:"))) {
                in_manual_pt_1_1 = true;
                in_manual_en_1_1 = false;
                line = QStringLiteral("### 1.1. NOTAS DE LANÇAMENTO: Versão %1").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("### 1.1. RELEASE NOTES:"))) {
                in_manual_en_1_1 = true;
                in_manual_pt_1_1 = false;
                line = QStringLiteral("### 1.1. RELEASE NOTES: Version %1").arg(context.en_version);
            } else if (line.startsWith(QStringLiteral("### ")) || line.startsWith(QStringLiteral("## "))) {
                in_manual_pt_1_1 = false;
                in_manual_en_1_1 = false;
            } else if (in_manual_pt_1_1 && line.startsWith(QStringLiteral("Data:"))) {
                line = QStringLiteral("Data: %1").arg(context.pt_date);
            } else if (in_manual_en_1_1 && line.startsWith(QStringLiteral("Date:"))) {
                line = QStringLiteral("Date: %1").arg(context.en_date);
            }
        } else if (key == QStringLiteral("release_doc")) {
            if (line.startsWith(QStringLiteral("# Release Notes — EisenPo v"))) {
                line = QStringLiteral("# Release Notes — EisenPo %1").arg(context.pt_readme_version);
            } else if (line.startsWith(QStringLiteral("# EisenPo — Versão "))
                       || line.startsWith(QStringLiteral("# EisenPo — Versao "))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((# EisenPo — Vers(?:ã|a)o )[\d\.]+(\s+.*))")),
                                     context.pt_version);
            } else if (line.startsWith(QStringLiteral("**Data de Lançamento:**"))
                       || line.startsWith(QStringLiteral("**Data de Lancamento:**"))) {
                line = QStringLiteral("**Data de Lançamento:** %1  ").arg(context.pt_date);
            } else if (line.startsWith(QStringLiteral("## Principais Recursos e Consolidação da Versão "))
                       || line.startsWith(QStringLiteral("## Principais Recursos e Consolidacao da Versao "))) {
                line = QStringLiteral("## Principais Recursos e Consolidação da Versão %1").arg(context.pt_version);
            } else if (line.startsWith(QStringLiteral("# EisenPo — Version "))) {
                replace_first_middle(line,
                                     QRegularExpression(QStringLiteral(R"((# EisenPo — Version )[\d\.]+(\s+.*))")),
                                     context.en_version);
            } else if (line.startsWith(QStringLiteral("**Release Date:**"))) {
                line = QStringLiteral("**Release Date:** %1  ").arg(context.en_date);
            } else if (line.startsWith(QStringLiteral("## Highlights & Consolidation of Version "))) {
                line = QStringLiteral("## Highlights & Consolidation of Version %1").arg(context.en_version);
            }
        } else if (key == QStringLiteral("eisenpo_notes_txt")) {
            if (!seen_notes_pt
                && (line.startsWith(QStringLiteral("NOTAS DE LANÇAMENTO:"))
                    || line.startsWith(QStringLiteral("NOTAS DE LANCAMENTO:")))) {
                seen_notes_pt = true;
                in_notes_pt = true;
                line = QStringLiteral("NOTAS DE LANÇAMENTO: Versão %1").arg(context.pt_version);
            } else if (in_notes_pt && line.startsWith(QStringLiteral("Data:"))) {
                line = QStringLiteral("Data: %1").arg(context.pt_date);
                in_notes_pt = false;
            } else if (!seen_notes_en && line.startsWith(QStringLiteral("RELEASE NOTES:"))) {
                seen_notes_en = true;
                in_notes_en = true;
                line = QStringLiteral("RELEASE NOTES: Version %1").arg(context.en_version);
            } else if (in_notes_en && line.startsWith(QStringLiteral("Date:"))) {
                line = QStringLiteral("Date: %1").arg(context.en_date);
                in_notes_en = false;
            }
        } else if (key == QStringLiteral("appx_manifest")) {
            if (line.contains(QStringLiteral("<Identity"))) {
                replace_first_middle(
                    line,
                    QRegularExpression(QStringLiteral(R"((\s*<Identity\b[^>]*\bVersion=")[\d\.]+("[^>]*/>))")),
                    context.pt_version);
            }
        }

        if (line != original) {
            changed = true;
        }
    }

    return changed;
}

QList<UpdateResult> apply_updates(const UpdateContext &context, const QString &base_dir)
{
    QList<UpdateResult> results;

    for (const FileSpec &spec : file_specs(base_dir)) {
        UpdateResult result;
        result.key = spec.key;
        result.group = spec.group.isEmpty() ? group_misc() : spec.group;
        result.relative_path = spec.relative_path;

        const QString content_key = spec_content_key(spec);
        const QString path = resolve_file_path(base_dir, spec.relative_path);
        const std::optional<QString> text = read_file_text(path);
        if (!text.has_value()) {
            result.ok = false;
            result.message = QStringLiteral("arquivo inexistente");
            results.append(result);
            continue;
        }

        QString target_rel = spec.relative_path;
        QString target_path = path;
        if (content_key == QStringLiteral("release_doc")) {
            target_rel = QStringLiteral("docs/release-v%1.md").arg(context.pt_version);
            target_path = QFileInfo(QDir(base_dir).filePath(target_rel)).absoluteFilePath();
        }
        const bool renamed_target = (QFileInfo(path).absoluteFilePath() != target_path);

        const QString line_ending = detect_line_ending(text.value());
        QStringList lines = split_lines_preserving_empty(text.value());
        const bool changed = apply_line_updates(content_key, &lines, context);

        if (!changed && !renamed_target) {
            const QString current_text = text.value();
            const QStringList expected_patterns = patterns().value(content_key);
            bool all_matched = !expected_patterns.isEmpty();
            for (const QString &pattern : expected_patterns) {
                const QRegularExpression rx(pattern, QRegularExpression::MultilineOption);
                if (!rx.match(current_text).hasMatch()) {
                    all_matched = false;
                    break;
                }
            }

            result.ok = all_matched;
            result.message = all_matched ? QStringLiteral("já atualizado")
                                         : QStringLiteral("padrões não encontrados / sem alteração");
            results.append(result);
            continue;
        }

        QString error_message;
        if (write_file_text(target_path, join_lines(lines, line_ending), &error_message)) {
            if (renamed_target && QFile::exists(path)
                && QFileInfo(path).canonicalFilePath() != QFileInfo(target_path).canonicalFilePath()) {
                QFile::remove(path);
                result.relative_path = target_rel;
                result.message = QStringLiteral("atualizado (%1 -> %2)").arg(spec.relative_path, target_rel);
            } else {
                result.message = QStringLiteral("atualizado");
            }
            result.ok = true;
        } else {
            result.ok = false;
            result.message = QStringLiteral("erro escrita: %1").arg(error_message);
        }

        results.append(result);
    }

    return results;
}

VersionEditorWidget::VersionEditorWidget(QWidget *parent) : QWidget(parent), selected_base_dir_(detect_project_root())
{
    setWindowTitle(QStringLiteral("Editor de Versões / Datas - EisenPo (PT/EN)"));
    resize(980, 640);

    const QDate today = QDate::currentDate();
    const QString default_version = build_version_from_date(today);
    const int default_day = today.isValid() ? today.day() : 1;
    const int default_month_index = today.isValid() ? qMax(0, today.month() - 1) : 0;
    const int default_year = today.isValid() ? today.year() : 2025;

    const QString icon_path = find_icon_path(selected_base_dir_, QStringLiteral("eisenpo.ico"));
    if (!icon_path.isEmpty()) {
        const QIcon icon(icon_path);
        QApplication::setWindowIcon(icon);
        setWindowIcon(icon);
    }

    auto *layout = new QVBoxLayout(this);

    auto *folder_layout = new QHBoxLayout();
    folder_layout->addWidget(new QLabel(QStringLiteral("Pasta dos arquivos:"), this));

    folder_path_ = new QLineEdit(selected_base_dir_, this);
    folder_path_->setReadOnly(true);
    folder_layout->addWidget(folder_path_);

    select_folder_btn_ = new QPushButton(QStringLiteral("Selecionar..."), this);
    folder_layout->addWidget(select_folder_btn_);
    layout->addLayout(folder_layout);

    auto *pt_group = new QGroupBox(QStringLiteral("Português (pt-BR)"), this);
    auto *pt_layout = new QHBoxLayout(pt_group);
    pt_layout->addWidget(new QLabel(QStringLiteral("Versão:"), pt_group));
    pt_version_ = new QLineEdit(default_version, pt_group);
    pt_layout->addWidget(pt_version_);

    pt_layout->addWidget(new QLabel(QStringLiteral("Dia:"), pt_group));
    pt_day_ = new QSpinBox(pt_group);
    pt_day_->setRange(1, 31);
    pt_day_->setValue(default_day);
    pt_layout->addWidget(pt_day_);

    pt_layout->addWidget(new QLabel(QStringLiteral("Mês:"), pt_group));
    pt_month_ = new QComboBox(pt_group);
    pt_month_->addItems(kPtMonths);
    pt_month_->setCurrentIndex(default_month_index);
    pt_layout->addWidget(pt_month_);

    pt_layout->addWidget(new QLabel(QStringLiteral("Ano:"), pt_group));
    pt_year_ = new QSpinBox(pt_group);
    pt_year_->setRange(1900, 3000);
    pt_year_->setValue(default_year);
    pt_layout->addWidget(pt_year_);
    layout->addWidget(pt_group);

    auto *en_group = new QGroupBox(QStringLiteral("English (en-US)"), this);
    auto *en_layout = new QHBoxLayout(en_group);
    en_layout->addWidget(new QLabel(QStringLiteral("Version:"), en_group));
    en_version_ = new QLineEdit(default_version, en_group);
    en_layout->addWidget(en_version_);

    en_layout->addWidget(new QLabel(QStringLiteral("Day:"), en_group));
    en_day_ = new QSpinBox(en_group);
    en_day_->setRange(1, 31);
    en_day_->setValue(default_day);
    en_layout->addWidget(en_day_);

    en_layout->addWidget(new QLabel(QStringLiteral("Month:"), en_group));
    en_month_ = new QComboBox(en_group);
    en_month_->addItems(kEnMonths);
    en_month_->setCurrentIndex(default_month_index);
    en_layout->addWidget(en_month_);

    en_layout->addWidget(new QLabel(QStringLiteral("Year:"), en_group));
    en_year_ = new QSpinBox(en_group);
    en_year_->setRange(1900, 3000);
    en_year_->setValue(default_year);
    en_layout->addWidget(en_year_);
    layout->addWidget(en_group);

    auto *btn_layout = new QHBoxLayout();
    check_btn_ = new QPushButton(QStringLiteral("Verificar arquivos"), this);
    save_btn_ = new QPushButton(QStringLiteral("Salvar alterações"), this);
    refresh_btn_ = new QPushButton(QStringLiteral("Recarregar verificação"), this);

    const QString check_icon = find_icon_path(selected_base_dir_, QStringLiteral("eisenpo.png"));
    if (!check_icon.isEmpty()) {
        check_btn_->setIcon(QIcon(check_icon));
    }
    const QString save_icon = find_icon_path(selected_base_dir_, QStringLiteral("eisenpo1.png"));
    if (!save_icon.isEmpty()) {
        save_btn_->setIcon(QIcon(save_icon));
    }
    const QString calendar_icon = find_icon_path(selected_base_dir_, QStringLiteral("calendar.png"));
    if (!calendar_icon.isEmpty()) {
        refresh_btn_->setIcon(QIcon(calendar_icon));
        select_folder_btn_->setIcon(QIcon(calendar_icon));
    }

    btn_layout->addWidget(check_btn_);
    btn_layout->addWidget(save_btn_);
    btn_layout->addWidget(refresh_btn_);
    layout->addLayout(btn_layout);

    status_tabs_ = new QTabWidget(this);
    status_tabs_->setDocumentMode(true);
    status_tabs_->setUsesScrollButtons(true);
    status_tabs_->setElideMode(Qt::ElideRight);
    layout->addWidget(status_tabs_, 1);

    connect(select_folder_btn_, &QPushButton::clicked, this, [this]() { selectBaseDirectory(); });
    connect(check_btn_, &QPushButton::clicked, this, [this]() { checkFiles(); });
    connect(save_btn_, &QPushButton::clicked, this, [this]() { saveChanges(); });
    connect(refresh_btn_, &QPushButton::clicked, this, [this]() { checkFiles(); });

    checkFiles();
}

void VersionEditorWidget::selectBaseDirectory()
{
    const QString selected = QFileDialog::getExistingDirectory(
        this, QStringLiteral("Selecione a pasta onde os arquivos estão"), selected_base_dir_);
    if (selected.isEmpty()) {
        return;
    }

    selected_base_dir_ = normalize_dir(selected);
    folder_path_->setText(selected_base_dir_);
    checkFiles();
}

void VersionEditorWidget::checkFiles()
{
    const QList<FileStatus> statuses = check_expected_lines(selected_base_dir_);
    QMap<QString, QStringList> grouped_lines;
    for (const QString &group : group_order()) {
        grouped_lines[group].append(QStringLiteral("Base: %1").arg(selected_base_dir_));
        grouped_lines[group].append(QString());
    }

    for (const FileStatus &status : statuses) {
        QStringList &lines = grouped_lines[status.group];
        lines.append(QStringLiteral("%1").arg(status.relative_path));
        lines.append(QStringLiteral("  chave: %1").arg(status.key));
        lines.append(QStringLiteral("  arquivo: %1").arg(status.exists ? QStringLiteral("existe") : QStringLiteral("ausente")));

        if (status.exists) {
            if (!status.found.isEmpty()) {
                lines.append(QStringLiteral("  encontrado:"));
                for (const QString &entry : status.found) {
                    lines.append(QStringLiteral("    - %1").arg(entry));
                }
            }

            if (!status.missing.isEmpty()) {
                lines.append(QStringLiteral("  ausente no conteúdo:"));
                for (const QString &entry : status.missing) {
                    lines.append(QStringLiteral("    - %1").arg(entry));
                }
            }
        }

        lines.append(QString());
    }

    QList<QPair<QString, QStringList>> groups;
    for (const QString &group : group_order()) {
        groups.append({group, grouped_lines.value(group)});
    }
    setStatusGroups(groups);
}

void VersionEditorWidget::saveChanges()
{
    const QString pt_version = pt_version_->text().trimmed();
    const int pt_day = pt_day_->value();
    const int pt_month_index = pt_month_->currentIndex();
    const int pt_year = pt_year_->value();

    const QString en_version = en_version_->text().trimmed();
    const int en_day = en_day_->value();
    const int en_month_index = en_month_->currentIndex();
    const int en_year = en_year_->value();

    const UpdateContext context = {
        pt_version,
        QStringLiteral("%1 de %2 de %3")
            .arg(QString::number(pt_day), kPtMonthsLower.value(pt_month_index), QString::number(pt_year)),
        en_version,
        QStringLiteral("%1 %2, %3")
            .arg(kEnMonths.value(en_month_index), QString::number(en_day), QString::number(en_year)),
        readme_version_token(pt_version),
        readme_version_token(en_version),
    };

    const QList<UpdateResult> results = apply_updates(context, selected_base_dir_);

    QMap<QString, QStringList> grouped_lines;
    for (const QString &group : group_order()) {
        grouped_lines[group].append(QStringLiteral("Resultados:"));
        grouped_lines[group].append(QString());
    }

    for (const UpdateResult &result : results) {
        QStringList &lines = grouped_lines[result.group];
        lines.append(QStringLiteral("%1: %2 - %3")
                         .arg(result.relative_path,
                              result.ok ? QStringLiteral("OK") : QStringLiteral("FAIL"),
                              result.message));
    }

    QList<QPair<QString, QStringList>> groups;
    for (const QString &group : group_order()) {
        groups.append({group, grouped_lines.value(group)});
    }
    setStatusGroups(groups);
}

void VersionEditorWidget::setStatusLines(const QStringList &lines)
{
    setStatusGroups({{group_misc(), lines}});
}

void VersionEditorWidget::setStatusGroups(const QList<QPair<QString, QStringList>> &groups)
{
    if (status_tabs_ == nullptr) {
        return;
    }

    const QString current_title =
        status_tabs_->currentIndex() >= 0 ? status_tabs_->tabText(status_tabs_->currentIndex()) : QString();

    while (status_tabs_->count() > 0) {
        QWidget *page = status_tabs_->widget(0);
        status_tabs_->removeTab(0);
        if (page != nullptr) {
            page->deleteLater();
        }
    }

    int target_index = 0;
    for (const auto &group : groups) {
        auto *editor = new QTextEdit(status_tabs_);
        editor->setReadOnly(true);
        editor->setLineWrapMode(QTextEdit::NoWrap);
        QStringList lines = group.second;
        if (lines.isEmpty()) {
            lines.append(QStringLiteral("Nenhum arquivo neste grupo."));
        }
        editor->setPlainText(lines.join(QLatin1Char('\n')));

        const int index = status_tabs_->addTab(editor, group.first);
        if (!current_title.isEmpty() && group.first == current_title) {
            target_index = index;
        }
    }

    if (status_tabs_->count() > 0) {
        status_tabs_->setCurrentIndex(qBound(0, target_index, status_tabs_->count() - 1));
    }
}

}  // namespace mocks::version_editor

int main(int argc, char *argv[])
{
    bool check_mode = false;
    QString explicit_root;

    for (int i = 1; i < argc; ++i) {
        const QString raw_arg = QString::fromLocal8Bit(argv[i]).trimmed();
        const QString lower_arg = raw_arg.toLower();
        if (lower_arg == QStringLiteral("--check") || lower_arg == QStringLiteral("-c")
            || lower_arg == QStringLiteral("--status")) {
            check_mode = true;
        } else if ((lower_arg == QStringLiteral("--root") || lower_arg == QStringLiteral("-r")) && i + 1 < argc) {
            explicit_root = QString::fromLocal8Bit(argv[++i]).trimmed();
        } else if (lower_arg.startsWith(QStringLiteral("--root="))) {
            explicit_root = raw_arg.mid(7).trimmed();
        } else if (!raw_arg.startsWith(QLatin1Char('-')) && explicit_root.isEmpty()) {
            explicit_root = raw_arg;
        }
    }

    if (check_mode) {
#if defined(Q_OS_WIN)
        if (AttachConsole(ATTACH_PARENT_PROCESS)) {
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut == nullptr || hOut == INVALID_HANDLE_VALUE || GetFileType(hOut) == FILE_TYPE_UNKNOWN) {
                FILE *fp = nullptr;
                freopen_s(&fp, "CONOUT$", "w", stdout);
                freopen_s(&fp, "CONOUT$", "w", stderr);
            }
        }
#endif
        QTextStream out(stdout);
        const QString base_dir = !explicit_root.isEmpty() ? mocks::version_editor::normalize_dir(explicit_root)
                                                          : mocks::version_editor::detect_project_root();
        const auto statuses = mocks::version_editor::check_expected_lines(base_dir);
        bool all_ok = true;
        for (const auto &status : statuses) {
            if (!status.exists) {
                all_ok = false;
                out << "[AUSENTE] " << status.relative_path << "\n";
            } else if (!status.missing.isEmpty()) {
                all_ok = false;
                out << "[PADRAO AUSENTE] " << status.relative_path << "\n";
            } else {
                out << "[OK] " << status.relative_path << "\n";
            }
        }
        out << (all_ok ? "TODOS OS ARQUIVOS VERIFICADOS COM SUCESSO.\n"
                       : "FALHAS NA VERIFICACAO DE ARQUIVOS.\n");
        out.flush();
        return all_ok ? 0 : 1;
    }

    QApplication app(argc, argv);
    mocks::version_editor::VersionEditorWidget widget;
    widget.show();
    return app.exec();
}
