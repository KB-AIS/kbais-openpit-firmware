#ifndef MAIN_PRESENTER_H
#define MAIN_PRESENTER_H

// qt
#include <QTimer>
#include <QWidget>

#include "GpsUpdateDto.h"

namespace Ui { class main_presenter; }

class main_presenter : public QWidget {
    Q_OBJECT

public:
    main_presenter(QWidget* parent = nullptr);

    ~main_presenter();

    Q_SLOT void handleGpsDataUpdated(const GpsUpdateDto& gpsUpdate);

    Q_SIGNAL void notifyTestUserEvent();

private:
    Ui::main_presenter* ui;

    /*!
     * \brief Timer to update current time on screen.
     */
    QTimer* timerUpdateTime;

};

#endif // MAIN_PRESENTER_H
