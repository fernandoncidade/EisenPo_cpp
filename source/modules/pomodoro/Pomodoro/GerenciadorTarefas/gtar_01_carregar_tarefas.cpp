#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_01_carregar_tarefas.hpp"

#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace GerenciadorTarefasInternal;

QVector<Tarefa> GerenciadorTarefas::carregar_tarefas()
{
    try {
        return load_tasks_from_path(arquivo_tarefas);
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao carregar tarefas: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao carregar tarefas"), true);
    }

    return {};
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::GerenciadorTarefasFns { QVector<Tarefa> carregar_tarefas(GerenciadorTarefas &self) { return self.carregar_tarefas(); } }
