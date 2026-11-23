#pragma once

#include <QStringList>
#include <QWidget>
#include <functional>

class QMouseEvent;
class QPaintEvent;

namespace source::GerenciamentoUI {

enum class TabOrientation {
    Vertical,
    Horizontal
};

class RotatedTabButton : public QWidget {
    Q_OBJECT

public:
    explicit RotatedTabButton(const QString &text, QWidget *parent = nullptr, TabOrientation orientation = TabOrientation::Vertical);

    void setOrientation(TabOrientation orientation);
    TabOrientation orientation() const;
    void setText(const QString &text);
    QString text() const;
    void setSizeReferenceTexts(const QStringList &texts);
    void setClickHandler(std::function<void()> handler);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void _recompute_size();

    QString _text;
    QStringList _size_reference_texts;
    TabOrientation _orientation = TabOrientation::Vertical;
    std::function<void()> _click_handler;
};

}  // namespace source::GerenciamentoUI
