#include "source/services/MostrarMenuContexto/mmc_10_build_display_and_tooltip.hpp"

#include <QCoreApplication>
#include <QDate>
#include <QDateEdit>
#include <QStringList>

#include "source/eisenhower_app.hpp"
#include "source/app/app_02_get_text.hpp"

namespace source::InterfaceCore::MostrarMenuContextoModules {
namespace {

QString get_text(const QString &text)
{
    return QCoreApplication::translate("InterfaceGrafica", text.toUtf8().constData());
}

}  // namespace

QPair<QString, QString> build_display_and_tooltip(source::EisenhowerMatrixApp *app,
                                                  const QString &base_text,
                                                  const QString &date_iso,
                                                  const QString &time_str,
                                                  int pomodoros)
{
    QString display_text = base_text;
    QStringList tooltip_lines;
    QString date_human;

    if (!date_iso.isEmpty()) {
        date_human = source::app::human_date_for_app(app, date_iso);
        if (!date_human.isEmpty()) {
            if (app != nullptr && app->reserved_space_eisenhower_enabled()) {
                display_text = time_str.isEmpty() ? QStringLiteral("%1 — %2").arg(base_text, date_human)
                                                  : QStringLiteral("%1 — %2 %3").arg(base_text, date_human, time_str);
            }
            tooltip_lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Data")), date_human);
        }
    }

    if (!time_str.isEmpty()) {
        tooltip_lines << QStringLiteral("%1: %2").arg(get_text(QStringLiteral("Horário")), time_str);
    }

    if (pomodoros >= 0 && app != nullptr && app->pomodoro_report_pomodoros_eisenhower_enabled()) {
        display_text += QStringLiteral("\n") + QCoreApplication::translate("PomodoroPlugin", "🍅 {n} pomodoros")
                                                   .replace(QStringLiteral("{n}"), QString::number(pomodoros));
    }

    return {display_text, tooltip_lines.join(QLatin1Char('\n'))};
}

}  // namespace source::InterfaceCore::MostrarMenuContextoModules
