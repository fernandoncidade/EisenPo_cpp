#include "source/modules/pomodoro/Pomodoro/GerenciadorTempo/gtemp_11_del.hpp"

#include "source/modules/pomodoro/Pomodoro/GerenciadorTempo/gtemp_internal.hpp"

#include <QColor>

namespace source::plugin::Pomodoro {
using namespace GerenciadorTempoInternal;
namespace {

QSet<QString> selected_task_ids(const GerenciadorTempo *self)
{
    QSet<QString> selected_ids;
    if (self == nullptr || self->tarefas == nullptr) {
        return selected_ids;
    }

    const QList<QListWidget *> lists = {
        self->tarefas->col_todo != nullptr ? static_cast<QListWidget *>(self->tarefas->col_todo->lista) : nullptr,
        self->tarefas->col_doing != nullptr ? static_cast<QListWidget *>(self->tarefas->col_doing->lista) : nullptr,
        self->tarefas->col_done != nullptr ? static_cast<QListWidget *>(self->tarefas->col_done->lista) : nullptr,
        self->col_doing_timer_list,
    };

    for (QListWidget *list : lists) {
        if (list == nullptr) {
            continue;
        }

        for (QListWidgetItem *item : list->selectedItems()) {
            if (item == nullptr || is_group_header(item) || !(item->flags() & Qt::ItemIsSelectable)) {
                continue;
            }

            const QString task_id = item->data(Qt::UserRole).toString().trimmed();
            if (!task_id.isEmpty()) {
                selected_ids.insert(task_id);
            }
        }
    }

    return selected_ids;
}

} // namespace

bool GerenciadorTempo::limpar_todas_as_tarefas()
{
    if (tarefas == nullptr) {
        return false;
    }

    tarefas->limpar_todas_as_tarefas();
    atualizar_coluna_doing_timer();
    return true;
}

void GerenciadorTempo::clear_all_selections(const QListWidget *except_list)
{
    const QList<QListWidget *> lists = {
        tarefas != nullptr && tarefas->col_todo != nullptr ? static_cast<QListWidget *>(tarefas->col_todo->lista) : nullptr,
        tarefas != nullptr && tarefas->col_doing != nullptr ? static_cast<QListWidget *>(tarefas->col_doing->lista) : nullptr,
        tarefas != nullptr && tarefas->col_done != nullptr ? static_cast<QListWidget *>(tarefas->col_done->lista) : nullptr,
        col_doing_timer_list,
    };

    for (QListWidget *list : lists) {
        if (list == nullptr || list == except_list) {
            continue;
        }

        if (!list->selectedItems().isEmpty() || list->currentItem() != nullptr) {
            if (QItemSelectionModel *selection_model = list->selectionModel(); selection_model != nullptr) {
                selection_model->clear();
            } else {
                list->clearSelection();
            }
            list->setCurrentItem(nullptr);
        }
    }
}

int GerenciadorTempo::total_task_count() const
{
    return tarefas != nullptr ? tarefas->tarefas.size() : 0;
}

int GerenciadorTempo::selected_task_count() const
{
    return selected_task_ids(this).size();
}

QString GerenciadorTempo::selected_task_color() const
{
    if (tarefas == nullptr) {
        return {};
    }

    const QSet<QString> selected_ids = selected_task_ids(this);
    for (const Tarefa &task : tarefas->tarefas) {
        if (selected_ids.contains(task.id) && QColor(task.cor_texto.trimmed()).isValid()) {
            return task.cor_texto.trimmed();
        }
    }
    return {};
}

int GerenciadorTempo::apply_color_to_selected_tasks(const QString &color)
{
    if (tarefas == nullptr || !QColor(color.trimmed()).isValid()) {
        return 0;
    }

    const QSet<QString> selected_ids = selected_task_ids(this);
    if (selected_ids.isEmpty()) {
        return 0;
    }

    int updated = 0;
    const QString normalized_color = QColor(color.trimmed()).name(QColor::HexRgb);
    for (Tarefa &task : tarefas->tarefas) {
        if (!selected_ids.contains(task.id)) {
            continue;
        }
        task.cor_texto = normalized_color;
        ++updated;
    }

    if (updated > 0) {
        tarefas->salvar_tarefas();
        tarefas->atualizar_listas();
        atualizar_coluna_doing_timer();
        emit task_counter_changed();
    }

    return updated;
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::GerenciadorTempoFns { void destroy(GerenciadorTempo &self) { self.cleanup(); } }
