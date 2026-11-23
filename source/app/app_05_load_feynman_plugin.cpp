#include "source/app/app_05_load_feynman_plugin.hpp"

#include <QDir>
#include <QFile>
#include <QListWidget>

#include "source/modules/feynman/feynman_plugin_interface.hpp"
#include "source/app/app_02_get_text.hpp"
#include "source/app/app_01_eisenhower_matrix_app.hpp"
#include "source/services/incore_11_atualizar_itens_tarefas.hpp"
#include "source/ui/ui_09_Calendar.hpp"
#include "source/ui/ui_14_CalendarPanel.hpp"
#include "source/ui/ui_12_CalendarDialog.hpp"
#include "source/utils/LinkedTitleUnderlineDelegate.hpp"
#include "source/modules/pomodoro/Pomodoro/pomodoro_support.hpp"

namespace source {
using namespace app;

void EisenhowerMatrixApp::_load_feynman_plugin()
{
    if (feynman_plugin_ != nullptr) {
        return;
    }

    auto *plugin = new source::plugin::FeynmanPlugin(this);
    feynman_plugin_ = plugin;
    feynman_plugin_path_ = QStringLiteral("integrated:feynman");
    plugin_load_errors_.remove(QStringLiteral("feynman"));
    _sync_feynman_host_bridge();
    feynman_plugin_->initialize(feynman_host_bridge_);
    feynman_module_page = nullptr; // Feynman now opens as a dialog
    _connect_feynman_task_counter_sources();
    logger()->info(QStringLiteral("Módulo Feynman unificado e inicializado com sucesso."));
}

void EisenhowerMatrixApp::_load_pomodoro_plugin()
{
    if (pomodoro_plugin_ != nullptr) {
        return;
    }

    auto *plugin = new source::plugin::PomodoroPlugin(this);
    pomodoro_plugin_ = plugin;
    pomodoro_plugin_path_ = QStringLiteral("integrated:pomodoro");
    plugin_load_errors_.remove(QStringLiteral("pomodoro"));
    _sync_pomodoro_host_bridge();
    pomodoro_plugin_->initialize(pomodoro_host_bridge_);
    pomodoro_module_page = nullptr;
    logger()->info(QStringLiteral("Módulo Pomodoro unificado e inicializado com sucesso."));
}

void EisenhowerMatrixApp::_load_agenda_plugin()
{
    if (agenda_plugin_ != nullptr) {
        return;
    }

    auto *plugin = new source::plugin::AgendaPlugin(this);
    agenda_plugin_ = plugin;
    agenda_plugin_path_ = QStringLiteral("integrated:agenda");
    plugin_load_errors_.remove(QStringLiteral("agenda"));
    _sync_agenda_host_bridge();
    agenda_plugin_->initialize(agenda_host_bridge_);
    logger()->info(QStringLiteral("Módulo Registro de Atividades unificado e inicializado com sucesso."));
}

void EisenhowerMatrixApp::_load_gantt_plugin()
{
    if (gantt_plugin_ != nullptr) {
        return;
    }

    auto *plugin = new source::plugin::GanttPlugin(this);
    gantt_plugin_ = plugin;
    gantt_plugin_path_ = QStringLiteral("integrated:gantt");
    plugin_load_errors_.remove(QStringLiteral("gantt"));
    _sync_gantt_host_bridge();
    gantt_plugin_->initialize(gantt_host_bridge_);
    logger()->info(QStringLiteral("Módulo Cronograma de Gantt & BIM 4D unificado e inicializado com sucesso."));
}

bool EisenhowerMatrixApp::has_feynman_plugin() const
{
    return feynman_plugin_ != nullptr;
}

QString EisenhowerMatrixApp::feynman_display_name() const
{
    return feynman_plugin_ != nullptr ? feynman_plugin_->display_name() : get_text(QStringLiteral("Feynman"));
}

QString EisenhowerMatrixApp::feynman_open_menu_text() const
{
    return feynman_plugin_ != nullptr ? feynman_plugin_->open_menu_text() : get_text(QStringLiteral("Abrir Feynman"));
}

QString EisenhowerMatrixApp::feynman_ui_layout_mode_key() const
{
    return feynman_ui_layout_mode_key_;
}

bool EisenhowerMatrixApp::set_feynman_ui_layout_mode_key(const QString &mode_key, bool persist)
{
    const QString normalized = normalized_feynman_ui_layout_key(mode_key);
    const QString previous = feynman_ui_layout_mode_key_;

    if (feynman_module_page != nullptr) {
        const bool task_panel_visible = feynman_task_panel_visible();
        bool applied = false;
        const bool invoked = QMetaObject::invokeMethod(feynman_module_page,
                                                       "set_ui_layout_mode_key",
                                                       Qt::DirectConnection,
                                                       Q_RETURN_ARG(bool, applied),
                                                       Q_ARG(QString, normalized));
        if (!invoked || !applied) {
            feynman_ui_layout_mode_key_ = previous;
            return false;
        }

        feynman_module_page->updateGeometry();
        relax_module_page(feynman_module_page);
        set_feynman_task_panel_visible(task_panel_visible, false);
        refresh_module_task_counter();
        _queue_window_geometry_stabilization();
    }

    feynman_ui_layout_mode_key_ = normalized;
    if (persist) {
        _save_task_layout_preference();
    }
    return true;
}

void EisenhowerMatrixApp::open_eisenhower()
{
}

static int priority_for_app_list(source::EisenhowerMatrixApp *app, QListWidget *list)
{
    if (app == nullptr || list == nullptr) {
        return 1;
    }
    if (list == app->quadrant1_list || list == app->quadrant1_completed_list || list == app->quadrant1_in_progress_list) {
        return 1;
    }
    if (list == app->quadrant2_list || list == app->quadrant2_completed_list || list == app->quadrant2_in_progress_list) {
        return 2;
    }
    if (list == app->quadrant3_list || list == app->quadrant3_completed_list || list == app->quadrant3_in_progress_list) {
        return 3;
    }
    if (list == app->quadrant4_list || list == app->quadrant4_completed_list || list == app->quadrant4_in_progress_list) {
        return 4;
    }
    return 1;
}

void EisenhowerMatrixApp::open_feynman_dialog_for_task(QListWidgetItem *item)
{
    if (feynman_plugin_ == nullptr || item == nullptr) {
        return;
    }

    QVariantMap payload = item->data(Qt::UserRole).toMap();
    QListWidget *src_list = item->listWidget();

    if (!payload.contains(QStringLiteral("priority")) && !payload.contains(QStringLiteral("prioridade"))) {
        if (src_list != nullptr) {
            const int prio = priority_for_app_list(this, src_list);
            payload.insert(QStringLiteral("priority"), prio);
            payload.insert(QStringLiteral("prioridade"), prio);
            payload.insert(QStringLiteral("quadrante"), prio);
        }
    } else {
        const QVariant pval = payload.contains(QStringLiteral("priority")) ? payload.value(QStringLiteral("priority"))
                                                                          : payload.value(QStringLiteral("prioridade"));
        payload.insert(QStringLiteral("priority"), pval);
        payload.insert(QStringLiteral("prioridade"), pval);
    }

    if (!payload.contains(QStringLiteral("dominio")) && payload.contains(QStringLiteral("domain"))) {
        payload.insert(QStringLiteral("dominio"), payload.value(QStringLiteral("domain")));
    } else if (!payload.contains(QStringLiteral("domain")) && payload.contains(QStringLiteral("dominio"))) {
        payload.insert(QStringLiteral("domain"), payload.value(QStringLiteral("dominio")));
    }

    if (!payload.contains(QStringLiteral("titulo")) && payload.contains(QStringLiteral("text"))) {
        payload.insert(QStringLiteral("titulo"), payload.value(QStringLiteral("text")));
    } else if (!payload.contains(QStringLiteral("text")) && payload.contains(QStringLiteral("titulo"))) {
        payload.insert(QStringLiteral("text"), payload.value(QStringLiteral("titulo")));
    }

    const QVariantMap result = feynman_plugin_->open_feynman_dialog_for_task(payload, this);
    
    if (!result.isEmpty()) {
        QVariantMap new_data = result;
        const QString text = new_data.value(QStringLiteral("text")).toString().isEmpty()
                                 ? new_data.value(QStringLiteral("titulo")).toString()
                                 : new_data.value(QStringLiteral("text")).toString();
        new_data.insert(QStringLiteral("text"), text);
        new_data.insert(QStringLiteral("titulo"), text);

        int new_prio = 1;
        if (new_data.contains(QStringLiteral("priority"))) {
            new_prio = new_data.value(QStringLiteral("priority")).toInt();
        } else if (new_data.contains(QStringLiteral("prioridade"))) {
            new_prio = new_data.value(QStringLiteral("prioridade")).toInt();
        }
        if (new_prio < 1 || new_prio > 4) new_prio = 1;

        new_data.insert(QStringLiteral("priority"), new_prio);
        new_data.insert(QStringLiteral("prioridade"), new_prio);
        new_data.insert(QStringLiteral("quadrante"), new_prio);

        if (new_data.contains(QStringLiteral("dominio"))) {
            new_data.insert(QStringLiteral("domain"), new_data.value(QStringLiteral("dominio")));
        } else if (new_data.contains(QStringLiteral("domain"))) {
            new_data.insert(QStringLiteral("dominio"), new_data.value(QStringLiteral("domain")));
        }

        // Determine target list if priority changed
        QListWidget *target_list = src_list;
        if (src_list != nullptr && new_prio >= 1 && new_prio <= 4) {
            if (is_in_progress_list(src_list)) {
                const QList<QListWidget *> in_progress = {quadrant1_in_progress_list,
                                                          quadrant2_in_progress_list,
                                                          quadrant3_in_progress_list,
                                                          quadrant4_in_progress_list};
                target_list = in_progress.at(new_prio - 1);
            } else if (is_completed_list(src_list)) {
                const QList<QListWidget *> completed = {quadrant1_completed_list,
                                                        quadrant2_completed_list,
                                                        quadrant3_completed_list,
                                                        quadrant4_completed_list};
                target_list = completed.at(new_prio - 1);
            } else {
                const QList<QListWidget *> normal = {quadrant1_list,
                                                     quadrant2_list,
                                                     quadrant3_list,
                                                     quadrant4_list};
                target_list = normal.at(new_prio - 1);
            }
        }

        const auto display_and_tooltip = display_and_tooltip_for_task(this, new_data, -1);

        if (target_list != nullptr && target_list != src_list && src_list != nullptr) {
            const int row = src_list->row(item);
            const Qt::CheckState check_state = item->checkState();
            if (row >= 0) {
                delete src_list->takeItem(row);
            }
            cleanup_time_groups(src_list);

            auto *new_item = new QListWidgetItem(display_and_tooltip.first);
            new_item->setFlags(new_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            new_item->setCheckState(check_state);
            new_item->setData(Qt::UserRole, new_data);
            new_item->setToolTip(display_and_tooltip.second);
            source::utils::apply_task_title_font(new_item, text, !new_data.value(QStringLiteral("file_path")).toString().isEmpty());

            if (target_list->count() == 1 && !(target_list->item(0)->flags() & Qt::ItemIsSelectable)) {
                target_list->clear();
            }
            insert_task_into_quadrant_list(target_list, new_item);
            cleanup_time_groups(target_list);
        } else {
            item->setData(Qt::UserRole, new_data);
            item->setText(display_and_tooltip.first);
            item->setToolTip(display_and_tooltip.second);
            source::utils::apply_task_title_font(item, text, !new_data.value(QStringLiteral("file_path")).toString().isEmpty());
            if (src_list != nullptr) {
                emit src_list->itemSelectionChanged();
            }
        }

        save_tasks();
        if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
            calendar_pane->calendar_panel->update_task_list();
        }
    }
}

void EisenhowerMatrixApp::open_feynman_edit_labels_dialog(QWidget *parent)
{
    if (feynman_plugin_ != nullptr) {
        feynman_plugin_->open_edit_labels_dialog(parent != nullptr ? parent : this);
    }
}

bool EisenhowerMatrixApp::has_pomodoro_plugin() const
{
    return pomodoro_plugin_ != nullptr;
}

QString EisenhowerMatrixApp::pomodoro_display_name() const
{
    return pomodoro_plugin_ != nullptr ? pomodoro_plugin_->display_name() : get_text(QStringLiteral("Pomodoro"));
}

QString EisenhowerMatrixApp::pomodoro_open_menu_text() const
{
    return pomodoro_plugin_ != nullptr ? pomodoro_plugin_->open_menu_text() : get_text(QStringLiteral("Abrir Pomodoro"));
}

QString EisenhowerMatrixApp::pomodoro_alarm_menu_text() const
{
    return pomodoro_plugin_ != nullptr ? pomodoro_plugin_->alarm_menu_text() : get_text(QStringLiteral("Alarme"));
}

QString EisenhowerMatrixApp::pomodoro_automatic_mode_menu_text() const
{
    return pomodoro_plugin_ != nullptr ? pomodoro_plugin_->automatic_mode_menu_text()
                                      : get_text(QStringLiteral("Modo automático"));
}

QString EisenhowerMatrixApp::pomodoro_report_pomodoros_menu_text() const
{
    return get_text(QStringLiteral("Reportar Pomodoros"));
}

QString EisenhowerMatrixApp::pomodoro_ui_layout_mode_key() const
{
    return pomodoro_ui_layout_mode_key_;
}

bool EisenhowerMatrixApp::set_pomodoro_ui_layout_mode_key(const QString &mode_key, bool persist)
{
    const QString normalized = normalized_pomodoro_ui_layout_key(mode_key);
    const QString previous = pomodoro_ui_layout_mode_key_;

    if (pomodoro_module_page != nullptr) {
        bool applied = false;
        const bool invoked = QMetaObject::invokeMethod(pomodoro_module_page,
                                                       "set_ui_layout_mode_key",
                                                       Qt::DirectConnection,
                                                       Q_RETURN_ARG(bool, applied),
                                                       Q_ARG(QString, normalized));
        if (!invoked || !applied) {
            pomodoro_ui_layout_mode_key_ = previous;
            return false;
        }

        pomodoro_module_page->updateGeometry();
        relax_module_page(pomodoro_module_page);
        set_pomodoro_in_progress_panel_visible(pomodoro_in_progress_panel_start_expanded, false);
        refresh_module_task_counter();
        _queue_window_geometry_stabilization();
    }

    pomodoro_ui_layout_mode_key_ = normalized;
    if (persist) {
        _save_task_layout_preference();
    }
    return true;
}

void EisenhowerMatrixApp::open_pomodoro()
{
}

bool EisenhowerMatrixApp::calendar_panel_visible() const
{
    return calendar_pane != nullptr ? calendar_pane->is_panel_expanded() : calendar_start_expanded;
}

void EisenhowerMatrixApp::set_calendar_panel_visible(bool visible, bool persist)
{
    if (poca_layout_active_) {
        return;
    }
    calendar_start_expanded = visible;
    if (calendar_pane != nullptr) {
        calendar_pane->set_panel_expanded(visible);
        _sync_splitter_for_calendar();
    }

    if (persist) {
        _save_task_layout_preference();
    }
}

bool EisenhowerMatrixApp::feynman_task_panel_visible() const
{
    bool visible = feynman_task_panel_start_expanded;
    if (feynman_module_page != nullptr) {
        QMetaObject::invokeMethod(
            feynman_module_page, "is_task_panel_expanded", Qt::DirectConnection, Q_RETURN_ARG(bool, visible));
    }
    return visible;
}

void EisenhowerMatrixApp::set_feynman_task_panel_visible(bool visible, bool persist)
{
    feynman_task_panel_start_expanded = visible;
    if (feynman_module_page != nullptr) {
        QMetaObject::invokeMethod(
            feynman_module_page, "set_task_panel_expanded", Qt::DirectConnection, Q_ARG(bool, visible));
    }

    if (persist) {
        _save_task_layout_preference();
    }
}

bool EisenhowerMatrixApp::pomodoro_in_progress_panel_visible() const
{
    bool visible = pomodoro_in_progress_panel_start_expanded;
    if (pomodoro_module_page != nullptr) {
        QMetaObject::invokeMethod(pomodoro_module_page,
                                  "is_in_progress_panel_expanded",
                                  Qt::DirectConnection,
                                  Q_RETURN_ARG(bool, visible));
    }
    return visible;
}

void EisenhowerMatrixApp::set_pomodoro_in_progress_panel_visible(bool visible, bool persist)
{
    pomodoro_in_progress_panel_start_expanded = visible;
    if (pomodoro_module_page != nullptr) {
        QMetaObject::invokeMethod(pomodoro_module_page,
                                  "set_in_progress_panel_expanded",
                                  Qt::DirectConnection,
                                  Q_ARG(bool, visible));
    }

    if (persist) {
        _save_task_layout_preference();
    }
}

bool EisenhowerMatrixApp::pomodoro_report_pomodoros_eisenhower_enabled() const
{
    return pomodoro_report_pomodoros_eisenhower_enabled_;
}

void EisenhowerMatrixApp::set_pomodoro_report_pomodoros_eisenhower_enabled(bool enabled, bool persist)
{
    pomodoro_report_pomodoros_eisenhower_enabled_ = enabled;
    pomodoro_report_pomodoros_enabled_ = enabled;
    if (persist) {
        _save_task_layout_preference();
    }
    source::InterfaceCore::incore_11_atualizar_itens_tarefas::atualizar_itens_tarefas(this);
}

bool EisenhowerMatrixApp::pomodoro_report_pomodoros_pomodoro_enabled() const
{
    return pomodoro_report_pomodoros_pomodoro_enabled_;
}

void EisenhowerMatrixApp::set_pomodoro_report_pomodoros_pomodoro_enabled(bool enabled, bool persist)
{
    pomodoro_report_pomodoros_pomodoro_enabled_ = enabled;
    if (persist) {
        _save_task_layout_preference();
    }
    atualizar_eisenhower_coluna_doing_timer();
    if (pomodoro_plugin_ != nullptr) {
        pomodoro_plugin_->refresh_task_views();
    }
}

bool EisenhowerMatrixApp::pomodoro_report_pomodoros_calendario_enabled() const
{
    return pomodoro_report_pomodoros_calendario_enabled_;
}

void EisenhowerMatrixApp::set_pomodoro_report_pomodoros_calendario_enabled(bool enabled, bool persist)
{
    pomodoro_report_pomodoros_calendario_enabled_ = enabled;
    if (persist) {
        _save_task_layout_preference();
    }
    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
        calendar_pane->calendar_panel->update_task_list();
    }
}

