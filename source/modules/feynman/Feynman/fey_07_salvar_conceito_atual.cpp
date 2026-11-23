#include "source/modules/feynman/Feynman/fey_07_salvar_conceito_atual.hpp"

#include <QComboBox>
#include <QTextEdit>

#include "source/modules/feynman/Feynman/feynman_metodo_helpers.hpp"

namespace source::plugin::Feynman {

void MetodoFeynman::set_task_context(const QVariantMap& task)
{
    task_context_ = task;
    sincronizando_descricao_ = true;
    
    QString titulo = task.value(QStringLiteral("titulo")).toString();
    if (titulo.isEmpty()) {
        titulo = task.value(QStringLiteral("text")).toString();
    }
    
    QString explicacao = task.value(QStringLiteral("explicacao")).toString();
    QString lacunas = task.value(QStringLiteral("lacunas")).toString();
    QString revisao = task.value(QStringLiteral("revisao")).toString();
    
    QString descricao = task.value(QStringLiteral("descricao")).toString();
    if (descricao.isEmpty()) {
        descricao = task.value(QStringLiteral("description")).toString();
    }

    if (!descricao.trimmed().isEmpty()) {
        DescriptionParts parts = parse_description_text(descricao);
        if (explicacao.isEmpty() && !parts.explanation.isEmpty()) {
            explicacao = parts.explanation;
        }
        if (lacunas.isEmpty() && !parts.gaps.isEmpty()) {
            lacunas = parts.gaps;
        }
        if (revisao.isEmpty() && !parts.review.isEmpty()) {
            revisao = parts.review;
        }
        if (titulo.isEmpty() && !parts.concept_text.isEmpty()) {
            titulo = parts.concept_text;
        }
    }

    input_titulo->setPlainText(titulo);
    texto_explicacao->setPlainText(explicacao);
    texto_lacunas->setPlainText(lacunas);
    texto_revisao->setPlainText(revisao);
    
    const QString descricao_gerada = composed_description(this, titulo, explicacao, lacunas, revisao);
    
    if (descricao.trimmed().isEmpty()) {
        descricao = descricao_gerada;
    }
    
    texto_descricao->setPlainText(descricao);
    descricao_gerada_atual_ = descricao_gerada;
    descricao_editada_manualmente_ = (descricao.trimmed() != descricao_gerada.trimmed());
    
    sincronizando_descricao_ = false;
    
    // Config combos
    int prio = -1;
    const QStringList priority_keys = {
        QStringLiteral("priority"),
        QStringLiteral("prioridade"),
        QStringLiteral("quadrant"),
        QStringLiteral("quadrante"),
        QStringLiteral("titulo_prioridade")
    };
    for (const QString &key : priority_keys) {
        if (task.contains(key) && !task.value(key).isNull() && !task.value(key).toString().trimmed().isEmpty()) {
            const QVariant val = task.value(key);
            if (key == QStringLiteral("quadrant") || key == QStringLiteral("quadrante")) {
                bool ok = false;
                int quad = val.toInt(&ok);
                if (ok) {
                    prio = (quad >= 0 && quad <= 3) ? quad + 1 : qBound(1, quad, 4);
                } else {
                    prio = normalized_priority_value(val);
                }
            } else {
                prio = normalized_priority_value(val);
            }
            if (prio >= 1 && prio <= 4) {
                break;
            }
        }
    }
    if (prio < 1 || prio > 4) {
        prio = 1;
    }
    
    const int idx_prio = combo_prioridade->findData(prio);
    if (idx_prio >= 0) {
        combo_prioridade->setCurrentIndex(idx_prio);
    }
    
    int domain = -1;
    const QStringList domain_keys = {
        QStringLiteral("domain"),
        QStringLiteral("dominio"),
        QStringLiteral("titulo_dominio")
    };
    for (const QString &key : domain_keys) {
        if (task.contains(key) && !task.value(key).isNull() && !task.value(key).toString().trimmed().isEmpty()) {
            domain = normalized_domain_value(task.value(key));
            if (domain >= 0 && domain <= 2) {
                break;
            }
        }
    }
    if (domain < 0 || domain > 2) {
        domain = 0;
    }
    
    const int idx_domain = combo_dominio->findData(domain);
    if (idx_domain >= 0) {
        combo_dominio->setCurrentIndex(idx_domain);
    }
}

void MetodoFeynman::salvar_conceito_atual()
{
	if (description_debounce_timer_ != nullptr && description_debounce_timer_->isActive()) {
		description_debounce_timer_->stop();
		atualizar_aba_descricao();
	}

	const QString titulo = input_titulo->toPlainText().trimmed();
	if (titulo.isEmpty()) {
		compat::MessageBox::feynman_warn_missing_title(this);
		return;
	}

	const QString explicacao = texto_explicacao->toPlainText();
	const QString lacunas = texto_lacunas->toPlainText();
	const QString revisao = texto_revisao->toPlainText();
	const QString descricao_editada = texto_descricao != nullptr ? texto_descricao->toPlainText().trimmed() : QString();
	const QString descricao_composta = composed_description(this, titulo, explicacao, lacunas, revisao).trimmed();

	QString descricao_final;
	if (has_external_description_base(task_context_)) {
		QString descricao_base = manual_description_text(descricao_editada, descricao_gerada_atual_);
		if (descricao_base.trimmed().isEmpty()) {
			descricao_base = task_context_.value(QStringLiteral("descricao")).toString();
		}
		descricao_final = description_with_external_base(descricao_base, descricao_composta).trimmed();
	} else {
		descricao_final = merge_description_edit(descricao_gerada_atual_, descricao_editada, descricao_composta).trimmed();
	}

	const bool descricao_customizada = descricao_final != descricao_composta;
	const int p_val = normalized_priority_value(combo_prioridade->currentData());
	const int d_val = normalized_domain_value(combo_dominio->currentData());

	task_context_.insert(QStringLiteral("titulo"), titulo);
	task_context_.insert(QStringLiteral("text"), titulo);
	task_context_.insert(QStringLiteral("explicacao"), explicacao);
	task_context_.insert(QStringLiteral("lacunas"), lacunas);
	task_context_.insert(QStringLiteral("revisao"), revisao);
	task_context_.insert(QStringLiteral("prioridade"), p_val);
	task_context_.insert(QStringLiteral("priority"), p_val);
	task_context_.insert(QStringLiteral("quadrante"), p_val);
	task_context_.insert(QStringLiteral("quadrant"), p_val - 1);
	task_context_.insert(QStringLiteral("titulo_prioridade"), priority_label(this, p_val));
	task_context_.insert(QStringLiteral("dominio"), d_val);
	task_context_.insert(QStringLiteral("domain"), d_val);
	task_context_.insert(QStringLiteral("titulo_dominio"), domain_label(this, d_val, false));
	task_context_.insert(QStringLiteral("descricao_customizada"), descricao_customizada);
	task_context_.insert(QStringLiteral("descricao"), descricao_final);
	task_context_.insert(QStringLiteral("description"), descricao_final);

	sincronizando_descricao_ = true;
	if (texto_descricao != nullptr) {
		texto_descricao->setPlainText(descricao_final);
	}
	descricao_gerada_atual_ = descricao_composta;
	sincronizando_descricao_ = false;
	descricao_editada_manualmente_ = descricao_customizada;
	
    accept(); // Close the dialog and return QDialog::Accepted
}

}  // namespace source::plugin::Feynman

namespace source::plugin::Feynman::MetodoFeynmanFns {
void salvar_conceito_atual(MetodoFeynman &self) { self.salvar_conceito_atual(); }
}
