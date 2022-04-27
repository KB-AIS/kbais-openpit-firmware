#ifndef DIAG_VIEW_H
#define DIAG_VIEW_H

// qt
#include <QtWidgets/QWidget>

#include "RxQt.h"

#include "opf/app/view/dmp/nav/nav_controller.h"

namespace Ui {

class diag_view;

}

class diag_view : public QWidget{
    Q_OBJECT

    Ui::diag_view* ui_;

    nav_controller& nav_controller_;

    rxcpp::composite_subscription subscriptions_;

public:
    diag_view(nav_controller& nav_controller);

    ~diag_view();

};

#endif // DIAG_VIEW_H
