#include "main_view.h"

// std
#include <chrono>
// oss
#include <range/v3/all.hpp>
#include <QtSvg/QSvgRenderer>

#include "ui_main_view.h"

using namespace std::chrono_literals;

constexpr auto UPDATE_DISPLAY_DATETIME_INTERVAL { 1s };

QString DATE_FMT { "ddd, d MMM yyyy" };

main_view::main_view(
    const i_gps_sensor_publisher& gps_sensor_publisher
,   const i_lcs_sensor_publisher& lcs_sensor_publisher
,   const i_state_changed_publisher& state_changed_publisher
,   const i_state_loading_changed_publisher& state_loading_changed_publisher
,   const nav_controller& nav_controller
)
    :   i_main_view()
    ,   ui { new Ui::main_view }
    ,   gps_sensor_publisher_ { gps_sensor_publisher }
    ,   lcs_sensor_publisher_ { lcs_sensor_publisher }
    ,   state_changed_publisher_ { state_changed_publisher }
    ,   state_loading_changed_publisher_ { state_loading_changed_publisher }
    ,   nav_controller_(nav_controller)
{
    ui->setupUi(this);

    ui->svg_weight->set_svg(QStringLiteral(":/views/svgs/weight.svg"));
    ui->svg_fuel->set_svg(QStringLiteral(":/views/svgs/fuel.svg"));

    setup_gauge_speed();

    subscriptions_ = rxcpp::composite_subscription();

    rxqt::from_signal(ui->btn_navToMain, &QPushButton::released)
        .subscribe(subscriptions_, [&](auto) { emit notify_test_user_event(); });

    rxqt::from_signal(ui->btn_navToDiag, &QPushButton::released)
        .subscribe(subscriptions_, [&](auto) { nav_controller_.Navigate(1); });
}

main_view::~main_view() {
    subscriptions_.unsubscribe();

    delete ui;
}

std::map<state_code_motioning, QString> state_code_motioning_to_readable {
    { state_code_motioning::moving,  "ДВИЖЕНИЕ" }
,   { state_code_motioning::staying, "ОСТАНОВКА" }
,   { state_code_motioning::parking, "ПАРКОВКА" }
};

std::map<state_code_loading, QString> state_code_loading_to_readable {
    { state_code_loading::loading,   "ЗАГРУЖАЕТСЯ" }
,   { state_code_loading::loaded,    "ЗАГРУЖЕН" }
,   { state_code_loading::unloading, "РАЗГРУЖАЕТСЯ" }
,   { state_code_loading::empty,     "РАЗГРУЖЕН" }
};

void
main_view::observe_on(const rxcpp::observe_on_one_worker& coordinator) {
    rxcpp::observable<>::interval(UPDATE_DISPLAY_DATETIME_INTERVAL, coordinator)
        .subscribe(subscriptions_, [this](auto) { update_display_date(); });

    gps_sensor_publisher_.get_observable()
        .observe_on(coordinator)
        .subscribe(
            subscriptions_
        ,   [this](GpsMessage msg) { update_display_spd(msg); }
        );

    lcs_sensor_publisher_.get_observable(coordinator)
        .subscribe(
            subscriptions_
        ,   [this](const lcs_sensor_message& x) { update_display_wgt(x); }
        );

    state_changed_publisher_.get_observable(coordinator).subscribe(
        subscriptions_
    ,   [this](state_code_motioning state) {
            ui->lbl_last_motioning_state->setText(state_code_motioning_to_readable.at(state));
        }
    );

    state_loading_changed_publisher_.get_observable(coordinator).subscribe(
        subscriptions_
    ,   [this](state_code_loading state) {
            ui->lbl_last_loading_state->setText(state_code_loading_to_readable.at(state));
        }
    );
}

void
main_view::update_display_date() {
    const auto now { QDateTime::currentDateTime() };
    ui->lbl_date->setText(now.toString(DATE_FMT));
}

QColor color_neon_yellow { 253, 254, 2 }, color_neon_red { 254, 0, 0 }, color_neon_green { 11, 255, 1 };

void
main_view::setup_gauge_speed() {
    constexpr qint32 GAUGE_DEGREE_S = -60, GAUGE_DEGREE_E = 240;

    constexpr qint32 GAUGE_SPD_MIN = 0, GAUGE_SPD_MAX = 80;

    auto setup_arc = [&]() {
        auto x = ui->gauge_speed->addArc(95);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
    };
    setup_arc();

    auto setup_degrees = [&]() {
        auto x = ui->gauge_speed->addDegrees(88);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setStep(10);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);

        auto y = ui->gauge_speed->addDegrees(88);
        y->setColor(color_neon_yellow);
        y->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        y->setStep(2.5);
        y->setSubDegree(true);
        y->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_degrees();

    auto setup_values = [&]() {
        auto x = ui->gauge_speed->addValues(65);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setFont("DS-Digital");
        x->setStep(10);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_values();

    auto setup_color_band = [&]() {
        auto x = ui->gauge_speed->addColorBand(90);
        x->setColors({ { color_neon_red, 80 }, { color_neon_green, 55 }, });
        x->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_color_band();

    auto setup_needle = [&]() {
        gauge_speed_needle_ = ui->gauge_speed->addNeedle(100);
        gauge_speed_needle_->setColor(color_neon_yellow);
        gauge_speed_needle_->setDegreeRange(GAUGE_DEGREE_S, GAUGE_DEGREE_E);
        gauge_speed_needle_->setNeedle(QcNeedleItem::TriangleNeedle);
        gauge_speed_needle_->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_needle();

    auto setup_label = [&]() {
        gauge_speed_label_ = ui->gauge_speed->addLabel(90);
        gauge_speed_label_->setColor(color_neon_yellow);
        gauge_speed_label_->setFont("DS-Digital");
    };
    setup_label();
}

void
main_view::update_display_spd(const GpsMessage& msg) {
    gauge_speed_needle_->setCurrentValue(msg.speed);

    const auto spd_text = QString("%1 КМ/Ч.").arg(std::round(msg.speed));
    gauge_speed_label_->setText(spd_text);
}

void main_view::update_display_wgt(const lcs_sensor_message& msg) {
    const auto wgt_txt { QStringLiteral("%1 Т.").arg(std::round(msg.weight / 100.0)) };
    ui->lbl_wgt->setText(wgt_txt);
}
