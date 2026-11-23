#pragma once

#include <QDialog>
#include <QString>
#include <QVector>

class QLabel;
class QTableWidget;
class QPushButton;

namespace source::plugin::Pomodoro {

struct DiscrepanteInfo {
    QString titulo;
    QString origem_id;
    int meta = 0;
};

class AvisoMetasCiclosDialog : public QDialog {
    Q_OBJECT

public:
    enum class UserChoice { Cancel, ResetAll, ProceedAnyway };

    enum Choice {
        Cancelar = 0,
        RedefinirTodas = 1,
        IniciarMesmoAssim = 2
    };

    explicit AvisoMetasCiclosDialog(QWidget *parent,
                                   int timer_cycles,
                                   const QVector<DiscrepanteInfo> &items);

    UserChoice user_choice() const { return choice_; }

protected:
    void changeEvent(QEvent *event) override;

private:
    void retranslate_ui();

    int timer_cycles_ = 4;
    QVector<DiscrepanteInfo> items_;
    QLabel *header_label_ = nullptr;
    QTableWidget *table_ = nullptr;
    QPushButton *btn_redefinir_ = nullptr;
    QPushButton *btn_iniciar_mesmo_assim_ = nullptr;
    QPushButton *btn_cancelar_ = nullptr;
    UserChoice choice_ = UserChoice::Cancel;
};

using CycleMismatchDialog = AvisoMetasCiclosDialog;

}  // namespace source::plugin::Pomodoro
