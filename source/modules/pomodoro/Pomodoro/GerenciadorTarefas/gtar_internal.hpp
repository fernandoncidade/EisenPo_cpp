#pragma once

#include <algorithm>
#include <optional>
#include <tuple>

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QApplication>
#include <QBrush>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QDir>
#include <QDoubleSpinBox>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMap>
#include <QMenu>
#include <QPalette>
#include <QPushButton>
#include <QScrollArea>
#include <QScopedValueRollback>
#include <QSet>
#include <QSignalBlocker>
#include <QSizePolicy>
#include <QSpinBox>
#include <QStringList>
#include <QStyle>
#include <QStyleOptionButton>
#include <QTabBar>
#include <QTabWidget>
#include <QTextEdit>
#include <QTime>
#include <QTimeEdit>
#include <QUuid>
#include <QVBoxLayout>
#include <QVariantList>

#include "source/modules/pomodoro/Pomodoro/tmp_03_GerenciadorTarefas.hpp"
#include "source/modules/pomodoro/Pomodoro/GerenciadorTarefas/gtar_25_prioridade_display.hpp"
#include "source/utils/AccentTabStyle.hpp"
#include "source/utils/LinkedTitleUnderlineDelegate.hpp"
#include "source/utils/UnifiedTaskStore.hpp"

namespace source::plugin::Pomodoro::GerenciadorTarefasInternal {

inline source::utils::LogManager *logger()
{
    return source::utils::LogManager::get_logger();
}

inline QString normalized_ui_layout_key(const QString &value)
{
    return layout::normalized_ui_layout_key(value);
}

inline bool is_robust_ui_layout(const QString &value)
{
    return layout::is_robust_ui_layout(value);
}

inline QString normalized_panel_layout_key(const QString &value)
{
    return layout::normalized_panel_layout_key(value);
}

constexpr int kTaskIdRole = Qt::UserRole;
constexpr int kGroupHeaderRole = Qt::UserRole + 1;
constexpr int kGroupKeyRole = Qt::UserRole + 2;

class IndicatorOnlyCheckBox : public QCheckBox {
public:
    explicit IndicatorOnlyCheckBox(QWidget *parent = nullptr) : QCheckBox(parent) {}

protected:
    bool hitButton(const QPoint &pos) const override
    {
        try {
            QStyleOptionButton option;
            initStyleOption(&option);
            const QRect indicator_rect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &option, this);
            const int spacing = style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing, &option, this);
            const QString text = this->text();
            const int text_width = fontMetrics().horizontalAdvance(text);
            const int text_height = fontMetrics().height();
            const int text_x = indicator_rect.right() + qMax(0, spacing);
            const int text_y = (height() - text_height) / 2;
            const QRect text_rect(text_x, text_y, qMax(0, text_width), qMax(0, text_height));
            return indicator_rect.united(text_rect).contains(pos);
        } catch (...) {
            return QCheckBox::hitButton(pos);
        }
    }
};

class AutoOpenTabWidget : public QTabWidget {
public:
    explicit AutoOpenTabWidget(GerenciadorTarefas *owner, QWidget *parent = nullptr)
        : QTabWidget(parent), owner_(owner)
    {
        setAcceptDrops(true);
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override
    {
        if (event != nullptr) {
            event->acceptProposedAction();
        }
    }

    void dragMoveEvent(QDragMoveEvent *event) override
    {
        try {
            if (event == nullptr) {
                return;
            }

            const QPoint tab_pos = tabBar()->mapFrom(this, event->position().toPoint());
            const int idx = tabBar()->tabAt(tab_pos);
            if (idx >= 0 && idx != currentIndex()) {
                setCurrentIndex(idx);
            }
            event->acceptProposedAction();
        } catch (...) {
            QTabWidget::dragMoveEvent(event);
        }
    }

    void dropEvent(QDropEvent *event) override
    {
        try {
            if (event == nullptr || owner_ == nullptr) {
                return;
            }

            const QPoint tab_pos = tabBar()->mapFrom(this, event->position().toPoint());
            const int idx = tabBar()->tabAt(tab_pos);
            if (idx < 0) {
                event->ignore();
                return;
            }

            setCurrentIndex(idx);
            auto *column = dynamic_cast<KanbanColumn *>(widget(idx));
            if (column == nullptr || column->lista == nullptr) {
                event->ignore();
                return;
            }

            auto *source_list = qobject_cast<QListWidget *>(event->source());
            auto *target_list = column->lista;
            if (source_list == nullptr || source_list == target_list) {
                event->ignore();
                return;
            }

            QList<QListWidgetItem *> items = source_list->selectedItems();
            if (items.isEmpty() && source_list->currentItem() != nullptr) {
                items.append(source_list->currentItem());
            }

            for (QListWidgetItem *item : items) {
                const int row = source_list->row(item);
                if (row >= 0) {
                    if (auto *moved = source_list->takeItem(row)) {
                        target_list->addItem(moved);
                    }
                }
            }

            owner_->_sincronizar_status_pos_movimento(target_list);
            owner_->atualizar_listas();
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } catch (...) {
            QTabWidget::dropEvent(event);
        }
    }

private:
    QPointer<GerenciadorTarefas> owner_;
};

class EditTaskDialog : public QDialog {
public:
    struct Values {
        QString title;
        QString file_path;
        QString description;
        QString date_iso;
        QString time_str;
        int pomodoros = 0;
        int ciclos_totais = 4;
        double duracao = 1.0;
        QString prioridade;
        int dominio = 0;
    };

