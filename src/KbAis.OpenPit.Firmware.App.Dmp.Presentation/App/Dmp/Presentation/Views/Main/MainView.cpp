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
    ,   m_qcg_ful(new QcGaugeWidget(this))
    ,   m_qcg_spd(new QcGaugeWidget(this))
    ,   m_tmUpdateDisplayTime(this)
{
    ui->setupUi(this);

    setup_gauge_ful();
    ui->hl_gauge_fuel->addWidget(m_qcg_ful);

    setup_gauge_spd();
    ui->hl_gauge_speed->addWidget(m_qcg_spd);

    m_subscriptions = rxcpp::composite_subscription();

    rxqt::from_signal(ui->btn_navToMain, &QPushButton::released)
        .subscribe(m_subscriptions, [&](auto) { emit notify_test_user_event(); });

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
        .subscribe(m_subscriptions, [this](FuelMessage msg) { update_gauge_ful(msg); });
    m_pub_gps_msg.get_observable()
        .observe_on(coordinator)
        .subscribe(m_subscriptions, [this](GpsMessage msg) { update_gague_spd(msg); });
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
        auto x = m_qcg_ful->addArc(100);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setValueRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    };

    setup_arc();

    auto ticA = m_qcg_ful->addDegrees(90);
    ticA->setColor(Qt::yellow);
    ticA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    ticA->setStep(5);
    ticA->setSubDegree(true);
    ticA->setValueRange(0, 100);

    auto valA = m_qcg_ful->addValues(94);
    valA->setColor(Qt::yellow);
    valA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    valA->setStep(25);
    valA->setValueRange(0, 100);

    auto cbdA = m_qcg_ful->addColorBand(70);
    cbdA->setColors({ { color_neon_red, 100 }, { color_neon_green, 50 }, { color_neon_yellow, 20 } });
    cbdA->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    cbdA->setValueRange(0, 100);

    m_qcg_ful_needle = m_qcg_ful->addNeedle(95);
    m_qcg_ful_needle->setColor(color_neon_yellow);
    m_qcg_ful_needle->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    m_qcg_ful_needle->setCurrentValue(0);

    m_qcg_ful->setStyleSheet("border-color: #feff00; border-width: 1px; border-style: solid;");
}

void
MainView::setup_gauge_spd() {
    constexpr qint32 GAUGE_DEGREE_S = -60, GAUGE_DEGREE_E = 240;

    constexpr qint32 GAUGE_SPD_MIN = 0, GAUGE_SPD_MAX = 80;

    auto setup_arc = [&]() {
        auto x = m_qcg_spd->addArc(95);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    };
    setup_arc();

    auto setup_degrees = [&]() {
        auto x = m_qcg_spd->addDegrees(88);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setStep(10);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);

        auto y = m_qcg_spd->addDegrees(88);
        y->setColor(color_neon_yellow);
        y->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        y->setStep(2.5);
        y->setSubDegree(true);
        y->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_degrees();

    auto setup_values = [&]() {
        auto x = m_qcg_spd->addValues(65);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setFont("DS-Digital");
        x->setStep(10);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_values();

    auto setup_color_band = [&]() {
        auto x = m_qcg_spd->addColorBand(90);
        x->setColors({ { color_neon_red, 80 }, { color_neon_green, 55 }, });
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_color_band();

    auto setup_needle = [&]() {
        m_qcg_spd_needle = m_qcg_spd->addNeedle(100);
        m_qcg_spd_needle->setColor(color_neon_yellow);
        m_qcg_spd_needle->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        m_qcg_spd_needle->setNeedle(QcNeedleItem::TriangleNeedle);
    };
    setup_needle();

    auto setup_label = [&]() {
        m_qcg_spd_label = m_qcg_spd->addLabel(90);
        m_qcg_spd_label->setColor(color_neon_yellow);
        m_qcg_spd_label->setFont("DS-Digital");
    };
    setup_label();

    m_qcg_spd->setStyleSheet("border-color: #feff00; border-width: 1px; border-style: solid;");
}

void
MainView::update_gauge_ful(const FuelMessage& msg) {
    m_qcg_ful_needle->setCurrentValue(msg.cur_fuel_level);

    if (msg.max_fuel_level > 0) {
        m_qcg_ful_needle->setValueRange(0, msg.max_fuel_level);
    }

    const auto ful_text = msg.is_value_valid
        ? QString("%1 Л.").arg(msg.cur_fuel_level)
        : QString("-- л.");
}

void
MainView::update_gague_spd(const GpsMessage& msg) {
    m_qcg_spd_needle->setCurrentValue(msg.speed);

    const auto spd_text = QString("%1 КМ/Ч.").arg(std::round(msg.speed));
    m_qcg_spd_label->setText(spd_text);
}
