#pragma once

#include <memory>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QPair>
#include <QPointer>
#include <QPoint>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantList>
#include <Qt>
#include <QtGlobal>

class QCheckBox;
class QComboBox;
class QDateEdit;
class QDialog;
class QDoubleSpinBox;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QMenu;
class QMenuBar;
class QPluginLoader;
class QPushButton;
class QShortcut;
class QSplitter;
class QScrollArea;
class QGridLayout;
class QStackedLayout;
class QHBoxLayout;
class QVBoxLayout;
class QTabWidget;
class QTimeEdit;
class QWidget;
class QEvent;
class QCloseEvent;
class QGroupBox;
class QResizeEvent;

namespace source::GerenciamentoUI {
class Calendar;
class CalendarPanel;
class RotatedTabButton;
}

namespace source::plugin::Pomodoro {
class PomodoroTimer;
}

namespace source::InterfaceCore {
class Arquivo;
}

namespace source::language {
class GerenciadorTraducao;
}

namespace source::plugin {
class IFeynmanPlugin;
class FeynmanHostBridge;
class IPomodoroPlugin;
class PomodoroHostBridge;
class IMapaMentalPlugin;
class MapaMentalHostBridge;
class IModeloPlugin;
class IAudiolivroPlugin;
class AudiolivroHostBridge;
class IAgendaPlugin;
class AgendaHostBridge;
class IGanttPlugin;
class GanttHostBridge;
}

namespace source {

class EisenhowerGanttHostAdapter;

class EisenhowerMatrixApp : public QMainWindow {
    Q_OBJECT

public:
    enum class TaskLayoutMode {
        Tabs,
        TabsHorizontal,
        Quadrants,
        Matrix2x2,
    };

    enum class TaskSortMode {
        None,
        DateAscending,
        DateDescending,
        TimeAscending,
        TimeDescending,
        AlphabeticalAscending,
        AlphabeticalDescending,
    };

    enum class TaskSortCriterion {
        Date,
        Alphabetical,
        Time,
    };

    explicit EisenhowerMatrixApp(source::language::GerenciadorTraducao *translation_manager,
                                 QWidget *parent = nullptr);
    ~EisenhowerMatrixApp() override;

    bool calendar_start_expanded = false;
    bool feynman_task_panel_start_expanded = false;
    bool pomodoro_in_progress_panel_start_expanded = false;
    int calendar_min_width = 360;
    int quadrants_min_width = 300;

    source::language::GerenciadorTraducao *gerenciador_traducao = nullptr;
    QString tasks_path;
    QString export_path;
    source::InterfaceCore::Arquivo *arquivo = nullptr;

    QSplitter *main_splitter = nullptr;
    source::GerenciamentoUI::Calendar *calendar_pane = nullptr;
    QWidget *menu_bar_host = nullptr;
    QWidget *module_task_counter_container = nullptr;
    QLabel *module_task_counter_label = nullptr;
    QWidget *eisenhower_module_page = nullptr;
    QWidget *feynman_module_page = nullptr;
    QWidget *pomodoro_module_page = nullptr;
    QWidget *mapamental_module_page = nullptr;
    QWidget *audiolivro_module_page = nullptr;
    QWidget *_main_content = nullptr;
    QWidget *_root_container = nullptr;
    QWidget *task_layout_host = nullptr;
    QWidget *quadrant_tabs_page = nullptr;
    QWidget *quadrants_page = nullptr;
    QWidget *matrix2x2_page = nullptr;
    QHBoxLayout *quadrants_layout = nullptr;
    QGridLayout *matrix2x2_layout = nullptr;
    QStackedLayout *task_layout_stack = nullptr;
    QPointer<QDialog> _sobre_dialog;
    QPointer<QDialog> _manual_dialog;
    QPointer<QDialog> _plugin_manager_dialog;
    QPointer<QDialog> _log_manager_dialog;

    QTabWidget *quadrant_tabs = nullptr;
    QTabWidget *quadrant1_status_tabs = nullptr;
    QTabWidget *quadrant2_status_tabs = nullptr;
    QTabWidget *quadrant3_status_tabs = nullptr;
    QTabWidget *quadrant4_status_tabs = nullptr;
    QWidget *quadrant1_tab = nullptr;
    QWidget *quadrant2_tab = nullptr;
    QWidget *quadrant3_tab = nullptr;
    QWidget *quadrant4_tab = nullptr;

