#include "source/ui/ui_13_RotatedTabButton.hpp"

#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QSize>

#include "source/utils/LogManager.hpp"

namespace source::GerenciamentoUI {
namespace {

source::utils::LogManager *logger()
{
    return source::utils::LogManager::get_logger();
}

}  // namespace

RotatedTabButton::RotatedTabButton(const QString &text, QWidget *parent, TabOrientation orientation)
    : QWidget(parent), _text(text), _orientation(orientation)
{
    try {
        if (_orientation == TabOrientation::Vertical) {
            setMinimumWidth(25);
        } else {
            setMinimumHeight(24);
        }
        _recompute_size();
    } catch (...) {
        logger()->error(QStringLiteral("Erro ao inicializar RotatedTabButton."));
    }
}

void RotatedTabButton::setOrientation(TabOrientation orientation)
{
    _orientation = orientation;
    _recompute_size();
    update();
}

TabOrientation RotatedTabButton::orientation() const
{
    return _orientation;
}

void RotatedTabButton::setText(const QString &text)
{
    try {
        _text = text;
        _recompute_size();
        update();
    } catch (...) {
        logger()->error(QStringLiteral("Erro ao definir texto do botão de aba."));
    }
}

QString RotatedTabButton::text() const
{
    return _text;
}

void RotatedTabButton::setSizeReferenceTexts(const QStringList &texts)
{
    _size_reference_texts = texts;
    _recompute_size();
    update();
}

void RotatedTabButton::_recompute_size()
{
    try {
        QFontMetrics fm(font());
        int text_w = fm.horizontalAdvance(_text);
        for (const QString &ref : _size_reference_texts) {
            text_w = qMax(text_w, fm.horizontalAdvance(ref));
        }
        const int text_h = fm.height();
        if (_orientation == TabOrientation::Vertical) {
            const int width = text_h + 5;
            const int height = text_w + 20;
            setFixedSize(QSize(width, height));
        } else {
            const int width = text_w + 24;
            const int height = text_h + 10;
            setFixedHeight(height);
            setMinimumWidth(width);
        }
    } catch (...) {
        logger()->error(QStringLiteral("Erro ao recalcular tamanho do botão de aba."));
    }
}

void RotatedTabButton::setClickHandler(std::function<void()> handler)
{
    _click_handler = std::move(handler);
}

void RotatedTabButton::mousePressEvent(QMouseEvent *event)
{
    try {
        if (_click_handler) {
            _click_handler();
        } else if (parentWidget() != nullptr) {
            QMetaObject::invokeMethod(parentWidget(), "toggle_panel");
        }
    } catch (...) {
        logger()->error(QStringLiteral("Erro ao processar clique no botão de aba."));
    }

    QWidget::mousePressEvent(event);
}

void RotatedTabButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    try {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.save();
        painter.fillRect(rect(), palette().button().color());
        painter.setPen(palette().dark().color());
        painter.drawRect(0, 0, width() - 1, height() - 1);

        if (_orientation == TabOrientation::Vertical) {
            painter.translate(0, height());
            painter.rotate(270);
            const QFontMetrics fm = painter.fontMetrics();
            const int text_w = fm.horizontalAdvance(_text);
            const qreal x = (height() - text_w) / 2.0;
            const qreal y = (width() + fm.ascent() - fm.descent()) / 2.0;
            painter.setPen(palette().buttonText().color());
            painter.drawText(QPointF(x, y), _text);
        } else {
            painter.setPen(palette().buttonText().color());
            painter.drawText(rect(), Qt::AlignCenter, _text);
        }
        painter.restore();
    } catch (...) {
        logger()->error(QStringLiteral("Erro ao pintar botão de aba."));
    }
}

}  // namespace source::GerenciamentoUI
