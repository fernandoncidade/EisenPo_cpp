#include "GanttTimelineWidget.hpp"
#include <QLinearGradient>
#include <QLocale>
#include <QMenu>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPolygonF>
#include <QToolTip>
#include <cmath>

namespace source::gantt {

namespace {
inline QString capitalize_name(QString str)
{
    if (str.endsWith(QLatin1Char('.'))) {
        str.chop(1);
    }
    if (!str.isEmpty()) {
        str = str.left(1).toUpper() + str.mid(1);
    }
    return str;
}
} // namespace

GanttTimelineWidget::GanttTimelineWidget(QWidget *parent)
    : QScrollArea(parent),
      canvas_(new GanttTimelineCanvas(this))
{
    setWidget(canvas_);
    setWidgetResizable(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    connect(canvas_, &GanttTimelineCanvas::item_clicked, this, &GanttTimelineWidget::item_clicked);
    connect(canvas_, &GanttTimelineCanvas::item_double_clicked, this, &GanttTimelineWidget::item_double_clicked);
    connect(canvas_, &GanttTimelineCanvas::request_delete_item, this, &GanttTimelineWidget::request_delete_item);
}

void GanttTimelineWidget::set_schedule(const Schedule &schedule)
{
    canvas_->set_data(schedule, zoom_, show_baseline_, QDateTime());
}

void GanttTimelineWidget::set_zoom(GanttZoom zoom)
{
    zoom_ = zoom;
    canvas_->set_data(canvas_->findChild<QWidget*>() ? Schedule() : Schedule(), zoom_, show_baseline_, QDateTime());
}

void GanttTimelineWidget::set_reference_date(const QDateTime &ref_date)
{
    canvas_->set_data(Schedule(), zoom_, show_baseline_, ref_date);
}

void GanttTimelineWidget::set_show_baseline(bool show)
{
    show_baseline_ = show;
    canvas_->set_data(Schedule(), zoom_, show_baseline_, QDateTime());
}

void GanttTimelineWidget::retranslate_ui()
{
    setLocale(QLocale());
    if (canvas_ != nullptr) {
        canvas_->retranslate_ui();
    }
}

void GanttTimelineWidget::changeEvent(QEvent *event)
{
    if (event && event->type() == QEvent::LanguageChange) {
        retranslate_ui();
    }
    QScrollArea::changeEvent(event);
}

// -------------------------------------------------------------------------
// GanttTimelineCanvas
// -------------------------------------------------------------------------

GanttTimelineCanvas::GanttTimelineCanvas(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
}

void GanttTimelineCanvas::retranslate_ui()
{
    setLocale(QLocale());
    QToolTip::hideText();
    update();
}

void GanttTimelineCanvas::changeEvent(QEvent *event)
{
    if (event && event->type() == QEvent::LanguageChange) {
        retranslate_ui();
    }
    QWidget::changeEvent(event);
}

void GanttTimelineCanvas::set_data(const Schedule &schedule, GanttZoom zoom, bool show_baseline, const QDateTime &ref_date)
{
    if (!schedule.items.isEmpty()) {
        schedule_ = schedule;
    }
    zoom_ = zoom;
    show_baseline_ = show_baseline;
    if (ref_date.isValid()) {
        reference_date_ = ref_date;
    }

    update_geometry_params();
    update();
}

void GanttTimelineCanvas::update_geometry_params()
{
    switch (zoom_) {
        case GanttZoom::Day:   pixels_per_hour_ = 4.0; break;
        case GanttZoom::Week:  pixels_per_hour_ = 1.0; break;
        case GanttZoom::Month: pixels_per_hour_ = 0.3; break;
    }

    min_date_ = schedule_.project_start_date();
    max_date_ = schedule_.project_finish_date();

    if (!min_date_.isValid()) {
        min_date_ = QDateTime::currentDateTime().addDays(-2);
    } else {
        min_date_ = min_date_.addDays(-3);
    }
    if (!max_date_.isValid()) {
        max_date_ = min_date_.addDays(30);
    } else {
        max_date_ = max_date_.addDays(7);
    }

    // Align min_date to start of day
    min_date_.setTime(QTime(0, 0));

    double total_hours = min_date_.secsTo(max_date_) / 3600.0;
    int total_width = static_cast<int>(total_hours * pixels_per_hour_) + 200;
    int total_height = header_height_ + (schedule_.items.size() * row_height_) + 50;

    resize(std::max(800, total_width), std::max(400, total_height));

    // Calculate layouts
    layouts_.clear();
    QList<ScheduleItem> items = schedule_.get_sorted_by_wbs();

    int row_idx = 0;
    for (const auto &item : items) {
        ItemLayout layout;
        layout.id = item.id;
        layout.is_milestone = item.is_milestone;
        layout.critical = item.critical;
        layout.progress = item.percent_complete;

        double y = header_height_ + (row_idx * row_height_);
        double x_start = date_to_x(item.start_date.isValid() ? item.start_date : min_date_);
        double x_end = date_to_x(item.end_date.isValid() ? item.end_date : item.start_date);

        if (item.is_milestone) {
            layout.bar_rect = QRectF(x_start - 7, y + 6, 14, 14);
        } else {
            double w = std::max(6.0, x_end - x_start);
            layout.bar_rect = QRectF(x_start, y + 5, w, show_baseline_ ? 12 : 18);

            QDateTime b_start = item.baseline_start_date;
            QDateTime b_end = item.baseline_end_date;
            if (!b_start.isValid()) {
                b_start = item.start_date;
            }
            if (!b_end.isValid()) {
                b_end = item.end_date.isValid() ? item.end_date : item.start_date;
            }
            if (b_start.isValid() && b_end.isValid()) {
                double bx_start = date_to_x(b_start);
                double bx_end = date_to_x(b_end);
                layout.baseline_rect = QRectF(bx_start, y + 18, std::max(6.0, bx_end - bx_start), 6);
            }
        }

        layouts_.insert(item.id, layout);
        row_idx++;
    }
}

double GanttTimelineCanvas::date_to_x(const QDateTime &dt) const
{
    if (!dt.isValid() || !min_date_.isValid()) return 0.0;
    double hours = min_date_.secsTo(dt) / 3600.0;
    return hours * pixels_per_hour_;
}

void GanttTimelineCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.fillRect(rect(), QColor(250, 250, 252));

    draw_grid(p);
    draw_dependencies(p);
    draw_bars(p);
    draw_reference_lines(p);
    draw_header(p);
}

void GanttTimelineCanvas::draw_grid(QPainter &p)
{
    if (!min_date_.isValid() || !max_date_.isValid()) return;

    QDateTime current = min_date_;
    p.setPen(QPen(QColor(230, 230, 235), 1, Qt::SolidLine));

    if (zoom_ == GanttZoom::Month) {
        // No Zoom Mês, destacar os limites de mês e semanas
        while (current <= max_date_) {
            double x = date_to_x(current);
            if (current.date().day() == 1) {
                p.setPen(QPen(QColor(200, 205, 215), 1.5, Qt::SolidLine));
                p.drawLine(QPointF(x, header_height_), QPointF(x, height()));
                p.setPen(QPen(QColor(235, 235, 240), 1, Qt::DashLine));
            } else if (current.date().dayOfWeek() == 1) { // Segunda-feira
                p.drawLine(QPointF(x, header_height_), QPointF(x, height()));
            }
            current = current.addDays(1);
        }
    } else {
        while (current <= max_date_) {
            double x = date_to_x(current);

            // Highlight weekend / non-working days
            if (!schedule_.calendar.is_working_day(current.date())) {
                double next_x = date_to_x(current.addDays(1));
                p.fillRect(QRectF(x, header_height_, next_x - x, height() - header_height_), QColor(240, 242, 245));
            }

            p.drawLine(QPointF(x, header_height_), QPointF(x, height()));
            current = current.addDays(1);
        }
    }

    // Horizontal row lines
    p.setPen(QPen(QColor(235, 235, 240), 1, Qt::SolidLine));
    int rows = schedule_.items.size();
    for (int i = 0; i <= rows; ++i) {
        int y = header_height_ + (i * row_height_);
        p.drawLine(0, y, width(), y);
    }
}

void GanttTimelineCanvas::draw_header(QPainter &p)
{
    QRectF header_rect(0, 0, width(), header_height_);
    p.fillRect(header_rect, QColor(245, 247, 250));
    p.setPen(QPen(QColor(210, 215, 222), 1));
    p.drawLine(0, header_height_, width(), header_height_);
    p.drawLine(0, header_height_ / 2, width(), header_height_ / 2);

    QFont font = p.font();
    font.setPointSize(8);
    font.setBold(true);
    p.setFont(font);

    if (!min_date_.isValid() || !max_date_.isValid()) return;

    if (zoom_ == GanttZoom::Month) {
        // ----------------------------------------------------
        // ZOOM MÊS: Linha superior = Ano; Linha inferior = Mês
        // ----------------------------------------------------
        int start_year = min_date_.date().year();
        int end_year = max_date_.date().year();

        // Linha Superior: Anos
        for (int y = start_year; y <= end_year; ++y) {
            QDate y_start_d(y, 1, 1);
            QDate next_y_d(y + 1, 1, 1);
            QDateTime dt_start = (QDateTime(y_start_d, QTime(0, 0)) < min_date_) ? min_date_ : QDateTime(y_start_d, QTime(0, 0));
            QDateTime dt_end = (QDateTime(next_y_d, QTime(0, 0)) > max_date_) ? max_date_ : QDateTime(next_y_d, QTime(0, 0));

            double x1 = date_to_x(dt_start);
            double x2 = date_to_x(dt_end);

            p.setPen(QPen(QColor(210, 215, 222), 1));
            p.drawLine(QPointF(x1, 0), QPointF(x1, header_height_ / 2));

            p.save();
            p.setClipRect(QRectF(x1, 0, x2 - x1, header_height_ / 2));
            p.setPen(QColor(55, 71, 79));
            p.drawText(QRectF(x1, 2, x2 - x1, header_height_ / 2 - 4),
                       Qt::AlignCenter,
                       QString::number(y));
            p.restore();
        }

        // Linha Inferior: Meses
        QDate cur_d = QDate(min_date_.date().year(), min_date_.date().month(), 1);
        QDate end_d = max_date_.date();

        while (cur_d <= end_d) {
            int y = cur_d.year();
            int m = cur_d.month();
            QDate m_first(y, m, 1);
            QDate next_m_first = m_first.addMonths(1);

            QDateTime dt_start = (QDateTime(m_first, QTime(0, 0)) < min_date_) ? min_date_ : QDateTime(m_first, QTime(0, 0));
            QDateTime dt_end = (QDateTime(next_m_first, QTime(0, 0)) > max_date_) ? max_date_ : QDateTime(next_m_first, QTime(0, 0));

            double x1 = date_to_x(dt_start);
            double x2 = date_to_x(dt_end);
            double w = x2 - x1;

            if (w > 0) {
                p.setPen(QPen(QColor(210, 215, 222), 1));
                p.drawLine(QPointF(x1, header_height_ / 2), QPointF(x1, header_height_));

                p.save();
                p.setClipRect(QRectF(x1, header_height_ / 2, w, header_height_ / 2));

                QFontMetrics fm(p.font());
                const QLocale loc = QLocale();
                QString full_m = capitalize_name(loc.monthName(m, QLocale::LongFormat));
                QString short_m = capitalize_name(loc.monthName(m, QLocale::ShortFormat));

                p.setPen(QColor(69, 90, 100));
                if (fm.horizontalAdvance(full_m) <= w - 6) {
                    p.drawText(QRectF(x1, header_height_ / 2, w, header_height_ / 2),
                               Qt::AlignCenter,
                               full_m);
                } else if (fm.horizontalAdvance(short_m) <= w - 4) {
                    p.drawText(QRectF(x1, header_height_ / 2, w, header_height_ / 2),
                               Qt::AlignCenter,
                               short_m);
                }

                p.restore();
            }

            cur_d = cur_d.addMonths(1);
        }
    } else {
        // ----------------------------------------------------
        // ZOOM SEMANA & ZOOM DIA: Top row = Mês e Ano delimitado; Bottom row = Dias
        // ----------------------------------------------------
        QDate cur_m = QDate(min_date_.date().year(), min_date_.date().month(), 1);
        QDate end_m = max_date_.date();

        // Linha Superior: Meses com delimitadores verticais
        while (cur_m <= end_m) {
            int y = cur_m.year();
            int m = cur_m.month();
            QDate m_first(y, m, 1);
            QDate next_m_first = m_first.addMonths(1);

            QDateTime dt_start = (QDateTime(m_first, QTime(0, 0)) < min_date_) ? min_date_ : QDateTime(m_first, QTime(0, 0));
            QDateTime dt_end = (QDateTime(next_m_first, QTime(0, 0)) > max_date_) ? max_date_ : QDateTime(next_m_first, QTime(0, 0));

            double x1 = date_to_x(dt_start);
            double x2 = date_to_x(dt_end);
            double w = x2 - x1;

            if (w > 0) {
                p.setPen(QPen(QColor(210, 215, 222), 1));
                p.drawLine(QPointF(x1, 0), QPointF(x1, header_height_ / 2));

                p.save();
                p.setClipRect(QRectF(x1, 0, w, header_height_ / 2));

                QFontMetrics fm(p.font());
                const QLocale loc = QLocale();
                const QString month_long = capitalize_name(loc.monthName(m, QLocale::LongFormat));
                const QString month_short = capitalize_name(loc.monthName(m, QLocale::ShortFormat));

                const QString full_txt = QStringLiteral("%1 %2").arg(month_long, QString::number(y));
                const QString short_txt = QStringLiteral("%1 %2").arg(month_short, QString::number(y).right(2));
                const QString month_only = month_short;

                p.setPen(QColor(55, 71, 79));
                if (fm.horizontalAdvance(full_txt) <= w - 8) {
                    p.drawText(QRectF(x1 + 4, 2, w - 8, header_height_ / 2 - 4),
                               Qt::AlignLeft | Qt::AlignVCenter,
                               full_txt);
                } else if (fm.horizontalAdvance(short_txt) <= w - 6) {
                    p.drawText(QRectF(x1 + 3, 2, w - 6, header_height_ / 2 - 4),
                               Qt::AlignCenter,
                               short_txt);
                } else if (fm.horizontalAdvance(month_only) <= w - 4) {
                    p.drawText(QRectF(x1 + 2, 2, w - 4, header_height_ / 2 - 4),
                               Qt::AlignCenter,
                               month_only);
                }

                p.restore();
            }

            cur_m = cur_m.addMonths(1);
        }

        // Linha Inferior: Dias
        QDateTime current = min_date_;
        const QLocale loc = QLocale();
        while (current <= max_date_) {
            double x = date_to_x(current);
            double next_day_x = date_to_x(current.addDays(1));
            double day_w = next_day_x - x;

            p.setPen(QPen(QColor(230, 230, 235), 1));
            p.drawLine(QPointF(x, header_height_ / 2), QPointF(x, header_height_));

            const int dow = current.date().dayOfWeek();
            const QString day_abbr = capitalize_name(loc.dayName(dow, QLocale::ShortFormat));

            QString day_label = (zoom_ == GanttZoom::Day && day_w >= 60)
                ? QStringLiteral("%1 %2").arg(QString::number(current.date().day()), day_abbr)
                : QString::number(current.date().day());

            p.setPen(schedule_.calendar.is_working_day(current.date()) ? QColor(69, 90, 100) : QColor(176, 190, 197));
            p.drawText(QRectF(x, header_height_ / 2, day_w, header_height_ / 2),
                       Qt::AlignCenter,
                       day_label);

            current = current.addDays(1);
        }
    }
}

void GanttTimelineCanvas::draw_bars(QPainter &p)
{
    QFont label_font = p.font();
    label_font.setPointSize(8);
    label_font.setBold(false);
    p.setFont(label_font);

    const QDateTime eval_date = reference_date_.isValid() ? reference_date_ : QDateTime::currentDateTime();

    for (auto it = layouts_.begin(); it != layouts_.end(); ++it) {
        const ItemLayout &l = it.value();
        const ScheduleItem *item = schedule_.find_item(l.id);
        if (!item) continue;

        const Bim4DStatus item_status = item->simulation_status(eval_date);
        const QColor status_color = item->status_font_color(eval_date);
        const bool is_bold = (item_status != Bim4DStatus::Unstarted);

        if (l.is_milestone) {
            // Draw diamond (nó de atividade)
            QPolygonF diamond;
            QPointF center = l.bar_rect.center();
            double size = 7.0;
            diamond << QPointF(center.x(), center.y() - size)
                    << QPointF(center.x() + size, center.y())
                    << QPointF(center.x(), center.y() + size)
                    << QPointF(center.x() - size, center.y());

            const GanttNodeColors node_colors = ScheduleItem::status_to_node_colors(item_status, l.critical);
            p.setBrush(node_colors.fill);
            p.setPen(QPen(node_colors.border, 1.5));
            p.drawPolygon(diamond);

            // Destaque visual refinado para nós de marcos concluídos
            if (item_status == Bim4DStatus::Executed) {
                p.setBrush(Qt::white);
                p.setPen(Qt::NoPen);
                p.drawEllipse(center, 2.0, 2.0);
            }

            // Label
            label_font.setBold(is_bold);
            p.setFont(label_font);
            p.setPen(status_color);
            p.drawText(QPointF(center.x() + 10, center.y() + 4), item->name);
        } else {
            // Draw baseline ghost bar if enabled
            if (show_baseline_ && l.baseline_rect.isValid()) {
                p.setBrush(QColor(200, 210, 218, 160));
                p.setPen(QPen(QColor(144, 164, 174), 1, Qt::DashLine));
                p.drawRoundedRect(l.baseline_rect, 2, 2);
            }

            // Draw active bar with status coloring & differentiated tonality for completed activities
            const GanttBarColors bar_colors = ScheduleItem::status_to_bar_colors(item_status, l.critical);

            p.setBrush(bar_colors.bg);
            p.setPen(QPen(bar_colors.border, 1.2));
            p.drawRoundedRect(l.bar_rect, 3, 3);

            // Progress fill (atividades concluídas recebem 100% de preenchimento diferenciado em verde)
            const double effective_prog = (item_status == Bim4DStatus::Executed) ? 100.0 : l.progress;
            if (effective_prog > 0.0) {
                double fill_w = (l.bar_rect.width() * qBound(0.0, effective_prog, 100.0)) / 100.0;
                QRectF prog_rect(l.bar_rect.x(), l.bar_rect.y(), fill_w, l.bar_rect.height());

                QLinearGradient prog_grad(prog_rect.topLeft(), prog_rect.bottomLeft());
                prog_grad.setColorAt(0.0, bar_colors.progress_start);
                prog_grad.setColorAt(1.0, bar_colors.progress_end);

                p.setBrush(prog_grad);
                p.setPen(Qt::NoPen);
                p.drawRoundedRect(prog_rect, 3, 3);

                // Re-aplica contorno da barra para acabamento nítido e profissional
                p.setBrush(Qt::NoBrush);
                p.setPen(QPen(bar_colors.border, 1.2));
                p.drawRoundedRect(l.bar_rect, 3, 3);

                // Emblema interno diferenciado:
                // Para atividades concluídas: símbolo '✓ 100%' ou '✓' em branco com alto contraste
                // Para atividades em andamento / atrasadas: porcentagem numérica '%', se houver espaço
                p.save();
                QFont badge_font = p.font();
                badge_font.setPointSize(7);
                badge_font.setBold(true);
                p.setFont(badge_font);
                p.setPen(Qt::white);

                if (item_status == Bim4DStatus::Executed) {
                    if (l.bar_rect.width() >= 48.0) {
                        p.drawText(l.bar_rect, Qt::AlignCenter, QStringLiteral("✓ 100%"));
                    } else if (l.bar_rect.width() >= 18.0) {
                        p.drawText(l.bar_rect, Qt::AlignCenter, QStringLiteral("✓"));
                    }
                } else if (fill_w >= 36.0 && effective_prog >= 10.0) {
                    p.drawText(QRectF(l.bar_rect.x(), l.bar_rect.y(), fill_w, l.bar_rect.height()),
                               Qt::AlignCenter,
                               QStringLiteral("%1%").arg(static_cast<int>(effective_prog)));
                }
                p.restore();
            }

            // Activity title text beside bar
            label_font.setBold(is_bold);
            p.setFont(label_font);
            p.setPen(status_color);
            p.drawText(QPointF(l.bar_rect.right() + 6, l.bar_rect.center().y() + 4),
                       QStringLiteral("%1 (%2%)").arg(item->name).arg(static_cast<int>(item->percent_complete)));
        }
    }
}

void GanttTimelineCanvas::draw_dependencies(QPainter &p)
{
    p.setRenderHint(QPainter::Antialiasing, true);

    for (const auto &dep : schedule_.dependencies) {
        if (!layouts_.contains(dep.predecessor_item_id) || !layouts_.contains(dep.successor_item_id)) {
            continue;
        }

        const ItemLayout &pred_l = layouts_[dep.predecessor_item_id];
        const ItemLayout &succ_l = layouts_[dep.successor_item_id];

        bool is_crit_link = pred_l.critical && succ_l.critical;
        p.setPen(QPen(is_crit_link ? QColor(211, 47, 47) : QColor(120, 144, 156), is_crit_link ? 2.0 : 1.2));
        p.setBrush(is_crit_link ? QColor(211, 47, 47) : QColor(120, 144, 156));

        QPointF p_from;
        QPointF p_to;

        switch (dep.type) {
            case DependencyType::FinishToStart:
                p_from = QPointF(pred_l.bar_rect.right(), pred_l.bar_rect.center().y());
                p_to = QPointF(succ_l.bar_rect.left(), succ_l.bar_rect.center().y());
                break;
            case DependencyType::StartToStart:
                p_from = QPointF(pred_l.bar_rect.left(), pred_l.bar_rect.center().y());
                p_to = QPointF(succ_l.bar_rect.left(), succ_l.bar_rect.center().y());
                break;
            case DependencyType::FinishToFinish:
                p_from = QPointF(pred_l.bar_rect.right(), pred_l.bar_rect.center().y());
                p_to = QPointF(succ_l.bar_rect.right(), succ_l.bar_rect.center().y());
                break;
            case DependencyType::StartToFinish:
                p_from = QPointF(pred_l.bar_rect.left(), pred_l.bar_rect.center().y());
                p_to = QPointF(succ_l.bar_rect.right(), succ_l.bar_rect.center().y());
                break;
        }

        // Draw orthogonal connector lines
        QPainterPath path;
        path.moveTo(p_from);

        if (p_to.x() >= p_from.x() + 10) {
            double mid_x = (p_from.x() + p_to.x()) / 2.0;
            path.lineTo(mid_x, p_from.y());
            path.lineTo(mid_x, p_to.y());
            path.lineTo(p_to.x(), p_to.y());
        } else {
            double offset_x = p_from.x() + 8;
            path.lineTo(offset_x, p_from.y());
            double mid_y = (p_from.y() + p_to.y()) / 2.0;
            path.lineTo(offset_x, mid_y);
            path.lineTo(p_to.x() - 8, mid_y);
            path.lineTo(p_to.x() - 8, p_to.y());
            path.lineTo(p_to.x(), p_to.y());
        }

        p.drawPath(path);

        // Arrowhead at destination
        QPolygonF arrow;
        double arrow_size = 5.0;
        arrow << p_to
              << QPointF(p_to.x() - arrow_size, p_to.y() - arrow_size * 0.7)
              << QPointF(p_to.x() - arrow_size, p_to.y() + arrow_size * 0.7);
        p.drawPolygon(arrow);
    }
}

void GanttTimelineCanvas::draw_reference_lines(QPainter &p)
{
    // Today line
    QDateTime now = QDateTime::currentDateTime();
    if (now >= min_date_ && now <= max_date_) {
        double now_x = date_to_x(now);
        p.setPen(QPen(QColor(244, 67, 54), 1.5, Qt::DashLine));
        p.drawLine(QPointF(now_x, header_height_), QPointF(now_x, height()));

        // Badge at top
        p.setBrush(QColor(244, 67, 54));
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(QRectF(now_x - 18, header_height_ - 14, 36, 12), 2, 2);
        p.setPen(Qt::white);
        QFont f = p.font();
        f.setPointSize(7);
        p.setFont(f);
        p.drawText(QRectF(now_x - 18, header_height_ - 14, 36, 12), Qt::AlignCenter, tr("HOJE"));
    }

    // 4D Simulation Reference date line
    if (reference_date_.isValid() && reference_date_ >= min_date_ && reference_date_ <= max_date_) {
        double ref_x = date_to_x(reference_date_);
        p.setPen(QPen(QColor(33, 150, 243), 2.0, Qt::SolidLine));
        p.drawLine(QPointF(ref_x, header_height_), QPointF(ref_x, height()));

        p.setBrush(QColor(33, 150, 243));
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(QRectF(ref_x - 22, header_height_ - 26, 44, 12), 2, 2);
        p.setPen(Qt::white);
        QFont f = p.font();
        f.setPointSize(7);
        p.setFont(f);
        p.drawText(QRectF(ref_x - 22, header_height_ - 26, 44, 12), Qt::AlignCenter, QStringLiteral("BIM 4D"));
    }
}

void GanttTimelineCanvas::mousePressEvent(QMouseEvent *event)
{
    for (auto it = layouts_.begin(); it != layouts_.end(); ++it) {
        if (it.value().bar_rect.contains(event->pos())) {
            const QString item_id = it.key();
            emit item_clicked(item_id);
            if (event->button() == Qt::RightButton) {
                QMenu menu(this);
                auto *act_edit = menu.addAction(tr("✏️ Editar Atividade..."));
                connect(act_edit, &QAction::triggered, this, [this, item_id]() {
                    emit item_double_clicked(item_id);
                });
                auto *act_del = menu.addAction(tr("❌ Excluir Atividade"));
                connect(act_del, &QAction::triggered, this, [this, item_id]() {
                    emit request_delete_item(item_id);
                });
                menu.exec(event->globalPosition().toPoint());
                return;
            }
            return;
        }
    }
    QWidget::mousePressEvent(event);
}

void GanttTimelineCanvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    for (auto it = layouts_.begin(); it != layouts_.end(); ++it) {
        if (it.value().bar_rect.contains(event->pos())) {
            emit item_double_clicked(it.key());
            return;
        }
    }
    QWidget::mouseDoubleClickEvent(event);
}

