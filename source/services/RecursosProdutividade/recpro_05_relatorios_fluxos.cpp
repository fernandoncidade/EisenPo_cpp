#include "source/services/RecursosProdutividade/recpro_05_relatorios_fluxos.hpp"
#include "source/services/RecursosProdutividade/recpro_01_utilitarios_compartilhados.hpp"

namespace source::InterfaceCore::ProductivityFeatures {

void export_calendar_ics(source::EisenhowerMatrixApp *app)
{
    if (app == nullptr) {
        return;
    }
    const QString path = QFileDialog::getSaveFileName(app,
                                                      get_text(QStringLiteral("Exportar calendário")),
                                                      QDir::home().filePath(QStringLiteral("tarefas.ics")),
                                                      get_text(QStringLiteral("iCalendar (*.ics)")));
    if (path.isEmpty()) {
        return;
    }

    QStringList lines = {
        QStringLiteral("BEGIN:VCALENDAR"),
        QStringLiteral("VERSION:2.0"),
        QStringLiteral("PRODID:-//EisenPo//Tasks//PT-BR"),
        QStringLiteral("CALSCALE:GREGORIAN"),
    };

    const QString dtstamp = QDateTime::currentDateTimeUtc().toString(QStringLiteral("yyyyMMdd'T'HHmmss'Z'"));
    for (const QVariant &value : records_for_app(app)) {
        const QVariantMap record = value.toMap();
        const QDate date = QDate::fromString(date_for_record(record), Qt::ISODate);
        if (!date.isValid()) {
            continue;
        }
        const QTime time = parse_time(time_for_record(record));
        QString uid = record.value(QStringLiteral("id")).toString();
        if (uid.trimmed().isEmpty()) {
            uid = QUuid::createUuid().toString(QUuid::WithoutBraces);
        }
        lines << QStringLiteral("BEGIN:VEVENT");
        lines << QStringLiteral("UID:%1@eisenpo").arg(uid);
        lines << QStringLiteral("DTSTAMP:%1").arg(dtstamp);
        if (time.isValid()) {
            lines << QStringLiteral("DTSTART:%1").arg(QDateTime(date, time).toString(QStringLiteral("yyyyMMdd'T'HHmmss")));
            lines << QStringLiteral("DURATION:PT30M");
        } else {
            lines << QStringLiteral("DTSTART;VALUE=DATE:%1").arg(date.toString(QStringLiteral("yyyyMMdd")));
        }
        lines << QStringLiteral("SUMMARY:%1").arg(ics_escape(title_for_record(record)));
        const QString description = record.value(QStringLiteral("descricao")).toString();
        if (!description.trimmed().isEmpty()) {
            lines << QStringLiteral("DESCRIPTION:%1").arg(ics_escape(description));
        }
        lines << QStringLiteral("END:VEVENT");
    }
    lines << QStringLiteral("END:VCALENDAR");

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(app, get_text(QStringLiteral("Erro")), file.errorString());
        return;
    }
    const QByteArray payload = lines.join(QStringLiteral("\r\n")).toUtf8() + QByteArray("\r\n");
    if (file.write(payload) != payload.size() || !file.commit()) {
        QMessageBox::warning(app, get_text(QStringLiteral("Erro")), file.errorString());
        return;
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void show_productivity_history(source::EisenhowerMatrixApp *app)
{
    QMap<QString, int> completed_by_week;
    int total_completed = 0;
    int total_active = 0;

    for (const QVariant &value : records_for_app(app)) {
        const QVariantMap record = value.toMap();
        if (record.value(QStringLiteral("concluida")).toBool()) {
            ++total_completed;
            QDate date = QDateTime::fromString(record.value(QStringLiteral("completed_at")).toString(), Qt::ISODate).date();
            if (!date.isValid()) {
                date = QDate::fromString(date_for_record(record), Qt::ISODate);
            }
            if (!date.isValid()) {
                date = QDate::currentDate();
            }
            completed_by_week[QStringLiteral("%1-W%2").arg(date.year()).arg(date.weekNumber(), 2, 10, QLatin1Char('0'))]++;
        } else {
            ++total_active;
        }
    }

    QStringList lines;
    lines << get_text(QStringLiteral("Histórico de produtividade"));
    lines << QString();
    lines << get_text(QStringLiteral("Total de tarefas concluídas: %1")).arg(total_completed);
    lines << get_text(QStringLiteral("Total de tarefas ativas: %1")).arg(total_active);

    if (!completed_by_week.isEmpty()) {
        lines << QString();
        lines << get_text(QStringLiteral("Concluídas por semana:"));
        for (auto it = completed_by_week.constBegin(); it != completed_by_week.constEnd(); ++it) {
            lines << get_text(QStringLiteral("%1: %2 tarefa(s)")).arg(it.key()).arg(it.value());
        }
    }

    QMessageBox::information(app, get_text(QStringLiteral("Histórico")), lines.join(QLatin1Char('\n')));
}

void run_study_flow(source::EisenhowerMatrixApp *app)
{
    if (app == nullptr) {
        return;
    }

    RuntimeTranslatedDialog dialog(app);
    dialog.setWindowTitle(get_text(QStringLiteral("Fluxo integrado de estudo")));
    dialog.resize(520, 380);
    auto *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto *label = new QLabel(get_text(QStringLiteral("Trecho para transformar em estudo:")), &dialog);
    layout->addWidget(label);

    auto *text_edit = new QPlainTextEdit(&dialog);
    text_edit->setPlaceholderText(get_text(QStringLiteral("Cole ou digite aqui o conteúdo ou resumo do conceito a ser estudado...")));
    layout->addWidget(text_edit, 1);

    auto *form = new QFormLayout();
    form->setContentsMargins(0, 0, 0, 0);
    form->setSpacing(8);

    auto *priority_combo = new QComboBox(&dialog);
    priority_combo->addItem(get_text(QStringLiteral("🔴 Importante e Urgente")), 1);
    priority_combo->addItem(get_text(QStringLiteral("🟠 Importante, mas Não Urgente")), 2);
    priority_combo->addItem(get_text(QStringLiteral("🟡 Não Importante, mas Urgente")), 3);
    priority_combo->addItem(get_text(QStringLiteral("🟢 Não Importante e Não Urgente")), 4);
    priority_combo->setCurrentIndex(1);

    auto *domain_combo = new QComboBox(&dialog);
    domain_combo->addItem(get_text(QStringLiteral("🥉 Iniciante (não consigo explicar)")), 0);
    domain_combo->addItem(get_text(QStringLiteral("🥈 Intermediário (explico com termos técnicos)")), 1);
    domain_combo->addItem(get_text(QStringLiteral("🥇 Avançado (explico de forma simples)")), 2);
    domain_combo->setCurrentIndex(0);

    auto *priority_label = new QLabel(get_text(QStringLiteral("Prioridade:")), &dialog);
    auto *domain_label = new QLabel(get_text(QStringLiteral("Nível de domínio:")), &dialog);

    form->addRow(priority_label, priority_combo);
    form->addRow(domain_label, domain_combo);
    layout->addLayout(form);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    translate_standard_buttons(app, buttons);
    layout->addWidget(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    const auto retranslate_ui = [&]() {
        dialog.setWindowTitle(get_text(QStringLiteral("Fluxo integrado de estudo")));
        label->setText(get_text(QStringLiteral("Trecho para transformar em estudo:")));
        text_edit->setPlaceholderText(get_text(QStringLiteral("Cole ou digite aqui o conteúdo ou resumo do conceito a ser estudado...")));
        priority_label->setText(get_text(QStringLiteral("Prioridade:")));
        domain_label->setText(get_text(QStringLiteral("Nível de domínio:")));

        priority_combo->setItemText(0, get_text(QStringLiteral("🔴 Importante e Urgente")));
        priority_combo->setItemText(1, get_text(QStringLiteral("🟠 Importante, mas Não Urgente")));
        priority_combo->setItemText(2, get_text(QStringLiteral("🟡 Não Importante, mas Urgente")));
        priority_combo->setItemText(3, get_text(QStringLiteral("🟢 Não Importante e Não Urgente")));

        domain_combo->setItemText(0, get_text(QStringLiteral("🥉 Iniciante (não consigo explicar)")));
        domain_combo->setItemText(1, get_text(QStringLiteral("🥈 Intermediário (explico com termos técnicos)")));
        domain_combo->setItemText(2, get_text(QStringLiteral("🥇 Avançado (explico de forma simples)")));

        translate_standard_buttons(app, buttons);
    };

    dialog.set_retranslate_handler([&]() {
        retranslate_ui();
    });

    if (app->gerenciador_traducao != nullptr) {
        QObject::connect(app->gerenciador_traducao, &GerenciadorTraducao::idioma_alterado, &dialog, [&]() {
            retranslate_ui();
        });
    }

    retranslate_ui();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString text = text_edit->toPlainText().trimmed();
    if (text.isEmpty()) {
        return;
    }

    const int selected_priority = priority_combo->currentData().toInt();
    const int selected_domain = domain_combo->currentData().toInt();

    QString title = text.section(QRegularExpression(QStringLiteral("[\\r\\n\\.]")), 0, 0).simplified().left(90);
    if (title.isEmpty()) {
        title = get_text(QStringLiteral("Novo conceito"));
    }

    QVariantList additions;
    const QString concept_id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    additions.append(source::utils::UnifiedTaskStore::normalize_record({
        {QStringLiteral("id"), concept_id},
        {QStringLiteral("modulo"), QStringLiteral("feynman")},
        {QStringLiteral("tipo"), QStringLiteral("conceito")},
        {QStringLiteral("titulo"), title},
        {QStringLiteral("explicacao"), text.left(4000)},
        {QStringLiteral("descricao_customizada"), true},
        {QStringLiteral("prioridade"), selected_priority},
        {QStringLiteral("dominio"), selected_domain},
        {QStringLiteral("categoria"), QStringLiteral("conceito")},
    }));
    QVariantMap review =
        eisenhower_task_record(get_text(QStringLiteral("Revisar: %1")).arg(title), QDate::currentDate().addDays(2), selected_priority, text.left(1000));
    review.insert(QStringLiteral("dominio"), selected_domain);
    review.insert(QStringLiteral("integracao_origem"), QStringLiteral("study_flow"));
    review.insert(QStringLiteral("integracao_task_id"), concept_id);
    additions.append(source::utils::UnifiedTaskStore::normalize_record(review));
    append_records_and_reload(app, additions);

    QMessageBox::information(app,
                             get_text(QStringLiteral("Fluxo integrado de estudo")),
                             get_text(QStringLiteral("Tarefa de estudo e revisão criadas com sucesso.")));
}

void show_templates_dialog(source::EisenhowerMatrixApp *app)
{
    if (app == nullptr) {
        return;
    }

    RuntimeTranslatedDialog dialog(app);
    dialog.setWindowTitle(get_text(QStringLiteral("Templates")));
    dialog.resize(520, 420);
    auto *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto *list_label = new QLabel(get_text(QStringLiteral("Escolha o template a ser aplicado:")), &dialog);
    layout->addWidget(list_label);

    auto *list = new QListWidget(&dialog);
    const QStringList templates = {
        get_text(QStringLiteral("Estudo")),
        get_text(QStringLiteral("Trabalho")),
        get_text(QStringLiteral("Revisão")),
        get_text(QStringLiteral("Planejamento semanal")),
    };
    list->addItems(templates);
    list->setCurrentRow(0);
    layout->addWidget(list, 1);

    auto *form = new QFormLayout();
    form->setContentsMargins(0, 0, 0, 0);
    form->setSpacing(8);

    auto *priority_combo = new QComboBox(&dialog);
    priority_combo->addItem(get_text(QStringLiteral("🔴 Importante e Urgente")), 1);
    priority_combo->addItem(get_text(QStringLiteral("🟠 Importante, mas Não Urgente")), 2);
    priority_combo->addItem(get_text(QStringLiteral("🟡 Não Importante, mas Urgente")), 3);
    priority_combo->addItem(get_text(QStringLiteral("🟢 Não Importante e Não Urgente")), 4);
    priority_combo->setCurrentIndex(1);

    auto *domain_combo = new QComboBox(&dialog);
    domain_combo->addItem(get_text(QStringLiteral("🥉 Iniciante (não consigo explicar)")), 0);
    domain_combo->addItem(get_text(QStringLiteral("🥈 Intermediário (explico com termos técnicos)")), 1);
    domain_combo->addItem(get_text(QStringLiteral("🥇 Avançado (explico de forma simples)")), 2);
    domain_combo->setCurrentIndex(0);

    auto *priority_label = new QLabel(get_text(QStringLiteral("Prioridade:")), &dialog);
    auto *domain_label = new QLabel(get_text(QStringLiteral("Nível de domínio:")), &dialog);

    form->addRow(priority_label, priority_combo);
    form->addRow(domain_label, domain_combo);
    layout->addLayout(form);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    translate_standard_buttons(app, buttons);
    layout->addWidget(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    const auto retranslate_templates_ui = [&]() {
        dialog.setWindowTitle(get_text(QStringLiteral("Templates")));
        list_label->setText(get_text(QStringLiteral("Escolha o template a ser aplicado:")));

        if (list->count() >= 4) {
            list->item(0)->setText(get_text(QStringLiteral("Estudo")));
            list->item(1)->setText(get_text(QStringLiteral("Trabalho")));
            list->item(2)->setText(get_text(QStringLiteral("Revisão")));
            list->item(3)->setText(get_text(QStringLiteral("Planejamento semanal")));
        }

        priority_label->setText(get_text(QStringLiteral("Prioridade:")));
        domain_label->setText(get_text(QStringLiteral("Nível de domínio:")));

        priority_combo->setItemText(0, get_text(QStringLiteral("🔴 Importante e Urgente")));
        priority_combo->setItemText(1, get_text(QStringLiteral("🟠 Importante, mas Não Urgente")));
        priority_combo->setItemText(2, get_text(QStringLiteral("🟡 Não Importante, mas Urgente")));
        priority_combo->setItemText(3, get_text(QStringLiteral("🟢 Não Importante e Não Urgente")));

        domain_combo->setItemText(0, get_text(QStringLiteral("🥉 Iniciante (não consigo explicar)")));
        domain_combo->setItemText(1, get_text(QStringLiteral("🥈 Intermediário (explico com termos técnicos)")));
        domain_combo->setItemText(2, get_text(QStringLiteral("🥇 Avançado (explico de forma simples)")));

        translate_standard_buttons(app, buttons);
    };

    dialog.set_retranslate_handler([&]() {
        retranslate_templates_ui();
    });

    if (app->gerenciador_traducao != nullptr) {
        QObject::connect(app->gerenciador_traducao, &GerenciadorTraducao::idioma_alterado, &dialog, [&]() {
            retranslate_templates_ui();
        });
    }

    retranslate_templates_ui();

    if (dialog.exec() != QDialog::Accepted || list->currentRow() < 0) {
        return;
    }

    const int chosen_priority = priority_combo->currentData().toInt();
    const int chosen_domain = domain_combo->currentData().toInt();

    const QDate today = QDate::currentDate();
    QVariantList additions;
    switch (list->currentRow()) {
    case 0:
        additions << eisenhower_task_record(get_text(QStringLiteral("Definir objetivo de estudo")), today, chosen_priority)
                  << eisenhower_task_record(get_text(QStringLiteral("Estudar bloco principal")), today, chosen_priority == 2 ? 1 : chosen_priority)
                  << eisenhower_task_record(get_text(QStringLiteral("Criar resumo do estudo")), today.addDays(1), chosen_priority);
        break;
    case 1:
        additions << eisenhower_task_record(get_text(QStringLiteral("Priorizar entregas do dia")), today, chosen_priority == 2 ? 1 : chosen_priority)
                  << eisenhower_task_record(get_text(QStringLiteral("Responder pendências urgentes")), today, chosen_priority == 2 ? 3 : chosen_priority)
                  << eisenhower_task_record(get_text(QStringLiteral("Planejar próxima entrega")), today.addDays(1), chosen_priority);
        break;
    case 2:
        additions << eisenhower_task_record(get_text(QStringLiteral("Revisar pontos fracos")), today, chosen_priority)
                  << eisenhower_task_record(get_text(QStringLiteral("Resolver exercício de fixação")), today.addDays(1), chosen_priority);
        break;
    case 3:
    default:
        for (int i = 0; i < 7; ++i) {
            additions << eisenhower_task_record(get_text(QStringLiteral("Planejamento do dia %1")).arg(i + 1),
                                                today.addDays(i),
                                                chosen_priority);
        }
        break;
    }

    for (auto &rec : additions) {
        QVariantMap map = rec.toMap();
        map.insert(QStringLiteral("dominio"), chosen_domain);
        rec = map;
    }

    append_records_and_reload(app, additions);
    QMessageBox::information(app,
                             get_text(QStringLiteral("Templates")),
                             get_text(QStringLiteral("Template aplicado com sucesso no quadrante selecionado.")));
}

}  // namespace source::InterfaceCore::ProductivityFeatures
