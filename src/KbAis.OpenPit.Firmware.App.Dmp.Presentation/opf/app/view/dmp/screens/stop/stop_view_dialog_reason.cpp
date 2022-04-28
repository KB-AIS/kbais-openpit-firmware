#include "stop_view_dialog_reason.h"
#include "ui_stop_view_dialog_reason.h"

stop_view_dialog_reason::stop_view_dialog_reason(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stop_view_dialog_reason)
{
    ui->setupUi(this);
}

stop_view_dialog_reason::~stop_view_dialog_reason()
{
    delete ui;
}
