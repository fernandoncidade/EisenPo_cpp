#include "source/modules/pomodoro/Pomodoro/tmp_04_KanbanListWidget.hpp"

#include <QDropEvent>
#include <QMouseEvent>
#include <QResizeEvent>

#include "source/modules/pomodoro/Pomodoro/pomodoro_support.hpp"
#include "source/modules/pomodoro/Pomodoro/tmp_05_GerenciadorTempo.hpp"
#include "source/utils/LogManager.hpp"

namespace source::plugin::Pomodoro {
_KanbanListWidget::_KanbanListWidget(QWidget *parent) : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionRectVisible(true);
}

void _KanbanListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        const QPoint pos = event->position().toPoint();
        QListWidgetItem *item = itemAt(pos);
        const bool is_selectable_item = item != nullptr && (item->flags() & Qt::ItemIsSelectable);
        if (!is_selectable_item) {
            if (!(event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier))) {
                clearSelection();
                setCurrentItem(nullptr);
            }
        }

        // Clear selections in other Pomodoro lists
        for (QWidget *w = parentWidget(); w != nullptr; w = w->parentWidget()) {
            if (auto *tempo = qobject_cast<GerenciadorTempo *>(w)) {
                tempo->clear_all_selections(this);
                break;
            }
        }

        // Clear selections in external lists (Eisenhower, Calendar)
        if (auto *bridge = compat::host_bridge()) {
            bridge->clear_external_selections();
        }
    }
    QListWidget::mousePressEvent(event);
}

void _KanbanListWidget::dropEvent(QDropEvent *event)
{
    auto *logger = source::utils::LogManager::get_logger();

    try {
        QListWidget::dropEvent(event);
        emit tarefas_soltadas();
    } catch (const std::exception &e) {
        logger->error(QStringLiteral("Erro no KanbanListWidget.dropEvent: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger->error(QStringLiteral("Erro no KanbanListWidget.dropEvent"), true);
    }
}

void _KanbanListWidget::resizeEvent(QResizeEvent *event)
{
    QListWidget::resizeEvent(event);
    doItemsLayout();
}

}  // namespace source::plugin::Pomodoro
