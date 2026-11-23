#include "GanttMainWindow.hpp"
#include <QActionGroup>
#include <QComboBox>
#include <QDir>
#include <QEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSaveFile>
#include <QSettings>
#include <QSpinBox>
#include <QTextEdit>
#include <QTimer>
#include "CsvMappingDialog.hpp"
#include "EisenhowerTaskImportDialog.hpp"
#include "GanttHolidaysDialog.hpp"
#include "GanttWorkingDaysDialog.hpp"
#include "TaskLinkDialog.hpp"
#include "../application/services/GanttTaskIntegrationBridge.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"
#include "source/utils/UnifiedTaskStore.hpp"

namespace source::gantt {

namespace {

QString table_layout_mode_to_string(GanttMainWindow::TableLayoutMode mode)
{
    switch (mode) {
    case GanttMainWindow::TableLayoutMode::Original:
        return QStringLiteral("original");
    case GanttMainWindow::TableLayoutMode::VerticalTopo:
        return QStringLiteral("vertical_topo");
    case GanttMainWindow::TableLayoutMode::VerticalInferior:
        return QStringLiteral("vertical_inferior");
    case GanttMainWindow::TableLayoutMode::HorizontalEsquerda:
        return QStringLiteral("horizontal_esquerda");
    case GanttMainWindow::TableLayoutMode::HorizontalDireita:
        return QStringLiteral("horizontal_direita");
    }
    return QStringLiteral("original");
}

GanttMainWindow::TableLayoutMode table_layout_mode_from_string(const QString &str, bool *ok = nullptr)
{
    const QString s = str.trimmed().toLower();
    if (ok) *ok = true;
    if (s == QStringLiteral("original")) {
        return GanttMainWindow::TableLayoutMode::Original;
    }
    if (s == QStringLiteral("vertical_topo") || s == QStringLiteral("verticaltopo") || s == QStringLiteral("vertical_top")) {
        return GanttMainWindow::TableLayoutMode::VerticalTopo;
    }
    if (s == QStringLiteral("vertical_inferior") || s == QStringLiteral("verticalinferior") || s == QStringLiteral("vertical_bottom")) {
        return GanttMainWindow::TableLayoutMode::VerticalInferior;
    }
    if (s == QStringLiteral("horizontal_esquerda") || s == QStringLiteral("horizontalesquerda") || s == QStringLiteral("horizontal_left")) {
        return GanttMainWindow::TableLayoutMode::HorizontalEsquerda;
    }
    if (s == QStringLiteral("horizontal_direita") || s == QStringLiteral("horizontaldireita") || s == QStringLiteral("horizontal_right")) {
        return GanttMainWindow::TableLayoutMode::HorizontalDireita;
    }
    bool is_num = false;
    const int num = s.toInt(&is_num);
    if (is_num && num >= 0 && num <= 4) {
        return static_cast<GanttMainWindow::TableLayoutMode>(num);
    }
    if (ok) *ok = false;
    return GanttMainWindow::TableLayoutMode::Original;
}

QString caminho_arquivo_layout_json()
{
    const QString persistente = source::utils::obter_caminho_persistente();
    return QDir(persistente).filePath(QStringLiteral("layout.json"));
}

} // namespace

GanttMainWindow::GanttMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlag(Qt::Window, true);
    setWindowTitle(tr("EisenPo — Gestão de Cronograma, Gantt & BIM 4D"));
    resize(1280, 760);

    // Initialize database
    QString db_err;
    database_.initialize(QString(), &db_err);

    setup_ui();
    setup_menus();

    load_or_create_default_schedule();

    // Register with integration bridge
    GanttTaskIntegrationBridge::instance().register_gantt_window(this);

    // Forward progress updates from Gantt to EisenPo operational tasks
    progress_svc_.set_host_sync_callback([](const QString &task_id, double percent, bool is_done) {
        GanttTaskIntegrationBridge::instance().sync_gantt_progress_to_eisenhower(task_id, percent, is_done);
    });

    update_status_bar();
}

GanttMainWindow::~GanttMainWindow()
{
    GanttTaskIntegrationBridge::instance().unregister_gantt_window(this);

    Schedule *s = current_schedule();
    if (s) {
        database_.save_schedule(*s);
    }
    database_.close();
}

void GanttMainWindow::setup_ui()
{
    wbs_tree_ = new GanttWbsTreeWidget(this);
    timeline_ = new GanttTimelineWidget(this);
    bim_panel_ = new Bim4DSimulationPanel(this);

    const TableLayoutMode saved_mode = load_table_layout_from_json();
    apply_table_layout(saved_mode);

    // Synchronize selection and actions from timeline
    connect(timeline_, &GanttTimelineWidget::item_clicked, this, &GanttMainWindow::on_activity_selected);
    connect(timeline_, &GanttTimelineWidget::item_double_clicked, this, [this](const QString &) {
        on_action_edit_activity();
    });
    connect(timeline_, &GanttTimelineWidget::request_delete_item, this, &GanttMainWindow::delete_activity_with_dialogs);

    // Synchronize actions from WBS tree
    connect(wbs_tree_, &GanttWbsTreeWidget::item_double_clicked, this, [this](const QString &) {
        on_action_edit_activity();
    });
    connect(wbs_tree_, &GanttWbsTreeWidget::request_delete_item, this, &GanttMainWindow::delete_activity_with_dialogs);

    // EisenPo integration signals from WBS tree
    connect(wbs_tree_, &GanttWbsTreeWidget::request_link_task, this, &GanttMainWindow::on_request_link_task);
    connect(wbs_tree_, &GanttWbsTreeWidget::request_unlink_task, this, &GanttMainWindow::on_request_unlink_task);
    connect(wbs_tree_, &GanttWbsTreeWidget::request_sync_task, this, &GanttMainWindow::on_request_sync_task);

    // Synchronize 4D simulation reference date and actions
    connect(bim_panel_, &Bim4DSimulationPanel::reference_date_changed, this, [this](const QDateTime &dt) {
        timeline_->set_reference_date(dt);
        if (wbs_tree_) {
            wbs_tree_->set_reference_date(dt);
        }
    });
    connect(bim_panel_, &Bim4DSimulationPanel::item_selected, this, &GanttMainWindow::on_activity_selected);
    connect(bim_panel_, &Bim4DSimulationPanel::request_delete_item, this, &GanttMainWindow::delete_activity_with_dialogs);

    // Status bar
    lbl_status_metrics_ = new QLabel(this);
    statusBar()->addWidget(lbl_status_metrics_, 1);
}

void GanttMainWindow::apply_table_layout(TableLayoutMode mode)
{
    current_layout_mode_ = mode;

    const bool wbs_visible = act_toggle_wbs_ ? act_toggle_wbs_->isChecked() : (wbs_tree_ ? wbs_tree_->isVisible() : true);
    const bool bim_visible = act_bim_ ? act_bim_->isChecked() : (bim_panel_ ? bim_panel_->isVisible() : true);

    if (wbs_tree_) wbs_tree_->setParent(this);
    if (timeline_) timeline_->setParent(this);
    if (bim_panel_) bim_panel_->setParent(this);

    if (vertical_splitter_) {
        delete vertical_splitter_;
        vertical_splitter_ = nullptr;
        main_splitter_ = nullptr;
    }

    const int total_w = qMax(800, width());
    const int total_h = qMax(600, height());

    switch (mode) {
    case TableLayoutMode::Original: {
        // Layout Original:
        // Splitter vertical principal:
        // - Top: Splitter horizontal com Tabela WBS e Diagrama de Gantt dividindo 50%/50% o espaço horizontal
        // - Bottom: Painel de Simulação 4D
        vertical_splitter_ = new QSplitter(Qt::Vertical, this);
        vertical_splitter_->setChildrenCollapsible(false);

        main_splitter_ = new QSplitter(Qt::Horizontal, vertical_splitter_);
        main_splitter_->setChildrenCollapsible(false);

        main_splitter_->addWidget(wbs_tree_);
        main_splitter_->addWidget(timeline_);
        main_splitter_->setStretchFactor(0, 1);
        main_splitter_->setStretchFactor(1, 1);
        main_splitter_->setSizes({total_w / 2, total_w / 2});

        vertical_splitter_->addWidget(main_splitter_);
        vertical_splitter_->addWidget(bim_panel_);
        vertical_splitter_->setStretchFactor(0, 3);
        vertical_splitter_->setStretchFactor(1, 1);
        vertical_splitter_->setSizes({(total_h * 3) / 4, total_h / 4});
        break;
    }
    case TableLayoutMode::VerticalTopo: {
        // Vertical Topo:
        // Tabelas "Tabela WBS" e "Simulação 4D" lado-a-lado no topo (50%/50% horizontal)
        // e o "Diagrama de Gantt" na parte inferior (50%/50% vertical)
        vertical_splitter_ = new QSplitter(Qt::Vertical, this);
        vertical_splitter_->setChildrenCollapsible(false);

        main_splitter_ = new QSplitter(Qt::Horizontal, vertical_splitter_);
        main_splitter_->setChildrenCollapsible(false);

        main_splitter_->addWidget(wbs_tree_);
        main_splitter_->addWidget(bim_panel_);
        main_splitter_->setStretchFactor(0, 1);
        main_splitter_->setStretchFactor(1, 1);
        main_splitter_->setSizes({total_w / 2, total_w / 2});

        vertical_splitter_->addWidget(main_splitter_);
        vertical_splitter_->addWidget(timeline_);
        vertical_splitter_->setStretchFactor(0, 1);
        vertical_splitter_->setStretchFactor(1, 1);
        vertical_splitter_->setSizes({total_h / 2, total_h / 2});
        break;
    }
    case TableLayoutMode::VerticalInferior: {
        // Vertical Inferior:
        // "Diagrama de Gantt" na parte superior (50%/50% vertical)
        // e as tabelas "Tabela WBS" e "Simulação 4D" lado-a-lado no canto inferior (50%/50% horizontal)
        vertical_splitter_ = new QSplitter(Qt::Vertical, this);
        vertical_splitter_->setChildrenCollapsible(false);

        main_splitter_ = new QSplitter(Qt::Horizontal, vertical_splitter_);
        main_splitter_->setChildrenCollapsible(false);

        main_splitter_->addWidget(wbs_tree_);
        main_splitter_->addWidget(bim_panel_);
        main_splitter_->setStretchFactor(0, 1);
        main_splitter_->setStretchFactor(1, 1);
        main_splitter_->setSizes({total_w / 2, total_w / 2});

        vertical_splitter_->addWidget(timeline_);
        vertical_splitter_->addWidget(main_splitter_);
        vertical_splitter_->setStretchFactor(0, 1);
        vertical_splitter_->setStretchFactor(1, 1);
        vertical_splitter_->setSizes({total_h / 2, total_h / 2});
        break;
    }
    case TableLayoutMode::HorizontalEsquerda: {
        // Horizontal Esquerda:
        // Tabelas no canto esquerdo da UI empilhadas (WBS no topo, Simulação 4D abaixo, 50%/50% vertical)
        // e Diagrama de Gantt no canto direito (50%/50% horizontal)
        vertical_splitter_ = new QSplitter(Qt::Horizontal, this);
        vertical_splitter_->setChildrenCollapsible(false);

        main_splitter_ = new QSplitter(Qt::Vertical, vertical_splitter_);
        main_splitter_->setChildrenCollapsible(false);

        main_splitter_->addWidget(wbs_tree_);
        main_splitter_->addWidget(bim_panel_);
        main_splitter_->setStretchFactor(0, 1);
        main_splitter_->setStretchFactor(1, 1);
        main_splitter_->setSizes({total_h / 2, total_h / 2});

        vertical_splitter_->addWidget(main_splitter_);
        vertical_splitter_->addWidget(timeline_);
        vertical_splitter_->setStretchFactor(0, 1);
        vertical_splitter_->setStretchFactor(1, 1);
        vertical_splitter_->setSizes({total_w / 2, total_w / 2});
        break;
    }
    case TableLayoutMode::HorizontalDireita: {
        // Horizontal Direita:
        // Diagrama de Gantt no canto esquerdo (50%/50% horizontal)
        // e tabelas no canto direito empilhadas (WBS no topo, Simulação 4D abaixo, 50%/50% vertical)
        vertical_splitter_ = new QSplitter(Qt::Horizontal, this);
        vertical_splitter_->setChildrenCollapsible(false);

        main_splitter_ = new QSplitter(Qt::Vertical, vertical_splitter_);
        main_splitter_->setChildrenCollapsible(false);

        main_splitter_->addWidget(wbs_tree_);
        main_splitter_->addWidget(bim_panel_);
        main_splitter_->setStretchFactor(0, 1);
        main_splitter_->setStretchFactor(1, 1);
        main_splitter_->setSizes({total_h / 2, total_h / 2});

        vertical_splitter_->addWidget(timeline_);
        vertical_splitter_->addWidget(main_splitter_);
        vertical_splitter_->setStretchFactor(0, 1);
        vertical_splitter_->setStretchFactor(1, 1);
        vertical_splitter_->setSizes({total_w / 2, total_w / 2});
        break;
    }
    }

    setCentralWidget(vertical_splitter_);

    if (wbs_tree_) wbs_tree_->setVisible(wbs_visible);
    if (bim_panel_) bim_panel_->setVisible(bim_visible);
    if (timeline_) timeline_->setVisible(true);

    auto update_bim_panel = [this]() {
        if (bim_panel_) {
            bim_panel_->update_control_bar_layout(bim_panel_->width());
        }
    };

    if (main_splitter_) {
        connect(main_splitter_, &QSplitter::splitterMoved, this, [update_bim_panel](int, int) {
            update_bim_panel();
            QTimer::singleShot(0, update_bim_panel);
        });
    }
    if (vertical_splitter_) {
        connect(vertical_splitter_, &QSplitter::splitterMoved, this, [update_bim_panel](int, int) {
            update_bim_panel();
            QTimer::singleShot(0, update_bim_panel);
        });
    }

    if (bim_panel_) {
        bim_panel_->update_control_bar_layout(bim_panel_->width());
    }

    save_table_layout_to_json(mode);

    update_layout_actions_checked();
}

