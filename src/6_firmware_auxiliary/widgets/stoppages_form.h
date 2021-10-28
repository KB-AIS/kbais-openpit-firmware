#ifndef STOPPAGES_FORM_H
#define STOPPAGES_FORM_H

#include <QWidget>

namespace Ui {
class stoppages_form;
}

class stoppages_form : public QWidget
{
    Q_OBJECT

public:
    explicit stoppages_form(QWidget *parent = nullptr);
    ~stoppages_form();

private:
    Ui::stoppages_form *ui;
};

#endif // STOPPAGES_FORM_H
