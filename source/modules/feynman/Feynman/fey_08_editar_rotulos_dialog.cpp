#include "source/modules/feynman/Feynman/fey_08_editar_rotulos_dialog.hpp"

#include <algorithm>

#include <QEvent>
#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QShowEvent>
#include <QVBoxLayout>

#include "source/modules/feynman/mod_05_metodo_feynman.hpp"

namespace source::plugin::Feynman {

EditarRotulosDialog::EditarRotulosDialog(QWidget *parent)
    : QDialog(parent)
{
    setup_ui();
    carregar_rotulos();
    atualizar_traducoes();
}

void EditarRotulosDialog::setup_ui()
{
    setModal(true);
    setMinimumSize(720, 290);
    resize(760, 310);

    auto *main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(16, 16, 16, 16);
    main_layout->setSpacing(12);

    auto *group_box = new QGroupBox(this);
    auto *grid_layout = new QGridLayout(group_box);
    grid_layout->setContentsMargins(14, 16, 14, 14);
    grid_layout->setHorizontalSpacing(12);
    grid_layout->setVerticalSpacing(10);

    QFont header_font = font();
    header_font.setBold(true);

    lbl_col_padrao_ = new QLabel(this);
    lbl_col_padrao_->setFont(header_font);
    grid_layout->addWidget(lbl_col_padrao_, 0, 0);

    lbl_col_atual_ = new QLabel(this);
    lbl_col_atual_->setFont(header_font);
    grid_layout->addWidget(lbl_col_atual_, 0, 1);

    lbl_col_novo_ = new QLabel(this);
    lbl_col_novo_->setFont(header_font);
    grid_layout->addWidget(lbl_col_novo_, 0, 2);

    for (int i = 0; i < source::utils::FEYNMAN_LABELS_COUNT; ++i) {
        auto *edit_padrao = new QLineEdit(this);
        edit_padrao->setReadOnly(true);
        edit_padrao->setFocusPolicy(Qt::NoFocus);
        inputs_padrao_[i] = edit_padrao;
        grid_layout->addWidget(edit_padrao, i + 1, 0);

        auto *edit_atual = new QLineEdit(this);
        edit_atual->setReadOnly(true);
        edit_atual->setFocusPolicy(Qt::NoFocus);
        inputs_atuais_[i] = edit_atual;
        grid_layout->addWidget(edit_atual, i + 1, 1);

        auto *edit_novo = new QLineEdit(this);
        edit_novo->setClearButtonEnabled(true);
        connect(edit_novo, &QLineEdit::returnPressed, this, &EditarRotulosDialog::on_aplicar_clicked);
        inputs_novos_[i] = edit_novo;
        grid_layout->addWidget(edit_novo, i + 1, 2);
    }

    grid_layout->setColumnStretch(0, 1);
    grid_layout->setColumnStretch(1, 1);
    grid_layout->setColumnStretch(2, 1);

    main_layout->addWidget(group_box);

    auto *botoes_layout = new QHBoxLayout();
    botoes_layout->setContentsMargins(0, 0, 0, 0);
    botoes_layout->setSpacing(6);

    btn_restaurar_ = gerenciador_botoes_.create_button();
    connect(btn_restaurar_, &QPushButton::clicked, this, &EditarRotulosDialog::on_restaurar_clicked);
    botoes_layout->addWidget(btn_restaurar_);

    botoes_layout->addStretch();

    btn_aplicar_ = gerenciador_botoes_.create_button();
    btn_aplicar_->setDefault(true);
    connect(btn_aplicar_, &QPushButton::clicked, this, &EditarRotulosDialog::on_aplicar_clicked);
    botoes_layout->addWidget(btn_aplicar_);

    btn_fechar_ = gerenciador_botoes_.create_button();
    connect(btn_fechar_, &QPushButton::clicked, this, &EditarRotulosDialog::on_fechar_clicked);
    botoes_layout->addWidget(btn_fechar_);

    main_layout->addLayout(botoes_layout);

    padronizar_botoes();
}

void EditarRotulosDialog::carregar_rotulos()
{
    const bool is_en = compat::is_english(this);

    for (int i = 0; i < source::utils::FEYNMAN_LABELS_COUNT; ++i) {
        const auto key = static_cast<source::utils::FeynmanLabelKey>(i);
        const QString padrao = source::utils::get_default_feynman_label(key, is_en);
        const QString atual = source::utils::get_effective_feynman_label(key, is_en);
        const QString placeholder = source::utils::get_feynman_placeholder(key, is_en);

        if (inputs_padrao_[i] != nullptr) {
            inputs_padrao_[i]->setText(padrao);
        }
        if (inputs_atuais_[i] != nullptr) {
            inputs_atuais_[i]->setText(atual);
        }
        if (inputs_novos_[i] != nullptr) {
            inputs_novos_[i]->setPlaceholderText(placeholder);
        }
    }
}

void EditarRotulosDialog::padronizar_botoes()
{
    MetodoFeynman *feynman = nullptr;
    for (QWidget *w = parentWidget(); w != nullptr; w = w->parentWidget()) {
        if (auto *f = qobject_cast<MetodoFeynman *>(w)) {
            feynman = f;
            break;
        }
    }

    if (feynman != nullptr) {
        int target_height = 0;
        if (feynman->btn_salvar != nullptr) {
            target_height = std::max(feynman->btn_salvar->sizeHint().height(),
                                     feynman->btn_salvar->minimumSizeHint().height());
        } else if (feynman->btn_fechar != nullptr) {
            target_height = std::max(feynman->btn_fechar->sizeHint().height(),
                                     feynman->btn_fechar->minimumSizeHint().height());
        }

        if (target_height > 0) {
            if (btn_restaurar_ != nullptr) {
                btn_restaurar_->setFixedHeight(target_height);
            }
            if (btn_aplicar_ != nullptr) {
                btn_aplicar_->setFixedHeight(target_height);
            }
            if (btn_fechar_ != nullptr) {
                btn_fechar_->setFixedHeight(target_height);
            }
        }

        if (feynman->btn_fechar != nullptr && btn_fechar_ != nullptr) {
            const int fechar_width = feynman->btn_fechar->sizeHint().width();
            if (fechar_width > 0) {
                btn_fechar_->setFixedWidth(fechar_width);
            }
        }
    }
}

void EditarRotulosDialog::atualizar_traducoes()
{
    const bool is_en = compat::is_english(this);

    setWindowTitle(compat::tr(this, QStringLiteral("Editar Rótulos"), QStringLiteral("Edit Labels")));

    if (lbl_col_padrao_ != nullptr) {
        lbl_col_padrao_->setText(compat::tr(this, QStringLiteral("Padrão"), QStringLiteral("Default")));
    }
    if (lbl_col_atual_ != nullptr) {
        lbl_col_atual_->setText(compat::tr(this, QStringLiteral("Atual"), QStringLiteral("Current")));
    }
    if (lbl_col_novo_ != nullptr) {
        lbl_col_novo_->setText(compat::tr(this, QStringLiteral("Novo"), QStringLiteral("New")));
    }

    for (int i = 0; i < source::utils::FEYNMAN_LABELS_COUNT; ++i) {
        const auto key = static_cast<source::utils::FeynmanLabelKey>(i);
        const QString padrao = source::utils::get_default_feynman_label(key, is_en);

        if (inputs_padrao_[i] != nullptr) {
            inputs_padrao_[i]->setText(padrao);
        }

        if (!source::utils::is_feynman_label_customized(key)) {
            if (inputs_atuais_[i] != nullptr) {
                inputs_atuais_[i]->setText(padrao);
            }
        }

        if (inputs_novos_[i] != nullptr) {
            inputs_novos_[i]->setPlaceholderText(source::utils::get_feynman_placeholder(key, is_en));
        }
    }

    if (btn_restaurar_ != nullptr) {
        gerenciador_botoes_.set_button_text(btn_restaurar_, compat::tr(this, QStringLiteral("Restaurar"), QStringLiteral("Restore")));
    }
    if (btn_aplicar_ != nullptr) {
        gerenciador_botoes_.set_button_text(btn_aplicar_, compat::tr(this, QStringLiteral("Aplicar"), QStringLiteral("Apply")));
    }
    if (btn_fechar_ != nullptr) {
        gerenciador_botoes_.set_button_text(btn_fechar_, compat::tr(this, QStringLiteral("Fechar"), QStringLiteral("Close")));
    }

    padronizar_botoes();
}

void EditarRotulosDialog::changeEvent(QEvent *event)
{
    if (event != nullptr && event->type() == QEvent::LanguageChange) {
        atualizar_traducoes();
    }
    QDialog::changeEvent(event);
}

void EditarRotulosDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    padronizar_botoes();
}

