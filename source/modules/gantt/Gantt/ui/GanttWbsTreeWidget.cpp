#include "GanttWbsTreeWidget.hpp"
#include <cmath>
#include <QHash>
#include <QApplication>
#include <QEvent>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMenu>
#include "BiDirectionalResizeHeaderView.hpp"
#include "GanttCellHoverDelegate.hpp"
#include "../application/services/GanttTaskIntegrationBridge.hpp"

namespace source::gantt {

GanttWbsTreeWidget::GanttWbsTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    // Install custom header that supports left-edge and right-edge resizing
    auto *custom_header = new BiDirectionalResizeHeaderView(Qt::Horizontal, this);
    setHeader(custom_header);

    setColumnCount(9);
    retranslate_ui();

    // Interactive resize mode allows user to drag column borders to resize
    header()->setSectionResizeMode(QHeaderView::Interactive);
    header()->setStretchLastSection(false);

    // Set reasonable default widths
    header()->resizeSection(0, 120);  // Código EAP
    header()->resizeSection(1, 250);  // Nome da Atividade
    header()->resizeSection(2, 130);  // Início
    header()->resizeSection(3, 130);  // Término
    header()->resizeSection(4, 80);   // Duração
    header()->resizeSection(5, 70);   // Avanço
    header()->resizeSection(6, 70);   // Crítica
    header()->resizeSection(7, 80);   // BIM
    header()->resizeSection(8, 200);  // Tarefa EisenPo

    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setUniformRowHeights(true);
    setRootIsDecorated(false);
    setItemsExpandable(false);
    setContextMenuPolicy(Qt::CustomContextMenu);

    // Install hover highlight delegate (lighter in dark theme, darker in light theme)
    setItemDelegate(new GanttCellHoverDelegate(this));
    setMouseTracking(true);
    if (viewport()) {
        viewport()->setMouseTracking(true);
        viewport()->setAttribute(Qt::WA_Hover, true);
    }

    // Column selection on header click (manual cell-by-cell selection, supporting single and multiple columns)
    header()->setSectionsClickable(true);
    connect(header(), &QHeaderView::sectionClicked, this, [this](int logicalIndex) {
        const auto modifiers = QApplication::keyboardModifiers();
        const bool ctrl = modifiers.testFlag(Qt::ControlModifier);
        const bool shift = modifiers.testFlag(Qt::ShiftModifier);

        if (!ctrl && !shift) {
            clearSelection();
        }

        int start_col = logicalIndex;
        int end_col = logicalIndex;
        if (shift && last_clicked_col_ >= 0) {
            start_col = std::min(last_clicked_col_, logicalIndex);
            end_col = std::max(last_clicked_col_, logicalIndex);
        } else {
            last_clicked_col_ = logicalIndex;
        }

        for (int col = start_col; col <= end_col; ++col) {
            for (int i = 0; i < topLevelItemCount(); ++i) {
                QTreeWidgetItem *it = topLevelItem(i);
                if (it) {
                    QModelIndex idx = indexFromItem(it, col);
                    selectionModel()->select(idx, QItemSelectionModel::Select);
                }
            }
        }
    });

    connect(this, &QTreeWidget::currentItemChanged, this, &GanttWbsTreeWidget::on_current_item_changed);
    connect(this, &QTreeWidget::itemDoubleClicked, this, &GanttWbsTreeWidget::on_item_double_clicked);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &GanttWbsTreeWidget::on_custom_context_menu);
}

void GanttWbsTreeWidget::retranslate_ui()
{
    setUpdatesEnabled(false);

    setHeaderLabels({
        tr("Código EAP"),
        tr("Nome da Atividade"),
        tr("Início"),
        tr("Término"),
        tr("Duração"),
        tr("Avanço"),
        tr("Crítica"),
        QStringLiteral("BIM"),
        tr("Tarefa EisenPo")
    });

    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *it = topLevelItem(i);
        if (!it) continue;
        const QString item_id = it->data(0, Qt::UserRole).toString();
        const ScheduleItem *sched_item = current_schedule_.find_item(item_id);
        if (sched_item) {
            it->setText(6, sched_item->critical ? tr("Sim").toUpper() : tr("Não").toUpper());
            const QString col8 = it->text(8);
            if (col8 == QStringLiteral("🔗 Vinculada") || col8 == QStringLiteral("🔗 Linked")) {
                it->setText(8, tr("🔗 Vinculada"));
            }
        }
    }

    update_items_styling();
    setUpdatesEnabled(true);
}


