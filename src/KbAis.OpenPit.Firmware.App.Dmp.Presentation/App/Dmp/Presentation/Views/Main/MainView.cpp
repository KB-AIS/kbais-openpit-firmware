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
,   const RxFuelMessagePublisher& pub_fuel_message
,   const NavController& nav_controller
)
    :   QWidget()
    ,   ui { new Ui::MainView }
    ,   m_gpsMessagePub(gpsPublisher)
    ,   m_messageSenderPub(messageSenderPub)
    ,   m_pub_fuel_message(pub_fuel_message)
    ,   m_nav_controller(nav_controller)
    ,   m_tmUpdateDisplayTime(this)
{
    ui->setupUi(this);

    setup_fuel_gauge();

    m_subscriptions = rxcpp::composite_subscription();

    rxqt::from_signal(ui->btn_navToMain, &QPushButton::released)
        .subscribe(m_subscriptions, [&](auto) { emit notifyTestUserEvent(); });

    rxqt::from_signal(ui->btn_navToDiag, &QPushButton::released)
        .subscribe(m_subscriptions, [&](auto) { m_nav_controller.Navigate(1); });

    rxqt::from_signal(&m_tmUpdateDisplayTime, &QTimer::timeout)
        .subscribe(m_subscriptions, [&](auto) { OnUpdateDisplayDate(); });

    m_tmUpdateDisplayTime.start(DISPLAYTIME_UPDATE_INTERVAL);
}

MainView::~MainView() {
    m_subscriptions.unsubscribe();

    delete ui;
}

void
MainView::provide_coordinator(const rxcpp::observe_on_one_worker& coordinator) {
    m_pub_fuel_message
        .get_obeservable_fuel_message()
        .observe_on(coordinator)
        .subscribe(m_subscriptions, [this](FuelMessage message) {
            qc_gauge_needle_fuel->setCurrentValue(message.fuel_level);
            qc_gauge_lable_fuel->setText(QString::number(message.fuel_level));
        });
}

void
MainView::OnUpdateDisplayDate() {
    const auto now { QDateTime::currentDateTime() };

    ui->lbl_date->setText(now.toString(DATE_FMT));
}

void
MainView::setup_fuel_gauge() {
    auto arcA = qc_gauge_fuel.addArc(80);
    arcA->setColor(Qt::yellow);
    arcA->setDegreeRange(0, 180);
    arcA->setValueRange(0, 180);

    auto ticA = qc_gauge_fuel.addDegrees(90);
    ticA->setColor(Qt::yellow);
    ticA->setDegreeRange(0, 180);
    ticA->setStep(10);
    ticA->setSubDegree(true);
    ticA->setValueRange(0, 100);

    auto valA = qc_gauge_fuel.addValues(94);
    valA->setColor(Qt::yellow);
    valA->setDegreeRange(0, 180);
    valA->setStep(25);
    valA->setValueRange(0, 100);

    auto cbdA = qc_gauge_fuel.addColorBand(70);
    cbdA->setColors({ { Qt::red, 100 }, { Qt::green, 50 }, { Qt::yellow, 20 } });
    cbdA->setDegreeRange(0, 180);
    cbdA->setValueRange(0, 100);

    qc_gauge_lable_fuel = qc_gauge_fuel.addLabel(20);
    qc_gauge_lable_fuel->setColor(Qt::darkYellow);
    qc_gauge_lable_fuel->setFont("Roboto");

    qc_gauge_needle_fuel = qc_gauge_fuel.addNeedle(60);
    qc_gauge_needle_fuel->setColor(Qt::darkYellow);
    qc_gauge_needle_fuel->setDegreeRange(0, 180);
    qc_gauge_needle_fuel->setNeedle(QcNeedleItem::DiamonNeedle);
    qc_gauge_needle_fuel->setValueRange(0, 2200); // TODO: Set from settings

    qc_gauge_lable_fuel->setText("--");
    qc_gauge_needle_fuel->setCurrentValue(1100);

    ui->hl_gauge_fuel->addWidget(&qc_gauge_fuel);
}
