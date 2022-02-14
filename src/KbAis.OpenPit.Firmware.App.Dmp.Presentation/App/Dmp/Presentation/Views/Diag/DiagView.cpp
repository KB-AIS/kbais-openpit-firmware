#include "DiagView.h"
#include "ui_DiagView.h"

DiagView::DiagView(
    NavController& navController
)
    :   QWidget()
    ,   m_navEmmiter(navController)
    ,   ui(new Ui::DiagView)
{
    ui->setupUi(this);

    rxqt::from_signal(ui->btn_navigateBack, &QPushButton::released)
        .subscribe(m_subsBag, [&](auto) { m_navEmmiter.Navigate(0); });
}

DiagView::~DiagView() {
    m_subsBag.unsubscribe();
    delete ui;
}
