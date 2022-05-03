#ifndef MAIN_VIEW_DIALOG_NAV_H
#define MAIN_VIEW_DIALOG_NAV_H

// qt
#include <QtWidgets/qdialog.h>

#include "opf/app/view/dmp/nav/nav_controller.h"

namespace Ui { class main_view_dialog_nav; }

class main_view_dialog_nav : public QDialog {
    Q_OBJECT

    Ui::main_view_dialog_nav* ui_;

    const nav_controller& nav_controller_;

public:
    explicit main_view_dialog_nav(
        const nav_controller& nav_controller
    ,   QWidget* parent = nullptr
    ) noexcept;

    ~main_view_dialog_nav();

};

#endif // MAIN_VIEW_DIALOG_NAV_H