void GanttWbsTreeWidget::populate(const Schedule &schedule)
{
    current_schedule_ = schedule;
    clear();
    QList<ScheduleItem> items = schedule.get_sorted_by_wbs();

    auto &bridge = GanttTaskIntegrationBridge::instance();

    // Fast O(1) title lookup for linked EisenPo tasks to prevent N disk reads in the loop
    QHash<QString, QString> task_titles;
    const QVariantList available_tasks = bridge.get_available_eisenhower_tasks();
    for (const auto &var : available_tasks) {
        const QVariantMap task = var.toMap();
        const QString tid = task.value(QStringLiteral("id")).toString().trimmed();
        if (!tid.isEmpty()) {
            QString task_title = task.value(QStringLiteral("text"), task.value(QStringLiteral("titulo"))).toString().trimmed();
            if (task_title.isEmpty()) {
                task_title = task.value(QStringLiteral("title")).toString().trimmed();
            }
            task_titles.insert(tid, task_title);
        }
    }

    const QDateTime eval_date = reference_date_.isValid() ? reference_date_ : QDateTime::currentDateTime();

    for (const auto &item : items) {
        auto *tree_item = new QTreeWidgetItem(this);
        tree_item->setData(0, Qt::UserRole, item.id);

        tree_item->setText(0, item.wbs_code);
        tree_item->setText(1, item.name);
        const QString start_str = item.start_date.isValid()
            ? (item.start_date.time().isValid() && item.start_date.time() != QTime(0, 0)
                   ? item.start_date.toString(QStringLiteral("dd/MM/yyyy HH:mm"))
                   : item.start_date.toString(QStringLiteral("dd/MM/yyyy")))
            : QStringLiteral("-");
        const QString end_str = item.end_date.isValid()
            ? (item.end_date.time().isValid() && item.end_date.time() != QTime(0, 0)
                   ? item.end_date.toString(QStringLiteral("dd/MM/yyyy HH:mm"))
                   : item.end_date.toString(QStringLiteral("dd/MM/yyyy")))
            : QStringLiteral("-");

        tree_item->setText(2, start_str);
        tree_item->setText(3, end_str);
        if (item.start_date.isValid()) {
            tree_item->setToolTip(2, item.start_date.toString(QStringLiteral("dd/MM/yyyy HH:mm")));
        }
        if (item.end_date.isValid()) {
            tree_item->setToolTip(3, item.end_date.toString(QStringLiteral("dd/MM/yyyy HH:mm")));
        }
        tree_item->setText(4, QString::number(item.duration_hours / 8.0, 'f', 1));
        const QString prog_str = QString::number(item.percent_complete, 'f', (item.percent_complete == std::floor(item.percent_complete)) ? 0 : 2) + QLatin1Char('%');
        tree_item->setText(5, prog_str);

        // 4D status font color & weight applied to activity columns (0..5)
        const Bim4DStatus status = item.simulation_status(eval_date);
        const QColor status_color = item.status_font_color(eval_date);
        const bool is_bold = (status != Bim4DStatus::Unstarted);

        for (int col = 0; col <= 5; ++col) {
            tree_item->setForeground(col, QBrush(status_color));
        }
        QFont f1 = font();
        f1.setBold(is_bold);
        tree_item->setFont(1, f1);

        if (item.critical) {
            tree_item->setText(6, tr("Sim").toUpper());
            tree_item->setForeground(6, QBrush(QColor(220, 20, 60))); // Crimson
        } else {
            tree_item->setText(6, tr("Não").toUpper());
            tree_item->setForeground(6, QBrush(QColor(128, 128, 128)));
        }

        // Count BIM links
        int bim_count = 0;
        for (const auto &b : schedule.bim_links) {
            if (b.schedule_item_id == item.id) bim_count++;
        }
        if (bim_count > 0) {
            tree_item->setText(7, QStringLiteral("%1 elem.").arg(bim_count));
            tree_item->setForeground(7, QBrush(QColor(33, 150, 243))); // Blue
        } else {
            tree_item->setText(7, QStringLiteral("-"));
        }

        // Linked EisenPo task info
        if (!item.task_id.trimmed().isEmpty()) {
            const QString tid = item.task_id.trimmed();
            auto it = task_titles.constFind(tid);
            const QString task_title = (it != task_titles.constEnd()) ? it.value() : QString();
            if (task_title.isEmpty()) {
                tree_item->setText(8, tr("🔗 Vinculada"));
            } else {
                // Show full task title without truncation
                tree_item->setText(8, QStringLiteral("🔗 %1").arg(task_title));
            }
            tree_item->setForeground(8, QBrush(QColor(46, 125, 50))); // Dark green
        } else {
            tree_item->setText(8, QStringLiteral("-"));
            tree_item->setForeground(8, QBrush(QColor(140, 140, 140)));
        }

        addTopLevelItem(tree_item);
    }
}

