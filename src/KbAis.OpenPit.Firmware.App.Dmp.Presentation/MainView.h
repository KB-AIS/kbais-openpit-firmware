#ifndef MAINVIEW_H
#define MAINVIEW_H

// qt
#include <QTimer>
#include <QWidget>

#include "NavigationEmmiter.h"
#include "RxQt.h"

namespace Ui { class MainView; }

class MainView : public QWidget {
    Q_OBJECT

public:
    MainView(const NavEmmiter& navigationEmmiter);

    ~MainView();

    Q_SIGNAL void notifyTestUserEvent();

private:
    Ui::MainView* ui;

    const NavEmmiter& m_navigationEmmiter;

    rxcpp::composite_subscription m_subscriptions;

    /*!
     * \brief Timer to update current time on screen.
     */
    QTimer m_tmUpdateDisplayTime;

    void OnUpdateDisplayTime();

};

#endif // MAINVIEW_H