bool EisenhowerMatrixApp::pomodoro_report_pomodoros_enabled() const
{
    return pomodoro_report_pomodoros_eisenhower_enabled_;
}

void EisenhowerMatrixApp::set_pomodoro_report_pomodoros_enabled(bool enabled, bool persist)
{
    set_pomodoro_report_pomodoros_eisenhower_enabled(enabled, persist);
}

bool EisenhowerMatrixApp::open_pomodoro_alarm_settings(QWidget *parent)
{
    if (pomodoro_plugin_ == nullptr) {
        return false;
    }

    pomodoro_plugin_->open_alarm_settings(parent != nullptr ? parent : this);
    if (auto *mgr = source::plugin::Pomodoro::compat::SoundManager::instance()) {
        mgr->load_config();
    }
    return true;
}

bool EisenhowerMatrixApp::open_pomodoro_automatic_mode_settings(QWidget *parent)
{
    if (pomodoro_plugin_ == nullptr) {
        return false;
    }

    pomodoro_plugin_->open_automatic_mode_settings(parent != nullptr ? parent : this);
    if (auto *mgr = source::plugin::Pomodoro::compat::SoundManager::instance()) {
        mgr->load_config();
    }
    return true;
}

bool EisenhowerMatrixApp::has_mapamental_plugin() const
{
    return mapamental_plugin_ != nullptr && mapamental_module_page != nullptr;
}

