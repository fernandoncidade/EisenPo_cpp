#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_14_aviso_metas_ciclos.hpp"

#include <QCoreApplication>
#include <QEvent>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace source::plugin::Pomodoro {

AvisoMetasCiclosDialog::AvisoMetasCiclosDialog(QWidget *parent,
                                               int timer_cycles,
                                               const QVector<DiscrepanteInfo> &items)
    : QDialog(parent), timer_cycles_(timer_cycles), items_(items)
{
    setModal(true);
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint)
                   | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    setMinimumSize(660, 360);

    auto *main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(16, 16, 16, 16);
    main_layout->setSpacing(12);

    header_label_ = new QLabel(this);
    header_label_->setWordWrap(true);
    main_layout->addWidget(header_label_);

    table_ = new QTableWidget(this);
    table_->setColumnCount(5);
    table_->setRowCount(items_.size());
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setAlternatingRowColors(true);
    table_->verticalHeader()->setVisible(false);
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    main_layout->addWidget(table_, 1);

    auto *btn_layout = new QHBoxLayout();
    btn_layout->setSpacing(8);

    btn_redefinir_ = new QPushButton(this);
    btn_iniciar_mesmo_assim_ = new QPushButton(this);
    btn_cancelar_ = new QPushButton(this);

    btn_layout->addWidget(btn_redefinir_);
    btn_layout->addWidget(btn_iniciar_mesmo_assim_);
    btn_layout->addStretch(1);
    btn_layout->addWidget(btn_cancelar_);
    main_layout->addLayout(btn_layout);

    connect(btn_redefinir_, &QPushButton::clicked, this, [this]() {
        choice_ = UserChoice::ResetAll;
        done(static_cast<int>(Choice::RedefinirTodas));
    });
    connect(btn_iniciar_mesmo_assim_, &QPushButton::clicked, this, [this]() {
        choice_ = UserChoice::ProceedAnyway;
        done(static_cast<int>(Choice::IniciarMesmoAssim));
    });
    connect(btn_cancelar_, &QPushButton::clicked, this, [this]() {
        choice_ = UserChoice::Cancel;
        reject();
    });

    retranslate_ui();
}

void AvisoMetasCiclosDialog::changeEvent(QEvent *event)
{
    if (event != nullptr && event->type() == QEvent::LanguageChange) {
        retranslate_ui();
    }
    QDialog::changeEvent(event);
}

void AvisoMetasCiclosDialog::retranslate_ui()
{
    setWindowTitle(QCoreApplication::translate("PomodoroPlugin", "⚠️ Aviso de Metas de Ciclos"));
    header_label_->setText(
        QCoreApplication::translate(
            "PomodoroPlugin",
            "<h3>⚠️ Discrepância em Tarefas em \"Em Progresso\"</h3>"
            "<p>O timer do Pomodoro está ajustado para <b>Ciclo 1/%1</b> (%1 ciclos).<br>"
            "As tarefas abaixo possuem <b>Meta de Ciclos Pomodoro (🎯)</b> diferente de %1 ciclos:</p>")
            .arg(timer_cycles_));

    table_->setHorizontalHeaderLabels({
        QCoreApplication::translate("PomodoroPlugin", "Tarefa"),
        QCoreApplication::translate("PomodoroPlugin", "Módulo / Origem"),
        QCoreApplication::translate("PomodoroPlugin", "Meta (🎯)"),
        QCoreApplication::translate("PomodoroPlugin", "Ciclo Timer (⚙️)"),
        QCoreApplication::translate("PomodoroPlugin", "Comportamento no Fluxo")
    });

    for (int r = 0; r < items_.size(); ++r) {
        const auto &info = items_[r];
        auto *item_titulo = new QTableWidgetItem(info.titulo);
        QString mod_text = QCoreApplication::translate("PomodoroPlugin", "🍅 Pomodoro");
        const QString origem = info.origem_id.trimmed().toCaseFolded();
        if (origem == QStringLiteral("eisenhower")) {
            mod_text = QCoreApplication::translate("PomodoroPlugin", "🏛️ Eisenhower");
        } else if (origem == QStringLiteral("feynman")) {
            mod_text = QCoreApplication::translate("PomodoroPlugin", "🧠 Feynman");
        }
        auto *item_origem = new QTableWidgetItem(mod_text);
        auto *item_meta = new QTableWidgetItem(QString::number(info.meta));
        item_meta->setTextAlignment(Qt::AlignCenter);
        auto *item_timer = new QTableWidgetItem(QString::number(timer_cycles_));
        item_timer->setTextAlignment(Qt::AlignCenter);

        QString comportamento;
        if (info.meta < timer_cycles_) {
            comportamento = QCoreApplication::translate(
                "PomodoroPlugin", "🎯 < ⚙️ Concluirá ao atingir %1 ciclos (antes do timer)").arg(info.meta);
        } else {
            comportamento = QCoreApplication::translate(
                "PomodoroPlugin", "🎯 > ⚙️ Não concluirá (permanece Em Progresso; timer para em %1)").arg(timer_cycles_);
        }
        auto *item_comp = new QTableWidgetItem(comportamento);

        table_->setItem(r, 0, item_titulo);
        table_->setItem(r, 1, item_origem);
        table_->setItem(r, 2, item_meta);
        table_->setItem(r, 3, item_timer);
        table_->setItem(r, 4, item_comp);
    }

    btn_redefinir_->setText(QCoreApplication::translate("PomodoroPlugin", "🔄 Redefinir todas para %1 ciclos").arg(timer_cycles_));
    btn_redefinir_->setToolTip(QCoreApplication::translate("PomodoroPlugin", "Altera a meta de todas as tarefas acima para o ciclo do timer e inicia."));

    btn_iniciar_mesmo_assim_->setText(QCoreApplication::translate("PomodoroPlugin", "▶️ Iniciar mesmo assim"));
    btn_iniciar_mesmo_assim_->setToolTip(QCoreApplication::translate("PomodoroPlugin", "Mantém as metas individuais de cada tarefa e inicia o timer."));

    btn_cancelar_->setText(QCoreApplication::translate("PomodoroPlugin", "❌ Cancelar"));
}

}  // namespace source::plugin::Pomodoro