    QLabel *quadrant1_label = nullptr;
    QLabel *quadrant2_label = nullptr;
    QLabel *quadrant3_label = nullptr;
    QLabel *quadrant4_label = nullptr;
    QLabel *quadrant1_completed_label = nullptr;
    QLabel *quadrant2_completed_label = nullptr;
    QLabel *quadrant3_completed_label = nullptr;
    QLabel *quadrant4_completed_label = nullptr;

    QLabel *quadrant1_in_progress_label = nullptr;
    QLabel *quadrant2_in_progress_label = nullptr;
    QLabel *quadrant3_in_progress_label = nullptr;
    QLabel *quadrant4_in_progress_label = nullptr;

    QGroupBox *quadrant1_group = nullptr;
    QGroupBox *quadrant2_group = nullptr;
    QGroupBox *quadrant3_group = nullptr;
    QGroupBox *quadrant4_group = nullptr;
    QGroupBox *quadrant1_completed_group = nullptr;
    QGroupBox *quadrant2_completed_group = nullptr;
    QGroupBox *quadrant3_completed_group = nullptr;
    QGroupBox *quadrant4_completed_group = nullptr;

    QGroupBox *quadrant1_in_progress_group = nullptr;
    QGroupBox *quadrant2_in_progress_group = nullptr;
    QGroupBox *quadrant3_in_progress_group = nullptr;
    QGroupBox *quadrant4_in_progress_group = nullptr;

    QListWidget *quadrant1_list = nullptr;
    QListWidget *quadrant2_list = nullptr;
    QListWidget *quadrant3_list = nullptr;
    QListWidget *quadrant4_list = nullptr;
    QListWidget *quadrant1_completed_list = nullptr;
    QListWidget *quadrant2_completed_list = nullptr;
    QListWidget *quadrant3_completed_list = nullptr;
    QListWidget *quadrant4_completed_list = nullptr;

    QListWidget *quadrant1_in_progress_list = nullptr;
    QListWidget *quadrant2_in_progress_list = nullptr;
    QListWidget *quadrant3_in_progress_list = nullptr;
    QListWidget *quadrant4_in_progress_list = nullptr;

    QLineEdit *task_input = nullptr;
    QPushButton *add_button = nullptr;
    QPushButton *calendar_button = nullptr;
    QCheckBox *date_checkbox = nullptr;
    QDateEdit *date_input = nullptr;
    QCheckBox *time_checkbox = nullptr;
    QTimeEdit *time_input = nullptr;
    QLabel *duration_label = nullptr;
    QDoubleSpinBox *duration_spin = nullptr;
    QLabel *priority_label = nullptr;
    QComboBox *quadrant_selector = nullptr;
    QLabel *domain_label = nullptr;
    QComboBox *domain_selector = nullptr;
    QPushButton *add_task_button = nullptr;
    QMenuBar *menu_bar = nullptr;
    QMenu *idioma_menu = nullptr;
    QShortcut *atalho_arquivo = nullptr;
    QShortcut *atalho_idioma = nullptr;
    QShortcut *atalho_opcoes = nullptr;
    QMap<QString, QVariant> widget_heights;
    int widget_height_override = 0;

    source::plugin::Pomodoro::PomodoroTimer *eisenhower_pomodoro_timer = nullptr;
    QGroupBox *eisenhower_doing_group = nullptr;
    QListWidget *eisenhower_doing_list = nullptr;
    source::GerenciamentoUI::CalendarPanel *eisenhower_poca_calendar_panel = nullptr;
    source::GerenciamentoUI::CalendarPanel *eisenhower_poca_calendar_panel2 = nullptr;
    QWidget *eisenhower_poca_bottom_spacer = nullptr;
    QWidget *eisenhower_left_widget = nullptr;
    QScrollArea *eisenhower_left_scroll = nullptr;
    QSplitter *eisenhower_content_splitter = nullptr;
    QScrollArea *quadrants_scroll_area = nullptr;
    QGroupBox *eisenhower_input_group = nullptr;
    QVBoxLayout *eisenhower_left_layout = nullptr;
    QWidget *quadrants_top_container = nullptr;
    QVBoxLayout *quadrants_top_layout = nullptr;
    QHBoxLayout *priority_row = nullptr;
    QHBoxLayout *domain_row = nullptr;
    QVBoxLayout *input_vlayout = nullptr;