bool EisenhowerMatrixApp::has_modelo_plugin() const
{
    return modelo_plugin_ != nullptr && modelo_plugin_->is_available();
}

QString EisenhowerMatrixApp::mapamental_display_name() const
{
    return mapamental_plugin_ != nullptr ? mapamental_plugin_->display_name() : get_text(QStringLiteral("Mapa Mental"));
}

QString EisenhowerMatrixApp::mapamental_open_menu_text() const
{
    return mapamental_plugin_ != nullptr ? mapamental_plugin_->open_menu_text()
                                         : get_text(QStringLiteral("Abrir Mapa Mental"));
}

QString EisenhowerMatrixApp::mapamental_layout_mode_key() const
{
    return mapamental_plugin_ != nullptr ? mapamental_plugin_->layout_mode_key() : QStringLiteral("vertical");
}

void EisenhowerMatrixApp::set_mapamental_layout_mode_key(const QString &mode_key)
{
    if (mapamental_plugin_ != nullptr) {
        mapamental_plugin_->set_layout_mode_key(mode_key);
    }
}

QString EisenhowerMatrixApp::mapamental_summary_method_key() const
{
    return mapamental_plugin_ != nullptr ? mapamental_plugin_->summary_method_key() : QStringLiteral("qwen");
}

void EisenhowerMatrixApp::set_mapamental_summary_method_key(const QString &method_key)
{
    if (mapamental_plugin_ != nullptr) {
        mapamental_plugin_->set_summary_method_key(method_key);
    }
}

