#pragma once

#include <atomic>
#include <memory>
#include <QFile>
#include <QMutex>
#include <QString>
#include <QTextStream>

namespace source::utils {

enum class LogLevel : int {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Critical = 4
};

class LogManager {
public:
    static LogManager *get_logger();
    static QString get_log_file();
    static void enable_startup_diagnostics();
    static void install_exception_hook();
    static void install_qt_message_handler();
    static void uninstall_qt_message_handler();
    static void close_startup_diagnostics();

    static QString ensure_unicode(const QString &message);
    static QString ensure_unicode(const QByteArray &message);

    void set_minimum_level(LogLevel level);
    LogLevel minimum_level() const;
    bool is_level_enabled(LogLevel level) const;

    void debug(const QString &message);
    void info(const QString &message);
    void warning(const QString &message);
    void error(const QString &message, bool exc_info = false);
    void critical(const QString &message, bool exc_info = true);

    ~LogManager();

private:
    LogManager();

    void configure_logging();
    QString persistent_base_dir() const;
    QString log_dir() const;
    QString startup_log_path() const;
    void write_startup_log(const QString &text);
    void cleanup_logs();
    void log(const QString &level, const QString &message);
    static void show_fatal_message(const QString &title, const QString &message);

    QMutex mutex_;
    QString log_file_;
    QString startup_log_file_;
    QFile startup_file_;
    QFile log_file_handle_;
    std::unique_ptr<QTextStream> log_stream_;
    std::atomic<LogLevel> minimum_level_{LogLevel::Info};
};

}  // namespace source::utils
