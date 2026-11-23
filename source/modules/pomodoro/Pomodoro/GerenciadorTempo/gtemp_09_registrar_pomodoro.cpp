#include "source/modules/pomodoro/Pomodoro/GerenciadorTempo/gtemp_09_registrar_pomodoro.hpp"

#include "source/modules/pomodoro/Pomodoro/GerenciadorTempo/gtemp_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace GerenciadorTempoInternal;

void GerenciadorTempo::registrar_pomodoro(const QString &tipo)
{
    try {
        if (tipo != QStringLiteral("foco") || tarefas == nullptr) {
            return;
        }

        const int fallback_ciclos = qMax(1, pomodoro != nullptr ? pomodoro->ciclos_permitidos : 4);
        const bool fluxo_ativo =
            tarefas->chk_fluxo_tarefas_concluidas != nullptr && tarefas->chk_fluxo_tarefas_concluidas->isChecked();

        bool changed = false;
        for (Tarefa &task : tarefas->tarefas) {
            if (task.status == QStringLiteral("Doing")) {
                ++task.pomodoros_completados;
                const int meta_task = task.ciclos_totais > 0 ? task.ciclos_totais : fallback_ciclos;
                if (fluxo_ativo && task.pomodoros_completados >= meta_task) {
                    task.status = QStringLiteral("Done");
                }
                changed = true;
            }
        }

        if (changed) {
            tarefas->salvar_tarefas();
            tarefas->atualizar_listas();
            atualizar_coluna_doing_timer();
        }
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao registrar pomodoro nas tarefas em progresso: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao registrar pomodoro nas tarefas em progresso"), true);
    }
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::GerenciadorTempoFns { void registrar_pomodoro(GerenciadorTempo &self, const QString &tipo) { self.registrar_pomodoro(tipo); } }
