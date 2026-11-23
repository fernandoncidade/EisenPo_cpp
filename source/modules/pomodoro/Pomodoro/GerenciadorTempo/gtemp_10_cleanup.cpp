#include "source/modules/pomodoro/Pomodoro/GerenciadorTempo/gtemp_10_cleanup.hpp"

#include "source/modules/pomodoro/Pomodoro/GerenciadorTempo/gtemp_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace GerenciadorTempoInternal;

void GerenciadorTempo::cleanup()
{
    try {
        if (pomodoro != nullptr && tempo_worker_ != nullptr) {
            disconnect(pomodoro, &PomodoroTimer::ciclo_completado, tempo_worker_, &_TimeWorker::on_pomodoro);
        }

        if (tempo_worker_ != nullptr) {
            disconnect(tempo_worker_, nullptr, this, nullptr);
            tempo_worker_->stop();
        }

        if (tempo_thread_ != nullptr) {
            if (tempo_thread_->isRunning()) {
                tempo_thread_->quit();
                tempo_thread_->wait();
            }
        }

        delete tempo_worker_;
        tempo_worker_ = nullptr;
        tempo_thread_ = nullptr;
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao limpar GerenciadorTempo: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao limpar GerenciadorTempo"), true);
    }
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::GerenciadorTempoFns { void cleanup(GerenciadorTempo &self) { self.cleanup(); } }