void GanttWbsTreeWidget::set_reference_date(const QDateTime &ref_date)
{
    if (reference_date_ == ref_date) return;
    reference_date_ = ref_date;
    update_items_styling();
}

void GanttWbsTreeWidget::update_items_styling()
{
    const QDateTime eval_date = reference_date_.isValid() ? reference_date_ : QDateTime::currentDateTime();
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *it = topLevelItem(i);
        if (!it) continue;
        const QString item_id = it->data(0, Qt::UserRole).toString();
        const ScheduleItem *sched_item = current_schedule_.find_item(item_id);
        if (sched_item) {
            const Bim4DStatus status = sched_item->simulation_status(eval_date);
            const QColor status_color = sched_item->status_font_color(eval_date);
            const bool is_bold = (status != Bim4DStatus::Unstarted);

            for (int col = 0; col <= 5; ++col) {
                it->setForeground(col, QBrush(status_color));
            }
            QFont f1 = font();
            f1.setBold(is_bold);
            it->setFont(1, f1);
        }
    }
}

QString GanttWbsTreeWidget::selected_item_id() const
{
    QTreeWidgetItem *cur = currentItem();
    if (cur) {
        return cur->data(0, Qt::UserRole).toString();
    }
    return QString();
}

void GanttWbsTreeWidget::select_item_by_id(const QString &item_id)
{
    if (item_id.trimmed().isEmpty()) return;
    if (selected_item_id() == item_id) return;

    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *it = topLevelItem(i);
        if (it && it->data(0, Qt::UserRole).toString() == item_id) {
            const int col = (currentColumn() >= 0) ? currentColumn() : 0;
            setCurrentItem(it, col);
            scrollToItem(it);
            break;
        }
    }
}

void GanttWbsTreeWidget::on_current_item_changed(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (current) {
        emit item_selected(current->data(0, Qt::UserRole).toString());
    }
}

void GanttWbsTreeWidget::on_item_double_clicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (item) {
        emit item_double_clicked(item->data(0, Qt::UserRole).toString());
    }
}

void GanttWbsTreeWidget::on_custom_context_menu(const QPoint &pos)
{
    QTreeWidgetItem *it = itemAt(pos);
    if (!it) return;

    const QString item_id = it->data(0, Qt::UserRole).toString();
    ScheduleItem *sched_item = current_schedule_.find_item(item_id);
    if (!sched_item) return;

    QMenu menu(this);
    if (sched_item->task_id.trimmed().isEmpty()) {
        auto *act_link = menu.addAction(tr("Vincular à Tarefa do EisenPo..."));
        connect(act_link, &QAction::triggered, this, [this, item_id]() {
            emit request_link_task(item_id);
        });
    } else {
        auto *act_sync = menu.addAction(tr("Sincronizar com EisenPo"));
        connect(act_sync, &QAction::triggered, this, [this, item_id]() {
            emit request_sync_task(item_id);
        });

        auto *act_unlink = menu.addAction(tr("Desvincular do EisenPo"));
        connect(act_unlink, &QAction::triggered, this, [this, item_id]() {
            emit request_unlink_task(item_id);
        });
    }

    menu.addSeparator();
    auto *act_edit = menu.addAction(tr("✏️ Editar Atividade..."));
    connect(act_edit, &QAction::triggered, this, [this, item_id]() {
        emit item_double_clicked(item_id);
    });

    auto *act_del = menu.addAction(tr("❌ Excluir Atividade"));
    connect(act_del, &QAction::triggered, this, [this, item_id]() {
        emit request_delete_item(item_id);
    });

    menu.exec(viewport()->mapToGlobal(pos));
}

void GanttWbsTreeWidget::keyPressEvent(QKeyEvent *event)
{
    if (event && event->key() == Qt::Key_Delete) {
        const QString item_id = selected_item_id();
        if (!item_id.isEmpty()) {
            emit request_delete_item(item_id);
            return;
        }
    }
    QTreeWidget::keyPressEvent(event);
}

} // namespace source::gantt