void EisenhowerMatrixApp::open_mapamental()
{
}

void EisenhowerMatrixApp::reload_mapamental_from_backup()
{
    if (mapamental_plugin_ != nullptr) {
        mapamental_plugin_->reload_shared_state();
        mapamental_plugin_->refresh_task_views();
    }
}

bool EisenhowerMatrixApp::has_audiolivro_plugin() const
{
    return audiolivro_plugin_ != nullptr && audiolivro_module_page != nullptr;
}

QString EisenhowerMatrixApp::audiolivro_display_name() const
{
    return audiolivro_plugin_ != nullptr ? audiolivro_plugin_->display_name() : get_text(QStringLiteral("Audiolivro"));
}

QString EisenhowerMatrixApp::audiolivro_open_menu_text() const
{
    return audiolivro_plugin_ != nullptr ? audiolivro_plugin_->open_menu_text()
                                         : get_text(QStringLiteral("Abrir Audiolivro"));
}

QVariantList EisenhowerMatrixApp::plugin_status_report() const
{
    const auto manifest_version = [](const QString &relative_path) {
        QFile file(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(relative_path));
        if (!file.exists()) {
            file.setFileName(QDir(QDir::currentPath()).absoluteFilePath(relative_path));
        }
        if (!file.open(QIODevice::ReadOnly)) {
            return QStringLiteral("2026.11.22.0");
        }
        const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        return document.isObject() ? document.object().value(QStringLiteral("version")).toString(QStringLiteral("2026.11.22.0"))
                                   : QStringLiteral("2026.11.22.0");
    };

    const auto entry = [this, manifest_version](const QString &key,
                                                const QString &name,
                                                bool loaded,
                                                const QString &path,
                                                const QString &manifest) {
        QVariantMap item;
        item.insert(QStringLiteral("key"), key);
        item.insert(QStringLiteral("name"), name);
        item.insert(QStringLiteral("loaded"), loaded);
        item.insert(QStringLiteral("status"),
                    loaded ? get_text(QStringLiteral("Carregado")) : get_text(QStringLiteral("Não carregado")));
        item.insert(QStringLiteral("version"), manifest_version(manifest));
        item.insert(QStringLiteral("path"), path);
        item.insert(QStringLiteral("error"), plugin_load_errors_.value(key));
        return item;
    };

    QVariantList report = {
        entry(QStringLiteral("feynman"),
              feynman_display_name(),
              has_feynman_plugin(),
              feynman_plugin_path_,
              QStringLiteral("plugin/feynman/feynman_plugin.json")),
        entry(QStringLiteral("pomodoro"),
              pomodoro_display_name(),
              has_pomodoro_plugin(),
              pomodoro_plugin_path_,
              QStringLiteral("plugin/pomodoro/pomodoro_plugin.json")),
    };
    if (has_modelo_plugin()) {
        report.append(entry(QStringLiteral("modelo"),
                            get_text(QStringLiteral("modelo qwen2.5-3b")),
                            true,
                            modelo_plugin_path_,
                            QStringLiteral("plugin/modelo/modelo_plugin.json")));
    }
    report.append(entry(QStringLiteral("agenda"),
              agenda_display_name(),
              has_agenda_plugin(),
              agenda_plugin_path_,
              QStringLiteral("plugin/agenda/agenda_plugin.json")));
    report.append(entry(QStringLiteral("gantt"),
              gantt_display_name(),
              has_gantt_plugin(),
              gantt_plugin_path_,
              QStringLiteral("plugin/gantt/gantt_plugin.json")));
    return report;
}

