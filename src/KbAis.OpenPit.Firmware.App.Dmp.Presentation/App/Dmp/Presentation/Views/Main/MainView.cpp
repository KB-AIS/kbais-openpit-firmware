#include "MainView.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QLabel>
// oss
#include <range/v3/all.hpp>

#include "ui_MainView.h"

using namespace std::chrono_literals;

const QString DATE_FMT { "ddd, d MMM yyyy" };

constexpr auto DISPLAYTIME_UPDATE_INTERVAL { 1s };

MainView::MainView(
    const IRxGpsSensorPublisher& gpsPublisher
,   const IRxMessageSendersDiagPub& messageSenderPub
,   const NavController& navigationEmmiter
)
    :   QWidget()
    ,   ui { new Ui::MainView }
    ,   m_gpsMessagePub(gpsPublisher)
    ,   m_messageSenderPub(messageSenderPub)
    ,   m_navigationEmmiter(navigationEmmiter)
    ,   m_tmUpdateDisplayTime(this)
{
    ui->setupUi(this);

    m_subscriptions = rxcpp::composite_subscription();

    rxqt::from_signal(ui->btn_navToMain, &QPushButton::released)
        .subscribe(m_subscriptions, [&](auto) { emit notifyTestUserEvent(); });

    rxqt::from_signal(ui->btn_navToDiag, &QPushButton::released)
        .subscribe(m_subscriptions, [&](auto) { m_navigationEmmiter.Navigate(1); });

    rxqt::from_signal(&m_tmUpdateDisplayTime, &QTimer::timeout)
        .subscribe(m_subscriptions, [&](auto) { OnUpdateDisplayDate(); });

    m_tmUpdateDisplayTime.start(DISPLAYTIME_UPDATE_INTERVAL);
}

MainView::~MainView() {
    m_subscriptions.unsubscribe();

    delete ui;
}

void
MainView::OnUpdateDisplayDate() {
    const auto now { QDateTime::currentDateTime() };

    ui->lbl_date->setText(now.toString(DATE_FMT));
}
