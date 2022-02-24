#ifndef MAINVIEW_H
#define MAINVIEW_H

// qt
#include <QTimer>
#include <QWidget>

#include "App/Dmp/Presentation/Utils/NavController.h"
#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "IRxGpsSensorPublisher.h"
#include "Modules/Sensors/Serials/RxFuelMessagePublisher.h"
#include "RxQt.h"
#include "Utils/Widgets/QcGauge/QcGaugeWidget.h"

namespace Ui { class MainView; }

class MainView : public QWidget {
    Q_OBJECT

    const RxFuelMessagePublisher& m_pub_fuel_message;

    QcGaugeWidget qc_gauge_fuel;

    QcLabelItem* qc_gauge_lable_fuel;

    QcNeedleItem* qc_gauge_needle_fuel;

    void setup_fuel_gauge();

public:
    MainView(
        const IRxGpsSensorPublisher& gpsPublisher
    ,   const IRxMessageSendersDiagPub& messageSenderPub
    ,   const RxFuelMessagePublisher& pub_fuel_message
    ,   const NavController& navigationEmmiter
    );

    ~MainView();

    void provide_coordinator(const rxcpp::observe_on_one_worker& coordinator);

    Q_SIGNAL void notifyTestUserEvent();

private:
    Ui::MainView* ui;

    const IRxGpsSensorPublisher& m_gpsMessagePub;

    const IRxMessageSendersDiagPub& m_messageSenderPub;

    const NavController& m_nav_controller;

    rxcpp::composite_subscription m_subscriptions;

    QTimer m_tmUpdateDisplayTime;

    void OnUpdateDisplayDate();

};

#endif // MAINVIEW_H
