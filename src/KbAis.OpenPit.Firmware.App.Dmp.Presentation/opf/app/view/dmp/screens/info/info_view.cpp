#include "info_view.h"
#include "ui_info_view.h"

info_view::info_view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::info_view)
{
    ui->setupUi(this);
}

info_view::~info_view()
{
    delete ui;
}