    bool eisenhower_pomodoro_expanded = true;
    int current_input_layout_mode_ = -1;

    void alternar_eisenhower_pomodoro();
    bool poca_layout_active() const;
    void set_poca_layout_active(bool active, bool persist = true);
    void reorganizar_painel_nova_tarefa();
    void ajustar_largura_combobox(QComboBox *combo);
    void ajustar_largura_seletores_nova_tarefa();
    void _aplicar_layout_modo_input_group(bool vertical);
    void _update_input_group_layout_for_width(int width);
    void atualizar_eisenhower_coluna_doing_timer();
    void redefinir_ciclos_em_progresso(int cycles);
    source::plugin::PomodoroHostBridge *pomodoro_host_bridge() const { return pomodoro_host_bridge_; }
    void abrir_arquivo_vinculado_eisenhower_doing(QListWidgetItem *item);
    void mostrar_menu_contexto_eisenhower_doing(const QPoint &pos);
    void _on_eisenhower_pomodoro_ciclo_completado(const QString &tipo);

    void criar_menu_configuracoes();
    void definir_idioma(const QString &codigo_idioma);
    void initUI();
    void _attach_module_task_counter_to_menu_bar();
    void _sync_module_task_counter_geometry();
    bool set_task_layout(TaskLayoutMode mode, bool persist = true);
    TaskLayoutMode current_task_layout() const;
    int _calcular_largura_minima_layout_tarefas() const;
    void _atualizar_largura_minima_eisenhower();
    bool set_task_sort_mode(TaskSortMode mode, bool persist = true);
    bool clear_task_sort_criterion(TaskSortCriterion criterion, bool persist = true);
    bool has_task_sort_criterion(TaskSortCriterion criterion) const;
    Qt::SortOrder task_sort_order(TaskSortCriterion criterion) const;
    int task_sort_priority(TaskSortCriterion criterion) const;
    bool has_active_task_sort() const;
    bool task_sort_less_for_payloads(const QVariantMap &left, const QVariantMap &right) const;
    void add_placeholder(QListWidget *list_widget, const QString &text);
    void insert_task_into_quadrant_list(QListWidget *list, QListWidgetItem *item);
    void cleanup_time_groups(QListWidget *list);
    void add_task();
    void handle_item_checked(QListWidgetItem *item, QListWidget *source_list, QListWidget *target_list);
    void move_item_between_lists(QListWidgetItem *item,
                                 QListWidget *source,
                                 QListWidget *target,
                                 Qt::CheckState new_check_state);
    bool remove_task(QListWidgetItem *item, QListWidget *list_widget, bool confirm = true);
    void save_tasks();
    void save_tasks_now();
    void load_tasks();
    void atualizar_textos();
    void atualizar_placeholders();
    void exibir_sobre();
    void show_context_menu(const QPoint &point, QListWidget *list_widget);
    void open_calendar();
    void open_eisenhower();
    void nova_sessao();
    void abrir_arquivo();
    void salvar();
    void salvar_como();
    void limpar_tudo();
    void sair_app();
    void edit_task_datetime(QListWidgetItem *item, QListWidget *list_widget);
    void move_task_to_quadrant(QListWidgetItem *item, QListWidget *source_list, QListWidget *target_list);
    bool handle_linked_file_open_request(QListWidgetItem *item, QWidget *parent = nullptr);
    bool has_feynman_plugin() const;
    QString feynman_display_name() const;
    QString feynman_open_menu_text() const;
    QString feynman_ui_layout_mode_key() const;
    bool set_feynman_ui_layout_mode_key(const QString &mode_key, bool persist = true);
    void open_feynman_dialog_for_task(QListWidgetItem *item);
    void open_feynman_edit_labels_dialog(QWidget *parent = nullptr);
    bool has_pomodoro_plugin() const;
    QString pomodoro_display_name() const;
    QString pomodoro_open_menu_text() const;
    QString pomodoro_alarm_menu_text() const;
    bool pomodoro_automatic_mode_enabled_ = false;
    bool pomodoro_report_pomodoros_enabled_ = false;
    bool pomodoro_report_pomodoros_eisenhower_enabled_ = false;
    bool pomodoro_report_pomodoros_pomodoro_enabled_ = true;
    bool pomodoro_report_pomodoros_calendario_enabled_ = false;
    QString pomodoro_automatic_mode_menu_text() const;
    QString pomodoro_ui_layout_mode_key() const;
    bool set_pomodoro_ui_layout_mode_key(const QString &mode_key, bool persist = true);
    void open_pomodoro();
    bool open_pomodoro_alarm_settings(QWidget *parent = nullptr);
    bool open_pomodoro_automatic_mode_settings(QWidget *parent = nullptr);