    EditTaskDialog(GerenciadorTarefas *owner,
                   const QString &initial_text,
                   const QString &initial_file,
                   const QString &initial_description,
                   const QString &initial_date,
                   const QString &initial_time,
                   int initial_pomodoros,
                   const QString &initial_priority,
                   int initial_dominio,
                   double initial_duracao = 1.0,
                   int initial_ciclos_totais = 4)
        : QDialog(owner), owner_(owner)
    {
        setModal(true);
        setWindowTitle(QCoreApplication::translate("PomodoroPlugin", "✏️ Editar Tarefa"));

        auto *layout = new QVBoxLayout(this);

        label_tarefa_ = new QLabel(QCoreApplication::translate("PomodoroPlugin", "Tarefa:"), this);
        layout->addWidget(label_tarefa_);
        title_ = new QLineEdit(this);
        title_->setText(initial_text);
        layout->addWidget(title_);

        label_prioridade_ = new QLabel(QCoreApplication::translate("PomodoroPlugin", "Prioridade:"), this);
        layout->addWidget(label_prioridade_);
        combo_prioridade_ = new QComboBox(this);
        combo_prioridade_->addItems({
            QCoreApplication::translate("PomodoroPlugin", "🔴 Importante e Urgente"),
            QCoreApplication::translate("PomodoroPlugin", "🟠 Importante, mas Nao Urgente"),
            QCoreApplication::translate("PomodoroPlugin", "🟡 Nao Importante, mas Urgente"),
            QCoreApplication::translate("PomodoroPlugin", "🟢 Nao Importante e Nao Urgente"),
        });
        const QMap<QString, int> priority_index = {
            {QStringLiteral("Importante e Urgente"), 0},
            {QStringLiteral("Importante, mas Nao Urgente"), 1},
            {QStringLiteral("Importante, mas Não Urgente"), 1},
            {QStringLiteral("Nao Importante, mas Urgente"), 2},
            {QStringLiteral("Não Importante, mas Urgente"), 2},
            {QStringLiteral("Nao Importante e Nao Urgente"), 3},
            {QStringLiteral("Não Importante e Não Urgente"), 3},
        };
        combo_prioridade_->setCurrentIndex(
            priority_index.value(Tarefa::normalizar_prioridade(initial_priority), 0));
        layout->addWidget(combo_prioridade_);

        label_dominio_ = new QLabel(QCoreApplication::translate("PomodoroPlugin", "Nivel de Dominio:"), this);
        layout->addWidget(label_dominio_);
        combo_dominio_ = new QComboBox(this);
        combo_dominio_->addItems({
            QCoreApplication::translate("PomodoroPlugin", "🥉 Iniciante (nao consigo explicar)"),
            QCoreApplication::translate("PomodoroPlugin", "🥈 Intermediario (explico com dificuldade)"),
            QCoreApplication::translate("PomodoroPlugin", "🥇 Avancado (explico facilmente)"),
        });
        combo_dominio_->setCurrentIndex(Tarefa::normalizar_dominio(initial_dominio));
        layout->addWidget(combo_dominio_);

        label_arquivo_ = new QLabel(
            QCoreApplication::translate("PomodoroPlugin", "Arquivo vinculado (arraste ou escolha):"), this);
        layout->addWidget(label_arquivo_);
        auto *file_row = new QHBoxLayout();
        file_field_ = new FileDropLineEdit(this, this);
        file_field_->setText(initial_file.isEmpty() ? QString() : QFileInfo(initial_file).fileName());
        file_field_->setToolTip(initial_file);
        connect(file_field_, &FileDropLineEdit::fileDropped, this, [this](const QString &path) {
            file_field_->setText(QFileInfo(path).fileName());
            file_field_->setToolTip(path);
        });
        file_row->addWidget(file_field_);

        btn_choose_ = new QPushButton(QCoreApplication::translate("PomodoroPlugin", "Escolher..."), this);
        connect(btn_choose_, &QPushButton::clicked, this, [this]() {
            const QString path =
                QFileDialog::getOpenFileName(this, QCoreApplication::translate("PomodoroPlugin", "Escolher arquivo"));
            if (!path.isEmpty()) {
                file_field_->setText(QFileInfo(path).fileName());
                file_field_->setToolTip(path);
            }
        });
        file_row->addWidget(btn_choose_);

        btn_clear_ = new QPushButton(QCoreApplication::translate("PomodoroPlugin", "Remover"), this);
        connect(btn_clear_, &QPushButton::clicked, this, [this]() {
            file_field_->clear();
            file_field_->setToolTip(QString());
        });
        file_row->addWidget(btn_clear_);

        btn_open_ = new QPushButton(QCoreApplication::translate("PomodoroPlugin", "Abrir"), this);
        connect(btn_open_, &QPushButton::clicked, this, [this]() {
            compat::open_local_path(file_field_->toolTip(), this);
        });
        file_row->addWidget(btn_open_);
        layout->addLayout(file_row);

        label_desc_ = new QLabel(QCoreApplication::translate("PomodoroPlugin", "Descricao (opcional):"), this);
        layout->addWidget(label_desc_);
        desc_ = new QTextEdit(this);
        desc_->setPlainText(initial_description);
        desc_->setMinimumHeight(120);
        layout->addWidget(desc_);

        auto *date_row = new QHBoxLayout();
        cb_date_ = new QCheckBox(QCoreApplication::translate("PomodoroPlugin", "Vincular data"), this);
        date_edit_ = new QDateEdit(this);
        date_edit_->setCalendarPopup(true);
        date_edit_->setLocale(compat::effective_locale(this));
        date_edit_->setDisplayFormat(compat::effective_date_format(this));
        date_edit_->setDate(QDate::currentDate());
        date_row->addWidget(cb_date_);
        date_row->addWidget(date_edit_);
        layout->addLayout(date_row);

        auto *time_row = new QHBoxLayout();
        cb_time_ = new QCheckBox(QCoreApplication::translate("PomodoroPlugin", "Vincular horario"), this);
        time_edit_ = new QTimeEdit(this);
        time_edit_->setDisplayFormat(QStringLiteral("HH:mm"));
        time_edit_->setTime(QTime::currentTime());
        time_row->addWidget(cb_time_);
        time_row->addWidget(time_edit_);
        layout->addLayout(time_row);

        cb_date_->setChecked(true);
        cb_time_->setChecked(true);

        if (!initial_date.isEmpty()) {
            const QDate date = QDate::fromString(initial_date, Qt::ISODate);
            if (date.isValid()) {
                cb_date_->setChecked(true);
                date_edit_->setDate(date);
            }
        }

        if (!initial_time.isEmpty()) {
            const QTime time = QTime::fromString(initial_time, QStringLiteral("HH:mm"));
            if (time.isValid()) {
                cb_time_->setChecked(true);
                time_edit_->setTime(time);
            }
        }

        const auto sync_time_enabled = [this]() {
            date_edit_->setEnabled(cb_date_->isChecked());
            time_edit_->setEnabled(cb_date_->isChecked() && cb_time_->isChecked());
        };
        connect(cb_date_, &QCheckBox::toggled, this, [sync_time_enabled](bool) { sync_time_enabled(); });
        connect(cb_time_, &QCheckBox::toggled, this, [sync_time_enabled](bool) { sync_time_enabled(); });
        sync_time_enabled();

        auto *pomo_row = new QHBoxLayout();
        label_pomodoros_ = new QLabel(QCoreApplication::translate("PomodoroPlugin", "Pomodoros concluidos:"), this);
        pomo_row->addWidget(label_pomodoros_);
        pomo_row->addStretch(1);
        spin_pomodoros_ = new QSpinBox(this);
        spin_pomodoros_->setMinimum(0);
        spin_pomodoros_->setMaximum(9999);
        spin_pomodoros_->setValue(qMax(0, initial_pomodoros));
        pomo_row->addWidget(spin_pomodoros_);
        layout->addLayout(pomo_row);

        auto *meta_row = new QHBoxLayout();
        label_ciclos_ = new QLabel(QCoreApplication::translate("PomodoroPlugin", "Meta de Ciclos Pomodoro (🎯):"), this);
        meta_row->addWidget(label_ciclos_);
        meta_row->addStretch(1);
        spin_ciclos_totais_ = new QSpinBox(this);
        spin_ciclos_totais_->setMinimum(1);
        spin_ciclos_totais_->setMaximum(999);
        spin_ciclos_totais_->setValue(qMax(1, initial_ciclos_totais));
        meta_row->addWidget(spin_ciclos_totais_);
        layout->addLayout(meta_row);

        auto *dur_row = new QHBoxLayout();
        label_duracao_ = new QLabel(QCoreApplication::translate("PomodoroPlugin", "Duração:"), this);
        dur_row->addWidget(label_duracao_);
        dur_row->addStretch(1);
        spin_duracao_ = new QDoubleSpinBox(this);
        spin_duracao_->setMinimum(0.0);
        spin_duracao_->setMaximum(1000.0);
        spin_duracao_->setSingleStep(0.5);
        spin_duracao_->setValue(initial_duracao > 0.0 ? initial_duracao : 1.0);
        spin_duracao_->setSuffix(QCoreApplication::translate("PomodoroPlugin", " dias"));
        dur_row->addWidget(spin_duracao_);
        layout->addLayout(dur_row);

        auto *buttons = new QHBoxLayout();
        buttons->addStretch(1);
        btn_cancel_ = new QPushButton(QCoreApplication::translate("PomodoroPlugin", "Cancelar"), this);
        btn_ok_ = new QPushButton(QCoreApplication::translate("PomodoroPlugin", "OK"), this);
        connect(btn_cancel_, &QPushButton::clicked, this, &QDialog::reject);
        connect(btn_ok_, &QPushButton::clicked, this, &QDialog::accept);
        buttons->addWidget(btn_cancel_);
        buttons->addWidget(btn_ok_);
        layout->addLayout(buttons);

    }

