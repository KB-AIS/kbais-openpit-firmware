#ifndef MAIN_FORM_H
#define MAIN_FORM_H

// Qt
#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QWidget>

namespace Ui {
class main_form;
}

/*!
 * \brief The main_form class
 */
class main_form : public QWidget
{
    Q_OBJECT

public:
    explicit main_form(QWidget *parent = nullptr);
    ~main_form();

public slots:
    void update_gps_data_slot();

private:
    Ui::main_form *ui;
    /*!
     * \brief Timer to update current time on screen.
     */
    QTimer *m_timer_update_time;
};

#endif // MAIN_FORM_H
