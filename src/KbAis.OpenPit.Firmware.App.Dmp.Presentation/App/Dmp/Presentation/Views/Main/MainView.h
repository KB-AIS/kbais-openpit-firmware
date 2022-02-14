#ifndef MAINVIEW_H
#define MAINVIEW_H

// qt
#include <QTimer>
#include <QWidget>

#include "App/Dmp/Presentation/Utils/NavController.h"
#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "IRxGpsSensorPublisher.h"
#include "RxQt.h"

namespace Ui { class MainView; }

class MainView : public QWidget {
    Q_OBJECT

public:
    MainView(
        const IRxGpsSensorPublisher& gpsPublisher
    ,   const IRxMessageSendersDiagPub& messageSenderPub
    ,   const NavController& navigationEmmiter
    );

    ~MainView();

    Q_SIGNAL void notifyTestUserEvent();

private:
    Ui::MainView* ui;

    const IRxGpsSensorPublisher& m_gpsMessagePub;

    const IRxMessageSendersDiagPub& m_messageSenderPub;

    const NavController& m_navigationEmmiter;

    rxcpp::composite_subscription m_subscriptions;

    QTimer m_tmUpdateDisplayTime;

    void OnUpdateDisplayDate();

};

#endif // MAINVIEW_H
