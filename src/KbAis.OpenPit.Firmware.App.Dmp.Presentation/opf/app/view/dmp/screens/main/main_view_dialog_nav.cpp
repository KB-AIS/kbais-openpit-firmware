#include "main_view_dialog_nav.h"
#include "ui_main_view_dialog_nav.h"

main_view_dialog_nav::main_view_dialog_nav(
    const nav_controller& nav_controller
,   QWidget* parent
) noexcept
    :   QDialog(parent)
    ,   ui_(new Ui::main_view_dialog_nav)
    ,   nav_controller_ { nav_controller }
{
    ui_->setupUi(this);

    QObject::connect(ui_->btn_nav_info, &QPushButton::released, [&]() {
        nav_controller_.nav_to(2);

        accept();
    });

    QObject::connect(ui_->btn_nav_diag, &QPushButton::released, [&]() {
        nav_controller_.nav_to(1);

        accept();
    });
}

main_view_dialog_nav::~main_view_dialog_nav() {
    delete ui_;
}
