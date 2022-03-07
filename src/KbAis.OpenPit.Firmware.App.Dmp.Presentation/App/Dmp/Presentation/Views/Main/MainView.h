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

    Ui::MainView* ui;

    const IRxGpsSensorPublisher&  m_pub_gps_msg;

    const RxFuelMessagePublisher& m_pub_ful_msg;

    const NavController& m_nav_controller;

    QcGaugeWidget* m_qcg_ful;
    QcNeedleItem*  m_qcg_ful_needle;

    QcGaugeWidget* m_qcg_spd;
    QcNeedleItem*  m_qcg_spd_needle;
    QcLabelItem*   m_qcg_spd_label;

    void setup_gauge_ful();

    void setup_gauge_spd();

    void update_gauge_ful(const FuelMessage& msg);

    void update_gague_spd(const GpsMessage& msg);

public:
    MainView(
        const IRxGpsSensorPublisher& pub_gps_msg
    ,   const RxFuelMessagePublisher& pub_ful_msg
    ,   const NavController& nav_controller
    );

    ~MainView();

    void provide_coordinator(const rxcpp::observe_on_one_worker& coordinator);

    Q_SIGNAL void notify_test_user_event();

private:
    rxcpp::composite_subscription m_subscriptions;

    QTimer m_tmUpdateDisplayTime;

    void OnUpdateDisplayDate();

};

#endif // MAINVIEW_H
