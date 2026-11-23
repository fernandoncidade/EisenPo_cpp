#pragma once

#include <QDialog>
#include <QPoint>
#include <QPointer>
#include <QStringList>
#include <QTimer>
#include <QVariantList>
#include <QVariantMap>

#include "source/modules/feynman/Feynman/feynman_support.hpp"

class QFrame;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QTextEdit;
class QComboBox;
class QTabWidget;
class QThread;
class QSplitter;
class QResizeEvent;
class QAction;
class QMenu;
class QMenuBar;

namespace source::plugin::Feynman {

class FeynmanWorker;

class MetodoFeynman final : public QDialog {
    Q_OBJECT

public:
    explicit MetodoFeynman(QWidget *parent = nullptr);
    ~MetodoFeynman() override;


    void setup_ui();
    void atualizar_traducoes();
    void salvar_conceito_atual();
    void limpar_campos();
    void set_task_context(const QVariantMap& task);
    QVariantMap get_task_context() const { return task_context_; }
    
    // Unused methods removed
    void atualizar_aba_descricao();
    void cleanup();
    bool eventFilter(QObject *obj, QEvent *event) override;

    void ajustar_largura_combo_dominio();
    void ajustar_largura_combo_prioridade();
    void ajustar_altura_combos_footer();
    void atualizar_largura_minima_painel_esquerdo();
    void atualizar_largura_minima_painel_direito();
    void atualizar_dimensoes_minimas_iniciais();

signals:
    void task_counter_changed();

public:
    source::utils::LogManager *logger_ = nullptr;
    QString arquivo_conceitos_;
    QVariantMap conceito_atual_;
    bool sincronizando_descricao_ = false;
    bool descricao_editada_manualmente_ = false;
    QString descricao_gerada_atual_;
    bool has_conceito_atual_ = false;
    compat::GerenciadorBotoesUI gerenciador_botoes_;
    FeynmanWorker *feynman_worker_ = nullptr;
    QThread *feynman_thread_ = nullptr;

    QVariantMap task_context_;
    QLabel *label_conceito = nullptr;
    QGroupBox *frame_conceito = nullptr;
    QTextEdit *input_titulo = nullptr;
    QLabel *label_passo1 = nullptr;
    QGroupBox *frame_explique = nullptr;
    QTextEdit *texto_explicacao = nullptr;
    QLabel *label_passo2 = nullptr;
    QGroupBox *frame_lacunas = nullptr;
    QTextEdit *texto_lacunas = nullptr;
    QLabel *label_passo3 = nullptr;
    QGroupBox *frame_revisao = nullptr;
    QTextEdit *texto_revisao = nullptr;
    QLabel *label_descricao = nullptr;
    QGroupBox *frame_descricao = nullptr;
    QTextEdit *texto_descricao = nullptr;
    QLabel *label_prioridade = nullptr;
    QComboBox *combo_prioridade = nullptr;
    QLabel *label_nivel = nullptr;
    QComboBox *combo_dominio = nullptr;
    QPushButton *btn_salvar = nullptr;
    QPushButton *btn_limpar = nullptr;
    QPushButton *btn_fechar = nullptr;
    QMenuBar *menu_bar_ = nullptr;
    QMenu *menu_configuracoes_ = nullptr;
    QAction *acao_editar_rotulos_ = nullptr;
    QTimer *theme_debounce_timer_ = nullptr;
    QTimer *description_debounce_timer_ = nullptr;
    void atualizar_placeholders_dinamicos();
    
};

}  // namespace source::plugin::Feynman
