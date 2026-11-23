#include "source/app/app_01_eisenhower_matrix_app.hpp"
#include "source/app/app_02_get_text.hpp"
#include "source/app/EisenhowerGanttHostAdapter.hpp"
#include "source/ui/ui_14_CalendarPanel.hpp"
#include "source/ui/ui_10_BadgeCalendarWidget.hpp"
#include "source/modules/pomodoro/Pomodoro/tmp_02_PomodoroTimer.hpp"
#include <QScrollArea>

namespace source {
using namespace app;

EisenhowerMatrixApp::EisenhowerMatrixApp(source::language::GerenciadorTraducao *translation_manager, QWidget *parent)
    : QMainWindow(parent), gerenciador_traducao(translation_manager)
{
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: iniciando construtor."));

    connect(this->gerenciador_traducao,
            &source::language::GerenciadorTraducao::idioma_alterado,
            this,
            [this](const QString &) { atualizar_textos(); });

    pomodoro_host_bridge_ = new source::plugin::PomodoroHostBridge();
    pomodoro_host_bridge_->set_language_code(gerenciador_traducao->obter_idioma_atual());
    pomodoro_host_bridge_->set_standard_buttons_handler([this](QObject *dialog) {
        if (gerenciador_traducao != nullptr) {
            gerenciador_traducao->traduzir_botoes_padrao(dialog);
        }
    });
    pomodoro_host_bridge_->set_task_to_eisenhower_handler(
        [this](const QVariantMap &task) { _upsert_task_from_external(task); });
    pomodoro_host_bridge_->set_task_to_feynman_handler(
        [this](const QVariantMap &task) { return _send_task_payload_to_feynman(task); });
    pomodoro_host_bridge_->set_feynman_available_checker([this]() { return has_feynman_plugin(); });
    pomodoro_host_bridge_->set_shared_state_changed_handler(
        [this](const QString &origin) { _queue_runtime_sync(origin); });
    pomodoro_host_bridge_->set_sync_operation_info_handler([this](const QString &operation, const QString &origin) {
        if (operation.compare(QStringLiteral("delete"), Qt::CaseInsensitive) == 0) {
            show_task_sync_delete_info(origin);
        } else {
            show_task_sync_add_info(origin);
        }
    });
    pomodoro_host_bridge_->set_task_deleted_handler([this](const QVariantMap &task, const QString &origin) {
        remove_task_from_synced_modules(task, origin);
    });
    pomodoro_host_bridge_->set_task_sort_state_checker([this]() { return has_active_task_sort(); });
    pomodoro_host_bridge_->set_task_sort_less_handler([this](const QVariantMap &left, const QVariantMap &right) {
        return task_sort_less_for_payloads(left, right);
    });
    pomodoro_host_bridge_->set_reserved_space_checker([this]() { return reserved_space_pomodoro_enabled(); });
    pomodoro_host_bridge_->set_watermark_checker([this]() { return reserved_watermark_pomodoro_enabled(); });
    pomodoro_host_bridge_->set_task_sync_enabled_checker([this]() { return sync_pomodoro_enabled(); });
    pomodoro_host_bridge_->set_report_pomodoros_checker([this]() { return pomodoro_report_pomodoros_pomodoro_enabled(); });
    pomodoro_host_bridge_->set_configured_cycles_changed_handler([this](int cycles) {
        if (eisenhower_pomodoro_timer != nullptr) {
            eisenhower_pomodoro_timer->set_ciclos_permitidos(cycles);
        }
    });
    pomodoro_host_bridge_->set_calendar_widget_factory([this](QWidget *parent) -> QWidget * {
        auto *cal = new source::GerenciamentoUI::CalendarPanel(this, parent);
        cal->set_calendar_only_mode(true);
        return cal;
    });
    pomodoro_host_bridge_->set_redefinir_ciclos_em_progresso_handler([this](int cycles) {
        redefinir_ciclos_em_progresso(cycles);
    });

    feynman_host_bridge_ = new source::plugin::FeynmanHostBridge();
    feynman_host_bridge_->set_language_code(gerenciador_traducao->obter_idioma_atual());
    feynman_host_bridge_->set_standard_buttons_handler([this](QObject *dialog) {
        if (gerenciador_traducao != nullptr) {
            gerenciador_traducao->traduzir_botoes_padrao(dialog);
        }
    });
    feynman_host_bridge_->set_task_to_eisenhower_handler(
        [this](const QVariantMap &task) { return _upsert_task_from_external(task); });
    feynman_host_bridge_->set_eisenhower_available_checker([]() { return true; });
    feynman_host_bridge_->set_task_to_pomodoro_handler([this](const QVariantMap &task) {
        if (!has_pomodoro_plugin() || pomodoro_plugin_ == nullptr) {
            return false;
        }

        QVariantMap payload = task;
        if (payload.value(QStringLiteral("origin")).toString().trimmed().isEmpty()) {
            payload.insert(QStringLiteral("origin"), QStringLiteral("feynman"));
        }

        pomodoro_plugin_->receive_eisenhower_task(payload);
        return true;
    });
    feynman_host_bridge_->set_pomodoro_available_checker([this]() { return has_pomodoro_plugin(); });
    feynman_host_bridge_->set_shared_state_changed_handler(
        [this](const QString &origin) { _queue_runtime_sync(origin); });
    feynman_host_bridge_->set_sync_operation_info_handler([this](const QString &operation, const QString &origin) {
        if (operation.compare(QStringLiteral("delete"), Qt::CaseInsensitive) == 0) {
            show_task_sync_delete_info(origin);
        } else {
            show_task_sync_add_info(origin);
        }
    });
    feynman_host_bridge_->set_task_deleted_handler([this](const QVariantMap &task, const QString &origin) {
        remove_task_from_synced_modules(task, origin);
    });
    feynman_host_bridge_->set_task_sort_state_checker([this]() { return has_active_task_sort(); });
    feynman_host_bridge_->set_task_sort_less_handler([this](const QVariantMap &left, const QVariantMap &right) {
        return task_sort_less_for_payloads(left, right);
    });
    feynman_host_bridge_->set_reserved_space_checker([this]() { return reserved_space_feynman_enabled(); });
    feynman_host_bridge_->set_watermark_checker([this]() { return reserved_watermark_feynman_enabled(); });
    feynman_host_bridge_->set_ui_layout_mode_key_checker([this]() { return feynman_ui_layout_mode_key(); });
    feynman_host_bridge_->set_task_sync_enabled_checker([this]() { return sync_feynman_enabled(); });

    agenda_host_bridge_ = new source::plugin::AgendaHostBridge();
    agenda_host_bridge_->set_language_code(gerenciador_traducao->obter_idioma_atual());
    agenda_host_bridge_->set_persistent_dir(source::utils::obter_caminho_persistente());
    agenda_host_bridge_->set_standard_buttons_handler([this](QObject *dialog) {
        if (gerenciador_traducao != nullptr) {
            gerenciador_traducao->traduzir_botoes_padrao(dialog);
        }
    });
    agenda_host_bridge_->set_task_to_eisenhower_handler([this](const QVariantMap &task) {
        const bool inserted = _upsert_task_from_external(task);
        if (inserted) {
            show_task_sync_add_info(QStringLiteral("agenda"));
        }
        return inserted;
    });
    agenda_host_bridge_->set_color_mode_changed_handler(
        [this](const QString &mode) { _apply_agenda_color_mode(mode); });

    gantt_host_adapter_ = std::make_unique<EisenhowerGanttHostAdapter>(this);
    gantt_host_bridge_ = new source::plugin::GanttHostBridge();
    gantt_host_bridge_->set_eisenhower_host(gantt_host_adapter_.get());
    gantt_host_bridge_->set_language_code(gerenciador_traducao->obter_idioma_atual());
    gantt_host_bridge_->set_persistent_dir(source::utils::obter_caminho_persistente());

    setWindowTitle(source::utils::titulo_janela_principal());
    setMinimumSize(kInitialWindowMinWidth, kInitialWindowMinHeight);
    resize(1380, 840);
    move(50, 50);

    const QString icon_path = source::utils::get_default_window_icon_path();
    if (!icon_path.isEmpty()) {
        setWindowIcon(QIcon(icon_path));
    }

    const QString persistent_dir = source::utils::obter_caminho_persistente();
    tasks_path = source::utils::UnifiedTaskStore::shared_tasks_path();
    layout_path = QDir(persistent_dir).filePath(QStringLiteral("layout.json"));
    const QString documents_dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    export_path = QDir(documents_dir.isEmpty() ? QDir::homePath() : documents_dir)
                      .filePath(QStringLiteral("tarefas.json"));
    const QString legacy_unified_tasks_path =
        QDir(persistent_dir).filePath(QStringLiteral("eisenhower_tarefas.json"));
    const QString legacy_tasks_path = QDir(persistent_dir).filePath(QStringLiteral("tasks.json"));

    if (!QFileInfo::exists(tasks_path)) {
        if (QFileInfo::exists(legacy_unified_tasks_path)) {
            QFile::copy(legacy_unified_tasks_path, tasks_path);
        } else if (QFileInfo::exists(legacy_tasks_path)) {
            QFile::copy(legacy_tasks_path, tasks_path);
        }
    }
    if (QFileInfo::exists(tasks_path)) {
        remove_legacy_task_datastores();
    }

    logger()->debug(QStringLiteral("EisenhowerMatrixApp: carregando preferências e UI principal."));
    _load_task_layout_preference();
    arquivo = new source::InterfaceCore::Arquivo(this);
    initUI();
    _apply_startup_geometry_constraints();
    _apply_theme_palette();
    load_tasks();
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: inicializando módulo Feynman."));
    _load_feynman_plugin();
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: módulo Feynman inicializado."));
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: inicializando módulo Agenda."));
    _load_agenda_plugin();
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: módulo Agenda inicializado."));
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: inicializando módulo Gantt."));
    _load_gantt_plugin();
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: módulo Gantt inicializado."));
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: inicializando módulo Pomodoro."));
    _load_pomodoro_plugin();
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: módulo Pomodoro inicializado."));
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: aplicando restrições finais de geometria."));
    _apply_startup_geometry_constraints();
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: restrições finais de geometria aplicadas."));
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: criando menu de configurações."));
    criar_menu_configuracoes();
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: menu de configurações criado."));
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: agendando estabilização de geometria."));
    _queue_window_geometry_stabilization();
    logger()->debug(QStringLiteral("EisenhowerMatrixApp: construtor finalizado."));
}

EisenhowerMatrixApp::~EisenhowerMatrixApp()
{
    // 1. Desinicializa os módulos para liberar pontes e recursos internos.
    if (gantt_plugin_) gantt_plugin_->initialize(nullptr);
    if (feynman_plugin_) feynman_plugin_->initialize(nullptr);
    if (pomodoro_plugin_) pomodoro_plugin_->initialize(nullptr);
    if (agenda_plugin_) agenda_plugin_->initialize(nullptr);

    // 2. Deleta explicitamente os widgets de UI criados pelos módulos se ainda ativos.
    if (feynman_module_page) delete feynman_module_page;
    if (pomodoro_module_page) delete pomodoro_module_page;

    QCoreApplication::removePostedEvents(this);
    blockSignals(true);

    const QList<QObject *> child_objects = findChildren<QObject *>();
    for (QObject *child : child_objects) {
        if (child == nullptr) {
            continue;
        }

        child->blockSignals(true);
        QObject::disconnect(child, nullptr, this, nullptr);
        QObject::disconnect(this, nullptr, child, nullptr);
    }

    feynman_plugin_ = nullptr;
    pomodoro_plugin_ = nullptr;
    agenda_plugin_ = nullptr;
    gantt_plugin_ = nullptr;
    feynman_module_page = nullptr;
    pomodoro_module_page = nullptr;

    delete feynman_host_bridge_;
    feynman_host_bridge_ = nullptr;

    delete pomodoro_host_bridge_;
    pomodoro_host_bridge_ = nullptr;

    delete agenda_host_bridge_;
    agenda_host_bridge_ = nullptr;

    delete gantt_host_bridge_;
    gantt_host_bridge_ = nullptr;
}

void EisenhowerMatrixApp::closeEvent(QCloseEvent *event)
{
    save_tasks_now();
    _flush_runtime_sync();

    if (gantt_plugin_ != nullptr) {
        gantt_plugin_->close_window();
    }

    for (QPointer<QDialog> dialog : {_sobre_dialog, _manual_dialog, _plugin_manager_dialog, _log_manager_dialog}) {
        if (dialog != nullptr) {
            dialog->close();
        }
    }

    QMainWindow::closeEvent(event);
}

void EisenhowerMatrixApp::changeEvent(QEvent *event)
{
    if (event != nullptr && (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange
                             || event->type() == QEvent::StyleChange)) {
        QTimer::singleShot(0, this, [this]() { _apply_theme_palette(); });
    } else if (event != nullptr
               && (event->type() == QEvent::WindowStateChange || event->type() == QEvent::ActivationChange
                   || event->type() == QEvent::ScreenChangeInternal)) {
        _queue_window_geometry_stabilization();
    }

    QMainWindow::changeEvent(event);
}

void EisenhowerMatrixApp::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    _queue_window_geometry_stabilization(80);
}

