#pragma once

#include <QCheckBox>
#include <QString>
#include <QWidget>

#include "source/modules/pomodoro/Pomodoro/pomodoro_support.hpp"
#include "source/modules/pomodoro/Pomodoro/PomodoroTimer/pomo_14_aviso_metas_ciclos.hpp"

class QLabel;
class QComboBox;
class QCheckBox;
class QGroupBox;
class QProgressBar;
class QPushButton;
class QTimer;

namespace source::plugin::Pomodoro {

class PomodoroTimer : public QWidget {
    Q_OBJECT

public:
    explicit PomodoroTimer(QWidget *parent = nullptr);
    PomodoroTimer(const QString &ui_layout_mode_key, QWidget *parent);
    ~PomodoroTimer() override;

    void setup_ui();
    void atualizar_traducoes();
    void atualizar_configuracoes();
    void toggle_timer();
    void atualizar_timer();
    void atualizar_display();
    void _executar_alerta(const QString &titulo, const QString &texto, bool fechar_automaticamente = false);
    void finalizar_ciclo();
    void resetar_timer();
    void resetar_ciclo();
    void pular_ciclo();
    void _aplicar_tema_dinamico();

    bool eventFilter(QObject *obj, QEvent *event) override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    using DiscrepancyCollector = std::function<QVector<DiscrepanteInfo>(int)>;
    using DiscrepancyResetter = std::function<void(int)>;

    void set_discrepancy_collector(DiscrepancyCollector collector);
    void set_discrepancy_resetter(DiscrepancyResetter resetter);
    bool verificar_discrepancias_metas();
    void set_ciclos_permitidos(int cycles);

protected:
    void changeEvent(QEvent *event) override;

signals:
    void ciclo_completado(const QString &tipo);
    void ciclos_configurados_changed(int cycles);

public:
    source::utils::LogManager *logger_ = nullptr;
    int tempo_foco = 25 * 60;
    int tempo_descanso_curto = 5 * 60;
    int tempo_descanso_longo = 15 * 60;
    int ciclos_permitidos = 4;
    int tempo_restante = 25 * 60;
    bool em_foco = true;
    int ciclos_completados = 0;
    bool timer_ativo = false;

    bool is_modo_automatico() const
    {
        return (chk_modo_automatico != nullptr && chk_modo_automatico->isChecked()) ||
               (sound_mgr_ != nullptr && sound_mgr_->get_automatic_mode_enabled());
    }

    compat::GerenciadorBotoesUI gerenciador_botoes_;
    compat::SoundManager *sound_mgr_ = nullptr;

    QTimer *timer = nullptr;
    QGroupBox *timer_group = nullptr;
    QLabel *label_tempo = nullptr;
    QProgressBar *progress = nullptr;
    QLabel *label_status = nullptr;
    QPushButton *btn_iniciar = nullptr;
    QPushButton *btn_resetar = nullptr;
    QPushButton *btn_resetar_ciclo = nullptr;
    QPushButton *btn_pular = nullptr;
    QGroupBox *config_group = nullptr;
    QLabel *label_foco = nullptr;
    QComboBox *spin_foco = nullptr;
    QLabel *label_descanso = nullptr;
    QComboBox *spin_descanso = nullptr;
    QLabel *label_ciclos = nullptr;
    QComboBox *spin_ciclos = nullptr;
    QCheckBox *chk_modo_automatico = nullptr;
    int input_ref_height_ = 0;
    QString ui_layout_mode_key_ = QStringLiteral("simple:tabs");
    bool discrepancia_verificada_ = false;
    DiscrepancyCollector discrepancy_collector_;
    DiscrepancyResetter discrepancy_resetter_;
};

}  // namespace source::plugin::Pomodoro
