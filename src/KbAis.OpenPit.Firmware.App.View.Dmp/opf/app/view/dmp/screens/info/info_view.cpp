#include "info_view.h"
#include "ui_info_view.h"

#include "opf/app/view/dmp/screens/stacked_view_idxs.h"

info_view::info_view(
    const nav_controller& nav_controller
)
    :   ui_(new Ui::info_view)
    ,   nav_controller_ { nav_controller }
{
    ui_->setupUi(this);

    QObject::connect(ui_->btn_home, &QPushButton::released, this, [&]() {
        nav_controller_.nav_to(0);
    });
}

info_view::~info_view() {
    delete ui_;
}

int info_view::get_view_idx() const noexcept {
    return opf::app::view::dmp::stacked_view_idxs::INFO;
}
