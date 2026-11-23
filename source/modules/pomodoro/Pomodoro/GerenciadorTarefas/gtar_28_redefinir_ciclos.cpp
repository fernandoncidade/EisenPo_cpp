#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_28_redefinir_ciclos.hpp"

#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace GerenciadorTarefasInternal;

QVector<DiscrepanteInfo> GerenciadorTarefas::tarefas_em_progresso_com_meta_discrepante(int ciclos_configurados) const
{
    QVector<DiscrepanteInfo> result;
    const int target = qMax(1, ciclos_configurados);
    for (const Tarefa &task : tarefas) {
        if (task.status == QStringLiteral("Doing")) {
            const int meta = task.ciclos_totais > 0 ? task.ciclos_totais : target;
            if (meta != target) {
                result.append({task.titulo, task.integracao_origem, meta});
            }
        }
    }
    return result;
}

void GerenciadorTarefas::redefinir_ciclos_em_progresso(int novos_ciclos)
{
    try {
        const int target = qMax(1, novos_ciclos);
        bool changed = false;
        for (Tarefa &task : tarefas) {
            if (task.status == QStringLiteral("Doing")) {
                if (task.ciclos_totais != target) {
                    task.ciclos_totais = target;
                    changed = true;
                }
            }
        }

        if (changed) {
            salvar_tarefas();
            atualizar_listas();
        }

        if (auto *bridge = compat::host_bridge()) {
            bridge->notify_redefinir_ciclos_em_progresso(target);
        }
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao redefinir ciclos em progresso: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao redefinir ciclos em progresso"), true);
    }
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::GerenciadorTarefasFns {

QVector<DiscrepanteInfo> tarefas_em_progresso_com_meta_discrepante(const GerenciadorTarefas &self, int ciclos_configurados)
{
    return self.tarefas_em_progresso_com_meta_discrepante(ciclos_configurados);
}

void redefinir_ciclos_em_progresso(GerenciadorTarefas &self, int novos_ciclos)
{
    self.redefinir_ciclos_em_progresso(novos_ciclos);
}

}  // namespace source::plugin::Pomodoro::GerenciadorTarefasFns
