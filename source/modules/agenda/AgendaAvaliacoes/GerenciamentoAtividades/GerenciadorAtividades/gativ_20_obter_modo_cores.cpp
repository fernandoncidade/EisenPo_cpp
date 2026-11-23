#include "source/modules/agenda/AgendaAvaliacoes/GerenciamentoAtividades/Gerenciador_Atividades.hpp"

QString GerenciamentoAtividades::obter_modo_cores(const QString &/*modulo*/) const {
    return modo_cores.isEmpty() ? QStringLiteral("coloridas") : modo_cores;
}
