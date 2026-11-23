#include "source/services/incore_04_definir_idioma.hpp"

#include <QAction>
#include <QCoreApplication>
#include <QMenu>
#include <QMessageBox>
#include <QPointer>
#include <QTimer>

#include "source/eisenhower_app.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"

namespace source::InterfaceCore::incore_04_definir_idioma {
namespace {

QPointer<QMessageBox> s_active_dialog;

QString get_text(const QString &text)
{
    return QCoreApplication::translate("InterfaceGrafica", text.toUtf8().constData());
}

void exibir_dialogo_traducao(source::EisenhowerMatrixApp *app, const QString &idioma_nome, bool sucesso)
{
    if (app == nullptr) {
        return;
    }

    // Cleanly close existing dialog if user triggers another change quickly
    if (s_active_dialog != nullptr) {
        s_active_dialog->close();
        s_active_dialog = nullptr;
    }

    auto *msg_box = new QMessageBox(
        sucesso ? QMessageBox::Information : QMessageBox::Warning,
        get_text(sucesso ? QStringLiteral("Sucesso") : QStringLiteral("Erro")),
        sucesso
            ? (get_text(QStringLiteral("Idioma \"{language}\" aplicado com sucesso!"))
                   .replace(QStringLiteral("{language}"), idioma_nome))
            : (get_text(QStringLiteral("Não foi possível aplicar o Idioma \"{language}\"!"))
                   .replace(QStringLiteral("{language}"), idioma_nome)),
        QMessageBox::Ok,
        app);

    msg_box->setAttribute(Qt::WA_DeleteOnClose, true);
    msg_box->setWindowModality(Qt::NonModal);
    msg_box->setModal(false);

    if (app->gerenciador_traducao != nullptr) {
        app->gerenciador_traducao->traduzir_botoes_padrao(msg_box);
    }

    // Auto-dismiss after 5 seconds if untouched, ensuring completely frictionless workflow
    QTimer::singleShot(5000, msg_box, [msg_box]() {
        if (msg_box != nullptr) {
            msg_box->close();
        }
    });

    s_active_dialog = msg_box;
    msg_box->show();
    msg_box->raise();
    msg_box->activateWindow();
}

}  // namespace

bool definir_idioma(source::EisenhowerMatrixApp *app, const QString &codigo_idioma)
{
    QString idioma_nome = codigo_idioma.trimmed();

    if (app == nullptr || app->gerenciador_traducao == nullptr) {
        return false;
    }

    idioma_nome = app->gerenciador_traducao->idiomas_disponiveis().value(codigo_idioma, idioma_nome);

    // 1. Immediately update checked state on language menu for instant feedback
    if (app->idioma_menu != nullptr) {
        const auto actions = app->idioma_menu->actions();
        for (QAction *acao : actions) {
            if (acao != nullptr) {
                acao->setChecked(acao->text() == idioma_nome);
            }
        }
    }

    // 2. Apply the translation in the translation manager
    const bool resultado = app->gerenciador_traducao->definir_idioma(codigo_idioma);

    // 3. Display the non-modal dialog window
    // Scheduled via singleShot(0) so the newly installed translations format the dialog text,
    // and user interaction with the GUI remains 100% unlocked, responsive and fluid throughout.
    QTimer::singleShot(0, app, [app, idioma_nome, resultado]() {
        exibir_dialogo_traducao(app, idioma_nome, resultado);
    });

    return resultado;
}

}  // namespace source::InterfaceCore::incore_04_definir_idioma

