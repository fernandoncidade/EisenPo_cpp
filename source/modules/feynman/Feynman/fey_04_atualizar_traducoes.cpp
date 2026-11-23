#include "source/modules/feynman/Feynman/fey_04_atualizar_traducoes.hpp"

#include <QAction>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>

#include "source/modules/feynman/Feynman/feynman_metodo_helpers.hpp"
#include "source/utils/FeynmanLabelsConfig.hpp"

namespace source::plugin::Feynman {

void MetodoFeynman::atualizar_traducoes()
{
	const bool is_en = compat::is_english(this);
	const QString tarefas_frame = compat::tr(this, QStringLiteral("📚 Minhas Tarefas:"), QStringLiteral("📚 My Tasks:"));

	const QString conceito_frame =
		source::utils::get_effective_feynman_label(source::utils::FeynmanLabelKey::Concept, is_en);
	const QString explique_frame =
		source::utils::get_effective_feynman_label(source::utils::FeynmanLabelKey::Explanation, is_en);
	const QString lacunas_frame =
		source::utils::get_effective_feynman_label(source::utils::FeynmanLabelKey::Gaps, is_en);
	const QString revisao_frame =
		source::utils::get_effective_feynman_label(source::utils::FeynmanLabelKey::Review, is_en);
	const QString descricao_frame =
		source::utils::get_effective_feynman_label(source::utils::FeynmanLabelKey::Description, is_en);

	const QString conceito_placeholder =
		compat::tr(this,
				   QStringLiteral("Conceito (ponto de vista sintetizado): Exemplo: Tensão de Cisalhamento."),
				   QStringLiteral("Concept (synthesized point of view): Example: Shear Stress."));
	const QString explique_placeholder =
		compat::tr(this,
				   QStringLiteral("Explique com Simplicidade (como para uma criança): Exemplo: Escreva sua explicação usando linguagem simples e exemplos do dia a dia."),
				   QStringLiteral("Explain Simply (as if to a child): Example: Write your explanation using simple language and everyday examples."));
	const QString lacunas_placeholder =
		compat::tr(this,
				   QStringLiteral("Lacunas Identificadas (o que você não conseguiu explicar bem?): Exemplo: Liste os pontos onde você travou ou ficou confuso."),
				   QStringLiteral("Identified Gaps (what could you not explain clearly?): Example: List the points where you got stuck or felt confused."));
	const QString revisao_placeholder =
		compat::tr(this,
				   QStringLiteral("Revisão e Simplificação Final (depois de estudar as lacunas): Exemplo: Reescreva sua explicação de forma ainda mais clara e completa."),
				   QStringLiteral("Final Review and Simplification (after studying the gaps): Example: Rewrite your explanation in an even clearer and more complete way."));
	const QString descricao_placeholder =
		compat::tr(this,
				   QStringLiteral("Descrição (compatível com Pomodoro e Eisenhower): Exemplo: A descrição reúne conceito, explicação, lacunas e revisão em um único texto compatível com o campo Descrição."),
				   QStringLiteral("Description (compatible with Pomodoro and Eisenhower): Example: The description joins concept, explanation, gaps, and review into a single text compatible with the Description field."));

	if (frame_conceito != nullptr) {
		frame_conceito->setTitle(conceito_frame);
	}
	if (frame_explique != nullptr) {
		frame_explique->setTitle(explique_frame);
	}
	if (frame_lacunas != nullptr) {
		frame_lacunas->setTitle(lacunas_frame);
	}
	if (frame_revisao != nullptr) {
		frame_revisao->setTitle(revisao_frame);
	}
	if (frame_descricao != nullptr) {
		frame_descricao->setTitle(descricao_frame);
	}

	if (auto *tabs = findChild<QTabWidget *>()) {
		if (frame_conceito) tabs->setTabText(tabs->indexOf(frame_conceito), conceito_frame);
		if (frame_explique) tabs->setTabText(tabs->indexOf(frame_explique), explique_frame);
		if (frame_lacunas) tabs->setTabText(tabs->indexOf(frame_lacunas), lacunas_frame);
		if (frame_revisao) tabs->setTabText(tabs->indexOf(frame_revisao), revisao_frame);
		if (frame_descricao) tabs->setTabText(tabs->indexOf(frame_descricao), descricao_frame);
	}

	set_placeholder_base_text(input_titulo, conceito_placeholder);
	set_placeholder_base_text(texto_explicacao, explique_placeholder);
	set_placeholder_base_text(texto_lacunas, lacunas_placeholder);
	set_placeholder_base_text(texto_revisao, revisao_placeholder);
	set_placeholder_base_text(texto_descricao, descricao_placeholder);
	atualizar_placeholders_dinamicos();

	label_prioridade->setText(compat::tr(this, QStringLiteral("Prioridade:"), QStringLiteral("Priority:")));
	label_nivel->setText(compat::tr(this, QStringLiteral("Nível de Domínio:"), QStringLiteral("Mastery Level:")));

	const int current_priority = normalized_priority_value(combo_prioridade->currentData());
	combo_prioridade->clear();
	combo_prioridade->addItem(priority_label(this, 1), 1);
	combo_prioridade->addItem(priority_label(this, 2), 2);
	combo_prioridade->addItem(priority_label(this, 3), 3);
	combo_prioridade->addItem(priority_label(this, 4), 4);
	const int priority_index = combo_prioridade->findData(current_priority);
	combo_prioridade->setCurrentIndex(priority_index >= 0 ? priority_index : 0);

	const int current_domain = normalized_domain_value(combo_dominio->currentData());
	combo_dominio->clear();
	combo_dominio->addItem(domain_label(this, 0, false), 0);
	combo_dominio->addItem(domain_label(this, 1, false), 1);
	combo_dominio->addItem(domain_label(this, 2, false), 2);
	const int domain_index = combo_dominio->findData(current_domain);
	combo_dominio->setCurrentIndex(domain_index >= 0 ? domain_index : 0);

	gerenciador_botoes_.set_button_text(
		btn_salvar, compat::tr(this, QStringLiteral("💾 Salvar Conceito"), QStringLiteral("💾 Save Concept")));
	gerenciador_botoes_.set_button_text(btn_limpar, compat::tr(this, QStringLiteral("🧽 Limpar"), QStringLiteral("🧽 Clear")));
	gerenciador_botoes_.set_button_text(btn_fechar, compat::tr(this, QStringLiteral("Fechar"), QStringLiteral("Close")));

	if (menu_configuracoes_ != nullptr) {
		menu_configuracoes_->setTitle(compat::tr(this, QStringLiteral("Configurações"), QStringLiteral("Settings")));
	}
	if (acao_editar_rotulos_ != nullptr) {
		acao_editar_rotulos_->setText(compat::tr(this, QStringLiteral("Editar Rótulos"), QStringLiteral("Edit Labels")));
	}

	atualizar_aba_descricao();
}

}  // namespace source::plugin::Feynman

namespace source::plugin::Feynman::MetodoFeynmanFns {
void atualizar_traducoes(MetodoFeynman &self) { self.atualizar_traducoes(); }
}
