#include "source/modules/pomodoro/Pomodoro/tmp_03_GerenciadorTarefas.hpp"

#include <exception>

#include <QSizePolicy>
#include <QWidget>

#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_internal.hpp"
#include "source/utils/UnifiedTaskStore.hpp"

namespace source::plugin::Pomodoro {

KanbanColumn::KanbanColumn(const QString &status_value, QWidget *parent)
    : QGroupBox(parent), status(status_value)
{
}

GerenciadorTarefas::GerenciadorTarefas(QWidget *parent)
    : GerenciadorTarefas(QStringLiteral("simple:tabs"), parent)
{
}

GerenciadorTarefas::GerenciadorTarefas(const QString &ui_layout_mode_key, QWidget *parent)
    : QWidget(parent),
      logger_(GerenciadorTarefasInternal::logger()),
      gerenciador_botoes_(this),
      ui_layout_mode_key_(GerenciadorTarefasInternal::normalized_ui_layout_key(ui_layout_mode_key))
{
    try {
        arquivo_tarefas = source::utils::UnifiedTaskStore::shared_tasks_path();
        tarefas = carregar_tarefas();

        setup_ui();
        installEventFilter(this);

        _aplicar_tema_dinamico_inputs();
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao inicializar GerenciadorTarefas: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao inicializar GerenciadorTarefas"), true);
    }
}

}  // namespace source::plugin::Pomodoro
