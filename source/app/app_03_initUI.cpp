#include "source/app/app_03_initUI.hpp"
#include <QScrollArea>

namespace source {
using namespace app;

void EisenhowerMatrixApp::criar_menu_configuracoes()
{
    source::InterfaceCore::criar_menu_configuracoes(this);
    _attach_module_task_counter_to_menu_bar();
}

void EisenhowerMatrixApp::definir_idioma(const QString &codigo_idioma)
{
    source::InterfaceCore::definir_idioma(this, codigo_idioma);
}

void EisenhowerMatrixApp::initUI()
{
    source::InterfaceCore::init_ui(this);

    QWidget *old_central = centralWidget();
    auto *container = new QWidget(this);
    auto *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    main_splitter = new QSplitter(Qt::Horizontal, container);
    main_splitter->setChildrenCollapsible(false);
    main_splitter->setHandleWidth((eisenhower_pomodoro_expanded) ? 6 : 10);

    calendar_pane = new source::GerenciamentoUI::Calendar(this, container);
    if (old_central != nullptr) {
        if (old_central->layout() != nullptr) {
            old_central->layout()->setContentsMargins(0, 4, 4, 4);
        }
        const int left_min_w = (eisenhower_pomodoro_expanded)
                                   ? (eisenhower_left_scroll != nullptr ? eisenhower_left_scroll->minimumWidth()
                                                                        : (eisenhower_left_widget != nullptr ? qMax(360, eisenhower_left_widget->minimumWidth()) : 360))
                                   : 0;
        const int min_quadrants_w = (quadrants_scroll_area != nullptr) ? quadrants_scroll_area->minimumWidth() : 120;
        // 4 = main_layout right(4) margin inside old_central (left margin is 0 against main_splitter)
        const int total_min = left_min_w + min_quadrants_w + (left_min_w > 0 ? 6 : 0) + 4;
        old_central->setMinimumWidth(total_min);
        relax_module_page(old_central);
        old_central->setMinimumWidth(total_min);
    }

    main_splitter->addWidget(calendar_pane);
    main_splitter->addWidget(old_central);
    main_splitter->setCollapsible(0, false);
    main_splitter->setCollapsible(1, false);
    main_splitter->setStretchFactor(0, 0);
    main_splitter->setStretchFactor(1, 1);
    connect(main_splitter, &QSplitter::splitterMoved, this, [this, old_central](int, int) {
        if (poca_layout_active_) {
            if (calendar_pane != nullptr && calendar_pane->isVisible()) {
                calendar_pane->hide();
            }
            if (main_splitter != nullptr) {
                main_splitter->setHandleWidth(0);
            }
            return;
        }
        if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
            calendar_pane->calendar_panel->update_calendars_for_width();
        }
        if (old_central != nullptr && main_splitter != nullptr) {
            const int left_min_w = (eisenhower_pomodoro_expanded)
                                       ? (eisenhower_left_scroll != nullptr ? eisenhower_left_scroll->minimumWidth()
                                                                            : (eisenhower_left_widget != nullptr ? qMax(360, eisenhower_left_widget->minimumWidth()) : 360))
                                       : 0;
            const int min_quadrants_w = (quadrants_scroll_area != nullptr) ? quadrants_scroll_area->minimumWidth() : 120;
            // 4 = main_layout right(4) margin inside old_central (left margin is 0 against main_splitter)
            const int total_min = left_min_w + min_quadrants_w + (left_min_w > 0 ? 6 : 0) + 4;
            const int min_right = qMax(total_min, old_central->minimumWidth());
            const int min_left = calendar_pane != nullptr ? calendar_pane->effective_min_width() : 25;
            const QList<int> sizes = main_splitter->sizes();
            if (sizes.size() >= 2) {
                if (sizes[1] < min_right) {
                    const int diff = min_right - sizes[1];
                    main_splitter->setSizes({qMax(min_left, sizes[0] - diff), min_right});
                    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
                        calendar_pane->calendar_panel->update_calendars_for_width();
                    }
                } else if (sizes[0] < min_left) {
                    const int diff = min_left - sizes[0];
                    main_splitter->setSizes({min_left, qMax(min_right, sizes[1] - diff)});
                    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
                        calendar_pane->calendar_panel->update_calendars_for_width();
                    }
                }
            }
            if (eisenhower_content_splitter != nullptr && eisenhower_pomodoro_expanded) {
                const QList<int> inner_sizes = eisenhower_content_splitter->sizes();
                if (inner_sizes.size() >= 2) {
                    const int current_right_w = (sizes.size() >= 2) ? qMax(min_right, sizes[1]) : old_central->width();
                    const int avail = current_right_w - 8 - 6;
                    int adj_left = inner_sizes[0];
                    int adj_right = inner_sizes[1];
                    if (adj_left < left_min_w) {
                        adj_left = left_min_w;
                        adj_right = qMax(min_quadrants_w, avail - adj_left);
                    } else if (adj_right < min_quadrants_w) {
                        adj_right = min_quadrants_w;
                        adj_left = qMax(left_min_w, avail - adj_right);
                    }
                    if (adj_left != inner_sizes[0] || adj_right != inner_sizes[1]) {
                        eisenhower_content_splitter->setSizes({adj_left, adj_right});
                    }
                }
            }
        }
    });
    layout->addWidget(main_splitter, 1);

    const int configured_panel_w = qMax(220, calendar_min_width);
    calendar_pane->calendar_panel->set_calendar_content_width(configured_panel_w);
    calendar_pane->setMinimumWidth(calendar_pane->effective_min_width());

    const int calendar_w = poca_layout_active_ ? 0 : qMax(25, calendar_pane->effective_min_width());
    const int right_min_w = (old_central != nullptr) ? qMax(300, old_central->minimumWidth()) : 300;
    const int total_w = qMax(width(), calendar_w + right_min_w);
    const int quadrant_w = qMax(right_min_w, total_w - calendar_w);
    if (poca_layout_active_) {
        main_splitter->setCollapsible(0, true);
        main_splitter->setHandleWidth(0);
        if (QSplitterHandle *handle = main_splitter->handle(1)) {
            handle->setEnabled(false);
        }
        main_splitter->setSizes({0, total_w});
        if (calendar_pane != nullptr) {
            calendar_pane->setMinimumWidth(0);
            calendar_pane->hide();
        }
    } else {
        main_splitter->setSizes({calendar_w, quadrant_w});
    }

    _main_content = old_central;
    eisenhower_module_page = container;
    
    module_task_counter_container = new QWidget(this);
    module_task_counter_container->setMinimumWidth(0);
    auto *counter_layout = new QHBoxLayout(module_task_counter_container);
    counter_layout->setContentsMargins(16, 0, 12, 0);
    counter_layout->setSpacing(0);
    counter_layout->addStretch(1);
    module_task_counter_label = new QLabel(module_task_counter_container);
    module_task_counter_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    module_task_counter_label->setMinimumWidth(0);
    counter_layout->addWidget(module_task_counter_label, 0, Qt::AlignRight | Qt::AlignVCenter);
    _attach_module_task_counter_to_menu_bar();

    for (QListWidget *list : {quadrant1_list,
                              quadrant2_list,
                              quadrant3_list,
                              quadrant4_list,
                              quadrant1_in_progress_list,
                              quadrant2_in_progress_list,
                              quadrant3_in_progress_list,
                              quadrant4_in_progress_list,
                              quadrant1_completed_list,
                              quadrant2_completed_list,
                              quadrant3_completed_list,
                              quadrant4_completed_list,
                              eisenhower_doing_list}) {
        _connect_task_counter_list(list);
    }
    if (calendar_pane != nullptr && calendar_pane->calendar_panel != nullptr) {
        _connect_task_counter_list(calendar_pane->calendar_panel->tasks_list);
    }

    setCentralWidget(eisenhower_module_page);
    _root_container = eisenhower_module_page;
    _hide_legacy_calendar_button();
    _apply_task_layout();
    refresh_module_task_counter();
    if (poca_layout_active_) {
        set_poca_layout_active(true, false);
    }
}