QString EisenhowerMatrixApp::audiolivro_font_settings_menu_text() const
{
    return audiolivro_plugin_ != nullptr ? audiolivro_plugin_->font_settings_menu_text()
                                         : get_text(QStringLiteral("Configuracao de Fontes"));
}

QVariantList EisenhowerMatrixApp::audiolivro_voice_options() const
{
    return audiolivro_plugin_ != nullptr ? audiolivro_plugin_->voice_options() : QVariantList{};
}

QString EisenhowerMatrixApp::audiolivro_current_voice_id() const
{
    return audiolivro_plugin_ != nullptr ? audiolivro_plugin_->current_voice_id() : QString();
}

void EisenhowerMatrixApp::set_audiolivro_voice_id(const QString &voice_id)
{
    if (audiolivro_plugin_ != nullptr) {
        audiolivro_plugin_->set_voice_id(voice_id);
    }
}

bool EisenhowerMatrixApp::open_audiolivro_font_settings(QWidget *parent)
{
    if (audiolivro_plugin_ == nullptr) {
        return false;
    }

    audiolivro_plugin_->open_font_settings(parent != nullptr ? parent : this);
    return true;
}

void EisenhowerMatrixApp::open_audiolivro()
{
}

bool EisenhowerMatrixApp::has_agenda_plugin() const
{
    return agenda_plugin_ != nullptr;
}

