#include "source/ui/CalendarDialog/agregador_01_CalendarDialog.hpp"

#include <QCoreApplication>
#include <QDate>
#include <QDateEdit>

#include "source/ui/agregador_01_ui.hpp"
#include "source/ui/ui_12_CalendarDialog.hpp"
#include "source/services/incore_13_prioridade_display.hpp"
#include "source/eisenhower_app.hpp"
#include "source/app/app_02_get_text.hpp"
#include "source/utils/LogManager.hpp"

namespace source::GerenciamentoUI::CalendarDialogModules::detail {

source::utils::LogManager *logger()
{
    return source::utils::LogManager::get_logger();
}

QString get_text(const QString &text)
{
    return QCoreApplication::translate("InterfaceGrafica", text.toUtf8().constData());
}

QList<QVariantMap> tasks_for_date(const source::GerenciamentoUI::CalendarDialog *self, const QDate &qdate)
{
    QList<QVariantMap> result;
    if (self == nullptr || !qdate.isValid()) {
        return result;
    }

    const QString iso = qdate.toString(Qt::ISODate);
    const QList<QVariantMap> tasks_all = source::GerenciamentoUI::task_utils::collect_tasks_from_app(self->app);
    for (const QVariantMap &task : tasks_all) {
        if (task.value(QStringLiteral("date")).toString() == iso) {
            result.append(task);
        }
    }
    return result;
}

QString tooltip_for_task(const source::GerenciamentoUI::CalendarDialog *self, const QVariantMap &task)
{
    QStringList lines;
    const QString file_path = task.value(QStringLiteral("file_path")).toString();
    if (!file_path.isEmpty()) {
        lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Arquivo")), file_path);
    }

    const QString date_str = task.value(QStringLiteral("date")).toString();
    if (!date_str.isEmpty()) {
        const QDate qd = QDate::fromString(date_str, Qt::ISODate);
        QString format = QStringLiteral("dd/MM/yyyy");
        if (self != nullptr && !self->_date_format.isEmpty()) {
            format = self->_date_format;
        } else if (self != nullptr && self->app != nullptr && self->app->date_input != nullptr) {
            format = self->app->date_input->displayFormat();
        }

        lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Data")),
                                              qd.isValid() ? qd.toString(format) : date_str);
    }

    const QString time_str = task.value(QStringLiteral("time")).toString();
    if (!time_str.isEmpty()) {
        lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Horário")), time_str);
    }

    const QString desc = task.value(QStringLiteral("description")).toString();
    if (!desc.isEmpty()) {
        lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Descrição")), desc);
    }

    const QVariant prio = task.value(QStringLiteral("priority"));
    if (prio.isValid() && !prio.toString().isEmpty()) {
        lines << QStringLiteral("%1: %2")
                     .arg(get_text(QStringLiteral("Prioridade")),
                          source::InterfaceCore::prioridade_para_texto(prio, self != nullptr ? self->app : nullptr));
    }

    const QVariant domain = task.value(QStringLiteral("dominio"));
    if (domain.isValid() && !domain.toString().trimmed().isEmpty()) {
        lines << QStringLiteral("%1: %2")
                     .arg(get_text(QStringLiteral("Nível de Domínio")),
                          source::InterfaceCore::dominio_para_texto(domain, false));
    }

    const double duration_val = task.contains(QStringLiteral("duracao"))
                                    ? task.value(QStringLiteral("duracao")).toDouble()
                                    : (task.contains(QStringLiteral("duration"))
                                           ? task.value(QStringLiteral("duration")).toDouble()
                                           : 1.0);
    lines << source::app::format_duration_tooltip_line(duration_val > 0.0 ? duration_val : 1.0);

    const int pomodoros = qMax(0, task.value(QStringLiteral("pomodoros"),
                                             task.value(QStringLiteral("pomodoros_completados"), 0)).toInt());
    if (self != nullptr && self->app != nullptr && self->app->pomodoro_report_pomodoros_calendario_enabled()) {
        lines << QCoreApplication::translate("PomodoroPlugin", "🍅 {n} pomodoros")
                     .replace(QStringLiteral("{n}"), QString::number(pomodoros));
    }

    return lines.join(QLatin1Char('\n'));
}

}  // namespace source::GerenciamentoUI::CalendarDialogModules::detail
