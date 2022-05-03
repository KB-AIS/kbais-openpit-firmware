#include "stop_view.h"
#include "ui_stop_view.h"

stop_view::stop_view(const nav_controller& nav_controller)
    :   ui_(new Ui::stop_view)
    ,   nav_controller_ { nav_controller }
    ,   dialog_reason_ { this }
{
    ui_->setupUi(this);

    ui_->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    rxqt::from_signal(ui_->btn_home, &QPushButton::released)
        .subscribe([this](auto) { nav_controller_.nav_to(0); });

    dialog_reason_.setWindowFlags(Qt::Popup);

    rxqt::from_signal(ui_->pushButton, &QPushButton::released)
        .subscribe([this](auto) {
            const auto g = geometry();

            dialog_reason_.move(g.center() - dialog_reason_.rect().center());

            dialog_reason_.open();
        });
}

stop_view::~stop_view() {
    delete ui_;
}
