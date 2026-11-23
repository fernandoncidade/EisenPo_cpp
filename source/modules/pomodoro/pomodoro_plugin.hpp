#pragma once

#include <QDateTime>
#include <QObject>
#include <QPointer>

#include "source/modules/pomodoro/Pomodoro/tmp_05_GerenciadorTempo.hpp"
#include "source/modules/pomodoro/pomodoro_host_bridge.hpp"
#include "source/modules/pomodoro/pomodoro_plugin_interface.hpp"

namespace source::plugin::Pomodoro::language {
class GerenciadorTraducao;
}

namespace source::plugin {

class PomodoroPlugin final : public QObject, public IPomodoroPlugin {
    Q_OBJECT
    Q_INTERFACES(source::plugin::IPomodoroPlugin)

public:
    explicit PomodoroPlugin(QObject *parent = nullptr);
    ~PomodoroPlugin() override;

    QString plugin_id() const override;
    QString display_name() const override;
    QString open_menu_text() const override;
    QString alarm_menu_text() const override;
    QString automatic_mode_menu_text() const override;
    void initialize(PomodoroHostBridge *host_bridge) override;
    QWidget *create_widget(QWidget *parent = nullptr) override;
    void open_alarm_settings(QWidget *parent = nullptr) override;
    void open_automatic_mode_settings(QWidget *parent = nullptr) override;
    void retranslate_ui() override;
    void receive_eisenhower_task(const QVariantMap &task) override;
    void reload_shared_state() override;
    void refresh_task_views() override;
    int selected_task_count() const override;
    QString selected_task_color() const override;
    int apply_color_to_selected_tasks(const QString &color) override;
    void reset_task_font_overrides(bool target_linked) override;
    QVariantList get_task_font_overrides(bool target_linked) const override;
    int configured_cycles() const override;
    void set_configured_cycles(int cycles) override;
    void redefinir_ciclos_em_progresso(int cycles) override;

private:
    PomodoroHostBridge *host_bridge_ = nullptr;
    source::plugin::Pomodoro::language::GerenciadorTraducao *translation_manager_ = nullptr;
    QPointer<source::plugin::Pomodoro::GerenciadorTempo> widget_;
    QDateTime last_file_mtime_;
};

}  // namespace source::plugin
