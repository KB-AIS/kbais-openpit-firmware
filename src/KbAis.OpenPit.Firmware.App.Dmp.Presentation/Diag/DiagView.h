#ifndef DIAGVIEW_H
#define DIAGVIEW_H

// qt
#include <QWidget>

#include "NavigationEmmiter.h"
#include "RxQt.h"

namespace Ui {

class DiagView;

}

class DiagView : public QWidget{
    Q_OBJECT

public:
    DiagView(NavEmmiter& navigationEmmiter);

    ~DiagView();

private:
    NavEmmiter& m_navEmmiter;

    Ui::DiagView* ui;

    rxcpp::composite_subscription m_subscriptions;

};

#endif // DIAGVIEW_H
