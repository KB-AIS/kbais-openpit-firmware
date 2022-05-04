#ifndef DIAG_VIEW_H
#define DIAG_VIEW_H

// qt
#include <QtWidgets/QWidget>

#include "rxqt.h"

#include "opf/app/view/dmp/nav/nav_controller.h"
#include "opf/app/view/dmp/screens/stacked_view.h"

namespace Ui { class diag_view; }

class diag_view : public QWidget, public i_stacked_view {
    Q_OBJECT

    Ui::diag_view* ui_;

    nav_controller& nav_controller_;

    rxcpp::composite_subscription subscriptions_;

public:
    diag_view(nav_controller& nav_controller);

    ~diag_view();

    int get_view_idx() const noexcept override;
};

#endif // DIAG_VIEW_H
