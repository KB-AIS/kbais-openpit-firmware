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

const QString TIME_EVEN_FMT { "hh:mm" }, TIME_FMT { "hh mm" };

const QString DATE_FMT { "ddd, d MMM yyyy" };

constexpr auto TIMER_UPDATE_TIME_INTERVAL { 1s };

MainView::MainView(
    const IRxGpsSensorPublisher& gpsPublisher
,   const IRxMessageSendersDiagPub& messageSenderPub
,   const NavEmmiter& navigationEmmiter
)
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


    gpsPublisher.GetObservable()
        .subscribe(m_subscriptions, [&](const GpsMessage& x) {
            const auto invalidSs = QString(R"(
                color: rgb(255, 0, 0);
                background-color: rgba(255, 255, 255, 0);
            )");

            const auto validSs = QString(R"(
                color: rgb(0, 255, 0);
                background-color: rgba(255, 255, 255, 0);
            )");

            ui->lbl_diagGps->setStyleSheet(x.isValid ? validSs : invalidSs);
        });

    messageSenderPub.GetObservableDiagInfo()
        .subscribe(m_subscriptions, [&](const std::vector<MessageSenderDiagInfo>& x) {
            auto isConnected = ranges::contains(x, QString("ConnectedState"), &MessageSenderDiagInfo::state_text);

            const auto invalidSs = QString(R"(
                color: rgb(255, 0, 0);
                background-color: rgba(255, 255, 255, 0);
            )");

            const auto validSs = QString(R"(
                color: rgb(0, 255, 0);
                background-color: rgba(255, 255, 255, 0);
            )");

            ui->lbl_diagSrv->setStyleSheet(isConnected ? validSs : invalidSs);
        });

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
