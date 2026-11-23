#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_23_eventFilter.hpp"

#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace GerenciadorTarefasInternal;

bool GerenciadorTarefas::eventFilter(QObject *obj, QEvent *event)
{
    try {
        if (event != nullptr) {
            switch (event->type()) {
            case QEvent::ApplicationPaletteChange:
            case QEvent::PaletteChange:
            case QEvent::StyleChange:
            case QEvent::ThemeChange:
                _aplicar_tema_dinamico_inputs();
                break;
            case QEvent::LanguageChange:
                atualizar_traducoes();
                break;
            case QEvent::Resize:
                if (obj == this || obj == parentWidget() || obj == window() || obj == status_tabs) {
                    ajustar_linha_botoes();
                }
                break;
            default:
                break;
            }
        }
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro no eventFilter do GerenciadorTarefas: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro no eventFilter do GerenciadorTarefas"), true);
    }

    return QWidget::eventFilter(obj, event);
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::GerenciadorTarefasFns { bool eventFilter(GerenciadorTarefas &self, QObject *obj, QEvent *event) { return self.eventFilter(obj, event); } }
