#include "diag_presenter.h"
#include "ui_diag_view.h"

diag_presenter::diag_presenter(QWidget *parent) : QWidget(parent),
    ui(new Ui::diag_presenter) {
    ui->setupUi(this);
}

diag_presenter::~diag_presenter()
{
    delete ui;
}