bool EisenhowerMatrixApp::poca_layout_active() const
{
    return poca_layout_active_;
}

void EisenhowerMatrixApp::set_poca_layout_active(bool active, bool persist)
{
    poca_layout_active_ = active;

    if (active) {
        if (!eisenhower_pomodoro_expanded) {
            set_pomodoro_panel_visible(true, false);
        }
        if (eisenhower_doing_group != nullptr) {
            eisenhower_doing_group->hide();
        }
        if (eisenhower_poca_calendar_panel != nullptr) {
            eisenhower_poca_calendar_panel->set_month_step(1);
            eisenhower_poca_calendar_panel->show();
            eisenhower_poca_calendar_panel->update_task_list();
            eisenhower_poca_calendar_panel->update_calendars_for_width();
        }
        if (eisenhower_poca_calendar_panel2 != nullptr) {
            eisenhower_poca_calendar_panel2->set_month_step(1);
            if (eisenhower_poca_calendar_panel != nullptr &&
                eisenhower_poca_calendar_panel->calendar != nullptr &&
                eisenhower_poca_calendar_panel2->calendar != nullptr) {
                const int yr = eisenhower_poca_calendar_panel->calendar->yearShown();
                const int mo = eisenhower_poca_calendar_panel->calendar->monthShown();
                const int k = eisenhower_poca_calendar_panel->calendar_count();
                const QDate target = QDate(yr, mo, 1).addMonths(k);
                eisenhower_poca_calendar_panel2->calendar->setCurrentPage(target.year(), target.month());
            }
            eisenhower_poca_calendar_panel2->show();
            eisenhower_poca_calendar_panel2->update_task_list();
            eisenhower_poca_calendar_panel2->update_calendars_for_width();
        }
        if (eisenhower_poca_bottom_spacer != nullptr) {
            eisenhower_poca_bottom_spacer->show();
        }
        if (calendar_pane != nullptr) {
            calendar_pane->setMinimumWidth(0);
            calendar_pane->hide();
        }
        if (main_splitter != nullptr) {
            main_splitter->setCollapsible(0, true);
            main_splitter->setHandleWidth(0);
            if (QSplitterHandle *handle = main_splitter->handle(1)) {
                handle->setEnabled(false);
            }
            const QList<int> current_sizes = main_splitter->sizes();
            const int total_w = current_sizes.size() >= 2 ? (current_sizes[0] + current_sizes[1]) : width();
            main_splitter->setSizes({0, total_w});
        }
        if (eisenhower_content_splitter != nullptr) {
            const int left_req = (eisenhower_left_scroll != nullptr)
                                     ? eisenhower_left_scroll->minimumWidth()
                                     : 396;
            const QList<int> c_sizes = eisenhower_content_splitter->sizes();
            if (c_sizes.size() >= 2 && c_sizes[0] < left_req) {
                const int diff = left_req - c_sizes[0];
                const int min_quad_w = (quadrants_scroll_area != nullptr) ? quadrants_scroll_area->minimumWidth() : 120;
                const int right_w = qMax(min_quad_w, c_sizes[1] - diff);
                eisenhower_content_splitter->setSizes({left_req, right_w});
            }
        }
    } else {
        if (eisenhower_poca_calendar_panel != nullptr) {
            eisenhower_poca_calendar_panel->hide();
        }
        if (eisenhower_poca_calendar_panel2 != nullptr) {
            eisenhower_poca_calendar_panel2->hide();
        }
        if (eisenhower_poca_bottom_spacer != nullptr) {
            eisenhower_poca_bottom_spacer->hide();
        }
        if (eisenhower_doing_group != nullptr) {
            eisenhower_doing_group->show();
        }
        if (main_splitter != nullptr) {
            main_splitter->setCollapsible(0, false);
        }
        if (calendar_pane != nullptr) {
            calendar_pane->setMinimumWidth(calendar_pane->effective_min_width());
            calendar_pane->show();
            calendar_pane->set_panel_expanded(calendar_panel_visible());
        }
        if (main_splitter != nullptr) {
            const int handle_w = eisenhower_pomodoro_expanded ? 6 : 10;
            main_splitter->setHandleWidth(calendar_panel_visible() ? handle_w : 0);
            if (QSplitterHandle *handle = main_splitter->handle(1)) {
                handle->setEnabled(calendar_panel_visible());
            }
        }
        _sync_splitter_for_calendar();
    }

    reorganizar_painel_nova_tarefa();
    _queue_window_geometry_stabilization();

    if (persist) {
        _save_task_layout_preference();
    }
}

}  // namespace source