void GanttTimelineCanvas::mouseMoveEvent(QMouseEvent *event)
{
    const QDateTime eval_date = reference_date_.isValid() ? reference_date_ : QDateTime::currentDateTime();

    for (auto it = layouts_.begin(); it != layouts_.end(); ++it) {
        if (it.value().bar_rect.contains(event->pos())) {
            const ScheduleItem *item = schedule_.find_item(it.key());
            if (item) {
                const QString str_sim = tr("SIM");
                const QString str_nao = tr("Não");
                const QString crit_text = item->critical
                    ? QStringLiteral("<font color='red'>%1</font>").arg(str_sim)
                    : str_nao;

                const Bim4DStatus item_status = item->simulation_status(eval_date);
                const QColor status_col = item->status_font_color(eval_date);
                QString status_name;
                switch (item_status) {
                    case Bim4DStatus::Executed:
                        status_name = tr("✅ Concluída / Executada");
                        break;
                    case Bim4DStatus::PlannedInProgress:
                        status_name = tr("🔄 Em Andamento");
                        break;
                    case Bim4DStatus::DelayedStart:
                        status_name = tr("⚠️ Atrasado (Início Pendente)");
                        break;
                    case Bim4DStatus::DelayedOverdue:
                        status_name = tr("⚠️ Atrasado (Prazo Excedido)");
                        break;
                    case Bim4DStatus::Unstarted:
                    default:
                        status_name = tr("⏳ Futuro (Não Iniciado)");
                        break;
                }

                const QLocale loc = locale();
                const QString dt_fmt = (loc.language() == QLocale::English)
                    ? QStringLiteral("MM/dd/yyyy HH:mm")
                    : QStringLiteral("dd/MM/yyyy HH:mm");

                const QString start_str = item->start_date.isValid() ? loc.toString(item->start_date, dt_fmt) : QStringLiteral("-");
                const QString end_str = item->end_date.isValid() ? loc.toString(item->end_date, dt_fmt) : QStringLiteral("-");
                const QString dur_str = QString::number(item->duration_hours / 8.0, 'f', 1);
                const QString prog_str = QString::number(static_cast<int>(item->percent_complete));
                const QString float_str = QString::number(item->total_float_hours, 'f', 1);

                QStringList lines;
                lines << QStringLiteral("<b>%1</b> (WBS %2)").arg(item->name, item->wbs_code);
                lines << QStringLiteral("%1: <font color='%2'><b>%3</b></font>").arg(tr("Status"), status_col.name(), status_name);
                lines << QStringLiteral("%1: %2").arg(tr("Início"), start_str);
                lines << QStringLiteral("%1: %2").arg(tr("Fim"), end_str);
                lines << QStringLiteral("%1: %2 %3").arg(tr("Duração"), dur_str, tr("dias"));
                lines << QStringLiteral("%1: %2").arg(tr("Progresso"), prog_str + QStringLiteral("%"));
                lines << QStringLiteral("%1: %2").arg(tr("Caminho Crítico"), crit_text);
                lines << QStringLiteral("%1: %2").arg(tr("Folga Total"), float_str + QStringLiteral("h"));

                const QString tip = lines.join(QStringLiteral("<br>"));
                QToolTip::showText(event->globalPosition().toPoint(), tip, this);
                return;
            }
        }
    }

    if (event->pos().y() <= header_height_ && pixels_per_hour_ > 0.0 && min_date_.isValid()) {
        double hours = event->pos().x() / pixels_per_hour_;
        QDateTime dt = min_date_.addSecs(static_cast<qint64>(hours * 3600.0));
        QDate date = dt.date();
        if (schedule_.calendar.is_holiday(date)) {
            const QLocale loc = locale();
            const QString hol_date_fmt = (loc.language() == QLocale::English)
                ? QStringLiteral("MM/dd/yyyy")
                : QStringLiteral("dd/MM/yyyy");
            QString name = schedule_.calendar.holiday_name(date);
            QString tip = tr("🏖️ Feriado: %1 (%2)").arg(name.isEmpty() ? tr("Feriado") : name, date.toString(hol_date_fmt));
            QToolTip::showText(event->globalPosition().toPoint(), tip, this);
            return;
        }
    }

    QToolTip::hideText();
    QWidget::mouseMoveEvent(event);
}

} // namespace source::gantt
