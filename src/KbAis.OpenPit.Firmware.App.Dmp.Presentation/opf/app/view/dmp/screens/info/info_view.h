#ifndef INFO_VIEW_H
#define INFO_VIEW_H

#include <QWidget>

namespace Ui {
class info_view;
}

class info_view : public QWidget
{
    Q_OBJECT

public:
    explicit info_view(QWidget *parent = nullptr);
    ~info_view();

private:
    Ui::info_view *ui;
};

#endif // INFO_VIEW_H
