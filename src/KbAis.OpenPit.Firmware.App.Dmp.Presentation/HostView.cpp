#include "HostView.h"
#include "ui_HostView.h"

HostView::HostView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HostView)
{
    ui->setupUi(this);
}

HostView::~HostView()
{
    delete ui;
}
