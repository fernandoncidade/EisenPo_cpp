#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_03_atualizar_configuracoes.hpp"

#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace PomodoroTimerInternal;

void PomodoroTimer::atualizar_configuracoes()
{
    try {
        const int novo_foco = spin_foco->currentText().toInt() * 60;
        const int novo_descanso_curto = spin_descanso->currentText().toInt() * 60;
        const int novos_ciclos = qMax(1, spin_ciclos->currentText().toInt());
        const int ciclos_atuais = qMax(1, ciclos_permitidos);

        const int total_antigo =
            em_foco ? tempo_foco
                    : ((ciclos_completados % ciclos_atuais == 0) ? tempo_descanso_longo : tempo_descanso_curto);

        const int decorrido = qMax(0, total_antigo - tempo_restante);

        tempo_foco = novo_foco;
        tempo_descanso_curto = novo_descanso_curto;
        ciclos_permitidos = novos_ciclos;
        discrepancia_verificada_ = false;

        const int total_novo =
            em_foco ? tempo_foco
                    : ((ciclos_completados % novos_ciclos == 0) ? tempo_descanso_longo : tempo_descanso_curto);

        tempo_restante = qMax(total_novo - decorrido, 0);
        progress->setMaximum(total_novo);
        progress->setValue(tempo_restante);
        atualizar_display();
        atualizar_traducoes();

        if (auto *bridge = compat::host_bridge()) {
            bridge->set_configured_cycles(novos_ciclos);
            bridge->notify_configured_cycles_changed(novos_ciclos);
        }
        emit ciclos_configurados_changed(novos_ciclos);
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao atualizar configurações do timer: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao atualizar configurações do timer"), true);
    }
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::PomodoroTimerFns { void atualizar_configuracoes(PomodoroTimer &self) { self.atualizar_configuracoes(); } }
