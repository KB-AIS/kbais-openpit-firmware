#ifndef HOSTWINDOW_H
#define HOSTWINDOW_H

#include <QMainWindow>

#include "App/Dmp/Presentation/Utils/NavController.h"
#include "App/Dmp/Presentation/Views/Diag/DiagView.h"
#include "App/Dmp/Presentation/Views/Main/main_view.h"
#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "IRxGpsSensorPublisher.h"
#include "RxQt.h"

namespace Ui {
    class HostWindow;
}

class HostWindow : public QMainWindow {
    Q_OBJECT

public:
    HostWindow(
        const i_gps_sensor_publisher& gpsDiagPub
    ,   const IRxMessageSendersDiagPub& netDiagPub
    ,   i_main_view& mainView
    ,   DiagView& diagView
    ,   nav_controller& navController
    );

    ~HostWindow();

private:
    Ui::HostWindow* ui;

    const i_gps_sensor_publisher& m_gpsDiagPub;

    const IRxMessageSendersDiagPub& m_netDiagPub;

    i_main_view& m_mainView;

    DiagView& m_diagView;

    nav_controller& m_navController;

    rxcpp::composite_subscription m_rxSubs;

    rxqt::run_loop m_rxRunLoop;

    QTimer m_tmUpdateDisplayTime;

    void SetupScreenStack();

    void SetupAppBar();

    void UpdateDisplayTime();

};

#endif // HOSTWINDOW_H
