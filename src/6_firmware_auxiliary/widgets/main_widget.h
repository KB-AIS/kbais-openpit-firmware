#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <memory>

#include <QTimer>
#include <QWidget>

enum class form_names {
    main,
    stoppages,
};

namespace Ui {
class host_widget;
}

class host_widget : public QWidget {
    Q_OBJECT

public:
    explicit host_widget(QWidget *parent = nullptr);
    ~host_widget();

private:
    Ui::host_widget *ui;

    QTimer *m_timer_update_time;
};

#endif // MAIN_WIDGET_H
