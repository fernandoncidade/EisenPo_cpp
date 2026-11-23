#pragma once

#include <QListWidget>

namespace source::plugin::Pomodoro {

class _KanbanListWidget : public QListWidget {
    Q_OBJECT

public:
    explicit _KanbanListWidget(QWidget *parent = nullptr);

signals:
    void tarefas_soltadas();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

}  // namespace source::plugin::Pomodoro
