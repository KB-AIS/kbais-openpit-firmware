#include "main_view.h"
#include "ui_main_view.h"

// std
#include <chrono>
// oss
#include <range/v3/all.hpp>

using namespace std::chrono_literals;

main_view::main_view(
    const i_gps_sensor_publisher& gps_sensor_publisher
,   const nav_controller& nav_controller
)
    :   i_main_view()
    ,   ui { new Ui::main_view }
    ,   gauge_speed_ { new gauge_speed }
    ,   dialog_nav_ { this }
    ,   gps_sensor_publisher_ { gps_sensor_publisher }
    ,   nav_controller_(nav_controller)
{
    ui->setupUi(this);

    gauge_speed_->setup_speed_gauge(ui->gau_spd);

    dialog_nav_.setWindowFlags(Qt::Popup);

    rxqt::from_signal(ui->btn_menu, &QPushButton::released)
        .subscribe([this](auto) {
            const auto g = geometry();

            dialog_nav_.move(g.center() - dialog_nav_.rect().center());

            dialog_nav_.show();
        });

    auto x = ui->gau_flv->addArc(95);
    x->setColor({ 253, 254, 2 });
    x->setDegreeRange(-60.0f, 90.0f);

    auto ticA = ui->gau_flv->addDegrees(90);
    ticA->setColor(Qt::yellow);
    ticA->setDegreeRange(-60.0f, 90.0f);
    ticA->setStep(5);
    ticA->setSubDegree(true);
    ticA->setValueRange(0, 100);

    auto valA =  ui->gau_flv->addValues(94);
    valA->setColor(Qt::yellow);
    valA->setDegreeRange(-60.0f, 90.0f);
    valA->setStep(25);
    valA->setValueRange(0, 100);

    auto y = ui->gau_wgt->addArc(95);
    y->setColor({ 253, 254, 2 });
    y->setDegreeRange(0.0f, 180.0f);

    auto ticB = ui->gau_wgt->addDegrees(90);
    ticB->setColor(Qt::yellow);
    ticB->setDegreeRange(0.0f, 180.0f);
    ticB->setStep(5);
    ticB->setSubDegree(true);
    ticB->setValueRange(0, 100);

    auto valB =  ui->gau_wgt->addValues(94);
    valB->setColor(Qt::yellow);
    valB->setDegreeRange(0.0f, 180.0f);
    valB->setStep(25);
    valB->setValueRange(0, 100);

}

main_view::~main_view() {
    subscriptions_.unsubscribe();

    delete gauge_speed_;

    delete ui;
}

void main_view::observe_on(const rxcpp::observe_on_one_worker& coordinator) {
    constexpr auto UPDATE_DISPLAY_DATETIME_INTERVAL { 1s };

    rxcpp::observable<>::interval(UPDATE_DISPLAY_DATETIME_INTERVAL, coordinator)
        .subscribe(subscriptions_, [this](auto) { handle_update_date(); });

    auto sub_handle_update_speed = rxcpp::make_subscriber<GpsMessage>(
        [this](const auto& x) { handle_update_speed(x); }
    );
    gps_sensor_publisher_.get_observable()
        .observe_on(coordinator)
        .subscribe(subscriptions_, sub_handle_update_speed);
}

QString DATE_FMT { QStringLiteral("ddd, d MMM yyyy") };

void main_view::handle_update_date() {
    const auto now { QDateTime::currentDateTime() };

    ui->lbl_date->setText(now.toString(DATE_FMT));
}

void main_view::handle_update_speed(const GpsMessage& msg) {
    gauge_speed_->needle->setCurrentValue(msg.speed);

    const auto lable_speed_text = QStringLiteral("%1 КМ/Ч.").arg(std::round(msg.speed));
    gauge_speed_->lable->setText(lable_speed_text);
}
