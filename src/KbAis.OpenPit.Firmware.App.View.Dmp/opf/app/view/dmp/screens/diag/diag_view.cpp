#include "diag_view.h"
#include "ui_diag_view.h"

#include "opf/app/view/dmp/screens/stacked_view_idxs.h"

diag_view::diag_view(
    nav_controller& nav_controller
)
    :   QWidget()
    ,   ui_(new Ui::diag_view)
    ,   nav_controller_ { nav_controller }
{
    ui_->setupUi(this);

    rxqt::from_signal(ui_->btn_home, &QPushButton::released)
        .subscribe(subscriptions_, [&](auto) { nav_controller_.nav_to(0); });
}

diag_view::~diag_view() {
    subscriptions_.unsubscribe();

    delete ui_;
}

int diag_view::get_view_idx() const noexcept {
    return opf::app::view::dmp::stacked_view_idxs::DIAG;
}