void EisenhowerMatrixApp::_attach_module_task_counter_to_menu_bar()
{
    if (module_task_counter_container == nullptr) {
        return;
    }

    if (menu_bar == nullptr) {
        menu_bar = new QMenuBar(this);
        menu_bar->setNativeMenuBar(false);
        menu_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }

    if (menu_bar_host == nullptr) {
        menu_bar_host = new QWidget(this);
        auto *host_layout = new QHBoxLayout(menu_bar_host);
        host_layout->setContentsMargins(0, 0, 8, 0);
        host_layout->setSpacing(0);
    }

    auto *host_layout = qobject_cast<QHBoxLayout *>(menu_bar_host->layout());
    if (host_layout == nullptr) {
        return;
    }

    if (host_layout->indexOf(menu_bar) < 0) {
        host_layout->insertWidget(0, menu_bar, 1, Qt::AlignLeft | Qt::AlignVCenter);
    }

    module_task_counter_container->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    if (host_layout->indexOf(module_task_counter_container) < 0) {
        host_layout->addWidget(module_task_counter_container, 0, Qt::AlignRight | Qt::AlignVCenter);
    }

    setMenuWidget(menu_bar_host);
    _sync_module_task_counter_geometry();
    _atualizar_largura_minima_eisenhower();
}

void EisenhowerMatrixApp::_sync_module_task_counter_geometry()
{
    if (menu_bar == nullptr || menu_bar_host == nullptr || module_task_counter_container == nullptr
        || module_task_counter_label == nullptr) {
        return;
    }

    const int menu_bar_height =
        qMax(qMax(menu_bar->height(), menu_bar->sizeHint().height()), module_task_counter_label->sizeHint().height());
    if (menu_bar_height <= 0) {
        return;
    }

    module_task_counter_container->setFixedHeight(menu_bar_height);
    module_task_counter_label->setFixedHeight(menu_bar_height);
    menu_bar_host->setMinimumHeight(menu_bar_height);
}

}  // namespace source
