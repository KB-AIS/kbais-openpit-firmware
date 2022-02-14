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

const auto INDICATOR_VALID_SS = QString(R"(
    color: green;
    background-color: rgba(184, 187, 38, 0);
)");

const auto INDICATOR_INVALID_SS = QString(R"(
    color: red;
    background-color: rgba(251, 73, 52, 0);
)");


MainView::MainView(
    const IRxGpsSensorPublisher& gpsPublisher
,   const IRxMessageSendersDiagPub& messageSenderPub
,   const NavEmmiter& navigationEmmiter
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
MainView::StartObserveOn(const rxcpp::observe_on_one_worker& coordinator) const {
    m_gpsMessagePub.GetObservable()
        .observe_on(coordinator)
        .subscribe(m_subscriptions, [&](const GpsMessage& x) {
            ui->lbl_indicatorGps
              ->setStyleSheet(x.isValid ? INDICATOR_INVALID_SS : INDICATOR_VALID_SS);
        });

    m_messageSenderPub.GetObservableDiagInfo()
        .observe_on(coordinator)
        .subscribe(m_subscriptions, [&](const std::vector<MessageSenderDiagInfo>& x) {
            auto isConnected =
                ranges::contains(x, QString { "ConnectedState" }, &MessageSenderDiagInfo::stateText);

            ui->lbl_indicatorSrv
              ->setStyleSheet(isConnected ? INDICATOR_INVALID_SS : INDICATOR_VALID_SS);
        });
}

void
MainView::OnUpdateDisplayTime() {
    const auto now { QDateTime::currentDateTime() };
    const auto delimeter = now.time().second() % 2;

    ui->lbl_time->setText(delimeter ? now.toString(TIME_EVEN_FMT) : now.toString(TIME_FMT));
    ui->lbl_date->setText(now.toString(DATE_FMT));
}
