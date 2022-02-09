#include "MainView.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QLabel>

#include "ui_MainView.h"

using namespace std::chrono_literals;

const QString TIME_EVEN_FMT { "hh:mm" }, TIME_FMT { "hh mm" };

const QString DATE_FMT { "ddd, d MMM yyyy" };

constexpr auto TIMER_UPDATE_TIME_INTERVAL { 1s };

MainView::MainView(const NavEmmiter& navigationEmmiter)
    :   QWidget()
    ,   ui { new Ui::MainView }
    ,   m_navigationEmmiter(navigationEmmiter)
    ,   m_tmUpdateDisplayTime(this)
{
    ui->setupUi(this);

    m_subscriptions = rxcpp::composite_subscription();

    rxqt::from_signal(ui->btn_navToMain, &QPushButton::released)
        .subscribe(m_subscriptions, [&](auto) { emit notifyTestUserEvent(); });

    rxqt::from_signal(ui->btn_navToDiag, &QPushButton::released)
        .subscribe(m_subscriptions, [&](auto) { m_navigationEmmiter.RequestNavTo(1); });

    rxqt::from_signal(&m_tmUpdateDisplayTime, &QTimer::timeout)
        .subscribe(m_subscriptions, [&](auto) { OnUpdateDisplayTime(); });

    m_tmUpdateDisplayTime.start(TIMER_UPDATE_TIME_INTERVAL);
}

MainView::~MainView() {
    m_subscriptions.unsubscribe();

    delete ui;
}

void
MainView::OnUpdateDisplayTime() {
    const auto now { QDateTime::currentDateTime() };
    const auto delimeter = now.time().second() % 2;

    ui->lbl_time->setText(delimeter ? now.toString(TIME_EVEN_FMT) : now.toString(TIME_FMT));
    ui->lbl_date->setText(now.toString(DATE_FMT));
}