    Values values() const
    {
        Values data;
        data.title = title_->text().trimmed();
        data.file_path = file_field_->toolTip().trimmed();
        data.description = desc_->toPlainText().trimmed();
        data.prioridade = combo_prioridade_->currentText();
        data.dominio = combo_dominio_ != nullptr ? combo_dominio_->currentIndex() : 0;
        data.pomodoros = spin_pomodoros_->value();
        data.ciclos_totais = spin_ciclos_totais_->value();
        data.duracao = spin_duracao_->value();

        if (cb_date_->isChecked()) {
            data.date_iso = date_edit_->date().toString(Qt::ISODate);

            if (cb_time_->isChecked()) {
                data.time_str = time_edit_->time().toString(QStringLiteral("HH:mm"));
            }
        }

        return data;
    }

    void retranslateUi()
    {
        setWindowTitle(QCoreApplication::translate("PomodoroPlugin", "✏️ Editar Tarefa"));

        if (label_tarefa_ != nullptr) {
            label_tarefa_->setText(QCoreApplication::translate("PomodoroPlugin", "Tarefa:"));
        }
        if (label_prioridade_ != nullptr) {
            label_prioridade_->setText(QCoreApplication::translate("PomodoroPlugin", "Prioridade:"));
        }
        if (combo_prioridade_ != nullptr) {
            const int curr_prio = combo_prioridade_->currentIndex();
            combo_prioridade_->blockSignals(true);
            combo_prioridade_->clear();
            combo_prioridade_->addItems({
                QCoreApplication::translate("PomodoroPlugin", "🔴 Importante e Urgente"),
                QCoreApplication::translate("PomodoroPlugin", "🟠 Importante, mas Nao Urgente"),
                QCoreApplication::translate("PomodoroPlugin", "🟡 Nao Importante, mas Urgente"),
                QCoreApplication::translate("PomodoroPlugin", "🟢 Nao Importante e Nao Urgente"),
            });
            if (curr_prio >= 0 && curr_prio < combo_prioridade_->count()) {
                combo_prioridade_->setCurrentIndex(curr_prio);
            }
            combo_prioridade_->blockSignals(false);
        }
        if (label_dominio_ != nullptr) {
            label_dominio_->setText(QCoreApplication::translate("PomodoroPlugin", "Nivel de Dominio:"));
        }
        if (combo_dominio_ != nullptr) {
            const int curr_domain = combo_dominio_->currentIndex();
            combo_dominio_->blockSignals(true);
            combo_dominio_->clear();
            combo_dominio_->addItems({
                QCoreApplication::translate("PomodoroPlugin", "🥉 Iniciante (nao consigo explicar)"),
                QCoreApplication::translate("PomodoroPlugin", "🥈 Intermediario (explico com dificuldade)"),
                QCoreApplication::translate("PomodoroPlugin", "🥇 Avancado (explico facilmente)"),
            });
            if (curr_domain >= 0 && curr_domain < combo_dominio_->count()) {
                combo_dominio_->setCurrentIndex(curr_domain);
            }
            combo_dominio_->blockSignals(false);
        }
        if (label_arquivo_ != nullptr) {
            label_arquivo_->setText(QCoreApplication::translate("PomodoroPlugin", "Arquivo vinculado (arraste ou escolha):"));
        }
        if (btn_choose_ != nullptr) {
            btn_choose_->setText(QCoreApplication::translate("PomodoroPlugin", "Escolher..."));
        }
        if (btn_clear_ != nullptr) {
            btn_clear_->setText(QCoreApplication::translate("PomodoroPlugin", "Remover"));
        }
        if (btn_open_ != nullptr) {
            btn_open_->setText(QCoreApplication::translate("PomodoroPlugin", "Abrir"));
        }
        if (label_desc_ != nullptr) {
            label_desc_->setText(QCoreApplication::translate("PomodoroPlugin", "Descricao (opcional):"));
        }
        if (cb_date_ != nullptr) {
            cb_date_->setText(QCoreApplication::translate("PomodoroPlugin", "Vincular data"));
        }
        if (cb_time_ != nullptr) {
            cb_time_->setText(QCoreApplication::translate("PomodoroPlugin", "Vincular horario"));
        }
        if (label_pomodoros_ != nullptr) {
            label_pomodoros_->setText(QCoreApplication::translate("PomodoroPlugin", "Pomodoros concluidos:"));
        }
        if (label_ciclos_ != nullptr) {
            label_ciclos_->setText(QCoreApplication::translate("PomodoroPlugin", "Meta de Ciclos Pomodoro (🎯):"));
        }
        if (label_duracao_ != nullptr) {
            label_duracao_->setText(QCoreApplication::translate("PomodoroPlugin", "Duração:"));
        }
        if (spin_duracao_ != nullptr) {
            spin_duracao_->setSuffix(QCoreApplication::translate("PomodoroPlugin", " dias"));
        }
        if (btn_cancel_ != nullptr) {
            btn_cancel_->setText(QCoreApplication::translate("PomodoroPlugin", "Cancelar"));
        }
        if (btn_ok_ != nullptr) {
            btn_ok_->setText(QCoreApplication::translate("PomodoroPlugin", "OK"));
        }
        if (date_edit_ != nullptr) {
            date_edit_->setLocale(compat::effective_locale(this));
            date_edit_->setDisplayFormat(compat::effective_date_format(this));
        }
    }

protected:
    void changeEvent(QEvent *event) override
    {
        QDialog::changeEvent(event);
        if (event != nullptr && event->type() == QEvent::LanguageChange) {
            retranslateUi();
        }
    }

private:
    QPointer<GerenciadorTarefas> owner_;
    QLineEdit *title_ = nullptr;
    QComboBox *combo_prioridade_ = nullptr;
    QComboBox *combo_dominio_ = nullptr;
    FileDropLineEdit *file_field_ = nullptr;
    QTextEdit *desc_ = nullptr;
    QCheckBox *cb_date_ = nullptr;
    QDateEdit *date_edit_ = nullptr;
    QCheckBox *cb_time_ = nullptr;
    QTimeEdit *time_edit_ = nullptr;
    QSpinBox *spin_pomodoros_ = nullptr;
    QSpinBox *spin_ciclos_totais_ = nullptr;
    QDoubleSpinBox *spin_duracao_ = nullptr;

