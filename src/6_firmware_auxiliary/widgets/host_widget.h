#ifndef HOST_WIDGET_H
#define HOST_WIDGET_H

#include <QWidget>

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
};

#endif // HOST_WIDGET_H
