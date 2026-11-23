#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "source/modules/feynman/Feynman/feynman_support.hpp"
#include "source/utils/FeynmanLabelsConfig.hpp"

namespace source::plugin::Feynman {

class EditarRotulosDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditarRotulosDialog(QWidget *parent = nullptr);
    ~EditarRotulosDialog() override = default;

signals:
    void labels_changed();

public slots:
    void atualizar_traducoes();

protected:
    void changeEvent(QEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void on_aplicar_clicked();
    void on_restaurar_clicked();
    void on_fechar_clicked();

private:
    void setup_ui();
    void carregar_rotulos();
    void padronizar_botoes();

    compat::GerenciadorBotoesUI gerenciador_botoes_{this};

    QLabel *lbl_col_padrao_{nullptr};
    QLabel *lbl_col_atual_{nullptr};
    QLabel *lbl_col_novo_{nullptr};

    QLineEdit *inputs_padrao_[source::utils::FEYNMAN_LABELS_COUNT]{nullptr};
    QLineEdit *inputs_atuais_[source::utils::FEYNMAN_LABELS_COUNT]{nullptr};
    QLineEdit *inputs_novos_[source::utils::FEYNMAN_LABELS_COUNT]{nullptr};

    QPushButton *btn_restaurar_{nullptr};
    QPushButton *btn_aplicar_{nullptr};
    QPushButton *btn_fechar_{nullptr};
};

}  // namespace source::plugin::Feynman
