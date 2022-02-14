#ifndef HOSTWINDOW_H
#define HOSTWINDOW_H

#include <QMainWindow>

#include "App/Dmp/Presentation/Utils/NavController.h"
#include "App/Dmp/Presentation/Views/Diag/DiagView.h"
#include "App/Dmp/Presentation/Views/Main/MainView.h"
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
        const IRxGpsSensorPublisher& gpsDiagPub
    ,   const IRxMessageSendersDiagPub& netDiagPub
    ,   MainView& mainView
    ,   DiagView& diagView
    ,   NavController& navController
    );

    ~HostWindow();

private:
    Ui::HostWindow* ui;

    const IRxGpsSensorPublisher& m_gpsDiagPub;

    const IRxMessageSendersDiagPub& m_netDiagPub;

    MainView& m_mainView;

    DiagView& m_diagView;

    NavController& m_navController;

    rxcpp::composite_subscription m_rxSubs;

    rxqt::run_loop m_rxRunLoop;

    QTimer m_tmUpdateDisplayTime;

    void SetupScreenStack();

    void SetupAppBar();

    void UpdateDisplayTime();

};

#endif // HOSTWINDOW_H