void EditarRotulosDialog::on_aplicar_clicked()
{
    QMap<QString, QString> custom = source::utils::load_feynman_custom_labels();
    bool changed = false;

    for (int i = 0; i < source::utils::FEYNMAN_LABELS_COUNT; ++i) {
        if (inputs_novos_[i] == nullptr) {
            continue;
        }
        const QString novo_texto = inputs_novos_[i]->text().trimmed();
        if (!novo_texto.isEmpty()) {
            const auto key = static_cast<source::utils::FeynmanLabelKey>(i);
            const QString key_id = source::utils::get_feynman_key_id(key);
            custom.insert(key_id, novo_texto);

            if (inputs_atuais_[i] != nullptr) {
                inputs_atuais_[i]->setText(novo_texto);
            }
            inputs_novos_[i]->clear();
            changed = true;
        }
    }

    if (changed) {
        source::utils::save_feynman_custom_labels(custom);
        emit labels_changed();
    }
}

void EditarRotulosDialog::on_restaurar_clicked()
{
    const bool is_en = compat::is_english(this);
    source::utils::restore_feynman_default_labels();

    for (int i = 0; i < source::utils::FEYNMAN_LABELS_COUNT; ++i) {
        const auto key = static_cast<source::utils::FeynmanLabelKey>(i);
        const QString padrao = source::utils::get_default_feynman_label(key, is_en);

        if (inputs_atuais_[i] != nullptr) {
            inputs_atuais_[i]->setText(padrao);
        }
        if (inputs_novos_[i] != nullptr) {
            inputs_novos_[i]->clear();
        }
    }

    emit labels_changed();
}

void EditarRotulosDialog::on_fechar_clicked()
{
    accept();
}

}  // namespace source::plugin::Feynman
