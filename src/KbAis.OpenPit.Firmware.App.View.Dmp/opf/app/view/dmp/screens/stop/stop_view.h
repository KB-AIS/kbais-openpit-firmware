#ifndef STOP_VIEW_H
#define STOP_VIEW_H

// qt
#include <QtWidgets/qwidget.h>

#include "rxqt.h"

#include "opf/app/view/dmp/nav/nav_controller.h"
#include "opf/app/view/dmp/screens/stop/stop_view_dialog_cause.h"

namespace Ui { class stop_view; }

class stop_view : public QWidget {
    Q_OBJECT

    Ui::stop_view* ui_;

    const nav_controller& nav_controller_;

    stop_view_dialog_cause dialog_reason_;

public:
    explicit stop_view(const nav_controller& nav_controller);

    ~stop_view();

};

#endif // STOP_VIEW_H
