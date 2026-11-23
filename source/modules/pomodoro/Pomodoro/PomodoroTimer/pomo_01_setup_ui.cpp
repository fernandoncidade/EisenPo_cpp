#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_01_setup_ui.hpp"

#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_internal.hpp"

namespace source::plugin::Pomodoro {
using namespace PomodoroTimerInternal;

void PomodoroTimer::setup_ui()
{
    try {
        auto *layout = new QVBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(6);
        layout->setSizeConstraint(QLayout::SetMinimumSize);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        timer_group = new QGroupBox(QCoreApplication::translate("PomodoroPlugin", "Temporizador"), this);
        timer_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        auto *timer_layout = new QVBoxLayout(timer_group);
        timer_layout->setContentsMargins(6, 12, 6, 6);
        timer_layout->setSpacing(6);

        label_tempo = new QLabel(QStringLiteral("25:00"), timer_group);
        label_tempo->setFont(QFont(QStringLiteral("Arial"), 40, QFont::Bold));
        label_tempo->setAlignment(Qt::AlignCenter);
        label_tempo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        timer_layout->addWidget(label_tempo);

        progress = new QProgressBar(timer_group);
        progress->setMaximum(tempo_foco);
        progress->setValue(tempo_foco);
        progress->setTextVisible(false);
        progress->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        timer_layout->addWidget(progress);

        label_status = new QLabel(timer_group);
        label_status->setFont(QFont(QStringLiteral("Arial"), 12, QFont::Bold));
        label_status->setAlignment(Qt::AlignCenter);
        label_status->setWordWrap(true);
        label_status->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        timer_layout->addWidget(label_status);

        layout->addWidget(timer_group);

        const bool robust_layout = is_robust_ui_layout(ui_layout_mode_key_);

        btn_iniciar = gerenciador_botoes_.create_button();
        connect(btn_iniciar, &QPushButton::clicked, this, &PomodoroTimer::toggle_timer);

        input_ref_height_ = qMax(1, btn_iniciar->sizeHint().height());

        btn_resetar = gerenciador_botoes_.create_button();
        connect(btn_resetar, &QPushButton::clicked, this, &PomodoroTimer::resetar_timer);

        btn_resetar_ciclo = gerenciador_botoes_.create_button();
        connect(btn_resetar_ciclo, &QPushButton::clicked, this, &PomodoroTimer::resetar_ciclo);

        btn_pular = gerenciador_botoes_.create_button();
        connect(btn_pular, &QPushButton::clicked, this, &PomodoroTimer::pular_ciclo);

        const auto fix_control_height = [this](QWidget *widget) {
            if (widget == nullptr || input_ref_height_ <= 0) {
                return;
            }

            QSizePolicy policy = widget->sizePolicy();
            policy.setVerticalPolicy(QSizePolicy::Fixed);
            widget->setSizePolicy(policy);
            widget->setFixedHeight(input_ref_height_);
        };

        for (QPushButton *button : {btn_iniciar, btn_resetar, btn_resetar_ciclo, btn_pular}) {
            fix_control_height(button);
        }

        if (robust_layout) {
            auto *controles = new QHBoxLayout();
            controles->setSpacing(6);
            controles->addWidget(btn_iniciar, 0, Qt::AlignVCenter);
            controles->addWidget(btn_resetar, 0, Qt::AlignVCenter);
            controles->addWidget(btn_resetar_ciclo, 0, Qt::AlignVCenter);
            controles->addWidget(btn_pular, 0, Qt::AlignVCenter);
            layout->addLayout(controles);
        }

        config_group = new QGroupBox(this);
        config_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        auto *config_layout = new QHBoxLayout(config_group);
        config_layout->setContentsMargins(6, 6, 6, 6);
        config_layout->setSpacing(robust_layout ? 6 : 12);

        QVBoxLayout *settings_column = nullptr;
        if (!robust_layout) {
            auto *buttons_column = new QVBoxLayout();
            buttons_column->setContentsMargins(0, 0, 0, 0);
            buttons_column->setSpacing(6);
            buttons_column->addWidget(btn_iniciar);
            buttons_column->addWidget(btn_resetar);
            buttons_column->addWidget(btn_resetar_ciclo);
            buttons_column->addWidget(btn_pular);
            buttons_column->addStretch(1);
            config_layout->addLayout(buttons_column, 0);

            settings_column = new QVBoxLayout();
            settings_column->setContentsMargins(0, 0, 0, 0);
            settings_column->setSpacing(6);
            config_layout->addStretch(1);
            config_layout->addLayout(settings_column, 0);
        }

        label_foco = new QLabel(config_group);
        label_foco->setFont(QFont(QStringLiteral("Arial"), 9, QFont::Bold));
        label_foco->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        fix_control_height(label_foco);
        if (robust_layout) {
            config_layout->addWidget(label_foco, 0, Qt::AlignRight);
        }

        spin_foco = new QComboBox(config_group);
        spin_foco->addItems({
            QStringLiteral("1"), QStringLiteral("5"), QStringLiteral("10"), QStringLiteral("15"), QStringLiteral("20"),
            QStringLiteral("25"), QStringLiteral("30"), QStringLiteral("35"), QStringLiteral("40"), QStringLiteral("45"),
            QStringLiteral("50"), QStringLiteral("55"), QStringLiteral("60"), QStringLiteral("65"), QStringLiteral("70"),
            QStringLiteral("75"), QStringLiteral("80"), QStringLiteral("85"), QStringLiteral("90"), QStringLiteral("95"),
            QStringLiteral("100"), QStringLiteral("105"), QStringLiteral("110"), QStringLiteral("115"), QStringLiteral("120"),
        });
        spin_foco->setCurrentText(QStringLiteral("25"));
        connect(spin_foco, &QComboBox::currentTextChanged, this, [this](const QString &) { atualizar_configuracoes(); });
        if (input_ref_height_ > 0) {
            spin_foco->setFixedHeight(input_ref_height_);
        }
        if (robust_layout) {
            config_layout->addWidget(spin_foco, 0, Qt::AlignLeft);
        } else {
            auto *foco_row = new QHBoxLayout();
            foco_row->setContentsMargins(0, 0, 0, 0);
            foco_row->setSpacing(6);
            foco_row->addWidget(label_foco, 0, Qt::AlignVCenter);
            foco_row->addWidget(spin_foco, 0, Qt::AlignVCenter);
            settings_column->addLayout(foco_row);
        }

        label_descanso = new QLabel(config_group);
        label_descanso->setFont(QFont(QStringLiteral("Arial"), 9, QFont::Bold));
        label_descanso->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        fix_control_height(label_descanso);
        if (robust_layout) {
            config_layout->addWidget(label_descanso, 0, Qt::AlignRight);
        }

        spin_descanso = new QComboBox(config_group);
        spin_descanso->addItems({
            QStringLiteral("1"), QStringLiteral("3"), QStringLiteral("5"), QStringLiteral("7"), QStringLiteral("10"),
            QStringLiteral("15"), QStringLiteral("20"), QStringLiteral("25"), QStringLiteral("30"), QStringLiteral("35"),
            QStringLiteral("40"), QStringLiteral("45"), QStringLiteral("50"), QStringLiteral("55"), QStringLiteral("60"),
        });
        spin_descanso->setCurrentText(QStringLiteral("5"));
        connect(spin_descanso, &QComboBox::currentTextChanged, this, [this](const QString &) { atualizar_configuracoes(); });
        if (input_ref_height_ > 0) {
            spin_descanso->setFixedHeight(input_ref_height_);
        }
        if (robust_layout) {
            config_layout->addWidget(spin_descanso, 0, Qt::AlignLeft);
        } else {
            auto *descanso_row = new QHBoxLayout();
            descanso_row->setContentsMargins(0, 0, 0, 0);
            descanso_row->setSpacing(6);
            descanso_row->addWidget(label_descanso, 0, Qt::AlignVCenter);
            descanso_row->addWidget(spin_descanso, 0, Qt::AlignVCenter);
            settings_column->addLayout(descanso_row);
        }

        label_ciclos = new QLabel(config_group);
        label_ciclos->setFont(QFont(QStringLiteral("Arial"), 9, QFont::Bold));
        label_ciclos->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        fix_control_height(label_ciclos);

        spin_ciclos = new QComboBox(config_group);
        for (int i = 1; i <= 20; ++i) {
            spin_ciclos->addItem(QString::number(i));
        }
        spin_ciclos->setCurrentText(QString::number(ciclos_permitidos));
        connect(spin_ciclos, &QComboBox::currentTextChanged, this, [this](const QString &) { atualizar_configuracoes(); });
        if (input_ref_height_ > 0) {
            spin_ciclos->setFixedHeight(input_ref_height_);
        }

        chk_modo_automatico = new QCheckBox(config_group);
        chk_modo_automatico->setChecked(sound_mgr_ != nullptr && sound_mgr_->get_automatic_mode_enabled());
        connect(chk_modo_automatico, &QCheckBox::toggled, this, [this](bool enabled) {
            if (sound_mgr_ != nullptr) {
                sound_mgr_->set_automatic_mode_enabled(enabled);
            }
        });

        if (robust_layout) {
            auto *ciclos_column = new QVBoxLayout();
            ciclos_column->setContentsMargins(0, 0, 0, 0);
            ciclos_column->setSpacing(2);

            auto *ciclos_row = new QHBoxLayout();
            ciclos_row->setContentsMargins(0, 0, 0, 0);
            ciclos_row->setSpacing(6);
            ciclos_row->addWidget(label_ciclos, 0, Qt::AlignVCenter);
            ciclos_row->addWidget(spin_ciclos, 0, Qt::AlignVCenter);
            ciclos_column->addLayout(ciclos_row);
            ciclos_column->addWidget(chk_modo_automatico, 0, Qt::AlignLeft);
            config_layout->addLayout(ciclos_column, 0);
        } else {
            auto *ciclos_row = new QHBoxLayout();
            ciclos_row->setContentsMargins(0, 0, 0, 0);
            ciclos_row->setSpacing(6);
            ciclos_row->addWidget(label_ciclos, 0, Qt::AlignVCenter);
            ciclos_row->addWidget(spin_ciclos, 0, Qt::AlignVCenter);
            settings_column->addLayout(ciclos_row);
            settings_column->addWidget(chk_modo_automatico, 0, Qt::AlignLeft);
            settings_column->addStretch(1);
        }

        layout->addWidget(config_group, 0);
        setLayout(layout);

        atualizar_traducoes();
        setMinimumHeight(layout->minimumSize().height());
        updateGeometry();
    } catch (const std::exception &e) {
        logger_->error(QStringLiteral("Erro ao configurar interface do PomodoroTimer: %1").arg(QString::fromUtf8(e.what())), true);
    } catch (...) {
        logger_->error(QStringLiteral("Erro ao configurar interface do PomodoroTimer"), true);
    }
}

}  // namespace source::plugin::Pomodoro

namespace source::plugin::Pomodoro::PomodoroTimerFns { void setup_ui(PomodoroTimer &self) { self.setup_ui(); } }