    QLabel *label_tarefa_ = nullptr;
    QLabel *label_prioridade_ = nullptr;
    QLabel *label_dominio_ = nullptr;
    QLabel *label_arquivo_ = nullptr;
    QPushButton *btn_choose_ = nullptr;
    QPushButton *btn_clear_ = nullptr;
    QPushButton *btn_open_ = nullptr;
    QLabel *label_desc_ = nullptr;
    QLabel *label_pomodoros_ = nullptr;
    QLabel *label_ciclos_ = nullptr;
    QLabel *label_duracao_ = nullptr;
    QPushButton *btn_cancel_ = nullptr;
    QPushButton *btn_ok_ = nullptr;
};

inline QDate parse_task_date(const QString &value)
{
    if (value.isEmpty()) {
        return {};
    }

    QDate date = QDate::fromString(value, QStringLiteral("yyyy-MM-dd"));
    if (!date.isValid()) {
        date = QDate::fromString(value, Qt::ISODate);
    }
    if (!date.isValid()) {
        date = QDate::fromString(value, QStringLiteral("dd/MM/yyyy"));
    }
    if (!date.isValid()) {
        date = QDate::fromString(value, QStringLiteral("MM/dd/yyyy"));
    }
    return date;
}

inline QTime parse_task_time(const QString &value)
{
    if (value.isEmpty()) {
        return {};
    }

    QTime time = QTime::fromString(value, QStringLiteral("HH:mm"));
    if (!time.isValid()) {
        time = QTime::fromString(value, QStringLiteral("HH:mm:ss"));
    }
    return time;
}

inline QString human_date_for_display(const QWidget *context, const QString &date_value)
{
    const QDate date = parse_task_date(date_value);
    return date.isValid() ? date.toString(compat::effective_date_format(context)) : date_value.trimmed();
}

inline QString visible_task_title(const QWidget *context,
                           const QString &base_text,
                           const QString &date_value,
                           const QString &time_value)
{
    QString display_text = base_text.trimmed();
    if (compat::host_bridge() == nullptr || !compat::host_bridge()->reserved_space_enabled()) {
        return display_text;
    }

    const QString human_date = human_date_for_display(context, date_value);
    if (human_date.isEmpty()) {
        return display_text;
    }

    const QString cleaned_time = time_value.trimmed();
    return cleaned_time.isEmpty() ? QStringLiteral("%1 — %2").arg(display_text, human_date)
                                  : QStringLiteral("%1 — %2 %3").arg(display_text, human_date, cleaned_time);
}

inline QString watermark_task_line(const QWidget *context, const QString &date_value, const QString &time_value)
{
    if (compat::host_bridge() == nullptr || !compat::host_bridge()->watermark_enabled()) {
        return {};
    }

    QStringList parts;
    const QString human_date = human_date_for_display(context, date_value);
    if (!human_date.trimmed().isEmpty()) {
        parts << QStringLiteral("%1: %2")
                     .arg(QCoreApplication::translate("PomodoroPlugin", "Data"), human_date.trimmed());
    }

    const QString cleaned_time = time_value.trimmed();
    if (!cleaned_time.isEmpty()) {
        parts << QStringLiteral("%1: %2")
                     .arg(QCoreApplication::translate("PomodoroPlugin", "Horario"), cleaned_time);
    }

    return parts.join(QStringLiteral("  |  "));
}

inline QString task_group_key(const QString &date_value, const QString &time_value)
{
    const QString cleaned_time = time_value.trimmed();
    if (cleaned_time.isEmpty()) {
        return {};
    }

    const QString cleaned_date = date_value.trimmed().isEmpty() ? QStringLiteral("9999-99-99") : date_value.trimmed();
    return QStringLiteral("%1|%2").arg(cleaned_date, cleaned_time);
}

inline bool is_group_header(QListWidgetItem *item)
{
    return item != nullptr && item->data(kGroupHeaderRole).toString() == QStringLiteral("group_header");
}

inline int priority_rank(const QString &value)
{
    static const QMap<QString, int> rank = {
        {QStringLiteral("Importante e Urgente"), 1},
        {QStringLiteral("Importante, mas Não Urgente"), 2},
        {QStringLiteral("Não Importante, mas Urgente"), 3},
        {QStringLiteral("Não Importante e Não Urgente"), 4},
    };
    return rank.value(Tarefa::normalizar_prioridade(value), 1);
}

inline QString task_status_for_list(const GerenciadorTarefas *self, const QListWidget *list)
{
    if (self == nullptr || list == nullptr) {
        return QStringLiteral("Todo");
    }

    if (self->col_todo != nullptr && self->col_todo->lista == list) {
        return QStringLiteral("Todo");
    }

    if (self->col_doing != nullptr && self->col_doing->lista == list) {
        return QStringLiteral("Doing");
    }

    return QStringLiteral("Done");
}

inline QString priority_from_item_text(const QString &line)
{
    if (line.startsWith(QStringLiteral("🔴"))) {
        return QStringLiteral("Importante e Urgente");
    }
    if (line.startsWith(QStringLiteral("🟠"))) {
        return QStringLiteral("Importante, mas Não Urgente");
    }
    if (line.startsWith(QStringLiteral("🟡"))) {
        return QStringLiteral("Não Importante, mas Urgente");
    }
    if (line.startsWith(QStringLiteral("🟢"))) {
        return QStringLiteral("Não Importante e Não Urgente");
    }
    return QString();
}

inline QString legacy_tasks_path()
{
    return QDir(compat::plugin_persistent_dir()).filePath(QStringLiteral("tarefas_pomodoro.json"));
}

inline QString fold_status_text(QString value)
{
    value = value.trimmed().toCaseFolded();
    value.replace(QStringLiteral("não"), QStringLiteral("nao"));
    value.replace(QStringLiteral("ã"), QStringLiteral("a"));
    value.replace(QStringLiteral("á"), QStringLiteral("a"));
    value.replace(QStringLiteral("â"), QStringLiteral("a"));
    value.replace(QStringLiteral("é"), QStringLiteral("e"));
    value.replace(QStringLiteral("ê"), QStringLiteral("e"));
    value.replace(QStringLiteral("í"), QStringLiteral("i"));
    value.replace(QStringLiteral("ó"), QStringLiteral("o"));
    value.replace(QStringLiteral("ô"), QStringLiteral("o"));
    value.replace(QStringLiteral("ú"), QStringLiteral("u"));
    value.replace(QStringLiteral("ç"), QStringLiteral("c"));
    value.replace(QLatin1Char(','), QLatin1Char(' '));
    value.replace(QLatin1Char('-'), QLatin1Char(' '));
    return value.simplified();
}

inline QString normalize_task_status(const QString &status, bool completed_hint = false)
{
    const QString folded = fold_status_text(status);
    if (folded.isEmpty()) {
        return completed_hint ? QStringLiteral("Done") : QStringLiteral("Todo");
    }

    if (folded == QStringLiteral("done") || folded == QStringLiteral("completed")
        || folded.contains(QStringLiteral("conclu")) || folded.contains(QStringLiteral("finaliz"))) {
        return QStringLiteral("Done");
    }

    if (folded == QStringLiteral("doing") || folded.contains(QStringLiteral("in progress"))
        || folded.contains(QStringLiteral("progress")) || folded.contains(QStringLiteral("progresso"))
        || folded.contains(QStringLiteral("andamento"))) {
        return QStringLiteral("Doing");
    }

    if (folded == QStringLiteral("todo") || folded == QStringLiteral("to do")
        || folded == QStringLiteral("afazer") || folded == QStringLiteral("a fazer")
        || folded == QStringLiteral("pending") || folded == QStringLiteral("pendente")) {
        return QStringLiteral("Todo");
    }

    return completed_hint ? QStringLiteral("Done") : QStringLiteral("Todo");
}

inline bool is_done_status(const QString &status)
{
    return status.compare(QStringLiteral("Done"), Qt::CaseInsensitive) == 0
        || status.compare(QStringLiteral("Completed"), Qt::CaseInsensitive) == 0;
}

inline QStringList steps_from_variant_list(const QVariantList &steps)
{
    QStringList result;
    result.reserve(steps.size());
    for (const QVariant &value : steps) {
        result.append(value.toString());
    }
    return result;
}

inline QVariantMap unified_record_from_task(const Tarefa &task)
{
    const int prioridade = priority_rank(task.prioridade);
    const QString status = normalize_task_status(task.status);
    QVariantList etapas;
    etapas.reserve(task.etapas.size());
    for (const QString &etapa : task.etapas) {
        etapas.append(etapa);
    }

    return source::utils::UnifiedTaskStore::normalize_record({
        {QStringLiteral("id"), task.id},
        {QStringLiteral("modulo"), QStringLiteral("pomodoro")},
        {QStringLiteral("tipo"), QStringLiteral("tarefa")},
        {QStringLiteral("titulo"), task.titulo},
        {QStringLiteral("descricao"), task.descricao},
        {QStringLiteral("prioridade"), prioridade},
        {QStringLiteral("quadrante"), prioridade},
        {QStringLiteral("categoria"), QStringLiteral("kanban")},
        {QStringLiteral("dominio"), task.dominio},
        {QStringLiteral("status"), status},
        {QStringLiteral("concluida"), is_done_status(status)},
        {QStringLiteral("data"), task.date},
        {QStringLiteral("hora"), task.time},
        {QStringLiteral("arquivo_vinculado"), task.file_path},
        {QStringLiteral("cor_texto"), task.cor_texto},
        {QStringLiteral("integracao_origem"), task.integracao_origem},
        {QStringLiteral("data_criacao"), task.data_criacao},
        {QStringLiteral("etapas"), etapas},
        {QStringLiteral("pomodoros"), task.pomodoros_completados},
        {QStringLiteral("pomodoros_completados"), task.pomodoros_completados},
        {QStringLiteral("ciclos_totais"), task.ciclos_totais > 0 ? task.ciclos_totais : 4},
        {QStringLiteral("ciclos_permitidos"), task.ciclos_totais > 0 ? task.ciclos_totais : 4},
        {QStringLiteral("font_config"), task.font_config},
        {QStringLiteral("sincronizada_com_eisenhower"), task.sincronizada_com_eisenhower},
        {QStringLiteral("duracao"), task.duracao > 0.0 ? task.duracao : 1.0},
        {QStringLiteral("duration"), task.duracao > 0.0 ? task.duracao : 1.0},
    });
}

inline std::optional<Tarefa> task_from_unified_record(const QVariantMap &record)
{
    const QVariantMap normalized = source::utils::UnifiedTaskStore::normalize_record(record);
    if (normalized.value(QStringLiteral("modulo")).toString().compare(QStringLiteral("pomodoro"), Qt::CaseInsensitive)
        != 0) {
        return std::nullopt;
    }

    const QString title = normalized.value(QStringLiteral("titulo")).toString().trimmed();
    if (title.isEmpty()) {
        return std::nullopt;
    }

    const QString prioridade =
        Tarefa::normalizar_prioridade(GerenciadorTarefasFns::prioridade_para_texto(normalized.value(QStringLiteral("prioridade"))));
    const QString status = normalize_task_status(normalized.value(QStringLiteral("status")).toString(),
                                                 normalized.value(QStringLiteral("concluida")).toBool());

    Tarefa task(title,
                normalized.value(QStringLiteral("descricao")).toString(),
                steps_from_variant_list(normalized.value(QStringLiteral("etapas")).toList()),
                prioridade,
                status,
                normalized.value(QStringLiteral("id")).toString(),
                normalized.value(QStringLiteral("arquivo_vinculado")).toString(),
                normalized.value(QStringLiteral("data")).toString(),
                normalized.value(QStringLiteral("hora")).toString(),
                Tarefa::normalizar_dominio(normalized.value(QStringLiteral("dominio"))),
                normalized.value(QStringLiteral("cor_texto")).toString(),
                normalized.value(QStringLiteral("integracao_origem")).toString());
    task.pomodoros_completados = normalized.value(QStringLiteral("pomodoros"),
                                                  normalized.value(QStringLiteral("pomodoros_completados"), 0)).toInt();
    task.ciclos_totais = qMax(1, normalized.value(QStringLiteral("ciclos_totais"),
                                      normalized.value(QStringLiteral("ciclos_permitidos"),
                                      normalized.value(QStringLiteral("total_ciclos"), 4))).toInt());
    task.sincronizada_com_eisenhower = normalized.value(QStringLiteral("sincronizada_com_eisenhower")).toBool();
    task.font_config = normalized.value(QStringLiteral("font_config")).toMap();
    const double task_dur = normalized.contains(QStringLiteral("duracao"))
                                ? normalized.value(QStringLiteral("duracao")).toDouble()
                                : (normalized.contains(QStringLiteral("duration"))
                                       ? normalized.value(QStringLiteral("duration")).toDouble()
                                       : 1.0);
    task.duracao = task_dur > 0.0 ? task_dur : 1.0;
    return task;
}

inline QVector<Tarefa> tasks_from_records(const QVariantList &records)
{
    QVector<Tarefa> tasks;
    tasks.reserve(records.size());
    for (const QVariant &value : records) {
        const auto maybe_task = task_from_unified_record(value.toMap());
        if (maybe_task.has_value()) {
            tasks.append(maybe_task.value());
        }
    }
    return tasks;
}

inline QVariantMap sort_payload_from_task(const Tarefa &task)
{
    return {
        {QStringLiteral("id"), task.id},
        {QStringLiteral("text"), task.titulo},
        {QStringLiteral("titulo"), task.titulo},
        {QStringLiteral("date"), task.date},
        {QStringLiteral("data"), task.date},
        {QStringLiteral("time"), task.time},
        {QStringLiteral("hora"), task.time},
        {QStringLiteral("priority"), priority_rank(task.prioridade)},
        {QStringLiteral("prioridade"), task.prioridade},
    };
}

inline bool has_host_task_sort()
{
    return compat::host_bridge() != nullptr && compat::host_bridge()->has_active_task_sort();
}

inline bool host_task_sort_less(const Tarefa *lhs, const Tarefa *rhs)
{
    if (lhs == nullptr || rhs == nullptr || compat::host_bridge() == nullptr) {
        return false;
    }

    return compat::host_bridge()->task_sort_less(sort_payload_from_task(*lhs), sort_payload_from_task(*rhs));
}

inline QVector<Tarefa> load_tasks_from_path(const QString &path)
{
    const QVariantList records = source::utils::UnifiedTaskStore::load_records(path);
    const bool sync_enabled = compat::host_bridge() == nullptr || compat::host_bridge()->task_sync_enabled();
    QVariantList pomodoro_records = sync_enabled
        ? source::utils::UnifiedTaskStore::project_records_for_module(records, QStringLiteral("pomodoro"))
        : source::utils::UnifiedTaskStore::filter_records_by_module(records, QStringLiteral("pomodoro"));
    if (pomodoro_records.isEmpty() && sync_enabled) {
        pomodoro_records = source::utils::UnifiedTaskStore::filter_records_by_module(records, QStringLiteral("pomodoro"));
    }
    return tasks_from_records(pomodoro_records);
}

inline QVariantList records_from_tasks(const QVector<Tarefa> &tasks)
{
    QVariantList records;
    records.reserve(tasks.size());
    for (const Tarefa &task : tasks) {
        records.append(unified_record_from_task(task));
    }
    return records;
}

}  // namespace source::plugin::Pomodoro::GerenciadorTarefasInternal