    bool pomodoro_report_pomodoros_eisenhower_enabled() const;
    void set_pomodoro_report_pomodoros_eisenhower_enabled(bool enabled, bool persist = true);
    bool pomodoro_report_pomodoros_pomodoro_enabled() const;
    void set_pomodoro_report_pomodoros_pomodoro_enabled(bool enabled, bool persist = true);
    bool pomodoro_report_pomodoros_calendario_enabled() const;
    void set_pomodoro_report_pomodoros_calendario_enabled(bool enabled, bool persist = true);

    bool pomodoro_report_pomodoros_enabled() const;
    void set_pomodoro_report_pomodoros_enabled(bool enabled, bool persist = true);
    QString pomodoro_report_pomodoros_menu_text() const;
    bool has_mapamental_plugin() const;
    bool has_modelo_plugin() const;
    QString mapamental_display_name() const;
    QString mapamental_open_menu_text() const;
    QString mapamental_layout_mode_key() const;
    void set_mapamental_layout_mode_key(const QString &mode_key);
    QString mapamental_summary_method_key() const;
    void set_mapamental_summary_method_key(const QString &method_key);
    void open_mapamental();
    void reload_mapamental_from_backup();
    bool has_audiolivro_plugin() const;
    QString audiolivro_display_name() const;
    QString audiolivro_open_menu_text() const;
    QString audiolivro_font_settings_menu_text() const;
    QVariantList audiolivro_voice_options() const;
    QString audiolivro_current_voice_id() const;
    void set_audiolivro_voice_id(const QString &voice_id);
    bool open_audiolivro_font_settings(QWidget *parent = nullptr);
    void open_audiolivro();
    bool has_agenda_plugin() const;
    QString agenda_display_name() const;
    QString agenda_open_menu_text() const;
    void open_agenda();
    void populate_agenda_delete_item_menu(QMenu *menu);
    void populate_agenda_color_menu(QMenu *menu);
    bool has_gantt_plugin() const;
    QString gantt_display_name() const;
    QString gantt_open_menu_text() const;
    void open_gantt();
    void open_gantt_and_highlight_activity(const QString &schedule_item_id);
    source::plugin::IGanttPlugin *gantt_plugin() const { return gantt_plugin_; }
    bool calendar_panel_visible() const;
    void set_calendar_panel_visible(bool visible, bool persist = true);
    bool feynman_task_panel_visible() const;
    void set_feynman_task_panel_visible(bool visible, bool persist = true);
    bool pomodoro_panel_visible() const;
    void set_pomodoro_panel_visible(bool visible, bool persist = true);
    bool pomodoro_in_progress_panel_visible() const;
    void set_pomodoro_in_progress_panel_visible(bool visible, bool persist = true);
    Q_SLOT void refresh_module_task_counter();
    void clear_all_task_selections_except(const QListWidget *except_list = nullptr);
    void clear_pomodoro_selections();
    bool send_task_to_feynman(QListWidgetItem *item, QListWidget *source_list = nullptr, bool activate_tab = false);
    bool send_task_to_pomodoro(QListWidgetItem *item, QListWidget *source_list = nullptr, bool activate_tab = false);
    bool clear_pomodoro_tasks_if_available();
    bool clear_feynman_tasks_if_available();
    void clear_pomodoro_custom_font_settings(bool target_linked);
    void clear_feynman_custom_font_settings(bool target_linked);
    void clear_mapamental_custom_font_settings(bool target_linked);
    QVariantList get_pomodoro_task_font_overrides(bool target_linked) const;
    QVariantList get_feynman_task_font_overrides(bool target_linked) const;
    QVariantList get_mapamental_task_font_overrides(bool target_linked) const;
    void reload_all_modules_from_shared_store();
    bool sync_eisenhower_enabled() const;
    bool sync_pomodoro_enabled() const;
    bool sync_feynman_enabled() const;
    void set_sync_eisenhower_enabled(bool enabled);
    void set_sync_pomodoro_enabled(bool enabled);
    void set_sync_feynman_enabled(bool enabled);
    bool has_active_task_sync(const QString &origin = QString()) const;
    void show_task_sync_add_info(const QString &origin = QString());
    void show_task_sync_delete_info(const QString &origin = QString(), QWidget *parent = nullptr);
    void remove_task_from_synced_modules(const QVariantMap &task, const QString &origin);
    bool reserved_space_eisenhower_enabled() const;
    bool reserved_space_pomodoro_enabled() const;
    bool reserved_space_feynman_enabled() const;
    void set_reserved_space_eisenhower_enabled(bool enabled);
    void set_reserved_space_pomodoro_enabled(bool enabled);
    void set_reserved_space_feynman_enabled(bool enabled);
    bool reserved_watermark_eisenhower_enabled() const;
    bool reserved_watermark_pomodoro_enabled() const;
    bool reserved_watermark_feynman_enabled() const;
    void set_reserved_watermark_eisenhower_enabled(bool enabled);
    void set_reserved_watermark_pomodoro_enabled(bool enabled);
    void set_reserved_watermark_feynman_enabled(bool enabled);
    int selected_plugin_task_count() const;
    QString selected_plugin_task_color() const;
    int apply_color_to_selected_plugin_tasks(const QString &color);
    void refresh_task_title_styles();
    QVariantList plugin_status_report() const;

