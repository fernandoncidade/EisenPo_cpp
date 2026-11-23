#include "source/app/app_10_atualizar_textos.hpp"
#include <QCoreApplication>
#include "source/services/incore_01_initUI.hpp"
#include "source/services/RecursosProdutividade/recpro_07_filtros.hpp"

namespace source {
using namespace app;

void EisenhowerMatrixApp::atualizar_textos()
{
    setUpdatesEnabled(false);
    source::InterfaceCore::incore_01_initUI::apply_locale_to_date_input(this);

    // 1. Sincronizar host bridges e retraduzir plugins primeiro
    // Isso garante que os nomes e textos de menu (ex.: gantt_open_menu_text(), agenda_open_menu_text())
    // já estejam no novo idioma antes de a barra de menus ser recriada.
    _sync_feynman_host_bridge();
    if (feynman_plugin_ != nullptr) {
        feynman_plugin_->retranslate_ui();
    }
    _sync_pomodoro_host_bridge();
    if (pomodoro_plugin_ != nullptr) {
        pomodoro_plugin_->retranslate_ui();
    }
    _sync_agenda_host_bridge();
    if (agenda_plugin_ != nullptr) {
        agenda_plugin_->retranslate_ui();
    }
    _sync_gantt_host_bridge();
    if (gantt_plugin_ != nullptr) {
        gantt_plugin_->retranslate_ui();
    }

    // 2. Atualizar textos principais da GUI, menus e filtros ativos
    source::InterfaceCore::atualizar_textos(this);
    source::InterfaceCore::ProductivityFeatures::apply_active_filters(this);
    _attach_module_task_counter_to_menu_bar();

    // 3. Atualizar painel de calendário
    if (calendar_pane != nullptr) {
        calendar_pane->on_language_changed();
    }

    setUpdatesEnabled(true);
}

void EisenhowerMatrixApp::atualizar_placeholders()
{
    source::InterfaceCore::atualizar_placeholders(this);
}

void EisenhowerMatrixApp::exibir_sobre()
{
    source::InterfaceCore::exibir_sobre(this);
}

void EisenhowerMatrixApp::show_context_menu(const QPoint &point, QListWidget *list_widget)
{
    source::InterfaceCore::show_context_menu(this, point, list_widget);
}

void EisenhowerMatrixApp::open_calendar()
{
    if (poca_layout_active_) {
        return;
    }
    if (calendar_pane != nullptr) {
        calendar_pane->toggle_panel(true);
        _sync_splitter_for_calendar();
    }
}

void EisenhowerMatrixApp::_sync_splitter_for_calendar()
{
    if (poca_layout_active_) {
        return;
    }
    if (main_splitter == nullptr || calendar_pane == nullptr) {
        return;
    }

    const QList<int> sizes = main_splitter->sizes();
    if (sizes.size() < 2) {
        return;
    }

    const int calendar_w = qMax(qMax(25, calendar_pane->minimumWidth()),
                                qMax(calendar_pane->width(), calendar_pane->sizeHint().width()));
    const int right_min_w = (_main_content != nullptr) ? _main_content->minimumWidth() : 486;
    const int total_w = (sizes.at(0) + sizes.at(1) > 0) ? sizes.at(0) + sizes.at(1) : qMax(width(), calendar_w + right_min_w);
    const int quadrant_w = qMax(right_min_w, total_w - calendar_w);
    main_splitter->setSizes({calendar_w, quadrant_w});
}

void EisenhowerMatrixApp::nova_sessao()
{
    arquivo->novo();
}

void EisenhowerMatrixApp::abrir_arquivo()
{
    arquivo->abrir_arquivo();
}

void EisenhowerMatrixApp::salvar()
{
    arquivo->salvar();
}

void EisenhowerMatrixApp::salvar_como()
{
    arquivo->salvar_como();
}

void EisenhowerMatrixApp::limpar_tudo()
{
    arquivo->limpar_tudo();
}

void EisenhowerMatrixApp::sair_app()
{
    arquivo->sair();
}

void EisenhowerMatrixApp::edit_task_datetime(QListWidgetItem *item, QListWidget *list_widget)
{
    source::GerenciamentoUI::edit_task_datetime(this, item, list_widget);
}

void EisenhowerMatrixApp::move_task_to_quadrant(QListWidgetItem *item,
                                                QListWidget *source_list,
                                                QListWidget *target_list)
{
    source::GerenciamentoUI::move_task_to_quadrant(this, item, source_list, target_list);
}

QListWidget *EisenhowerMatrixApp::quadrant_list(int quadrant, bool completed) const
{
    switch (quadrant) {
    case 1:
        return completed ? quadrant1_completed_list : quadrant1_list;
    case 2:
        return completed ? quadrant2_completed_list : quadrant2_list;
    case 3:
        return completed ? quadrant3_completed_list : quadrant3_list;
    case 4:
        return completed ? quadrant4_completed_list : quadrant4_list;
    default:
        return nullptr;
    }
}

bool EisenhowerMatrixApp::is_completed_list(const QListWidget *list) const
{
    return list == quadrant1_completed_list || list == quadrant2_completed_list || list == quadrant3_completed_list
           || list == quadrant4_completed_list;
}

bool EisenhowerMatrixApp::is_in_progress_list(const QListWidget *list) const
{
    return list == quadrant1_in_progress_list || list == quadrant2_in_progress_list || list == quadrant3_in_progress_list
           || list == quadrant4_in_progress_list;
}

QListWidget *EisenhowerMatrixApp::quadrant_in_progress_list(int quadrant) const
{
    switch (quadrant) {
    case 1:
        return quadrant1_in_progress_list;
    case 2:
        return quadrant2_in_progress_list;
    case 3:
        return quadrant3_in_progress_list;
    case 4:
        return quadrant4_in_progress_list;
    default:
        return nullptr;
    }
}

}  // namespace source
