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
    const IRxGpsSensorPublisher& pub_gps_msg
,   const RxFuelMessagePublisher& pub_ful_msg
,   const NavController& nav_controller
)
    :   QWidget()
    ,   ui { new Ui::MainView }
    ,   m_pub_gps_msg(pub_gps_msg)
    ,   m_pub_ful_msg(pub_ful_msg)
    ,   m_nav_controller(nav_controller)
    ,   qc_gauge_fuel(new QcGaugeWidget(this))
    ,   qc_gauge_speed(new QcGaugeWidget(this))
    ,   m_tmUpdateDisplayTime(this)
{
    ui->setupUi(this);

    setup_gauge_ful();
    ui->hl_gauge_fuel->addWidget(qc_gauge_fuel);

    setup_gauge_spd();
    ui->hl_gauge_speed->addWidget(qc_gauge_speed);

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
    m_pub_ful_msg.get_obeservable_fuel_message()
        .observe_on(coordinator)
        .subscribe(m_subscriptions, [this](FuelMessage msg) { upd_gauge_ful(msg); });
    m_pub_gps_msg.GetObservable()
        .observe_on(coordinator)
        .subscribe(m_subscriptions, [this](GpsMessage msg) { upd_gague_spd(msg); });
}

void
MainView::OnUpdateDisplayDate() {
    const auto now { QDateTime::currentDateTime() };

    ui->lbl_date->setText(now.toString(DATE_FMT));
}

const QColor color_neon_yellow { 253, 254, 2 };
const QColor color_neon_red    { 254, 0, 0 };
const QColor color_neon_green  { 11, 255, 1 };

void
MainView::setup_gauge_ful() {
    constexpr qint32 GAUGE_DEGREE_S = 140, GAUGE_DEGREE_E = 270;

    auto setup_arc = [&]() {
        auto x = qc_gauge_fuel->addArc(100);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setValueRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    };

    setup_arc();

    auto ticA = qc_gauge_fuel->addDegrees(90);
    ticA->setColor(Qt::yellow);
    ticA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    ticA->setStep(5);
    ticA->setSubDegree(true);
    ticA->setValueRange(0, 100);

    auto valA = qc_gauge_fuel->addValues(94);
    valA->setColor(Qt::yellow);
    valA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    valA->setStep(25);
    valA->setValueRange(0, 100);

    auto cbdA = qc_gauge_fuel->addColorBand(70);
    cbdA->setColors({ { color_neon_red, 100 }, { color_neon_green, 50 }, { color_neon_yellow, 20 } });
    cbdA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    cbdA->setValueRange(0, 100);

    //qc_gauge_fuel->setStyleSheet("border-color: #feff00; border-width: 1px; border-style: solid;");

    qc_gauge_fuel_label = qc_gauge_fuel->addLabel(20);
    qc_gauge_fuel_label->setColor(Qt::darkYellow);
    qc_gauge_fuel_label->setFont("Roboto");

    qc_gauge_fuel_needle = qc_gauge_fuel->addNeedle(95);
    qc_gauge_fuel_needle->setColor(color_neon_yellow);
    qc_gauge_fuel_needle->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);

    qc_gauge_fuel_label->setText("--");
    qc_gauge_fuel_needle->setCurrentValue(0);
}

void
MainView::setup_gauge_spd() {
    constexpr qint32 GAUGE_DEGREE_S = -60, GAUGE_DEGREE_E = 240;

    auto setup_arc = [&]() {
        auto x = qc_gauge_speed->addArc(100);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setValueRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    };

    setup_arc();

    //qc_gauge_speed->setStyleSheet("border-color: #feff00; border-width: 1px; border-style: solid;");

    auto ticA = qc_gauge_speed->addDegrees(90);
    ticA->setColor(Qt::yellow);
    ticA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    ticA->setStep(10);
    ticA->setSubDegree(true);
    ticA->setValueRange(0, 100);

    auto valA = qc_gauge_speed->addValues(85);
    valA->setColor(Qt::yellow);
    valA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    valA->setStep(25);
    valA->setValueRange(0, 100);

    auto cbdA = qc_gauge_speed->addColorBand(95);
    cbdA->setColors({ { color_neon_green, 100 }, { color_neon_yellow, 50 }, { color_neon_red, 20 } });
    cbdA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    cbdA->setValueRange(0, 100);

    qc_gauge_speed_label = qc_gauge_speed->addLabel(20);
    qc_gauge_speed_label->setColor(Qt::darkYellow);
    qc_gauge_speed_label->setFont("Roboto");

    qc_gauge_speed_needle = qc_gauge_speed->addNeedle(100);
    qc_gauge_speed_needle->setColor(color_neon_yellow);
    qc_gauge_speed_needle->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);

    qc_gauge_speed_label->setText("--");
    qc_gauge_speed_needle->setCurrentValue(0);
}

void
MainView::upd_gauge_ful(const FuelMessage& msg) {
    qc_gauge_fuel_needle->setCurrentValue(msg.cur_fuel_level);

    if (msg.max_fuel_level > 0) {
        qc_gauge_fuel_needle->setValueRange(0, msg.max_fuel_level);
    }

    const auto gauge_lable_fuel_text = msg.is_value_valid
        ? QString("%1 Л.").arg(msg.cur_fuel_level)
        : QString("-- л.");

    qc_gauge_fuel_label->setText(gauge_lable_fuel_text);
}

void
MainView::upd_gague_spd(const GpsMessage& msg) {
    auto spd_text = QString("%1 КМ/Ч.").arg(QString::number(msg.speed, 'g', 0));

    qc_gauge_speed_label->setText(spd_text);
}