    bool is_in_progress_list(const QListWidget *list) const;
    QListWidget *quadrant_list(int quadrant, bool completed) const;
    QListWidget *quadrant_in_progress_list(int quadrant) const;
    bool is_completed_list(const QListWidget *list) const;

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    struct TaskSortCriterionState {
        TaskSortCriterion criterion = TaskSortCriterion::Date;
        Qt::SortOrder order = Qt::AscendingOrder;
    };

    QString layout_path;
    TaskLayoutMode task_layout_mode_ = TaskLayoutMode::Tabs;
    QList<TaskSortCriterionState> task_sort_criteria_;
    qint64 task_insertion_counter_ = 0;
    int last_quadrant_tab_index_ = 0;
    QString feynman_plugin_path_;
    QPluginLoader *feynman_plugin_loader_ = nullptr;
    source::plugin::IFeynmanPlugin *feynman_plugin_ = nullptr;
    source::plugin::FeynmanHostBridge *feynman_host_bridge_ = nullptr;
    QString pomodoro_plugin_path_;
    QPluginLoader *pomodoro_plugin_loader_ = nullptr;
    source::plugin::IPomodoroPlugin *pomodoro_plugin_ = nullptr;
    source::plugin::PomodoroHostBridge *pomodoro_host_bridge_ = nullptr;
    QString mapamental_plugin_path_;
    QPluginLoader *mapamental_plugin_loader_ = nullptr;
    source::plugin::IMapaMentalPlugin *mapamental_plugin_ = nullptr;
    source::plugin::MapaMentalHostBridge *mapamental_host_bridge_ = nullptr;
    QString modelo_plugin_path_;
    QPluginLoader *modelo_plugin_loader_ = nullptr;
    source::plugin::IModeloPlugin *modelo_plugin_ = nullptr;
    QString audiolivro_plugin_path_;
    QPluginLoader *audiolivro_plugin_loader_ = nullptr;
    source::plugin::IAudiolivroPlugin *audiolivro_plugin_ = nullptr;
    source::plugin::AudiolivroHostBridge *audiolivro_host_bridge_ = nullptr;
    QString agenda_plugin_path_;
    QPluginLoader *agenda_plugin_loader_ = nullptr;
    source::plugin::IAgendaPlugin *agenda_plugin_ = nullptr;
    source::plugin::AgendaHostBridge *agenda_host_bridge_ = nullptr;
    QString gantt_plugin_path_;
    QPluginLoader *gantt_plugin_loader_ = nullptr;
    source::plugin::IGanttPlugin *gantt_plugin_ = nullptr;
    source::plugin::GanttHostBridge *gantt_host_bridge_ = nullptr;
    std::unique_ptr<EisenhowerGanttHostAdapter> gantt_host_adapter_;
    QString agenda_color_mode_;
    QString pending_shared_origin_;
    QStringList pending_runtime_sync_origins_;
    bool runtime_sync_flush_scheduled_ = false;
    QStringList pending_save_origins_;
    bool task_save_flush_scheduled_ = false;
    bool sync_eisenhower_enabled_ = true;
    bool sync_pomodoro_enabled_ = true;
    bool sync_feynman_enabled_ = true;
    bool reserved_space_eisenhower_enabled_ = true;
    bool reserved_space_pomodoro_enabled_ = false;
    bool reserved_space_feynman_enabled_ = false;
    bool reserved_watermark_eisenhower_enabled_ = true;
    bool reserved_watermark_pomodoro_enabled_ = false;
    bool reserved_watermark_feynman_enabled_ = false;
    QString feynman_ui_layout_mode_key_ = QStringLiteral("tabs");
    QString pomodoro_ui_layout_mode_key_ = QStringLiteral("simple:tabs");
    bool poca_layout_active_ = false;
    bool window_geometry_stabilization_queued_ = false;
    QMap<QString, QString> plugin_load_errors_;

