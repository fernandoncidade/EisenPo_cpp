#include "source/modules/feynman/Feynman/fey_03_setup_ui.hpp"

#include "source/utils/LinkedTitleUnderlineDelegate.hpp"

#include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include "source/modules/feynman/Feynman/fey_08_editar_rotulos_dialog.hpp"
#include <QProxyStyle>
#include <QPushButton>
#include <QScrollArea>
#include <QSizePolicy>
#include <QSplitter>
#include <QStyleOption>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

#include "source/modules/feynman/Feynman/feynman_metodo_helpers.hpp"
#include "source/utils/AccentTabStyle.hpp"

namespace source::plugin::Feynman {
namespace {

class BorderlessReportTabPaneStyle : public QProxyStyle {
public:
	using QProxyStyle::QProxyStyle;

	void drawPrimitive(PrimitiveElement element,
					   const QStyleOption *option,
					   QPainter *painter,
					   const QWidget *widget = nullptr) const override
	{
		if (element == PE_FrameTabWidget) {
			return;
		}

		QProxyStyle::drawPrimitive(element, option, painter, widget);
	}

	QRect subElementRect(SubElement element,
						 const QStyleOption *option,
						 const QWidget *widget = nullptr) const override
	{
		QRect rect = QProxyStyle::subElementRect(element, option, widget);
		if ((element == SE_TabWidgetTabPane || element == SE_TabWidgetTabContents) && option != nullptr
			&& qobject_cast<const QTabWidget *>(widget) != nullptr) {
			rect.setBottom(option->rect.bottom());
		}
		return rect;
	}
};

}  // namespace

void MetodoFeynman::setup_ui()
{

	auto *layout = new QVBoxLayout(this);
	layout->setContentsMargins(12, 12, 12, 12);
	layout->setSpacing(8);

	menu_bar_ = new QMenuBar(this);
	menu_configuracoes_ = menu_bar_->addMenu(compat::tr(this, QStringLiteral("Configurações"), QStringLiteral("Settings")));
	acao_editar_rotulos_ = menu_configuracoes_->addAction(compat::tr(this, QStringLiteral("Editar Rótulos"), QStringLiteral("Edit Labels")));
	connect(acao_editar_rotulos_, &QAction::triggered, this, [this]() {
		EditarRotulosDialog dialog(this);
		connect(&dialog, &EditarRotulosDialog::labels_changed, this, [this]() {
			atualizar_traducoes();
			atualizar_aba_descricao();
		});
		dialog.exec();
	});
	layout->setMenuBar(menu_bar_);
	
	input_titulo = create_placeholder_text_edit(this);
	input_titulo->setObjectName(QStringLiteral("feynman_user_input"));
	configure_compact_scroll_area(input_titulo);
	texto_explicacao = create_placeholder_text_edit(this);
	texto_explicacao->setObjectName(QStringLiteral("feynman_user_input"));
	configure_compact_scroll_area(texto_explicacao);
	texto_lacunas = create_placeholder_text_edit(this);
	texto_lacunas->setObjectName(QStringLiteral("feynman_user_input"));
	configure_compact_scroll_area(texto_lacunas);
	texto_revisao = create_placeholder_text_edit(this);
	texto_revisao->setObjectName(QStringLiteral("feynman_user_input"));
	configure_compact_scroll_area(texto_revisao);
	texto_descricao = create_placeholder_text_edit(this);
	texto_descricao->setObjectName(QStringLiteral("feynman_user_input"));
	configure_compact_scroll_area(texto_descricao);

	const QList<QTextEdit *> placeholder_editors = {
		input_titulo,
		texto_explicacao,
		texto_lacunas,
		texto_revisao,
		texto_descricao,
	};
	for (QTextEdit *editor : placeholder_editors) {
		if (editor == nullptr) {
			continue;
		}
		editor->installEventFilter(this);
		if (editor->viewport() != nullptr) {
			editor->viewport()->installEventFilter(this);
		}
	}

	QString layout_key = compat::host_bridge() != nullptr ? compat::host_bridge()->ui_layout_mode_key() : QStringLiteral("vertical");

	frame_conceito = create_report_frame(nullptr, nullptr, input_titulo);
	frame_explique = create_report_frame(nullptr, nullptr, texto_explicacao);
	frame_lacunas = create_report_frame(nullptr, nullptr, texto_lacunas);
	frame_revisao = create_report_frame(nullptr, nullptr, texto_revisao);
	frame_descricao = create_report_frame(nullptr, nullptr, texto_descricao);

	if (layout_key == QStringLiteral("tabs")) {
		auto *tabs = new QTabWidget(this);
		tabs->setDocumentMode(true);
		
		tabs->addTab(frame_conceito, QString());
		tabs->addTab(frame_explique, QString());
		tabs->addTab(frame_lacunas, QString());
		tabs->addTab(frame_revisao, QString());
		tabs->addTab(frame_descricao, QString());

		layout->addWidget(tabs, 1);
	} else {
		auto *scroll = new QScrollArea(this);
		scroll->setWidgetResizable(true);
		scroll->setFrameShape(QFrame::NoFrame);
		scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

		auto *content = new QWidget(scroll);
		QBoxLayout *reports_layout = nullptr;
		if (layout_key == QStringLiteral("horizontal")) {
			reports_layout = new QHBoxLayout(content);
		} else {
			reports_layout = new QVBoxLayout(content);
		}
		reports_layout->setContentsMargins(0, 0, 0, 0);
		reports_layout->setSpacing(6);

		const QList<QGroupBox *> frames = {
			frame_conceito, frame_explique, frame_lacunas, frame_revisao, frame_descricao};
		for (QGroupBox *frame : frames) {
			if (frame != nullptr) {
				frame->setMinimumHeight(150);
				reports_layout->addWidget(frame, 1);
			}
		}

		scroll->setWidget(content);
		layout->addWidget(scroll, 1);
	}

	// Footer
	label_prioridade = new QLabel(this);
	configure_compact_label(label_prioridade);
	label_prioridade->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	combo_prioridade = new QComboBox(this);
	label_nivel = new QLabel(this);
	configure_compact_label(label_nivel);
	label_nivel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	combo_dominio = new QComboBox(this);

	btn_salvar = gerenciador_botoes_.create_button();
	connect(btn_salvar, &QPushButton::clicked, this, &MetodoFeynman::salvar_conceito_atual);
	
	btn_limpar = gerenciador_botoes_.create_button();
	connect(btn_limpar, &QPushButton::clicked, this, &MetodoFeynman::limpar_campos);

	btn_fechar = gerenciador_botoes_.create_button();
	connect(btn_fechar, &QPushButton::clicked, this, &QDialog::close);

	auto *footer_layout = new QHBoxLayout();
	footer_layout->setContentsMargins(0, 0, 0, 0);
	footer_layout->setSpacing(6);
	footer_layout->addStretch();
	footer_layout->addWidget(label_prioridade);
	footer_layout->addWidget(combo_prioridade);
	footer_layout->addWidget(label_nivel);
	footer_layout->addWidget(combo_dominio);
	footer_layout->addWidget(btn_salvar);
	if (btn_limpar != nullptr) {
		footer_layout->addWidget(btn_limpar);
	}
	if (btn_fechar != nullptr) {
		footer_layout->addWidget(btn_fechar);
	}

	layout->addLayout(footer_layout);

	if (auto *app = QApplication::instance()) {
		app->installEventFilter(this);
	}
	if (description_debounce_timer_ == nullptr) {
		description_debounce_timer_ = new QTimer(this);
		description_debounce_timer_->setSingleShot(true);
		description_debounce_timer_->setInterval(300);
		connect(description_debounce_timer_, &QTimer::timeout, this, [this]() {
			atualizar_aba_descricao();
			atualizar_placeholders_dinamicos();
		});
	}

	const auto sync_description_from_fields = [this]() {
		atualizar_placeholders_dinamicos();
		if (description_debounce_timer_ != nullptr) {
			description_debounce_timer_->start();
		} else {
			atualizar_aba_descricao();
		}
	};
	connect(input_titulo, &QTextEdit::textChanged, this, sync_description_from_fields);
	connect(texto_explicacao, &QTextEdit::textChanged, this, sync_description_from_fields);
	connect(texto_lacunas, &QTextEdit::textChanged, this, sync_description_from_fields);
	connect(texto_revisao, &QTextEdit::textChanged, this, sync_description_from_fields);
	connect(texto_descricao, &QTextEdit::textChanged, this, [this]() {
		if (sincronizando_descricao_ || texto_descricao == nullptr) {
			return;
		}

		descricao_editada_manualmente_ =
			texto_descricao->toPlainText().trimmed() != descricao_gerada_atual_.trimmed();

		if (descricao_editada_manualmente_) {
			DescriptionParts parts = parse_description_text(texto_descricao->toPlainText());
			
			if (!parts.structured && !texto_descricao->toPlainText().trimmed().isEmpty()) {
			    parts.concept_text = texto_descricao->toPlainText().trimmed();
			    parts.structured = true;
			}

			if (parts.structured) {
				sincronizando_descricao_ = true;
				if (input_titulo != nullptr && input_titulo->toPlainText() != parts.concept_text) {
					input_titulo->setPlainText(parts.concept_text);
				}
				if (texto_explicacao != nullptr && texto_explicacao->toPlainText() != parts.explanation) {
					texto_explicacao->setPlainText(parts.explanation);
				}
				if (texto_lacunas != nullptr && texto_lacunas->toPlainText() != parts.gaps) {
					texto_lacunas->setPlainText(parts.gaps);
				}
				if (texto_revisao != nullptr && texto_revisao->toPlainText() != parts.review) {
					texto_revisao->setPlainText(parts.review);
				}
				descricao_gerada_atual_ = composed_description(this, 
					input_titulo != nullptr ? input_titulo->toPlainText() : QString(),
					texto_explicacao != nullptr ? texto_explicacao->toPlainText() : QString(),
					texto_lacunas != nullptr ? texto_lacunas->toPlainText() : QString(),
					texto_revisao != nullptr ? texto_revisao->toPlainText() : QString());
				sincronizando_descricao_ = false;
			}
		}

		if (texto_descricao->toPlainText().trimmed().isEmpty()) {
			atualizar_aba_descricao();
		}
		atualizar_placeholders_dinamicos();
	});

	sync_backgrounds(*this);
	atualizar_traducoes();
	
	QTimer::singleShot(0, this, [this]() {
		ajustar_largura_combo_prioridade();
		ajustar_largura_combo_dominio();
		ajustar_altura_combos_footer();
		atualizar_placeholders_dinamicos();
	});
}

void MetodoFeynman::atualizar_placeholders_dinamicos()
{
	const QList<QTextEdit *> placeholder_editors = {
		input_titulo,
		texto_explicacao,
		texto_lacunas,
		texto_revisao,
		texto_descricao,
	};
	for (QTextEdit *editor : placeholder_editors) {
		if (editor == nullptr) {
			continue;
		}
		if (editor->viewport() != nullptr) {
			editor->viewport()->update();
		} else {
			editor->update();
		}
	}
}


}  // namespace source::plugin::Feynman

namespace source::plugin::Feynman::MetodoFeynmanFns {
void setup_ui(MetodoFeynman &self) { self.setup_ui(); }
}