GanttMainWindow::TableLayoutMode GanttMainWindow::load_table_layout_from_json() const
{
    const QString file_path = caminho_arquivo_layout_json();
    QFile file(file_path);
    if (file.open(QIODevice::ReadOnly)) {
        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            const QJsonObject obj = doc.object();

            if (obj.contains(QStringLiteral("gantt_table_layout"))) {
                const QJsonValue val = obj.value(QStringLiteral("gantt_table_layout"));
                if (val.isString()) {
                    bool ok = false;
                    const TableLayoutMode m = table_layout_mode_from_string(val.toString(), &ok);
                    if (ok) return m;
                } else if (val.isDouble()) {
                    const int n = val.toInt();
                    if (n >= 0 && n <= 4) return static_cast<TableLayoutMode>(n);
                }
            }

            if (obj.contains(QStringLiteral("gantt_layout"))) {
                const QJsonValue val = obj.value(QStringLiteral("gantt_layout"));
                if (val.isString()) {
                    bool ok = false;
                    const TableLayoutMode m = table_layout_mode_from_string(val.toString(), &ok);
                    if (ok) return m;
                } else if (val.isDouble()) {
                    const int n = val.toInt();
                    if (n >= 0 && n <= 4) return static_cast<TableLayoutMode>(n);
                }
            }

            if (obj.contains(QStringLiteral("gantt_table_layout_mode"))) {
                const int n = obj.value(QStringLiteral("gantt_table_layout_mode")).toInt(-1);
                if (n >= 0 && n <= 4) return static_cast<TableLayoutMode>(n);
            }
        }
    }

    QSettings settings;
    int saved_mode = settings.value(QStringLiteral("Gantt/table_layout_mode"), static_cast<int>(TableLayoutMode::Original)).toInt();
    if (saved_mode >= 0 && saved_mode <= 4) {
        return static_cast<TableLayoutMode>(saved_mode);
    }

    return TableLayoutMode::Original;
}

void GanttMainWindow::save_table_layout_to_json(TableLayoutMode mode)
{
    const QString file_path = caminho_arquivo_layout_json();
    const QFileInfo file_info(file_path);
    QDir().mkpath(file_info.absolutePath());

    QJsonObject config;
    QFile read_file(file_path);
    if (read_file.open(QIODevice::ReadOnly)) {
        const QJsonDocument doc = QJsonDocument::fromJson(read_file.readAll());
        if (doc.isObject()) {
            config = doc.object();
        }
        read_file.close();
    }

    const QString mode_str = table_layout_mode_to_string(mode);

    if (config.value(QStringLiteral("gantt_table_layout")).toString() == mode_str &&
        config.value(QStringLiteral("gantt_layout")).toString() == mode_str &&
        config.value(QStringLiteral("gantt_table_layout_mode")).toInt(-1) == static_cast<int>(mode)) {
        return;
    }

    config.insert(QStringLiteral("gantt_table_layout"), mode_str);
    config.insert(QStringLiteral("gantt_layout"), mode_str);
    config.insert(QStringLiteral("gantt_table_layout_mode"), static_cast<int>(mode));

    QSaveFile save_file(file_path);
    if (!save_file.open(QIODevice::WriteOnly)) {
        source::utils::LogManager::get_logger()->error(
            QStringLiteral("Erro ao abrir layout.json para escrita: %1").arg(file_path));
        return;
    }

    const QByteArray payload = QJsonDocument(config).toJson(QJsonDocument::Indented);
    if (save_file.write(payload) != payload.size() || !save_file.commit()) {
        source::utils::LogManager::get_logger()->error(
            QStringLiteral("Erro ao salvar layout do Gantt em: %1").arg(file_path));
    }

    QSettings settings;
    settings.setValue(QStringLiteral("Gantt/table_layout_mode"), static_cast<int>(mode));
}

void GanttMainWindow::update_layout_actions_checked()
{
    if (act_layout_original_) act_layout_original_->setChecked(current_layout_mode_ == TableLayoutMode::Original);
    if (act_layout_vertical_topo_) act_layout_vertical_topo_->setChecked(current_layout_mode_ == TableLayoutMode::VerticalTopo);
    if (act_layout_vertical_inferior_) act_layout_vertical_inferior_->setChecked(current_layout_mode_ == TableLayoutMode::VerticalInferior);
    if (act_layout_horizontal_esquerda_) act_layout_horizontal_esquerda_->setChecked(current_layout_mode_ == TableLayoutMode::HorizontalEsquerda);
    if (act_layout_horizontal_direita_) act_layout_horizontal_direita_->setChecked(current_layout_mode_ == TableLayoutMode::HorizontalDireita);
}