    void _apply_theme_palette();
    void _hide_legacy_calendar_button();
    bool _window_geometry_adjustment_deferred() const;
    void _queue_window_geometry_stabilization(int delay_ms = 0);
    void _clamp_window_to_screen();
    void _load_task_layout_preference();
    void _save_task_layout_preference() const;
    bool _apply_task_layout();
    bool _attach_quadrant_panels_to_tabs(bool horizontal_layout = false);
    bool _attach_quadrant_panels_to_columns();
    bool _attach_quadrant_panels_to_matrix_2x2();
    bool _open_linked_file_path(const QString &path) const;

    void _load_feynman_plugin();
    void _sync_feynman_host_bridge();
    void _load_pomodoro_plugin();
    void _sync_pomodoro_host_bridge();
    void _load_agenda_plugin();
    void _sync_agenda_host_bridge();
    void _load_gantt_plugin();
    void _sync_gantt_host_bridge();
    void _apply_agenda_color_mode(const QString &mode);

    void _connect_task_counter_list(QListWidget *list);
    void _connect_feynman_task_counter_sources();
    void _connect_pomodoro_task_counter_sources();
    void _connect_mapamental_task_counter_sources();
    void _connect_audiolivro_task_counter_sources();
    QPair<int, int> _eisenhower_task_counts() const;
    QPair<int, int> _feynman_task_counts() const;
    QPair<int, int> _pomodoro_task_counts() const;
    QPair<int, int> _mapamental_task_counts() const;
    QPair<int, int> _audiolivro_task_counts() const;
    void _sort_task_list(QListWidget *list);
    void _insert_task_item_by_time_group(QListWidget *list, QListWidgetItem *item);
    void _apply_task_item_appearance(QListWidgetItem *item) const;
    QListWidgetItem *_create_task_item(const QVariantMap &data, Qt::CheckState check_state) const;
    bool _task_sort_less(const QVariantMap &left, const QVariantMap &right) const;
    int _task_sort_criterion_index(TaskSortCriterion criterion) const;
    void _ensure_task_metadata(QVariantMap &data);
    qint64 _task_insertion_order(const QVariantMap &data) const;
    void _apply_startup_geometry_constraints();
    bool _send_task_payload_to_feynman(const QVariantMap &task, bool activate_tab = false);
    bool _upsert_task_from_external(const QVariantMap &task);
    void _set_linked_file_for_item(QListWidgetItem *item, const QString &path);
    bool _is_group_header(QListWidgetItem *item) const;
    QString _time_group_label(const QVariantMap &data) const;
    QString _time_group_key(const QVariantMap &data) const;
    QPair<int, int> _time_key(const QString &time_str) const;
    void _sync_splitter_for_calendar();
    void _queue_runtime_sync(const QString &origin);
    void _flush_runtime_sync();
    void _flush_debounced_task_save();
    void _perform_task_save(const QStringList &origins);
    void _reload_eisenhower_from_shared_store();
    void _reload_pomodoro_from_shared_store();
    void _reload_feynman_from_shared_store();
    void _refresh_plugin_task_views();

    friend class EisenhowerGanttHostAdapter;
};

}  // namespace source
