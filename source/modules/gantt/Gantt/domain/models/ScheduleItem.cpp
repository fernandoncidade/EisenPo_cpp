#include "ScheduleItem.hpp"
#include <QUuid>

namespace source::gantt {

ScheduleItem::ScheduleItem()
    : id(QUuid::createUuid().toString(QUuid::WithoutBraces)),
      created_at(QDateTime::currentDateTime()),
      updated_at(QDateTime::currentDateTime())
{
}

ScheduleItem::ScheduleItem(const QString &item_name)
    : id(QUuid::createUuid().toString(QUuid::WithoutBraces)),
      name(item_name),
      created_at(QDateTime::currentDateTime()),
      updated_at(QDateTime::currentDateTime())
{
}

bool ScheduleItem::is_valid(QString *error_message) const
{
    if (name.trimmed().isEmpty()) {
        if (error_message) *error_message = QStringLiteral("Nome da atividade não pode ser vazio.");
        return false;
    }
    if (percent_complete < 0.0 || percent_complete > 100.0) {
        if (error_message) *error_message = QStringLiteral("Percentual de conclusão deve estar entre 0 e 100.");
        return false;
    }
    if (is_milestone && duration_hours != 0.0) {
        if (error_message) *error_message = QStringLiteral("Marcos (milestones) devem ter duração zero.");
        return false;
    }
    if (!is_milestone && duration_hours < 0.0) {
        if (error_message) *error_message = QStringLiteral("Duração não pode ser negativa.");
        return false;
    }
    if (start_date.isValid() && end_date.isValid() && end_date < start_date) {
        if (error_message) *error_message = QStringLiteral("Data de fim não pode ser anterior à data de início.");
        return false;
    }
    return true;
}

void ScheduleItem::sanitize()
{
    if (percent_complete < 0.0) percent_complete = 0.0;
    if (percent_complete > 100.0) percent_complete = 100.0;

    if (is_milestone) {
        duration_hours = 0.0;
        if (start_date.isValid()) {
            end_date = start_date;
        }
    }

    if (percent_complete >= 100.0) {
        status = ActivityStatus::Done;
    } else if (percent_complete > 0.0 && status == ActivityStatus::NotStarted) {
        status = ActivityStatus::InProgress;
    }
}

double ScheduleItem::variance_start_hours() const
{
    if (!baseline_start_date.isValid() || !start_date.isValid()) {
        return 0.0;
    }
    // Positive means delayed (start is later than baseline)
    return baseline_start_date.secsTo(start_date) / 3600.0;
}

double ScheduleItem::variance_end_hours() const
{
    if (!baseline_end_date.isValid() || !end_date.isValid()) {
        return 0.0;
    }
    // Positive means delayed (finish is later than baseline)
    return baseline_end_date.secsTo(end_date) / 3600.0;
}

bool ScheduleItem::is_delayed() const
{
    if (status == ActivityStatus::Blocked) {
        return true;
    }
    const QDateTime now = QDateTime::currentDateTime();
    if (status != ActivityStatus::Done && end_date.isValid() && end_date < now) {
        return true;
    }
    if (status != ActivityStatus::Done && percent_complete == 0.0 && start_date.isValid() && start_date < now) {
        return true;
    }
    if (variance_end_hours() > 0.1) {
        return true;
    }
    return false;
}

Bim4DStatus ScheduleItem::simulation_status(const QDateTime &eval_date) const
{
    const QDateTime ref_date = eval_date.isValid() ? eval_date : QDateTime::currentDateTime();

    if (percent_complete >= 100.0 || status == ActivityStatus::Done) {
        return Bim4DStatus::Executed;
    }
    if (status == ActivityStatus::Blocked) {
        return Bim4DStatus::DelayedOverdue;
    }
    if (status == ActivityStatus::InProgress || percent_complete > 0.0) {
        const bool is_overdue = end_date.isValid() ? (end_date < ref_date) : false;
        if (is_overdue) {
            return Bim4DStatus::DelayedOverdue;
        }
        return Bim4DStatus::PlannedInProgress;
    }
    const bool is_delayed_start = start_date.isValid() ? (start_date < ref_date) : false;
    if (is_delayed_start) {
        return Bim4DStatus::DelayedStart;
    }
    return Bim4DStatus::Unstarted;
}

QColor ScheduleItem::status_to_font_color(Bim4DStatus status)
{
    switch (status) {
        case Bim4DStatus::Executed:
            return QColor(QStringLiteral("#2E7D32")); // Green from "✅ Executado:"
        case Bim4DStatus::PlannedInProgress:
            return QColor(QStringLiteral("#1565C0")); // Blue from "🔄 Em Andamento:"
        case Bim4DStatus::DelayedStart:
        case Bim4DStatus::DelayedOverdue:
            return QColor(QStringLiteral("#C62828")); // Crimson/Red from "⚠️ Atrasado:"
        case Bim4DStatus::Unstarted:
        default:
            return QColor(QStringLiteral("#757575")); // Neutral Gray from "⏳ Futuro:"
    }
}

QColor ScheduleItem::status_font_color(const QDateTime &eval_date) const
{
    return status_to_font_color(simulation_status(eval_date));
}

GanttBarColors ScheduleItem::status_to_bar_colors(Bim4DStatus status, bool is_critical)
{
    GanttBarColors colors;
    switch (status) {
        case Bim4DStatus::Executed:
            // Concluídas: Tonalidade verde diferenciada, indicando finalização com clareza e elegância.
            // Para tarefas concluídas, o caminho crítico não impõe risco de atraso futuro,
            // garantindo a tonalidade verde harmoniosa em toda a atividade.
            colors.bg = QColor(QStringLiteral("#C8E6C9"));             // Verde menta suave
            colors.border = QColor(QStringLiteral("#1B5E20"));         // Verde floresta escuro
            colors.progress_start = QColor(QStringLiteral("#43A047")); // Verde esmeralda vivo
            colors.progress_end = QColor(QStringLiteral("#2E7D32"));   // Verde escuro floresta
            break;

        case Bim4DStatus::PlannedInProgress:
            // Em Andamento: Azul corporativo com fundo azul suave
            colors.bg = QColor(QStringLiteral("#BBDEFB"));             // Azul céu suave
            colors.border = is_critical ? QColor(QStringLiteral("#D32F2F")) 
                                        : QColor(QStringLiteral("#0D47A1")); // Vermelho se crítico ou azul marinho
            colors.progress_start = QColor(QStringLiteral("#2196F3")); // Azul brilhante
            colors.progress_end = QColor(QStringLiteral("#1565C0"));   // Azul cobalto
            break;

        case Bim4DStatus::DelayedStart:
        case Bim4DStatus::DelayedOverdue:
            // Atrasado: Vermelho / Carmim de alerta
            colors.bg = QColor(QStringLiteral("#FFCDD2"));             // Vermelho coral suave
            colors.border = QColor(QStringLiteral("#B71C1C"));         // Vermelho escuro forte
            colors.progress_start = QColor(QStringLiteral("#E53935")); // Vermelho alerta
            colors.progress_end = QColor(QStringLiteral("#C62828"));   // Vermelho escuro
            break;

        case Bim4DStatus::Unstarted:
        default:
            // Futuro: Cinza neutro
            colors.bg = QColor(QStringLiteral("#EEEEEE"));             // Cinza claro neutro
            colors.border = is_critical ? QColor(QStringLiteral("#D32F2F")) 
                                        : QColor(QStringLiteral("#9E9E9E")); // Cinza ardósia ou vermelho crítico
            colors.progress_start = QColor(QStringLiteral("#BDBDBD"));
            colors.progress_end = QColor(QStringLiteral("#9E9E9E"));
            break;
    }
    return colors;
}

GanttNodeColors ScheduleItem::status_to_node_colors(Bim4DStatus status, bool is_critical)
{
    GanttNodeColors colors;
    switch (status) {
        case Bim4DStatus::Executed:
            // Concluído: Verde floresta com contorno firme
            colors.fill = QColor(QStringLiteral("#2E7D32"));
            colors.border = QColor(QStringLiteral("#1B5E20"));
            break;

        case Bim4DStatus::PlannedInProgress:
            // Em andamento: Azul cobalto
            colors.fill = QColor(QStringLiteral("#1565C0"));
            colors.border = is_critical ? QColor(QStringLiteral("#D32F2F")) : QColor(QStringLiteral("#0D47A1"));
            break;

        case Bim4DStatus::DelayedStart:
        case Bim4DStatus::DelayedOverdue:
            // Atrasado: Carmim / Vermelho
            colors.fill = QColor(QStringLiteral("#C62828"));
            colors.border = QColor(QStringLiteral("#B71C1C"));
            break;

        case Bim4DStatus::Unstarted:
        default:
            // Futuro: Cinza neutro
            colors.fill = is_critical ? QColor(QStringLiteral("#EF9A9A")) : QColor(QStringLiteral("#E0E0E0"));
            colors.border = is_critical ? QColor(QStringLiteral("#D32F2F")) : QColor(QStringLiteral("#757575"));
            break;
    }
    return colors;
}

GanttBarColors ScheduleItem::bar_colors(const QDateTime &eval_date) const
{
    return status_to_bar_colors(simulation_status(eval_date), critical);
}

GanttNodeColors ScheduleItem::node_colors(const QDateTime &eval_date) const
{
    return status_to_node_colors(simulation_status(eval_date), critical);
}

QJsonObject ScheduleItem::to_json() const
{
    QJsonObject json;
    json[QStringLiteral("id")] = id;
    json[QStringLiteral("schedule_id")] = schedule_id;
    json[QStringLiteral("task_id")] = task_id;
    json[QStringLiteral("wbs_code")] = wbs_code;
    json[QStringLiteral("name")] = name;
    json[QStringLiteral("description")] = description;
    json[QStringLiteral("start_date")] = start_date.isValid() ? start_date.toString(Qt::ISODate) : QString();
    json[QStringLiteral("end_date")] = end_date.isValid() ? end_date.toString(Qt::ISODate) : QString();
    json[QStringLiteral("duration_hours")] = duration_hours;
    json[QStringLiteral("percent_complete")] = percent_complete;
    json[QStringLiteral("status")] = activity_status_to_string(status);
    json[QStringLiteral("is_milestone")] = is_milestone;
    json[QStringLiteral("critical")] = critical;
    json[QStringLiteral("early_start")] = early_start.isValid() ? early_start.toString(Qt::ISODate) : QString();
    json[QStringLiteral("early_finish")] = early_finish.isValid() ? early_finish.toString(Qt::ISODate) : QString();
    json[QStringLiteral("late_start")] = late_start.isValid() ? late_start.toString(Qt::ISODate) : QString();
    json[QStringLiteral("late_finish")] = late_finish.isValid() ? late_finish.toString(Qt::ISODate) : QString();
    json[QStringLiteral("total_float_hours")] = total_float_hours;
    json[QStringLiteral("free_float_hours")] = free_float_hours;
    json[QStringLiteral("baseline_start_date")] = baseline_start_date.isValid() ? baseline_start_date.toString(Qt::ISODate) : QString();
    json[QStringLiteral("baseline_end_date")] = baseline_end_date.isValid() ? baseline_end_date.toString(Qt::ISODate) : QString();
    json[QStringLiteral("baseline_duration_hours")] = baseline_duration_hours;
    json[QStringLiteral("external_system")] = external_system;
    json[QStringLiteral("external_id")] = external_id;
    json[QStringLiteral("version")] = version;
    json[QStringLiteral("created_at")] = created_at.toString(Qt::ISODate);
    json[QStringLiteral("updated_at")] = updated_at.toString(Qt::ISODate);
    return json;
}

ScheduleItem ScheduleItem::from_json(const QJsonObject &json)
{
    ScheduleItem item;
    if (json.contains(QStringLiteral("id"))) item.id = json[QStringLiteral("id")].toString();
    if (json.contains(QStringLiteral("schedule_id"))) item.schedule_id = json[QStringLiteral("schedule_id")].toString();
    if (json.contains(QStringLiteral("task_id"))) item.task_id = json[QStringLiteral("task_id")].toString();
    if (json.contains(QStringLiteral("wbs_code"))) item.wbs_code = json[QStringLiteral("wbs_code")].toString();
    if (json.contains(QStringLiteral("name"))) item.name = json[QStringLiteral("name")].toString();
    if (json.contains(QStringLiteral("description"))) item.description = json[QStringLiteral("description")].toString();
    if (json.contains(QStringLiteral("start_date"))) item.start_date = QDateTime::fromString(json[QStringLiteral("start_date")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("end_date"))) item.end_date = QDateTime::fromString(json[QStringLiteral("end_date")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("duration_hours"))) item.duration_hours = json[QStringLiteral("duration_hours")].toDouble();
    if (json.contains(QStringLiteral("percent_complete"))) item.percent_complete = json[QStringLiteral("percent_complete")].toDouble();
    if (json.contains(QStringLiteral("status"))) item.status = string_to_activity_status(json[QStringLiteral("status")].toString());
    if (json.contains(QStringLiteral("is_milestone"))) item.is_milestone = json[QStringLiteral("is_milestone")].toBool();
    if (json.contains(QStringLiteral("critical"))) item.critical = json[QStringLiteral("critical")].toBool();
    if (json.contains(QStringLiteral("early_start"))) item.early_start = QDateTime::fromString(json[QStringLiteral("early_start")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("early_finish"))) item.early_finish = QDateTime::fromString(json[QStringLiteral("early_finish")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("late_start"))) item.late_start = QDateTime::fromString(json[QStringLiteral("late_start")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("late_finish"))) item.late_finish = QDateTime::fromString(json[QStringLiteral("late_finish")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("total_float_hours"))) item.total_float_hours = json[QStringLiteral("total_float_hours")].toDouble();
    if (json.contains(QStringLiteral("free_float_hours"))) item.free_float_hours = json[QStringLiteral("free_float_hours")].toDouble();
    if (json.contains(QStringLiteral("baseline_start_date"))) item.baseline_start_date = QDateTime::fromString(json[QStringLiteral("baseline_start_date")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("baseline_end_date"))) item.baseline_end_date = QDateTime::fromString(json[QStringLiteral("baseline_end_date")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("baseline_duration_hours"))) item.baseline_duration_hours = json[QStringLiteral("baseline_duration_hours")].toDouble();
    if (json.contains(QStringLiteral("external_system"))) item.external_system = json[QStringLiteral("external_system")].toString();
    if (json.contains(QStringLiteral("external_id"))) item.external_id = json[QStringLiteral("external_id")].toString();
    if (json.contains(QStringLiteral("version"))) item.version = json[QStringLiteral("version")].toInt(1);
    if (json.contains(QStringLiteral("created_at"))) item.created_at = QDateTime::fromString(json[QStringLiteral("created_at")].toString(), Qt::ISODate);
    if (json.contains(QStringLiteral("updated_at"))) item.updated_at = QDateTime::fromString(json[QStringLiteral("updated_at")].toString(), Qt::ISODate);
    item.sanitize();
    return item;
}

} // namespace source::gantt
