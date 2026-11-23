#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_04_toggle_timer.hpp"

#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace PomodoroTimerInternal;

void PomodoroTimer::toggle_timer()
{
    try {
        if (timer_ativo) {
            timer->stop();
            timer_ativo = false;
            gerenciador_botoes_.set_button_text(btn_iniciar, QCoreApplication::translate("PomodoroPlugin", "▶️ Continuar"));
            return;
        }

        const int ciclos_total = qMax(1, ciclos_permitidos);
        if (em_foco && ciclos_completados >= ciclos_total) {
            _executar_alerta(
                QCoreApplication::translate("PomodoroPlugin", "⛔ Limite de ciclos atingido"),
                QCoreApplication::translate(
                    "PomodoroPlugin",
                    "Você atingiu {total}/{total} ciclos de foco.\nAumente o valor em \"Ciclo 1/{total}\" para continuar.")
                    .replace(QStringLiteral("{total}"), QString::number(ciclos_total)));
            atualizar_traducoes();
            return;
        }

        if (!verificar_discrepancias_metas()) {
            return;
        }

        timer->start(1000);
        timer_ativo = true;
        gerenciador_botoes_.set_button_text(btn_iniciar, QCoreApplication::translate("PomodoroPlugin", "⏸️ Pausar"));
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao alternar estado do timer: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao alternar estado do timer"), true);
    }
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::PomodoroTimerFns { void toggle_timer(PomodoroTimer &self) { self.toggle_timer(); } }
