#ifndef HOSTVIEW_H
#define HOSTVIEW_H

#include <QWidget>

namespace Ui {
class HostView;
}

class HostView : public QWidget
{
    Q_OBJECT

public:
    explicit HostView(QWidget *parent = nullptr);
    ~HostView();

private:
    Ui::HostView *ui;
};

#endif // HOSTVIEW_H