QString EisenhowerMatrixApp::agenda_display_name() const
{
    return agenda_plugin_ != nullptr ? agenda_plugin_->display_name() : get_text(QStringLiteral("Registro de Atividades"));
}

QString EisenhowerMatrixApp::agenda_open_menu_text() const
{
    return agenda_plugin_ != nullptr ? agenda_plugin_->open_menu_text()
                                     : get_text(QStringLiteral("Registro de Atividades"));
}

void EisenhowerMatrixApp::open_agenda()
{
    if (agenda_plugin_ != nullptr) {
        agenda_plugin_->open_window(this);
    }
}

void EisenhowerMatrixApp::populate_agenda_delete_item_menu(QMenu *menu)
{
    if (agenda_plugin_ != nullptr) {
        agenda_plugin_->populate_delete_item_menu(menu);
    }
}

void EisenhowerMatrixApp::populate_agenda_color_menu(QMenu *menu)
{
    if (agenda_plugin_ != nullptr) {
        agenda_plugin_->populate_color_menu(menu);
    }
}

bool EisenhowerMatrixApp::has_gantt_plugin() const
{
    return gantt_plugin_ != nullptr;
}

QString EisenhowerMatrixApp::gantt_display_name() const
{
    return gantt_plugin_ != nullptr ? gantt_plugin_->display_name() : get_text(QStringLiteral("Cronograma de Gantt & BIM 4D"));
}

QString EisenhowerMatrixApp::gantt_open_menu_text() const
{
    return gantt_plugin_ != nullptr ? gantt_plugin_->open_menu_text()
                                    : get_text(QStringLiteral("Cronograma de Gantt BIM 4D"));
}

void EisenhowerMatrixApp::open_gantt()
{
    if (gantt_plugin_ != nullptr) {
        gantt_plugin_->open_window(this);
    }
}

void EisenhowerMatrixApp::open_gantt_and_highlight_activity(const QString &schedule_item_id)
{
    if (gantt_plugin_ != nullptr) {
        gantt_plugin_->open_gantt_and_highlight_activity(schedule_item_id, this);
    }
}

}  // namespace source
