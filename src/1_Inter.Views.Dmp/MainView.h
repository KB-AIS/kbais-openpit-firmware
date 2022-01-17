#ifndef MAINVIEW_H
#define MAINVIEW_H

// qt
#include <QTimer>
#include <QWidget>
// oss
#include <plog/Log.h>
#include <rxcpp/rx.hpp>

// cfw::infra::sensors::gps
#include "GpsMessage.h"
#include "IRxGpsSensorPublisher.h"

namespace Ui { class MainView; }

class MainView : public QWidget {
    Q_OBJECT

public:
    MainView(const IRxGpsSensorPublisher& gpsSensorPublisher);

    MainView(const MainView& v) : QWidget() {};

    ~MainView();

    Q_SIGNAL void notifyTestUserEvent();

private:
    Ui::MainView* ui;

    rxcpp::composite_subscription subs;

    /*!
     * \brief Timer to update current time on screen.
     */
    QTimer* tUpdateDisplayTime;

};

#endif // MAINVIEW_H
