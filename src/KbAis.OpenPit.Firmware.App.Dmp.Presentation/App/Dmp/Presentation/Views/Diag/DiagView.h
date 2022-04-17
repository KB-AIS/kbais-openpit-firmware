#ifndef DIAGVIEW_H
#define DIAGVIEW_H

// qt
#include <QWidget>

#include "App/Dmp/Presentation/Utils/NavController.h"
#include "RxQt.h"

namespace Ui {

class DiagView;

}

class DiagView : public QWidget{
    Q_OBJECT

public:
    DiagView(nav_controller& navigationEmmiter);

    ~DiagView();

private:
    nav_controller& m_navEmmiter;

    Ui::DiagView* ui;

    rxcpp::composite_subscription m_subsBag;

};

#endif // DIAGVIEW_H
