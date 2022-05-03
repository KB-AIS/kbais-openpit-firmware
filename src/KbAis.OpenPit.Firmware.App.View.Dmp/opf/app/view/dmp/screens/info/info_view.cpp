#include "info_view.h"
#include "ui_info_view.h"

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
