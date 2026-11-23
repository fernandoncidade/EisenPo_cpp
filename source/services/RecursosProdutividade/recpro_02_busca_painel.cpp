#include "source/services/RecursosProdutividade/recpro_02_busca_painel.hpp"
#include "source/services/RecursosProdutividade/recpro_01_utilitarios_compartilhados.hpp"

namespace source::InterfaceCore::ProductivityFeatures {

void show_global_search(source::EisenhowerMatrixApp *app)
{
    show_model_dialog(app,
                      get_text(QStringLiteral("Busca global")),
                      collect_global_search_entries(app),
                      true);
}

void show_today_panel(source::EisenhowerMatrixApp *app)
{
    const QDate today = QDate::currentDate();
    const QDate horizon = today.addDays(7);
    QVariantList entries;
    for (const QVariant &value : records_for_app(app)) {
        const QVariantMap record = value.toMap();
        const QDate date = QDate::fromString(date_for_record(record), Qt::ISODate);
        if (!date.isValid() || date < today || date > horizon) {
            continue;
        }
        const QString title = title_for_record(record);
        const QString detail = QStringLiteral("%1%2%3")
                                   .arg(date.toString(QStringLiteral("dd/MM/yyyy")),
                                        time_for_record(record).isEmpty() ? QString() : QStringLiteral(" "),
                                        time_for_record(record));
        entries.append(display_entry(record.value(QStringLiteral("modulo")).toString(),
                                     title,
                                     detail,
                                     searchable_text(record),
                                     record));
    }
    show_model_dialog(app, get_text(QStringLiteral("Hoje / Próximos 7 dias")), entries, false);
}

void configure_recurrence_for_item(source::EisenhowerMatrixApp *app, QListWidgetItem *item)
{
    if (app == nullptr || item == nullptr) {
        return;
    }

    QDialog dialog(app);
    dialog.setWindowTitle(get_text(QStringLiteral("Recorrência")));
    auto *layout = new QFormLayout(&dialog);
    auto *combo = new QComboBox(&dialog);
    combo->addItem(get_text(QStringLiteral("Sem recorrência")), QString());
    combo->addItem(get_text(QStringLiteral("Diária")), QStringLiteral("daily"));
    combo->addItem(get_text(QStringLiteral("Semanal")), QStringLiteral("weekly"));
    combo->addItem(get_text(QStringLiteral("Mensal")), QStringLiteral("monthly"));

    auto *until_enabled = new QCheckBox(get_text(QStringLiteral("Definir data final")), &dialog);
    auto *until = new QDateEdit(QDate::currentDate().addMonths(3), &dialog);
    until->setCalendarPopup(true);
    until->setEnabled(false);
    QObject::connect(until_enabled, &QCheckBox::toggled, until, &QDateEdit::setEnabled);

    QVariantMap data = item->data(Qt::UserRole).toMap();
    QString current = data.value(QStringLiteral("recurrence")).toString().trimmed();
    if (current.isEmpty()) {
        const QJsonObject rec_settings = read_json_object(recurring_tasks_settings_path());
        if (rec_settings.value(QStringLiteral("apply_default")).toBool()) {
            current = rec_settings.value(QStringLiteral("default_recurrence")).toString().trimmed();
        }
    }
    const int idx = combo->findData(current);
    combo->setCurrentIndex(idx >= 0 ? idx : 0);
    const QDate current_until = QDate::fromString(data.value(QStringLiteral("recurrence_until")).toString(), Qt::ISODate);
    if (current_until.isValid()) {
        until_enabled->setChecked(true);
        until->setDate(current_until);
    }

    layout->addRow(get_text(QStringLiteral("Frequência")), combo);
    layout->addRow(until_enabled, until);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    translate_standard_buttons(app, buttons);
    layout->addRow(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    data.insert(QStringLiteral("recurrence"), combo->currentData().toString());
    data.insert(QStringLiteral("recurrence_until"),
                until_enabled->isChecked() ? until->date().toString(Qt::ISODate) : QString());
    item->setData(Qt::UserRole, data);
    app->save_tasks_now();
}


}  // namespace source::InterfaceCore::ProductivityFeatures
