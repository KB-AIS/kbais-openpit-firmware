#include "main_view_dialog_nav.h"
#include "ui_main_view_dialog_nav.h"

main_view_dialog_nav::main_view_dialog_nav(QWidget *parent)
    :   QDialog(parent)
    ,   ui(new Ui::main_view_dialog_nav)
{
    ui->setupUi(this);
}

main_view_dialog_nav::~main_view_dialog_nav()
{
    delete ui;
}
