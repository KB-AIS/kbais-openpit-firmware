#include "stop_view_dialog_cause.h"
#include "ui_stop_view_dialog_reason.h"

stop_view_dialog_cause::stop_view_dialog_cause(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stop_view_dialog_reason)
{
    ui->setupUi(this);
}

stop_view_dialog_cause::~stop_view_dialog_cause()
{
    delete ui;
}
