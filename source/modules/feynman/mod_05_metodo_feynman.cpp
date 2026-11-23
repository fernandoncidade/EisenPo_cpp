#include "source/modules/feynman/mod_05_metodo_feynman.hpp"

#include <algorithm>

#include <QApplication>
#include <QComboBox>
#include <QCoreApplication>
#include <QEvent>
#include <QFont>
#include <QFontMetrics>
#include <QGroupBox>
#include <QLabel>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QResizeEvent>
#include <QSignalBlocker>
#include <QSizePolicy>
#include <QSplitter>
#include <QSplitterHandle>
#include <QTabBar>
#include <QTabWidget>
#include <QTextEdit>
#include <QThread>
#include <QTimer>

#include "source/modules/feynman/Feynman/fey_01_FeynmanWorker.hpp"
#include "source/modules/feynman/Feynman/feynman_metodo_helpers.hpp"
#include "source/utils/UnifiedTaskStore.hpp"

namespace source::plugin::Feynman {

MetodoFeynman::MetodoFeynman(QWidget *parent)
    : QDialog(parent),
      logger_(source::utils::LogManager::get_logger()),
      gerenciador_botoes_(this)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowTitle(compat::tr(this, QStringLiteral("Descrição Feynman"), QStringLiteral("Feynman Description")));
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    setup_ui();
}

MetodoFeynman::~MetodoFeynman()
{
    if (auto *app = QApplication::instance()) {
        app->removeEventFilter(this);
    }
}



void MetodoFeynman::atualizar_aba_descricao()
{
    if (texto_descricao == nullptr || sincronizando_descricao_) {
        return;
    }

    const QString descricao_gerada = composed_description(this,
                                                          input_titulo != nullptr ? input_titulo->toPlainText() : QString(),
                                                          texto_explicacao != nullptr ? texto_explicacao->toPlainText() : QString(),
                                                          texto_lacunas != nullptr ? texto_lacunas->toPlainText() : QString(),
                                                          texto_revisao != nullptr ? texto_revisao->toPlainText() : QString());
    QString descricao;
    const DescriptionParts current_parts = parse_description_text(texto_descricao->toPlainText());
    const QString current_preamble = current_parts.preamble;

    const bool is_external = has_external_description_base(task_context_) || (has_conceito_atual_ && has_external_description_base(conceito_atual_));
    if (is_external) {
        QString descricao_base = current_preamble;
        if (descricao_base.trimmed().isEmpty()) {
            if (has_conceito_atual_) {
                descricao_base = parse_description_text(conceito_atual_.value(QStringLiteral("descricao")).toString()).preamble;
            } else {
                descricao_base = parse_description_text(task_context_.value(QStringLiteral("descricao")).toString()).preamble;
            }
        }
        descricao = description_with_external_base(descricao_base, descricao_gerada).trimmed();
    } else {
        if (!current_preamble.isEmpty()) {
            descricao = QStringLiteral("%1\n\n%2").arg(current_preamble, descricao_gerada);
        } else {
            descricao = descricao_gerada;
        }
    }
    if (texto_descricao->toPlainText() == descricao && descricao_gerada_atual_ == descricao_gerada) {
        return;
    }

    sincronizando_descricao_ = true;
    texto_descricao->setPlainText(descricao);
    descricao_gerada_atual_ = descricao_gerada;
    descricao_editada_manualmente_ = descricao != descricao_gerada.trimmed();
    sincronizando_descricao_ = false;
}

bool MetodoFeynman::eventFilter(QObject *obj, QEvent *event)
{
    if (event != nullptr) {
        const QEvent::Type type = event->type();
        const bool is_theme_event =
            type == QEvent::ApplicationPaletteChange || type == QEvent::PaletteChange || type == QEvent::StyleChange
            || type == QEvent::ThemeChange;
        const bool is_placeholder_event = type == QEvent::Resize || type == QEvent::Show
            || type == QEvent::LayoutRequest || type == QEvent::FontChange;
        const bool watches_object = obj == QApplication::instance();
        const bool watches_placeholder =
            obj == input_titulo || obj == texto_explicacao || obj == texto_lacunas || obj == texto_revisao
            || obj == texto_descricao
            || (input_titulo != nullptr && obj == input_titulo->viewport())
            || (texto_explicacao != nullptr && obj == texto_explicacao->viewport())
            || (texto_lacunas != nullptr && obj == texto_lacunas->viewport())
            || (texto_revisao != nullptr && obj == texto_revisao->viewport())
            || (texto_descricao != nullptr && obj == texto_descricao->viewport());
        if (watches_object && is_theme_event) {
            if (theme_debounce_timer_ == nullptr) {
                theme_debounce_timer_ = new QTimer(this);
                theme_debounce_timer_->setSingleShot(true);
                theme_debounce_timer_->setInterval(50);
                connect(theme_debounce_timer_, &QTimer::timeout, this, [this]() {
                    sync_backgrounds(*this);
                });
            }
            theme_debounce_timer_->start();
        }
        if (obj == QApplication::instance() && type == QEvent::ApplicationFontChange) {
            atualizar_placeholders_dinamicos();
        }
        if (watches_placeholder && is_placeholder_event) {
            atualizar_placeholders_dinamicos();
        }
    }

    return QWidget::eventFilter(obj, event);
}

void MetodoFeynman::ajustar_largura_combo_dominio()
{
    if (combo_dominio == nullptr) {
        return;
    }

    QFontMetrics metrics(combo_dominio->font());
    int max_width = 0;
    for (int i = 0; i < combo_dominio->count(); ++i) {
        max_width = std::max(max_width, metrics.horizontalAdvance(combo_dominio->itemText(i)));
    }

    combo_dominio->setFixedWidth(max_width + 48);
}

void MetodoFeynman::ajustar_largura_combo_prioridade()
{
    if (combo_prioridade == nullptr) {
        return;
    }

    QFontMetrics metrics(combo_prioridade->font());
    int max_width = 0;
    for (int i = 0; i < combo_prioridade->count(); ++i) {
        max_width = std::max(max_width, metrics.horizontalAdvance(combo_prioridade->itemText(i)));
    }

    combo_prioridade->setFixedWidth(max_width + 48);
}

void MetodoFeynman::ajustar_altura_combos_footer()
{
    if ((combo_dominio == nullptr && combo_prioridade == nullptr) || btn_salvar == nullptr) {
        return;
    }

    const int button_height = std::max(btn_salvar->sizeHint().height(), btn_salvar->minimumSizeHint().height());
    const int combo_height = std::max(button_height, 24);
    if (combo_prioridade != nullptr) {
        combo_prioridade->setFixedHeight(combo_height);
    }
    if (combo_dominio != nullptr) {
        combo_dominio->setFixedHeight(combo_height);
    }
}



}  // namespace source::plugin::Feynman
