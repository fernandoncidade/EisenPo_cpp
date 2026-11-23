#pragma once

#include <QString>
#include <QVector>

#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_14_aviso_metas_ciclos.hpp"

namespace source::plugin::Pomodoro {
class GerenciadorTarefas;
}

namespace source::plugin::Pomodoro::GerenciadorTarefasFns {
QVector<DiscrepanteInfo> tarefas_em_progresso_com_meta_discrepante(const GerenciadorTarefas &self, int ciclos_configurados);
void redefinir_ciclos_em_progresso(GerenciadorTarefas &self, int novos_ciclos);
}