void GanttMainWindow::setup_menus()
{
    menu_bar_ = menuBar();

    // -------------------------------------------------------------
    // Menu: Arquivo
    // -------------------------------------------------------------
    menu_arquivo_ = menu_bar_->addMenu(tr("Arquivo"));

    act_new_ = new QAction(tr("➕ Nova Atividade"), this);
    act_new_->setToolTip(tr("Criar nova atividade no cronograma"));
    connect(act_new_, &QAction::triggered, this, &GanttMainWindow::on_action_new_activity);
    menu_arquivo_->addAction(act_new_);

    act_edit_ = new QAction(tr("✏️ Editar"), this);
    act_edit_->setToolTip(tr("Editar atividade selecionada"));
    connect(act_edit_, &QAction::triggered, this, &GanttMainWindow::on_action_edit_activity);
    menu_arquivo_->addAction(act_edit_);

    act_del_ = new QAction(tr("❌ Excluir"), this);
    act_del_->setToolTip(tr("Excluir atividade selecionada"));
    act_del_->setShortcut(QKeySequence::Delete);
    connect(act_del_, &QAction::triggered, this, &GanttMainWindow::on_action_delete_activity);
    menu_arquivo_->addAction(act_del_);
    addAction(act_del_);

    menu_arquivo_->addSeparator();

    act_import_csv_ = new QAction(tr("📥 Importar CSV"), this);
    act_import_csv_->setToolTip(tr("Importar cronograma de arquivo CSV"));
    connect(act_import_csv_, &QAction::triggered, this, &GanttMainWindow::on_action_import_csv);
    menu_arquivo_->addAction(act_import_csv_);

    act_export_csv_ = new QAction(tr("📤 Exportar CSV"), this);
    act_export_csv_->setToolTip(tr("Exportar cronograma atual para formato CSV"));
    connect(act_export_csv_, &QAction::triggered, this, &GanttMainWindow::on_action_export_csv);
    menu_arquivo_->addAction(act_export_csv_);

    // -------------------------------------------------------------
    // Menu: Configurações
    // -------------------------------------------------------------
    menu_configuracoes_ = menu_bar_->addMenu(tr("Configurações"));

    act_dep_ = new QAction(tr("🔗 Vínculo/Predecessora"), this);
    act_dep_->setToolTip(tr("Criar dependência entre atividades (FS, SS, FF, SF)"));
    connect(act_dep_, &QAction::triggered, this, &GanttMainWindow::on_action_add_dependency);
    menu_configuracoes_->addAction(act_dep_);

    act_cpm_ = new QAction(tr("⚡ Recalcular CPM"), this);
    act_cpm_->setToolTip(tr("Recalcular Caminho Crítico e reagendar datas automaticamente"));
    connect(act_cpm_, &QAction::triggered, this, &GanttMainWindow::on_action_recalculate_cpm);
    menu_configuracoes_->addAction(act_cpm_);

    menu_configuracoes_->addSeparator();

    // -------------------------------------------------------------
    // Configurações > Dias Úteis
    // -------------------------------------------------------------
    menu_dias_uteis_ = menu_configuracoes_->addMenu(tr("📅 Dias Úteis"));

    act_config_dias_uteis_ = new QAction(tr("⚙️ Configurar Dias Úteis..."), this);
    act_config_dias_uteis_->setToolTip(tr("Definir quais dias da semana são considerados úteis e carga horária diária"));
    connect(act_config_dias_uteis_, &QAction::triggered, this, &GanttMainWindow::on_action_configure_working_days);
    menu_dias_uteis_->addAction(act_config_dias_uteis_);

    menu_dias_uteis_->addSeparator();

    const QString day_labels[7] = {
        tr("Segunda-feira"),
        tr("Terça-feira"),
        tr("Quarta-feira"),
        tr("Quinta-feira"),
        tr("Sexta-feira"),
        tr("Sábado"),
        tr("Domingo")
    };

    for (int i = 0; i < 7; ++i) {
        int day_num = i + 1;
        act_days_[i] = new QAction(day_labels[i], this);
        act_days_[i]->setCheckable(true);
        connect(act_days_[i], &QAction::toggled, this, [this, day_num](bool checked) {
            on_working_day_toggled(day_num, checked);
        });
        menu_dias_uteis_->addAction(act_days_[i]);
    }

    // -------------------------------------------------------------
    // Configurações > Feriados
    // -------------------------------------------------------------
    act_feriados_ = new QAction(tr("🏖️ Feriados..."), this);
    act_feriados_->setToolTip(tr("Gerenciar feriados e dias não úteis no calendário do cronograma"));
    connect(act_feriados_, &QAction::triggered, this, &GanttMainWindow::on_action_manage_holidays);
    menu_configuracoes_->addAction(act_feriados_);

    menu_configuracoes_->addSeparator();

    act_base_ = new QAction(tr("📌 Salvar Baseline"), this);
    act_base_->setToolTip(tr("Congelar linha de base atual do cronograma"));
    connect(act_base_, &QAction::triggered, this, &GanttMainWindow::on_action_create_baseline);
    menu_configuracoes_->addAction(act_base_);

    act_comp_ = new QAction(tr("📊 Comparar Baseline"), this);
    act_comp_->setToolTip(tr("Exibir relatório de desvios e alertas de atraso crítico"));
    connect(act_comp_, &QAction::triggered, this, &GanttMainWindow::on_action_compare_baseline);
    menu_configuracoes_->addAction(act_comp_);

    act_toggle_base_ = new QAction(tr("👁️ Linha Fantasma"), this);
    act_toggle_base_->setCheckable(true);
    act_toggle_base_->setChecked(true);
    act_toggle_base_->setToolTip(tr("Exibir/ocultar barras fantasmas de baseline"));
    connect(act_toggle_base_, &QAction::toggled, this, &GanttMainWindow::on_action_toggle_baseline);
    menu_configuracoes_->addAction(act_toggle_base_);

    menu_configuracoes_->addSeparator();

    auto *zoom_group = new QActionGroup(this);
    zoom_group->setExclusive(true);

    act_zoom_day_ = new QAction(tr("🔍 Zoom Dia"), this);
    act_zoom_day_->setCheckable(true);
    zoom_group->addAction(act_zoom_day_);
    connect(act_zoom_day_, &QAction::triggered, this, &GanttMainWindow::on_action_zoom_day);
    menu_configuracoes_->addAction(act_zoom_day_);

    act_zoom_week_ = new QAction(tr("🔍 Zoom Semana"), this);
    act_zoom_week_->setCheckable(true);
    zoom_group->addAction(act_zoom_week_);
    connect(act_zoom_week_, &QAction::triggered, this, &GanttMainWindow::on_action_zoom_week);
    menu_configuracoes_->addAction(act_zoom_week_);

    act_zoom_month_ = new QAction(tr("🔍 Zoom Mês"), this);
    act_zoom_month_->setCheckable(true);
    zoom_group->addAction(act_zoom_month_);
    connect(act_zoom_month_, &QAction::triggered, this, &GanttMainWindow::on_action_zoom_month);
    menu_configuracoes_->addAction(act_zoom_month_);

    act_zoom_day_->setChecked(true);
    update_zoom_highlight();

    menu_configuracoes_->addSeparator();

    act_toggle_wbs_ = new QAction(tr("📋 Tabela WBS"), this);
    act_toggle_wbs_->setCheckable(true);
    act_toggle_wbs_->setChecked(true);
    act_toggle_wbs_->setToolTip(tr("Exibir/ocultar tabela de atividades WBS"));
    connect(act_toggle_wbs_, &QAction::toggled, this, &GanttMainWindow::on_action_toggle_wbs_table);
    menu_configuracoes_->addAction(act_toggle_wbs_);

    act_bim_ = new QAction(tr("🏗️ Simulação 4D"), this);
    act_bim_->setCheckable(true);
    act_bim_->setChecked(true);
    act_bim_->setToolTip(tr("Exibir/ocultar painel de simulação BIM 4D"));
    connect(act_bim_, &QAction::toggled, this, &GanttMainWindow::on_action_toggle_bim_panel);
    menu_configuracoes_->addAction(act_bim_);

    menu_configuracoes_->addSeparator();

    // -------------------------------------------------------------
    // Configurações > Layout > Tabelas
    // -------------------------------------------------------------
    menu_layout_ = menu_configuracoes_->addMenu(tr("📐 Layout"));
    menu_tabelas_ = menu_layout_->addMenu(tr("📊 Tabelas"));

    group_layout_tabelas_ = new QActionGroup(this);
    group_layout_tabelas_->setExclusive(true);

    act_layout_original_ = new QAction(tr("Original"), this);
    act_layout_original_->setCheckable(true);
    act_layout_original_->setToolTip(tr("Disposição padrão das tabelas e do diagrama de Gantt"));
    group_layout_tabelas_->addAction(act_layout_original_);
    menu_tabelas_->addAction(act_layout_original_);
    connect(act_layout_original_, &QAction::triggered, this, [this]() {
        apply_table_layout(TableLayoutMode::Original);
    });

    act_layout_vertical_topo_ = new QAction(tr("Vertical Topo"), this);
    act_layout_vertical_topo_->setCheckable(true);
    act_layout_vertical_topo_->setToolTip(tr("Tabelas WBS e Simulação 4D no topo lado a lado, Diagrama de Gantt abaixo"));
    group_layout_tabelas_->addAction(act_layout_vertical_topo_);
    menu_tabelas_->addAction(act_layout_vertical_topo_);
    connect(act_layout_vertical_topo_, &QAction::triggered, this, [this]() {
        apply_table_layout(TableLayoutMode::VerticalTopo);
    });

    act_layout_vertical_inferior_ = new QAction(tr("Vertical Inferior"), this);
    act_layout_vertical_inferior_->setCheckable(true);
    act_layout_vertical_inferior_->setToolTip(tr("Diagrama de Gantt no topo, Tabelas WBS e Simulação 4D abaixo lado a lado"));
    group_layout_tabelas_->addAction(act_layout_vertical_inferior_);
    menu_tabelas_->addAction(act_layout_vertical_inferior_);
    connect(act_layout_vertical_inferior_, &QAction::triggered, this, [this]() {
        apply_table_layout(TableLayoutMode::VerticalInferior);
    });

    act_layout_horizontal_esquerda_ = new QAction(tr("Horizontal Esquerda"), this);
    act_layout_horizontal_esquerda_->setCheckable(true);
    act_layout_horizontal_esquerda_->setToolTip(tr("Tabelas WBS e Simulação 4D empilhadas à esquerda, Diagrama de Gantt à direita"));
    group_layout_tabelas_->addAction(act_layout_horizontal_esquerda_);
    menu_tabelas_->addAction(act_layout_horizontal_esquerda_);
    connect(act_layout_horizontal_esquerda_, &QAction::triggered, this, [this]() {
        apply_table_layout(TableLayoutMode::HorizontalEsquerda);
    });

    act_layout_horizontal_direita_ = new QAction(tr("Horizontal Direita"), this);
    act_layout_horizontal_direita_->setCheckable(true);
    act_layout_horizontal_direita_->setToolTip(tr("Diagrama de Gantt à esquerda, Tabelas WBS e Simulação 4D empilhadas à direita"));
    group_layout_tabelas_->addAction(act_layout_horizontal_direita_);
    menu_tabelas_->addAction(act_layout_horizontal_direita_);
    connect(act_layout_horizontal_direita_, &QAction::triggered, this, [this]() {
        apply_table_layout(TableLayoutMode::HorizontalDireita);
    });

    update_layout_actions_checked();

    menu_configuracoes_->addSeparator();

    act_sync_all_ = new QAction(tr("🔄 Sincronizar com EisenPo"), this);
    act_sync_all_->setToolTip(tr("Sincronizar tarefas da Matriz de Eisenhower no cronograma"));
    connect(act_sync_all_, &QAction::triggered, this, &GanttMainWindow::on_action_sync_all_eisenhower_tasks);
    menu_configuracoes_->addAction(act_sync_all_);

    act_import_eisen_ = new QAction(tr("📋 Importar do EisenPo"), this);
    act_import_eisen_->setToolTip(tr("Importar tarefas do EisenPo selecionadas para o cronograma"));
    connect(act_import_eisen_, &QAction::triggered, this, &GanttMainWindow::on_action_import_eisenhower_tasks);
    menu_configuracoes_->addAction(act_import_eisen_);
}


Schedule *GanttMainWindow::current_schedule()
{
    if (current_schedule_id_.isEmpty()) return nullptr;
    return schedule_svc_.get_schedule(current_schedule_id_);
}

void GanttMainWindow::load_or_create_default_schedule()
{
    QList<Schedule> loaded = database_.list_schedules();
    if (!loaded.isEmpty()) {
        current_schedule_id_ = loaded.first().id;
        for (auto &s : loaded) {
            bool modified = false;
            auto it = s.items.begin();
            while (it != s.items.end()) {
                const QString lower_name = it->name.toLower();
                if ((lower_name.contains(QStringLiteral("arma")) && lower_name.contains(QStringLiteral("pilar"))) ||
                    (lower_name.contains(QStringLiteral("instal")) && lower_name.contains(QStringLiteral("pavimento")))) {
                    it = s.items.erase(it);
                    modified = true;
                } else {
                    ++it;
                }
            }
            if (modified) {
                database_.save_schedule(s);
            }
            schedule_svc_.add_schedule(s);
        }
    } else {
        // Create initial construction schedule from residential template
        Schedule s = schedule_svc_.create_from_template(
            QStringLiteral("Residencial"),
            QStringLiteral("obra_01"),
            QStringLiteral("Edifício Residencial Horizonte - Cronograma Mestre")
        );
        current_schedule_id_ = s.id;

        // Add some default BIM links
        int count = 0;
        for (const auto &item : s.items) {
            if (count < 6) {
                BimLink link(item.id, QStringLiteral("2O2Fr$t4X7Zf8NOew3FL9%1").arg(count), BimSource::IFC);
                link.category = QStringLiteral("IfcBuildingElement");
                link.level = QStringLiteral("Pavimento %1").arg(count + 1);
                s.add_bim_link(link);
                count++;
            }
        }

        // Calculate initial CPM
        planning_svc_.recalculate(s);
        // Create initial baseline
        planning_svc_.create_baseline(s, QStringLiteral("Baseline Aprovada V1"), QStringLiteral("Eng. Responsável"));

        database_.save_schedule(s);
    }

    // Automatically synchronize tasks from EisenPo into the active schedule
    sync_all_eisenhower_tasks(true);

    refresh_view();
}

void GanttMainWindow::refresh_view()
{
    Schedule *s = current_schedule();
    if (!s) return;

    if (bim_panel_) {
        bim_panel_->set_schedule(*s);
    }
    const QDateTime ref_dt = bim_panel_ ? bim_panel_->current_reference_date() : QDateTime();
    if (wbs_tree_) {
        wbs_tree_->set_reference_date(ref_dt);
        wbs_tree_->populate(*s);
    }
    if (timeline_) {
        timeline_->set_schedule(*s);
        if (ref_dt.isValid()) {
            timeline_->set_reference_date(ref_dt);
        }
    }
    update_status_bar();
    update_working_days_menu();
}

void GanttMainWindow::update_status_bar()
{
    Schedule *s = current_schedule();
    if (s) {
        lbl_status_metrics_->setText(
            tr("Atividades: %1 | Duração: %2 dias | Progresso Geral: %3% | Críticas: %4 | Atrasadas: %5")
                .arg(s->items.size())
                .arg(QString::number(s->total_duration_hours() / 8.0, 'f', 1))
                .arg(static_cast<int>(s->overall_percent_complete()))
                .arg(s->critical_items_count())
                .arg(s->delayed_items_count())
        );
    } else {
        lbl_status_metrics_->setText(tr("Nenhum cronograma aberto."));
    }
}

