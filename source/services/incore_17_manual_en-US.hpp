#pragma once

#include <QList>
#include <QString>

#include "source/services/incore_12_Manual.hpp"

namespace source::InterfaceCore::incore_17_manual_en_US {

using source::InterfaceCore::incore_12_Manual::ManualSection;

QString manual_intro_text();
QString manual_toc_title();

// Returns English manual sections for the Eisenhower Core, including
// the native integration of all four productivity modules (Pomodoro, Feynman, Activity Registration, and Gantt & 4D BIM),
// unified translation catalog with dynamic runtime translation, non-blocking asynchronous task persistence,
// the PoCa layout (Pomodoro + Dual Calendar), symmetric 4-frame alignment,
// dynamic resizing with scrollbar overlay protection, instant startup,
// instant opening of Productivity Tools dialogs via O(1) in-memory extraction,
// live dynamic runtime retranslation of productivity tools dialogs,
// and guidance on dual licensing, 100% offline privacy, and release notes in Options -> About plus interactive search in Options -> Manual.
QList<ManualSection> en_document();
QList<ManualSection> feynman_en_document();
// Returns manual sections dedicated to the Pomodoro Timer and Time Manager module,
// including integration with dual calendars in the PoCa layout.
QList<ManualSection> pomodoro_en_document();
QList<ManualSection> mapamental_en_document();
QList<ManualSection> audiolivro_en_document();
QList<ManualSection> agenda_en_document();
// Returns manual sections dedicated to Schedule Management, Gantt & 4D BIM, including:
// overview, menu bar (File and Settings), WBS tree and CPM analysis with unified chromatic status font styling,
// advanced table interactivity (bidirectional resizing, multi-selection, and theme-adaptive highlight),
// table layout configurations (Original 50%/50%, Top Vertical, Bottom Vertical, Left Horizontal, Right Horizontal) with atomic persistence in layout.json and automatic restoration on startup,
// deletion confirmation dialogs parented directly to the Gantt window with multi-monitor support and Ghost Line persistence,
// interactive Gantt timeline with harmonized chromatic boxes and detailed dynamic localized tooltips, Eisenhower task linkage with discrete physical progress by Pomodoro cycles and real-time synchronization, 4D BIM construction simulation with 3-mode adaptive control bar,
// working days and daily work shift configuration, and holiday manager with interactive calendar and 18-country national library.
QList<ManualSection> gantt_en_document();
QList<ManualSection> final_sections_en_document();

QList<ManualSection> get_manual_document(bool include_pomodoro_plugin = false,
                                         bool include_feynman_plugin = false,
                                         bool include_mapamental_plugin = false,
                                         bool include_audiolivro_plugin = false,
                                         bool include_agenda_plugin = false,
                                         bool include_gantt_plugin = false);

}  // namespace source::InterfaceCore::incore_17_manual_en_US
