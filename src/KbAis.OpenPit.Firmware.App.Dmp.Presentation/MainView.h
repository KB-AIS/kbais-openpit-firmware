#ifndef MAINVIEW_H
#define MAINVIEW_H

// qt
#include <QTimer>
#include <QWidget>

#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "IRxGpsSensorPublisher.h"
#include "NavigationEmmiter.h"
#include "RxQt.h"

namespace Ui { class MainView; }

class MainView : public QWidget {
    Q_OBJECT

public:
    MainView(
        const IRxGpsSensorPublisher& gpsPublisher
    ,   const IRxMessageSendersDiagPub& messageSenderPub
    ,   const NavEmmiter& navigationEmmiter
    );

    ~MainView();

    Q_SIGNAL void notifyTestUserEvent();

    void StartObserveOn(const rxcpp::observe_on_one_worker& coordinator) const;

private:
    Ui::MainView* ui;

    const IRxGpsSensorPublisher& m_gpsMessagePub;

    const IRxMessageSendersDiagPub& m_messageSenderPub;

    const NavEmmiter& m_navigationEmmiter;

    rxcpp::composite_subscription m_subscriptions;

    QTimer m_tmUpdateDisplayTime;

    void OnUpdateDisplayTime();

};

#endif // MAINVIEW_H
