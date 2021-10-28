#include "host_widget.h"
#include "ui_host_widget.h"

host_widget::host_widget(QWidget *parent) :
    QWidget(parent), ui(new Ui::host_widget) {
    ui->setupUi(this);
}

host_widget::~host_widget() {
    delete ui;
}
