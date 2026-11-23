#include "source/modules/pomodoro/Pomodoro/tmp_02_PomodoroTimer.hpp"

#include <exception>

#include <QCoreApplication>
#include <QSignalBlocker>
#include <QTimer>

#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_14_aviso_metas_ciclos.hpp"
#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_internal.hpp"

namespace source::plugin::Pomodoro {

PomodoroTimer::PomodoroTimer(QWidget *parent)
    : PomodoroTimer(QStringLiteral("simple:tabs"), parent)
{
}

PomodoroTimer::PomodoroTimer(const QString &ui_layout_mode_key, QWidget *parent)
    : QWidget(parent),
      logger_(PomodoroTimerInternal::logger()),
      gerenciador_botoes_(this),
      sound_mgr_(compat::SoundManager::instance()),
      ui_layout_mode_key_(PomodoroTimerInternal::normalized_ui_layout_key(ui_layout_mode_key))
{
    try {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &PomodoroTimer::atualizar_timer);

        setup_ui();

        if (auto *app = QCoreApplication::instance()) {
            app->installEventFilter(this);
        }

        _aplicar_tema_dinamico();
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao inicializar PomodoroTimer: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao inicializar PomodoroTimer"), true);
    }
}

PomodoroTimer::~PomodoroTimer()
{
    if (auto *app = QCoreApplication::instance()) {
        app->removeEventFilter(this);
    }
}

void PomodoroTimer::set_discrepancy_collector(DiscrepancyCollector collector)
{
    discrepancy_collector_ = std::move(collector);
}

void PomodoroTimer::set_discrepancy_resetter(DiscrepancyResetter resetter)
{
    discrepancy_resetter_ = std::move(resetter);
}

void PomodoroTimer::set_ciclos_permitidos(int cycles)
{
    const int val = qMax(1, cycles);
    if (ciclos_permitidos != val) {
        ciclos_permitidos = val;
        discrepancia_verificada_ = false;
        if (spin_ciclos != nullptr) {
            const int idx = spin_ciclos->findText(QString::number(val));
            if (idx >= 0 && spin_ciclos->currentIndex() != idx) {
                const QSignalBlocker blocker(spin_ciclos);
                spin_ciclos->setCurrentIndex(idx);
            }
        }
        atualizar_display();
        atualizar_traducoes();
        emit ciclos_configurados_changed(val);
    }
}

bool PomodoroTimer::verificar_discrepancias_metas()
{
    if (discrepancia_verificada_) {
        return true;
    }

    const int cfg_cycles = qMax(1, ciclos_permitidos);
    QVector<DiscrepanteInfo> discrepantes;

    if (discrepancy_collector_) {
        discrepantes = discrepancy_collector_(cfg_cycles);
    }

    if (discrepantes.isEmpty()) {
        discrepancia_verificada_ = true;
        return true;
    }

    AvisoMetasCiclosDialog dialog(this, cfg_cycles, discrepantes);
    const int res = dialog.exec();
    if (dialog.user_choice() == AvisoMetasCiclosDialog::UserChoice::ResetAll
        || res == static_cast<int>(AvisoMetasCiclosDialog::Choice::RedefinirTodas)) {
        if (discrepancy_resetter_) {
            discrepancy_resetter_(cfg_cycles);
        }

        discrepancia_verificada_ = true;
        return true;
    } else if (dialog.user_choice() == AvisoMetasCiclosDialog::UserChoice::ProceedAnyway
               || res == static_cast<int>(AvisoMetasCiclosDialog::Choice::IniciarMesmoAssim)) {
        discrepancia_verificada_ = true;
        return true;
    }

    return false;
}

}  // namespace source::plugin::Pomodoro

