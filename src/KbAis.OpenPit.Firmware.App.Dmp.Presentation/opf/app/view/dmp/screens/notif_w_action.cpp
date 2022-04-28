#include "notif_w_action.h"
#include "ui_notif_w_action.h"

notif_w_action::notif_w_action(QWidget *parent)
    :   QWidget(parent)
    ,   ui_(new Ui::notif_w_action)
{
    ui_->setupUi(this);
}

notif_w_action::~notif_w_action() {
    delete ui_;
}

void notif_w_action::show_notification(
    [[maybe_unused]] const std::string& text, std::function<void()> action
) {
    action();
}