void GanttMainWindow::on_activity_selected(const QString &item_id)
{
    Schedule *s = current_schedule();
    if (!s) return;

    // If wbs_tree_ already has this item selected, avoid calling setCurrentItem
    // to prevent overriding the user's selected cell or column to column 0.
    if (wbs_tree_ && wbs_tree_->selected_item_id() == item_id) {
        return;
    }

    for (int i = 0; i < wbs_tree_->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = wbs_tree_->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toString() == item_id) {
            const int col = (wbs_tree_->currentColumn() >= 0) ? wbs_tree_->currentColumn() : 0;
            wbs_tree_->setCurrentItem(item, col);
            return;
        }
    }

    for (const auto &act : s->items) {
        if (act.task_id == item_id || act.id == item_id) {
            for (int i = 0; i < wbs_tree_->topLevelItemCount(); ++i) {
                QTreeWidgetItem *item = wbs_tree_->topLevelItem(i);
                if (item->data(0, Qt::UserRole).toString() == act.id) {
                    const int col = (wbs_tree_->currentColumn() >= 0) ? wbs_tree_->currentColumn() : 0;
                    wbs_tree_->setCurrentItem(item, col);
                    return;
                }
            }
        }
    }
}

void GanttMainWindow::on_action_new_activity()
{
    Schedule *s = current_schedule();
    if (!s) return;

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Nova Atividade"));
    auto *layout = new QVBoxLayout(&dlg);
    auto *form = new QFormLayout();

    auto *edit_wbs = new QLineEdit(&dlg);
    edit_wbs->setText(QStringLiteral("1.%1").arg(s->items.size() + 1));
    form->addRow(tr("Código WBS:"), edit_wbs);

    auto *edit_name = new QLineEdit(&dlg);
    form->addRow(tr("Nome da Atividade:"), edit_name);

    auto *spin_dur = new QDoubleSpinBox(&dlg);
    spin_dur->setRange(0.0, 1000.0);
    spin_dur->setValue(5.0);
    spin_dur->setSuffix(tr(" dias"));
    form->addRow(tr("Duração:"), spin_dur);

    auto *edit_guid = new QLineEdit(&dlg);
    edit_guid->setPlaceholderText(QStringLiteral("Ex: 2O2Fr$t4X7Zf8NOew3FL9r"));
    form->addRow(tr("IFC GUID (BIM 4D):"), edit_guid);

    layout->addLayout(form);

    auto *btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    if (auto *btn_ok = btns->button(QDialogButtonBox::Ok)) btn_ok->setText(tr("Salvar"));
    if (auto *btn_cancel = btns->button(QDialogButtonBox::Cancel)) btn_cancel->setText(tr("Cancelar"));
    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(btns);

    if (dlg.exec() == QDialog::Accepted) {
        if (edit_name->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Aviso"), tr("O nome da atividade não pode ser vazio."));
            return;
        }

        ScheduleItem item(edit_name->text().trimmed());
        item.wbs_code = edit_wbs->text().trimmed();
        item.duration_hours = spin_dur->value() * 8.0;
        item.is_milestone = (spin_dur->value() == 0.0);

        s->add_item(item);

        if (!edit_guid->text().trimmed().isEmpty()) {
            BimLink link(item.id, edit_guid->text().trimmed(), BimSource::IFC);
            s->add_bim_link(link);
        }

        planning_svc_.recalculate(*s);

        // Ensure newly created activity has baseline dates and snapshot in active baseline
        ScheduleItem *added = s->find_item(item.id);
        if (added) {
            if (!added->baseline_start_date.isValid()) {
                added->baseline_start_date = added->start_date;
            }
            if (!added->baseline_end_date.isValid()) {
                added->baseline_end_date = added->end_date;
            }
            added->baseline_duration_hours = added->duration_hours;
            if (!s->active_baseline_id.isEmpty() && s->baselines.contains(s->active_baseline_id)) {
                BaselineItemSnapshot b_item;
                b_item.item_id = added->id;
                b_item.start_date = added->baseline_start_date;
                b_item.end_date = added->baseline_end_date;
                b_item.duration_hours = added->baseline_duration_hours;
                s->baselines[s->active_baseline_id].items.insert(added->id, b_item);
            }
        }

        database_.save_schedule(*s);
        refresh_view();
    }
}

namespace {

class DiscreteProgressSpinBox : public QDoubleSpinBox
{
public:
    explicit DiscreteProgressSpinBox(const QVector<double> &allowed_values, QWidget *parent = nullptr)
        : QDoubleSpinBox(parent), allowed_values_(allowed_values)
    {
        if (allowed_values_.isEmpty()) {
            allowed_values_ = {0.0, 100.0};
        }
        setRange(allowed_values_.first(), allowed_values_.last());
    }

    void stepBy(int steps) override
    {
        if (allowed_values_.isEmpty()) {
            QDoubleSpinBox::stepBy(steps);
            return;
        }
        const int cur_idx = find_closest_index(value());
        const int target_idx = qBound(0, cur_idx + steps, static_cast<int>(allowed_values_.size()) - 1);
        setValue(allowed_values_.at(target_idx));
    }

    int find_closest_index(double val) const
    {
        if (allowed_values_.isEmpty()) return 0;
        int best_idx = 0;
        double min_diff = std::abs(val - allowed_values_[0]);
        for (int i = 1; i < allowed_values_.size(); ++i) {
            const double diff = std::abs(val - allowed_values_[i]);
            if (diff < min_diff) {
                min_diff = diff;
                best_idx = i;
            }
        }
        return best_idx;
    }

    double snap(double val) const
    {
        if (allowed_values_.isEmpty()) return val;
        return allowed_values_.at(find_closest_index(val));
    }

private:
    QVector<double> allowed_values_;
};

} // namespace

