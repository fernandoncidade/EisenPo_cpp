#pragma once

#include <algorithm>
#include <exception>

#include <QAbstractItemView>
#include <QApplication>
#include <QColor>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QDialog>
#include <QEvent>
#include <QFont>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPalette>
#include <QProgressBar>
#include <QPushButton>
#include <QSizePolicy>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>

#include "source/modules/pomodoro/Pomodoro/tmp_02_PomodoroTimer.hpp"

namespace source::plugin::Pomodoro::PomodoroTimerInternal {

inline source::utils::LogManager *logger()
{
    return source::utils::LogManager::get_logger();
}

inline bool is_light_color(const QColor &color)
{
    const double lum =
        (0.2126 * color.red() + 0.7152 * color.green() + 0.0722 * color.blue()) / 255.0;
    return lum >= 0.85;
}

inline QString normalized_ui_layout_key(const QString &value)
{
    return layout::normalized_ui_layout_key(value);
}

inline bool is_robust_ui_layout(const QString &value)
{
    return layout::is_robust_ui_layout(value);
}

}  // namespace source::plugin::Pomodoro::PomodoroTimerInternal
