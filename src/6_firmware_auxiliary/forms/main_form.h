#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QWidget>

namespace Ui {
class main_form;
}

class main_form : public QWidget
{
    Q_OBJECT

public:
    explicit main_form(QWidget *parent = nullptr);
    ~main_form();

private:
    Ui::main_form *ui;

    QTimer *m_timer_update_time;
};

#endif // MAIN_FORM_H
