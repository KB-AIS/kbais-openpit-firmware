#ifndef MAIN_PRESENTER_H
#define MAIN_PRESENTER_H

// qt
#include <QTimer>
#include <QWidget>

namespace Ui {

class main_presenter;

}

/*!
 * \brief The main_form class
 */
class main_presenter : public QWidget {
    Q_OBJECT

public:
    main_presenter(QWidget* parent = nullptr);

    ~main_presenter();

public slots:
    void update_gps_data_slot();

private:
    Ui::main_presenter* ui;

    /*!
     * \brief Timer to update current time on screen.
     */
    QTimer* m_timer_update_time;

};

#endif // MAIN_PRESENTER_H
