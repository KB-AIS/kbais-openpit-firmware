#include "DiagView.h"
#include "ui_DiagView.h"

DiagView::DiagView(
    NavEmmiter& navEmmiter
)
    :   QWidget()
    ,   m_navEmmiter(navEmmiter)
    ,   ui(new Ui::DiagView)
{
    ui->setupUi(this);

    m_subscriptions = rxcpp::composite_subscription();

    rxqt::from_signal(ui->btn_navToMain, &QPushButton::released)
        .subscribe(m_subscriptions, [&](auto) { m_navEmmiter.RequestNavTo(0); });
}

DiagView::~DiagView() {
    m_subscriptions.unsubscribe();

    delete ui;
}
