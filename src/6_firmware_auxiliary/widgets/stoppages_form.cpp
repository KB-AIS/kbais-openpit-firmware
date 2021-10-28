#include "stoppages_form.h"
#include "ui_stoppages_form.h"

stoppages_form::stoppages_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stoppages_form)
{
    ui->setupUi(this);
}

stoppages_form::~stoppages_form()
{
    delete ui;
}