void GanttMainWindow::on_action_edit_activity()
{
    Schedule *s = current_schedule();
    if (!s) return;

    QString item_id = wbs_tree_->selected_item_id();
    ScheduleItem *item = s->find_item(item_id);
    if (!item) {
        QMessageBox::information(this, tr("Informação"), tr("Selecione uma atividade para editar."));
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Editar Atividade — %1").arg(item->name));
    auto *layout = new QVBoxLayout(&dlg);
    auto *form = new QFormLayout();

    auto *edit_wbs = new QLineEdit(item->wbs_code, &dlg);
    form->addRow(tr("WBS:"), edit_wbs);

    auto *edit_name = new QLineEdit(item->name, &dlg);
    form->addRow(tr("Nome:"), edit_name);

    auto *spin_dur = new QDoubleSpinBox(&dlg);
    spin_dur->setRange(0.0, 1000.0);
    spin_dur->setValue(item->duration_hours / 8.0);
    spin_dur->setSuffix(tr(" dias"));
    form->addRow(tr("Duração:"), spin_dur);

    // Determina a meta de pomodoros definida para a tarefa associada
    int meta_pomodoros = 4;
    if (!item->task_id.trimmed().isEmpty()) {
        const QVariantMap task = GanttTaskIntegrationBridge::instance().get_task_by_id(item->task_id);
        if (!task.isEmpty()) {
            if (task.contains(QStringLiteral("ciclos_totais")) && task.value(QStringLiteral("ciclos_totais")).toInt() > 0) {
                meta_pomodoros = task.value(QStringLiteral("ciclos_totais")).toInt();
            } else if (task.contains(QStringLiteral("ciclos_permitidos")) && task.value(QStringLiteral("ciclos_permitidos")).toInt() > 0) {
                meta_pomodoros = task.value(QStringLiteral("ciclos_permitidos")).toInt();
            } else if (task.contains(QStringLiteral("total_ciclos")) && task.value(QStringLiteral("total_ciclos")).toInt() > 0) {
                meta_pomodoros = task.value(QStringLiteral("total_ciclos")).toInt();
            } else if (task.contains(QStringLiteral("meta_pomodoros")) && task.value(QStringLiteral("meta_pomodoros")).toInt() > 0) {
                meta_pomodoros = task.value(QStringLiteral("meta_pomodoros")).toInt();
            } else if (task.contains(QStringLiteral("meta")) && task.value(QStringLiteral("meta")).toInt() > 0) {
                meta_pomodoros = task.value(QStringLiteral("meta")).toInt();
            }
        }
    }
    if (meta_pomodoros == 4) {
        const auto available = GanttTaskIntegrationBridge::instance().get_available_eisenhower_tasks();
        for (const auto &v : available) {
            const auto m = v.toMap();
            const QString m_id = m.value(QStringLiteral("id")).toString().trimmed();
            const QString m_title = m.value(QStringLiteral("titulo"), m.value(QStringLiteral("text"))).toString().trimmed();
            if ((!item->task_id.trimmed().isEmpty() && m_id == item->task_id.trimmed()) ||
                (!m_title.isEmpty() && m_title.compare(item->name.trimmed(), Qt::CaseInsensitive) == 0)) {
                if (m.contains(QStringLiteral("ciclos_totais")) && m.value(QStringLiteral("ciclos_totais")).toInt() > 0) {
                    meta_pomodoros = m.value(QStringLiteral("ciclos_totais")).toInt();
                    break;
                } else if (m.contains(QStringLiteral("ciclos_permitidos")) && m.value(QStringLiteral("ciclos_permitidos")).toInt() > 0) {
                    meta_pomodoros = m.value(QStringLiteral("ciclos_permitidos")).toInt();
                    break;
                } else if (m.contains(QStringLiteral("total_ciclos")) && m.value(QStringLiteral("total_ciclos")).toInt() > 0) {
                    meta_pomodoros = m.value(QStringLiteral("total_ciclos")).toInt();
                    break;
                } else if (m.contains(QStringLiteral("meta_pomodoros")) && m.value(QStringLiteral("meta_pomodoros")).toInt() > 0) {
                    meta_pomodoros = m.value(QStringLiteral("meta_pomodoros")).toInt();
                    break;
                }
            }
        }
    }
    meta_pomodoros = qMax(1, meta_pomodoros);
    const double step = 100.0 / static_cast<double>(meta_pomodoros);

    int decimals = 0;
    if (std::abs(step - std::round(step)) > 0.001) {
        if (std::abs((step * 10.0) - std::round(step * 10.0)) > 0.01) {
            decimals = 2;
        } else {
            decimals = 1;
        }
    }

    QVector<double> allowed_values;
    for (int k = 0; k <= meta_pomodoros; ++k) {
        const double v = (k == meta_pomodoros) ? 100.0 : (k * step);
        allowed_values.append(v);
    }

    auto *prog_layout = new QHBoxLayout();
    auto *combo_prog = new QComboBox(&dlg);
    auto *spin_prog = new DiscreteProgressSpinBox(allowed_values, &dlg);
    spin_prog->setDecimals(decimals);
    spin_prog->setSingleStep(step);
    spin_prog->setSuffix(QStringLiteral("%"));

    for (int k = 0; k <= meta_pomodoros; ++k) {
        const double v = allowed_values.at(k);
        const QString pct_str = QString::number(v, 'f', decimals) + QStringLiteral("%");
        const QString item_label = (k == 1)
            ? tr("%1 (%2/%3 pomodoro)").arg(pct_str).arg(k).arg(meta_pomodoros)
            : tr("%1 (%2/%3 pomodoros)").arg(pct_str).arg(k).arg(meta_pomodoros);
        combo_prog->addItem(item_label, v);
    }

    const double initial_snapped = spin_prog->snap(item->percent_complete);
    spin_prog->setValue(initial_snapped);
    combo_prog->setCurrentIndex(spin_prog->find_closest_index(initial_snapped));

    QObject::connect(combo_prog, QOverload<int>::of(&QComboBox::currentIndexChanged), &dlg, [spin_prog, combo_prog](int idx) {
        if (idx >= 0) {
            const double val = combo_prog->itemData(idx).toDouble();
            if (std::abs(spin_prog->value() - val) > 0.001) {
                QSignalBlocker blocker(spin_prog);
                spin_prog->setValue(val);
            }
        }
    });

    QObject::connect(spin_prog, QOverload<double>::of(&QDoubleSpinBox::valueChanged), &dlg, [spin_prog, combo_prog](double val) {
        const int idx = spin_prog->find_closest_index(val);
        if (combo_prog->currentIndex() != idx) {
            QSignalBlocker blocker(combo_prog);
            combo_prog->setCurrentIndex(idx);
        }
    });

    QObject::connect(spin_prog, &QDoubleSpinBox::editingFinished, &dlg, [spin_prog, combo_prog]() {
        const double snapped = spin_prog->snap(spin_prog->value());
        if (std::abs(spin_prog->value() - snapped) > 0.001) {
            QSignalBlocker blocker(spin_prog);
            spin_prog->setValue(snapped);
        }
        const int idx = spin_prog->find_closest_index(snapped);
        if (combo_prog->currentIndex() != idx) {
            QSignalBlocker blocker(combo_prog);
            combo_prog->setCurrentIndex(idx);
        }
    });

    QStringList allowed_steps;
    for (int k = 0; k <= meta_pomodoros; ++k) {
        const double v = allowed_values.at(k);
        allowed_steps << (QString::number(v, 'f', decimals) + QStringLiteral("%"));
    }
    const QString tooltip_text = tr("Meta: %1 pomodoro(s). Passos: %2")
                                     .arg(meta_pomodoros)
                                     .arg(allowed_steps.join(QStringLiteral(", ")));
    combo_prog->setToolTip(tooltip_text);
    spin_prog->setToolTip(tooltip_text);

    prog_layout->addWidget(combo_prog, 1);
    prog_layout->addWidget(spin_prog, 1);
    form->addRow(tr("Progresso Físico:"), prog_layout);

    layout->addLayout(form);

    auto *btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    if (auto *btn_ok = btns->button(QDialogButtonBox::Ok)) btn_ok->setText(tr("Salvar"));
    if (auto *btn_cancel = btns->button(QDialogButtonBox::Cancel)) btn_cancel->setText(tr("Cancelar"));
    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(btns);

    if (dlg.exec() == QDialog::Accepted) {
        item->wbs_code = edit_wbs->text().trimmed();
        item->name = edit_name->text().trimmed();
        item->duration_hours = spin_dur->value() * 8.0;
        item->is_milestone = (spin_dur->value() == 0.0);

        if (item->task_id.trimmed().isEmpty()) {
            const auto available = GanttTaskIntegrationBridge::instance().get_available_eisenhower_tasks();
            for (const auto &v : available) {
                const auto m = v.toMap();
                const QString m_title = m.value(QStringLiteral("text"), m.value(QStringLiteral("titulo"))).toString().trimmed();
                if (!m_title.isEmpty() && m_title.compare(item->name.trimmed(), Qt::CaseInsensitive) == 0) {
                    item->task_id = m.value(QStringLiteral("id")).toString().trimmed();
                    break;
                }
            }
        }

        if (!item->task_id.isEmpty()) {
            GanttTaskIntegrationBridge::instance().sync_gantt_duration_to_eisenhower(item->task_id, spin_dur->value());
        }

        const double final_percent = spin_prog->snap(spin_prog->value());
        item->percent_complete = final_percent;
        if (final_percent >= 100.0) {
            item->status = ActivityStatus::Done;
        } else if (final_percent > 0.0) {
            item->status = ActivityStatus::InProgress;
        } else {
            item->status = ActivityStatus::NotStarted;
        }

        progress_svc_.record_progress(*s, item->id, final_percent, tr("Atualização via interface Gantt"));
        if (!item->task_id.isEmpty()) {
            GanttTaskIntegrationBridge::instance().sync_gantt_progress_to_eisenhower(
                item->task_id, final_percent, final_percent >= 100.0);
        }

        planning_svc_.recalculate(*s);
        database_.save_schedule(*s);
        refresh_view();
    }
}

QString GanttMainWindow::current_selected_item_id() const
{
    if (wbs_tree_ && !wbs_tree_->selected_item_id().trimmed().isEmpty()) {
        return wbs_tree_->selected_item_id().trimmed();
    }
    if (bim_panel_ && !bim_panel_->selected_item_id().trimmed().isEmpty()) {
        return bim_panel_->selected_item_id().trimmed();
    }
    return {};
}

void GanttMainWindow::on_action_delete_activity()
{
    const QString item_id = current_selected_item_id();
    if (item_id.isEmpty()) {
        QMessageBox::information(this,
                                 tr("🗑️ Remover Tarefa"),
                                 tr("Nenhuma atividade selecionada para exclusão."));
        return;
    }
    delete_activity_with_dialogs(item_id);
}

void GanttMainWindow::delete_activity_with_dialogs(const QString &item_id)
{
    Schedule *s = current_schedule();
    if (!s) return;

    ScheduleItem *item = s->find_item(item_id);
    if (!item) return;

    const QString activity_name = item->name;
    const QString task_id = item->task_id.trimmed();

    // -----------------------------------------------------------------
    // Diálogo 1: Confirmação de Exclusão (padrão EisenPo / Pomodoro)
    // -----------------------------------------------------------------
    QMessageBox msg(this);
    msg.setWindowModality(Qt::WindowModal);
    msg.setIcon(QMessageBox::Question);
    msg.setWindowTitle(tr("🗑️ Remover Tarefa"));
    msg.setText(tr("Deseja remover a tarefa '%1'?").arg(activity_name));
    QAbstractButton *btn_yes = msg.addButton(tr("Sim"), QMessageBox::YesRole);
    msg.addButton(tr("Não"), QMessageBox::NoRole);
    msg.setDefaultButton(QMessageBox::No);
    msg.exec();

    if (msg.clickedButton() != btn_yes) {
        return;
    }

    // -----------------------------------------------------------------
    // Diálogo 2: Aviso de Sincronização entre Módulos
    // Disparado se a tarefa estiver vinculada e houver múltiplos métodos sincronizados.
    // O ponteiro 'this' é passado para garantir que a janela modal apareça sobre
    // a janela do Gantt (mesmo em monitores secundários), sem puxar a GUI principal.
    // -----------------------------------------------------------------
    auto &bridge = GanttTaskIntegrationBridge::instance();
    if (!task_id.isEmpty()) {
        bridge.show_task_sync_delete_info(QStringLiteral("eisenhower"), this);
    }

    // -----------------------------------------------------------------
    // Execução da Exclusão:
    // 1. No EisenPo (remove de todos os quadrantes, sincroniza tarefas.json e outros plugins)
    // -----------------------------------------------------------------
    if (!task_id.isEmpty()) {
        bridge.remove_eisenhower_task_by_id(task_id);
    }

    // 2. No Cronograma Gantt (remove item, vínculos BIM, dependências, recalcula CPM e salva)
    s->remove_item(item_id);
    planning_svc_.recalculate(*s);
    database_.save_schedule(*s);
    refresh_view();

    // -----------------------------------------------------------------
    // Diálogo 3: Confirmação de Sucesso da Exclusão
    // -----------------------------------------------------------------
    QMessageBox info_box(this);
    info_box.setWindowModality(Qt::WindowModal);
    info_box.setIcon(QMessageBox::Information);
    info_box.setWindowTitle(tr("🗑️ Remover Tarefa"));
    info_box.setText(tr("Tarefa removida com sucesso."));
    info_box.setStandardButtons(QMessageBox::Ok);
    info_box.exec();
}

void GanttMainWindow::on_action_add_dependency()
{
    Schedule *s = current_schedule();
    if (!s || s->items.size() < 2) {
        QMessageBox::information(this, tr("Aviso"), tr("É necessário ter pelo menos 2 atividades para criar uma dependência."));
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Adicionar Dependência entre Atividades"));
    auto *layout = new QVBoxLayout(&dlg);
    auto *form = new QFormLayout();

    auto *combo_pred = new QComboBox(&dlg);
    auto *combo_succ = new QComboBox(&dlg);

    QList<ScheduleItem> items = s->get_sorted_by_wbs();
    for (const auto &it : items) {
        QString label = QStringLiteral("[%1] %2").arg(it.wbs_code, it.name);
        combo_pred->addItem(label, it.id);
        combo_succ->addItem(label, it.id);
    }
    if (combo_succ->count() > 1) {
        combo_succ->setCurrentIndex(1);
    }

    form->addRow(tr("Predecessora:"), combo_pred);
    form->addRow(tr("Sucessora:"), combo_succ);

    auto *combo_type = new QComboBox(&dlg);
    combo_type->addItem(tr("FS (Término para Início)"), QStringLiteral("FS"));
    combo_type->addItem(tr("SS (Início para Início)"), QStringLiteral("SS"));
    combo_type->addItem(tr("FF (Término para Término)"), QStringLiteral("FF"));
    combo_type->addItem(tr("SF (Início para Término)"), QStringLiteral("SF"));
    form->addRow(tr("Tipo de Vínculo:"), combo_type);

    auto *spin_lag = new QDoubleSpinBox(&dlg);
    spin_lag->setRange(-100.0, 100.0);
    spin_lag->setValue(0.0);
    spin_lag->setSuffix(tr(" dias"));
    form->addRow(tr("Latência (Lag/Lead):"), spin_lag);

    layout->addLayout(form);

    auto *btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    if (auto *btn_ok = btns->button(QDialogButtonBox::Ok)) btn_ok->setText(tr("Adicionar"));
    if (auto *btn_cancel = btns->button(QDialogButtonBox::Cancel)) btn_cancel->setText(tr("Cancelar"));
    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(btns);

    if (dlg.exec() == QDialog::Accepted) {
        QString pred_id = combo_pred->currentData().toString();
        QString succ_id = combo_succ->currentData().toString();
        DependencyType dep_type = string_to_dependency_type(combo_type->currentData().toString());
        double lag_hours = spin_lag->value() * 8.0;

        Dependency dep(pred_id, succ_id, dep_type, lag_hours);
        QString err;
        if (!s->add_dependency(dep, &err)) {
            QMessageBox::critical(this, tr("Erro na Dependência"), err);
            return;
        }

        planning_svc_.recalculate(*s);
        database_.save_schedule(*s);
        refresh_view();
    }
}

void GanttMainWindow::on_action_recalculate_cpm()
{
    Schedule *s = current_schedule();
    if (!s) return;

    CpmEngine::CpmResult res = planning_svc_.recalculate(*s);
    if (!res.success) {
        QMessageBox::critical(this, tr("Erro CPM"), res.error_message);
        return;
    }

    database_.save_schedule(*s);
    refresh_view();
    QMessageBox::information(this, tr("Cálculo CPM Concluído"),
                             tr("Caminho Crítico recalculado com sucesso!\n"
                                "Duração Total: %1 dias úteis\n"
                                "Atividades Críticas: %2")
                                 .arg(QString::number(res.total_duration_hours / 8.0, 'f', 1))
                                 .arg(res.critical_activities_count));
}

void GanttMainWindow::update_working_days_menu()
{
    Schedule *s = current_schedule();
    if (!s) return;

    for (int i = 0; i < 7; ++i) {
        if (act_days_[i]) {
            QSignalBlocker blocker(act_days_[i]);
            act_days_[i]->setChecked(s->calendar.working_days.contains(i + 1));
        }
    }
}

void GanttMainWindow::on_working_day_toggled(int day_num, bool checked)
{
    Schedule *s = current_schedule();
    if (!s) return;

    if (!checked) {
        if (s->calendar.working_days.size() <= 1 && s->calendar.working_days.contains(day_num)) {
            QMessageBox::warning(this, tr("Aviso"), tr("O cronograma deve conter ao menos um dia útil na semana."));
            update_working_days_menu();
            return;
        }
        s->calendar.working_days.remove(day_num);
    } else {
        s->calendar.working_days.insert(day_num);
    }

    database_.save_schedule(*s);
    CpmEngine::CpmResult res = planning_svc_.recalculate(*s);
    if (res.success) {
        database_.save_schedule(*s);
    }
    refresh_view();
}

void GanttMainWindow::on_action_configure_working_days()
{
    Schedule *s = current_schedule();
    if (!s) return;

    GanttWorkingDaysDialog dlg(s->calendar, this);
    if (dlg.exec() == QDialog::Accepted) {
        s->calendar.working_days = dlg.selected_working_days();
        s->calendar.hours_per_day = dlg.selected_hours_per_day();
        database_.save_schedule(*s);
        CpmEngine::CpmResult res = planning_svc_.recalculate(*s);
        if (res.success) {
            database_.save_schedule(*s);
        }
        refresh_view();
    }
}

void GanttMainWindow::on_action_manage_holidays()
{
    Schedule *s = current_schedule();
    if (!s) return;

    GanttHolidaysDialog dlg(s->calendar, this);
    if (dlg.exec() == QDialog::Accepted) {
        s->calendar.exceptions = dlg.updated_exceptions();
        database_.save_schedule(*s);
        CpmEngine::CpmResult res = planning_svc_.recalculate(*s);
        if (res.success) {
            database_.save_schedule(*s);
        }
        refresh_view();
    }
}

void GanttMainWindow::on_action_create_baseline()
{
    Schedule *s = current_schedule();
    if (!s) return;

    Baseline b = planning_svc_.create_baseline(*s, tr("Baseline de Obra - %1").arg(QDateTime::currentDateTime().toString(QStringLiteral("dd/MM/yyyy HH:mm"))));
    database_.save_schedule(*s);
    refresh_view();

    QMessageBox::information(this, tr("Baseline Criada"),
                             tr("Linha de base '%1' salva e congelada com sucesso.").arg(b.name));
}

void GanttMainWindow::on_action_compare_baseline()
{
    Schedule *s = current_schedule();
    if (!s) return;

    BaselineComparisonReport rep = planning_svc_.compare_baseline(*s);

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Relatório de Desvios — Baseline vs Realizado"));
    dlg.resize(680, 420);
    auto *layout = new QVBoxLayout(&dlg);

    auto *lbl_sum = new QLabel(
        tr("<b>Baseline Comparada:</b> %1<br>"
           "<b>Desvio Total do Projeto:</b> %2 dias<br>"
           "<b>Atividades Críticas Atrasadas:</b> %3")
            .arg(rep.baseline_name)
            .arg(QString::number(rep.total_project_delay_days, 'f', 1))
            .arg(rep.critical_delayed_count),
        &dlg
    );
    layout->addWidget(lbl_sum);

    auto *txt = new QTextEdit(&dlg);
    txt->setReadOnly(true);

    QString html = tr("<h3>Alertas de Atraso:</h3><ul>");
    if (rep.critical_alerts.isEmpty()) {
        html += tr("<li><font color='green'>Nenhum atraso crítico detectado no cronograma.</font></li>");
    } else {
        for (const auto &alert : rep.critical_alerts) {
            html += QStringLiteral("<li><font color='red'><b>%1</b></font></li>").arg(alert);
        }
    }
    html += QStringLiteral("</ul>");
    txt->setHtml(html);
    layout->addWidget(txt);

    auto *btn = new QPushButton(tr("Fechar"), &dlg);
    connect(btn, &QPushButton::clicked, &dlg, &QDialog::accept);
    layout->addWidget(btn);

    dlg.exec();
}

void GanttMainWindow::on_action_toggle_baseline(bool checked)
{
    timeline_->set_show_baseline(checked);
    timeline_->update();
}

void GanttMainWindow::update_zoom_highlight()
{
    if (!act_zoom_day_ || !act_zoom_week_ || !act_zoom_month_ || !timeline_) return;

    const GanttZoom zoom = timeline_->current_zoom();
    if (zoom == GanttZoom::Day) {
        act_zoom_day_->setChecked(true);
    } else if (zoom == GanttZoom::Week) {
        act_zoom_week_->setChecked(true);
    } else if (zoom == GanttZoom::Month) {
        act_zoom_month_->setChecked(true);
    }

    if (toolbar_) {
        QWidget *w_day = toolbar_->widgetForAction(act_zoom_day_);
        QWidget *w_week = toolbar_->widgetForAction(act_zoom_week_);
        QWidget *w_month = toolbar_->widgetForAction(act_zoom_month_);

        const QString active_style = QStringLiteral(
            "QToolButton {"
            "  background-color: rgba(46, 125, 50, 0.15);"
            "  color: #1b5e20;"
            "  border: 1px solid #4caf50;"
            "  border-radius: 3px;"
            "  padding: 2px 6px;"
            "  font-weight: normal;"
            "}"
            "QToolButton:hover {"
            "  background-color: rgba(46, 125, 50, 0.25);"
            "  border: 1px solid #2e7d32;"
            "  color: #1b5e20;"
            "}"
        );
        const QString inactive_style = QString();

        if (w_day) w_day->setStyleSheet(zoom == GanttZoom::Day ? active_style : inactive_style);
        if (w_week) w_week->setStyleSheet(zoom == GanttZoom::Week ? active_style : inactive_style);
        if (w_month) w_month->setStyleSheet(zoom == GanttZoom::Month ? active_style : inactive_style);
    }
}

void GanttMainWindow::on_action_zoom_day()
{
    timeline_->set_zoom(GanttZoom::Day);
    update_zoom_highlight();
}

void GanttMainWindow::on_action_zoom_week()
{
    timeline_->set_zoom(GanttZoom::Week);
    update_zoom_highlight();
}

void GanttMainWindow::on_action_zoom_month()
{
    timeline_->set_zoom(GanttZoom::Month);
    update_zoom_highlight();
}

void GanttMainWindow::on_action_export_csv()
{
    Schedule *s = current_schedule();
    if (!s) return;

    QString file_path = QFileDialog::getSaveFileName(this, tr("Exportar Cronograma para CSV"),
                                                     tr("cronograma_%1.csv").arg(s->name),
                                                     tr("Arquivos CSV (*.csv)"));
    if (file_path.isEmpty()) return;

    QString err;
    if (!csv_svc_.export_to_file(*s, file_path, &err)) {
        QMessageBox::critical(this, tr("Erro na Exportação"), err);
    } else {
        QMessageBox::information(this, tr("Exportação Concluída"), tr("Arquivo exportado com sucesso: %1").arg(file_path));
    }
}

void GanttMainWindow::on_action_import_csv()
{
    Schedule *s = current_schedule();
    if (!s) return;

    QString file_path = QFileDialog::getOpenFileName(this, tr("Importar Atividades de CSV"),
                                                     QString(), tr("Arquivos CSV (*.csv)"));
    if (file_path.isEmpty()) return;

    CsvMappingDialog map_dlg(QStringList(), this);
    if (map_dlg.exec() == QDialog::Accepted) {
        CsvImportResult res = csv_svc_.import_from_file(*s, file_path, map_dlg.mapping());
        if (!res.success) {
            QMessageBox::critical(this, tr("Erro na Importação"), res.error_messages.join(QStringLiteral("\n")));
        } else {
            planning_svc_.recalculate(*s);
            database_.save_schedule(*s);
            refresh_view();
            QMessageBox::information(this, tr("Importação Concluída"),
                                     tr("Importadas com sucesso:\n"
                                        "- %1 atividades\n"
                                        "- %2 dependências\n"
                                        "- %3 vínculos BIM")
                                         .arg(res.items_imported)
                                         .arg(res.dependencies_imported)
                                         .arg(res.bim_links_imported));
        }
    }
}

void GanttMainWindow::on_action_toggle_wbs_table(bool checked)
{
    if (wbs_tree_) {
        wbs_tree_->setVisible(checked);
    }
}

void GanttMainWindow::on_action_toggle_bim_panel(bool checked)
{
    bim_panel_->setVisible(checked);
}

void GanttMainWindow::focus_activity(const QString &item_id)
{
    if (wbs_tree_) {
        wbs_tree_->select_item_by_id(item_id);
    }
    on_activity_selected(item_id);
}

void GanttMainWindow::on_action_import_eisenhower_tasks()
{
    EisenhowerTaskImportDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted && dlg.imported_count() > 0) {
        load_or_create_default_schedule();
        refresh_view();
    }
}

void GanttMainWindow::on_request_link_task(const QString &item_id)
{
    Schedule *sched = current_schedule();
    if (!sched) return;
    ScheduleItem *item = sched->find_item(item_id);
    if (!item) return;

    EisenhowerTaskImportDialog dlg(this);
    dlg.setWindowTitle(tr("Vincular Atividade à Tarefa EisenPo"));
    if (dlg.exec() == QDialog::Accepted) {
        load_or_create_default_schedule();
        refresh_view();
    }
}

void GanttMainWindow::on_request_unlink_task(const QString &item_id)
{
    Schedule *sched = current_schedule();
    if (!sched) return;
    ScheduleItem *item = sched->find_item(item_id);
    if (!item || item->task_id.trimmed().isEmpty()) return;

    if (QMessageBox::question(this, tr("Desvincular"),
                              tr("Deseja desvincular a atividade '%1' da tarefa EisenPo?").arg(item->name),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        GanttTaskIntegrationBridge::instance().unlink_task(item->task_id);
        load_or_create_default_schedule();
        refresh_view();
    }
}

void GanttMainWindow::on_request_sync_task(const QString &item_id)
{
    Schedule *sched = current_schedule();
    if (!sched) return;
    ScheduleItem *item = sched->find_item(item_id);
    if (!item || item->task_id.trimmed().isEmpty()) return;

    QVariantMap task = GanttTaskIntegrationBridge::instance().get_task_by_id(item->task_id);
    if (task.isEmpty()) {
        QMessageBox::warning(this, tr("Sincronização"), tr("Tarefa vinculada não encontrada no armazenamento do EisenPo."));
        return;
    }

    const bool done = task.value(QStringLiteral("concluida")).toBool();
    GanttTaskIntegrationBridge::instance().on_eisenhower_task_completed(item->task_id, done);
    load_or_create_default_schedule();
    refresh_view();
    QMessageBox::information(this, tr("Sincronização"), tr("Avanço sincronizado com a tarefa EisenPo com sucesso!"));
}

void GanttMainWindow::on_eisenhower_task_duration_changed(const QString &task_id, double duration_days)
{
    if (task_id.trimmed().isEmpty()) {
        return;
    }
    if (duration_days <= 0.0) {
        duration_days = 1.0;
    }

    Schedule *s = current_schedule();
    if (!s) return;

    ScheduleItem *item = s->find_item_by_task_id(task_id);
    if (!item) return;

    item->duration_hours = duration_days * 8.0;
    item->is_milestone = (duration_days == 0.0);
    item->end_date = s->calendar.add_working_hours(item->start_date, item->duration_hours);

    planning_svc_.recalculate(*s);
    database_.save_schedule(*s);
    refresh_view();
}

void GanttMainWindow::on_eisenhower_task_removed(const QString &task_id)
{
    if (task_id.trimmed().isEmpty()) {
        return;
    }

    Schedule *s = current_schedule();
    if (!s) return;

    ScheduleItem *item = s->find_item_by_task_id(task_id);
    if (!item) return;

    const QString item_id = item->id;
    s->remove_item(item_id);

    planning_svc_.recalculate(*s);
    database_.save_schedule(*s);
    refresh_view();
}

void GanttMainWindow::on_action_sync_all_eisenhower_tasks()
{
    sync_all_eisenhower_tasks(false);
}

void GanttMainWindow::sync_all_eisenhower_tasks(bool replace_template_if_empty)
{
    const QVariantList tasks = GanttTaskIntegrationBridge::instance().get_available_eisenhower_tasks();
    Schedule *s = current_schedule();
    if (!s) return;

    if (tasks.isEmpty()) {
        QStringList to_remove;
        for (const auto &it : s->items) {
            if (!it.task_id.isEmpty()) {
                to_remove.append(it.id);
            }
        }
        if (!to_remove.isEmpty()) {
            for (const QString &id : to_remove) {
                s->remove_item(id);
            }
            planning_svc_.recalculate(*s);
            database_.save_schedule(*s);
            refresh_view();
            if (statusBar()) {
                statusBar()->showMessage(tr("✅ Tarefas do EisenPo removidas do cronograma."), 5000);
            }
        }
        return;
    }

    // Purge unwanted test entries from current schedule
    bool items_purged = false;
    auto item_it = s->items.begin();
    while (item_it != s->items.end()) {
        const QString lower_name = item_it->name.toLower();
        const QString task_id = item_it->task_id;
        if ((lower_name.contains(QStringLiteral("arma")) && lower_name.contains(QStringLiteral("pilar"))) ||
            (lower_name.contains(QStringLiteral("instal")) && lower_name.contains(QStringLiteral("pavimento"))) ||
            lower_name.contains(QStringLiteral("tarefa concluída teste")) ||
            lower_name.contains(QStringLiteral("tarefa concluida teste")) ||
            lower_name.contains(QStringLiteral("tarefa ciclos teste")) ||
            lower_name.contains(QStringLiteral("tarefa atrasada teste")) ||
            task_id.startsWith(QStringLiteral("TASK-CYCLE-"))) {
            item_it = s->items.erase(item_it);
            items_purged = true;
        } else {
            ++item_it;
        }
    }
    if (items_purged) {
        database_.save_schedule(*s);
    }

    // Check if the current schedule was just the generic dummy template with no user tasks
    bool is_just_dummy_template = false;
    if (replace_template_if_empty && (s->name.contains(QStringLiteral("Residencial Horizonte")) || s->description.contains(QStringLiteral("Residencial")))) {
        int linked_count = 0;
        for (const auto &it : s->items) {
            if (!it.task_id.isEmpty()) linked_count++;
        }
        if (linked_count <= 1) { // 0 or the 1 initial test item
            is_just_dummy_template = true;
        }
    }

    if (is_just_dummy_template) {
        s->name = QStringLiteral("Cronograma Operacional - EisenPo");
        s->description = QStringLiteral("Cronograma sincronizado diretamente com as tarefas da Matriz de Eisenhower");
        s->items.clear();
        s->dependencies.clear();
        s->bim_links.clear();
    }

    int added_count = 0;
    int updated_count = 0;
    int removed_count = 0;

    // First pass: collect active task IDs and calculate total pending tasks
    QSet<QString> active_task_ids;
    int total_pending_tasks = 0;
    for (const auto &var : tasks) {
        const QVariantMap task = var.toMap();
        const QString task_id = task.value(QStringLiteral("id")).toString().trimmed();
        if (task_id.isEmpty()) continue;

        QString title = task.value(QStringLiteral("text")).toString().trimmed();
        if (title.isEmpty()) title = task.value(QStringLiteral("titulo")).toString().trimmed();
        if (title.isEmpty()) title = task.value(QStringLiteral("title")).toString().trimmed();
        if (title.isEmpty()) continue;

        const QString lower_title = title.toLower();
        if ((lower_title.contains(QStringLiteral("arma")) && lower_title.contains(QStringLiteral("pilar"))) ||
            (lower_title.contains(QStringLiteral("instal")) && lower_title.contains(QStringLiteral("pavimento"))) ||
            lower_title.contains(QStringLiteral("tarefa concluída teste")) ||
            lower_title.contains(QStringLiteral("tarefa concluida teste")) ||
            lower_title.contains(QStringLiteral("tarefa ciclos teste")) ||
            lower_title.contains(QStringLiteral("tarefa atrasada teste")) ||
            task_id.startsWith(QStringLiteral("TASK-CYCLE-"))) {
            continue;
        }

        active_task_ids.insert(task_id);

        bool done = false;
        if (task.contains(QStringLiteral("concluida"))) done = task.value(QStringLiteral("concluida")).toBool();
        else if (task.contains(QStringLiteral("completed"))) done = task.value(QStringLiteral("completed")).toBool();
        else {
            const QString st = task.value(QStringLiteral("status")).toString().toLower();
            done = (st == QStringLiteral("done") || st == QStringLiteral("concluida") || st == QStringLiteral("completed") || st == QStringLiteral("executado"));
        }

        const QString status_str = task.value(QStringLiteral("status")).toString().toLower();
        const QString category_str = task.value(QStringLiteral("category")).toString().toLower();
        const int pomodoros_completados = task.value(QStringLiteral("pomodoros_completados"), task.value(QStringLiteral("pomodoros"), 0)).toInt();
        const int ciclos_completados = task.value(QStringLiteral("ciclos_completados"), task.value(QStringLiteral("ciclos_feitos"), pomodoros_completados)).toInt();

        bool in_progress = false;
        if (!done) {
            if (status_str.contains(QStringLiteral("progress")) ||
                status_str.contains(QStringLiteral("andamento")) ||
                status_str == QStringLiteral("doing") ||
                category_str.contains(QStringLiteral("progresso")) ||
                category_str.contains(QStringLiteral("andamento")) ||
                category_str.contains(QStringLiteral("doing")) ||
                ciclos_completados > 0 ||
                task.value(QStringLiteral("em_progresso")).toBool()) {
                in_progress = true;
            }
        }

        if (!done && !in_progress) {
            total_pending_tasks++;
        }
    }

    // Prune activities previously synced from EisenPo whose tasks were removed in EisenPo
    QStringList items_to_remove;
    for (const auto &it : s->items) {
        if (!it.task_id.isEmpty() && !active_task_ids.contains(it.task_id)) {
            items_to_remove.append(it.id);
        }
    }
    for (const QString &item_id : items_to_remove) {
        s->remove_item(item_id);
        removed_count++;
    }

    QHash<int, int> quadrant_counters;

    // Initialize quadrant counters from remaining items
    for (const auto &it : s->items) {
        if (it.wbs_code.contains(QLatin1Char('.'))) {
            const QStringList parts = it.wbs_code.split(QLatin1Char('.'));
            bool ok = false;
            int q = parts.value(0).toInt(&ok);
            int idx = parts.value(1).toInt();
            if (ok && q >= 1 && q <= 4) {
                if (idx > quadrant_counters[q]) quadrant_counters[q] = idx;
            }
        }
    }

    for (const auto &var : tasks) {
        const QVariantMap task = var.toMap();
        const QString task_id = task.value(QStringLiteral("id")).toString().trimmed();
        if (task_id.isEmpty()) continue;

        QString title = task.value(QStringLiteral("text")).toString().trimmed();
        if (title.isEmpty()) title = task.value(QStringLiteral("titulo")).toString().trimmed();
        if (title.isEmpty()) title = task.value(QStringLiteral("title")).toString().trimmed();
        if (title.isEmpty()) continue;

        const QString lower_title = title.toLower();
        if ((lower_title.contains(QStringLiteral("arma")) && lower_title.contains(QStringLiteral("pilar"))) ||
            (lower_title.contains(QStringLiteral("instal")) && lower_title.contains(QStringLiteral("pavimento"))) ||
            lower_title.contains(QStringLiteral("tarefa concluída teste")) ||
            lower_title.contains(QStringLiteral("tarefa concluida teste")) ||
            lower_title.contains(QStringLiteral("tarefa ciclos teste")) ||
            lower_title.contains(QStringLiteral("tarefa atrasada teste")) ||
            task_id.startsWith(QStringLiteral("TASK-CYCLE-"))) {
            continue;
        }

        int quadrant = 0;
        if (task.contains(QStringLiteral("priority")) && task.value(QStringLiteral("priority")).toInt() > 0) {
            quadrant = task.value(QStringLiteral("priority")).toInt();
        } else if (task.contains(QStringLiteral("prioridade")) && task.value(QStringLiteral("prioridade")).toInt() > 0) {
            quadrant = task.value(QStringLiteral("prioridade")).toInt();
        } else if (task.contains(QStringLiteral("quadrante")) && task.value(QStringLiteral("quadrante")).toInt() > 0) {
            quadrant = task.value(QStringLiteral("quadrante")).toInt();
        }
        if (quadrant < 1 || quadrant > 4) quadrant = 1;

        // Parse date/time
        QString date_str = task.value(QStringLiteral("date")).toString().trimmed();
        if (date_str.isEmpty()) date_str = task.value(QStringLiteral("data")).toString().trimmed();

        QString time_str = task.value(QStringLiteral("time")).toString().trimmed();
        if (time_str.isEmpty()) time_str = task.value(QStringLiteral("hora")).toString().trimmed();
        if (time_str.isEmpty()) time_str = task.value(QStringLiteral("horario")).toString().trimmed();

        QDate start_d = QDate::fromString(date_str, Qt::ISODate);
        if (!start_d.isValid()) start_d = QDate::fromString(date_str, QStringLiteral("dd/MM/yyyy"));
        if (!start_d.isValid()) start_d = QDate::fromString(date_str, QStringLiteral("yyyy-MM-dd"));
        if (!start_d.isValid()) start_d = QDate::currentDate();

        QTime start_t = QTime::fromString(time_str, QStringLiteral("HH:mm"));
        if (!start_t.isValid()) start_t = QTime::fromString(time_str, QStringLiteral("H:mm"));
        if (!start_t.isValid()) start_t = QTime::fromString(time_str, QStringLiteral("HH:mm:ss"));
        if (!start_t.isValid()) start_t = QTime(8, 0);

        const QDateTime start_dt(start_d, start_t);
        ScheduleItem *existing = s->find_item_by_task_id(task_id);

        double duration = 8.0;
        if (task.contains(QStringLiteral("duracao")) && task.value(QStringLiteral("duracao")).toDouble() > 0.0) {
            duration = task.value(QStringLiteral("duracao")).toDouble() * 8.0;
        } else if (task.contains(QStringLiteral("duration")) && task.value(QStringLiteral("duration")).toDouble() > 0.0) {
            duration = task.value(QStringLiteral("duration")).toDouble() * 8.0;
        } else if (existing && existing->duration_hours > 0.0) {
            duration = existing->duration_hours;
        }

        const QDateTime end_dt = s->calendar.add_working_hours(start_dt, duration);

        const QDateTime now = QDateTime::currentDateTime();

        bool done = false;
        if (task.contains(QStringLiteral("concluida"))) done = task.value(QStringLiteral("concluida")).toBool();
        else if (task.contains(QStringLiteral("completed"))) done = task.value(QStringLiteral("completed")).toBool();
        else {
            const QString st = task.value(QStringLiteral("status")).toString().toLower();
            done = (st == QStringLiteral("done") || st == QStringLiteral("concluida") || st == QStringLiteral("completed") || st == QStringLiteral("executado"));
        }

        const QString status_str = task.value(QStringLiteral("status")).toString().toLower();
        const QString category_str = task.value(QStringLiteral("category")).toString().toLower();
        const int pomodoros_completados = task.value(QStringLiteral("pomodoros_completados"), task.value(QStringLiteral("pomodoros"), 0)).toInt();
        const int ciclos_completados = task.value(QStringLiteral("ciclos_completados"), task.value(QStringLiteral("ciclos_feitos"), pomodoros_completados)).toInt();

        bool in_progress = false;
        if (!done) {
            if (status_str.contains(QStringLiteral("progress")) ||
                status_str.contains(QStringLiteral("andamento")) ||
                status_str == QStringLiteral("doing") ||
                category_str.contains(QStringLiteral("progresso")) ||
                category_str.contains(QStringLiteral("andamento")) ||
                category_str.contains(QStringLiteral("doing")) ||
                ciclos_completados > 0 ||
                task.value(QStringLiteral("em_progresso")).toBool()) {
                in_progress = true;
            }
        }

        int ciclos_total = task.value(QStringLiteral("ciclos_totais"),
                           task.value(QStringLiteral("ciclos_permitidos"),
                           task.value(QStringLiteral("total_ciclos"),
                           task.value(QStringLiteral("ciclos_alvo"), 0)))).toInt();
        if (ciclos_total <= 0) {
            ciclos_total = task.value(QStringLiteral("ciclos"), 0).toInt();
        }
        if (ciclos_total <= 0) {
            ciclos_total = 4; // Padrão de 4 ciclos
        }

        double percent = 0.0;
        ActivityStatus act_status = ActivityStatus::NotStarted;
        const bool is_past_due = (!done && start_d.isValid() && (start_d < QDate::currentDate() || (start_d == QDate::currentDate() && end_dt < now)));

        if (done) {
            percent = 100.0;
            act_status = ActivityStatus::Done;
        } else {
            percent = source::utils::calcular_percentual_conclusao(ciclos_completados, ciclos_total);
            if (percent >= 100.0) {
                act_status = ActivityStatus::Done;
            } else if (percent > 0.0 || in_progress) {
                act_status = is_past_due ? ActivityStatus::Blocked : ActivityStatus::InProgress;
            } else {
                act_status = is_past_due ? ActivityStatus::Blocked : ActivityStatus::NotStarted;
            }
        }

        if (existing) {
            if (!existing->wbs_code.startsWith(QString::number(quadrant) + QLatin1Char('.'))) {
                quadrant_counters[quadrant]++;
                existing->wbs_code = QStringLiteral("%1.%2").arg(quadrant).arg(quadrant_counters[quadrant]);
            }
            existing->name = title;
            existing->description = task.value(QStringLiteral("description"), task.value(QStringLiteral("descricao"))).toString();
            existing->start_date = start_dt;
            existing->duration_hours = duration;
            existing->end_date = end_dt;
            existing->percent_complete = percent;
            existing->status = act_status;
            updated_count++;
        } else {
            quadrant_counters[quadrant]++;
            ScheduleItem item(title);
            item.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
            item.schedule_id = s->id;
            item.task_id = task_id;
            item.wbs_code = QStringLiteral("%1.%2").arg(quadrant).arg(quadrant_counters[quadrant]);
            item.start_date = start_dt;
            item.duration_hours = duration;
            item.end_date = end_dt;
            item.baseline_start_date = start_dt;
            item.baseline_end_date = end_dt;
            item.baseline_duration_hours = duration;
            item.description = task.value(QStringLiteral("description"), task.value(QStringLiteral("descricao"))).toString();
            item.percent_complete = percent;
            item.status = act_status;
            s->add_item(item);
            added_count++;
        }
    }

    if (added_count > 0 || updated_count > 0 || removed_count > 0) {
        planning_svc_.recalculate(*s);
        if (s->baselines.isEmpty()) {
            planning_svc_.create_baseline(*s, QStringLiteral("Baseline Inicial"), QStringLiteral("Sistema"));
        } else if (!s->active_baseline_id.isEmpty() && s->baselines.contains(s->active_baseline_id)) {
            for (auto &it : s->items) {
                if (!it.baseline_start_date.isValid()) {
                    it.baseline_start_date = it.start_date;
                }
                if (!it.baseline_end_date.isValid()) {
                    it.baseline_end_date = it.end_date;
                }
                if (it.baseline_duration_hours <= 0.0) {
                    it.baseline_duration_hours = it.duration_hours;
                }
                if (!s->baselines[s->active_baseline_id].items.contains(it.id)) {
                    BaselineItemSnapshot b_item;
                    b_item.item_id = it.id;
                    b_item.start_date = it.baseline_start_date;
                    b_item.end_date = it.baseline_end_date;
                    b_item.duration_hours = it.baseline_duration_hours;
                    s->baselines[s->active_baseline_id].items.insert(it.id, b_item);
                }
            }
        }
        database_.save_schedule(*s);
        refresh_view();
        if (statusBar()) {
            statusBar()->showMessage(tr("✅ Cronograma sincronizado com EisenPo (%1 adicionada(s), %2 atualizada(s), %3 removida(s)).")
                                         .arg(added_count).arg(updated_count).arg(removed_count), 5000);
        }
    }
}

void GanttMainWindow::_retranslate_actions()
{
    if (menu_arquivo_) {
        menu_arquivo_->setTitle(tr("Arquivo"));
    }
    if (menu_configuracoes_) {
        menu_configuracoes_->setTitle(tr("Configurações"));
    }

    if (act_toggle_wbs_) {
        act_toggle_wbs_->setText(tr("📋 Tabela WBS"));
        act_toggle_wbs_->setToolTip(tr("Exibir/ocultar tabela de atividades WBS"));
    }
    if (act_new_) {
        act_new_->setText(tr("➕ Nova Atividade"));
        act_new_->setToolTip(tr("Criar nova atividade no cronograma"));
    }
    if (act_edit_) {
        act_edit_->setText(tr("✏️ Editar"));
        act_edit_->setToolTip(tr("Editar atividade selecionada"));
    }
    if (act_del_) {
        act_del_->setText(tr("❌ Excluir"));
        act_del_->setToolTip(tr("Excluir atividade selecionada"));
    }
    if (act_dep_) {
        act_dep_->setText(tr("🔗 Vínculo/Predecessora"));
        act_dep_->setToolTip(tr("Criar dependência entre atividades (FS, SS, FF, SF)"));
    }
    if (act_cpm_) {
        act_cpm_->setText(tr("⚡ Recalcular CPM"));
        act_cpm_->setToolTip(tr("Recalcular Caminho Crítico e reagendar datas automaticamente"));
    }
    if (menu_dias_uteis_) {
        menu_dias_uteis_->setTitle(tr("📅 Dias Úteis"));
    }
    if (act_config_dias_uteis_) {
        act_config_dias_uteis_->setText(tr("⚙️ Configurar Dias Úteis..."));
        act_config_dias_uteis_->setToolTip(tr("Definir quais dias da semana são considerados úteis e carga horária diária"));
    }
    const QString day_labels[7] = {
        tr("Segunda-feira"),
        tr("Terça-feira"),
        tr("Quarta-feira"),
        tr("Quinta-feira"),
        tr("Sexta-feira"),
        tr("Sábado"),
        tr("Domingo")
    };
    for (int i = 0; i < 7; ++i) {
        if (act_days_[i]) {
            act_days_[i]->setText(day_labels[i]);
        }
    }
    if (act_feriados_) {
        act_feriados_->setText(tr("🏖️ Feriados..."));
        act_feriados_->setToolTip(tr("Gerenciar feriados e dias não úteis no calendário do cronograma"));
    }
    if (act_base_) {
        act_base_->setText(tr("📌 Salvar Baseline"));
        act_base_->setToolTip(tr("Congelar linha de base atual do cronograma"));
    }
    if (act_comp_) {
        act_comp_->setText(tr("📊 Comparar Baseline"));
        act_comp_->setToolTip(tr("Exibir relatório de desvios e alertas de atraso crítico"));
    }
    if (act_toggle_base_) {
        act_toggle_base_->setText(tr("👁️ Linha Fantasma"));
        act_toggle_base_->setToolTip(tr("Exibir/ocultar barras fantasmas de baseline"));
    }
    if (act_zoom_day_)     act_zoom_day_->setText(tr("🔍 Zoom Dia"));
    if (act_zoom_week_)    act_zoom_week_->setText(tr("🔍 Zoom Semana"));
    if (act_zoom_month_)   act_zoom_month_->setText(tr("🔍 Zoom Mês"));
    update_zoom_highlight();
    if (act_bim_) {
        act_bim_->setText(tr("🏗️ Simulação 4D"));
        act_bim_->setToolTip(tr("Exibir/ocultar painel de simulação BIM 4D"));
    }
    if (act_import_csv_) {
        act_import_csv_->setText(tr("📥 Importar CSV"));
        act_import_csv_->setToolTip(tr("Importar cronograma de arquivo CSV"));
    }
    if (act_export_csv_) {
        act_export_csv_->setText(tr("📤 Exportar CSV"));
        act_export_csv_->setToolTip(tr("Exportar cronograma atual para formato CSV"));
    }
    if (act_sync_all_) {
        act_sync_all_->setText(tr("🔄 Sincronizar com EisenPo"));
        act_sync_all_->setToolTip(tr("Sincronizar tarefas da Matriz de Eisenhower no cronograma"));
    }
    if (act_import_eisen_) {
        act_import_eisen_->setText(tr("📋 Importar do EisenPo"));
        act_import_eisen_->setToolTip(tr("Importar tarefas do EisenPo selecionadas para o cronograma"));
    }
    if (menu_layout_) {
        menu_layout_->setTitle(tr("📐 Layout"));
    }
    if (menu_tabelas_) {
        menu_tabelas_->setTitle(tr("📊 Tabelas"));
    }
    if (act_layout_original_) {
        act_layout_original_->setText(tr("Original"));
        act_layout_original_->setToolTip(tr("Disposição padrão das tabelas e do diagrama de Gantt"));
    }
    if (act_layout_vertical_topo_) {
        act_layout_vertical_topo_->setText(tr("Vertical Topo"));
        act_layout_vertical_topo_->setToolTip(tr("Tabelas WBS e Simulação 4D no topo lado a lado, Diagrama de Gantt abaixo"));
    }
    if (act_layout_vertical_inferior_) {
        act_layout_vertical_inferior_->setText(tr("Vertical Inferior"));
        act_layout_vertical_inferior_->setToolTip(tr("Diagrama de Gantt no topo, Tabelas WBS e Simulação 4D abaixo lado a lado"));
    }
    if (act_layout_horizontal_esquerda_) {
        act_layout_horizontal_esquerda_->setText(tr("Horizontal Esquerda"));
        act_layout_horizontal_esquerda_->setToolTip(tr("Tabelas WBS e Simulação 4D empilhadas à esquerda, Diagrama de Gantt à direita"));
    }
    if (act_layout_horizontal_direita_) {
        act_layout_horizontal_direita_->setText(tr("Horizontal Direita"));
        act_layout_horizontal_direita_->setToolTip(tr("Diagrama de Gantt à esquerda, Tabelas WBS e Simulação 4D empilhadas à direita"));
    }
    update_layout_actions_checked();
}

void GanttMainWindow::retranslate_ui()
{
    if (is_retranslating_) {
        return;
    }
    is_retranslating_ = true;

    setWindowTitle(tr("EisenPo — Gestão de Cronograma, Gantt & BIM 4D"));

    if (toolbar_) {
        toolbar_->setWindowTitle(tr("Barra de Ferramentas"));
    }

    _retranslate_actions();

    // Se a janela não está visível na tela, economiza ciclos de CPU ignorando
    // a reconstrução pesada de itens e árvores até o momento em que for exibida.
    if (!isVisible()) {
        needs_full_retranslate_ = true;
        is_retranslating_ = false;
        return;
    }

    setUpdatesEnabled(false);

    if (wbs_tree_) wbs_tree_->retranslate_ui();
    if (bim_panel_) bim_panel_->retranslate_ui();
    if (timeline_) timeline_->retranslate_ui();

    update_status_bar();

    setUpdatesEnabled(true);
    needs_full_retranslate_ = false;
    is_retranslating_ = false;
}

void GanttMainWindow::changeEvent(QEvent *event)
{
    if (event && event->type() == QEvent::LanguageChange) {
        retranslate_ui();
    }
    QMainWindow::changeEvent(event);
}

void GanttMainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    if (is_first_show_) {
        is_first_show_ = false;
        apply_table_layout(current_layout_mode_);
    }
    if (needs_full_retranslate_) {
        retranslate_ui();
    }
}

} // namespace source::gantt
