#include "main_view.h"
#include "ui_main_view.h"

// std
#include <chrono>
// oss
#include <range/v3/all.hpp>
#include <plog/Log.h>

#include "opf/app/view/dmp/utils/neon_colors.h"

using namespace std::chrono_literals;

main_view::main_view(
    const i_gps_sensor_publisher& gps_sensor_publisher
,   const nav_controller& nav_controller
)
    :   i_main_view()
    ,   ui { new Ui::main_view }
    ,   nav_controller_(nav_controller)
    ,   dialog_nav_ { nav_controller_, this }
    ,   gps_sensor_publisher_ { gps_sensor_publisher }
{
    ui->setupUi(this);

    gauge_speed_subview_.setup_speed_gauge(ui->gau_spd);

    dialog_nav_.setWindowFlags(Qt::Popup);

    rxqt::from_signal(ui->btn_menu, &QPushButton::released)
        .subscribe([this](auto) {
            const auto g = geometry();

            dialog_nav_.move(g.center() - dialog_nav_.rect().center());

            dialog_nav_.open();
        });

    const int s = -30.0f, e = 210.0f;
    auto gau_flv_arc = ui->gau_flv->addArc(95);
    gau_flv_arc->setColor(opf::app::view::utils::colors::neon_yellow);
    gau_flv_arc->setDegreeRange(s, e);

    auto gau_flv_deg = ui->gau_flv->addDegrees(88);
    gau_flv_deg->setColor(opf::app::view::utils::colors::neon_yellow);
    gau_flv_deg->setDegreeRange(s, e);
    gau_flv_deg->setStep(2000 / 10);
    gau_flv_deg->setSubDegree(true);
    gau_flv_deg->setValueRange(0, 2000);

    auto gau_flv_val =  ui->gau_flv->addValues(60);
    gau_flv_val->setColor(opf::app::view::utils::colors::neon_yellow);
    gau_flv_val->setDegreeRange(s, e);
    gau_flv_val->setFont("DS-Digital");
    gau_flv_val->setStep(2000 / 5);
    gau_flv_val->setValueRange(0, 2000);

    auto gau_flv_bnd = ui->gau_flv->addColorBand(90);
    gau_flv_bnd->setColors({
        { opf::app::view::utils::colors::neon_red,    2000 }
    ,   { opf::app::view::utils::colors::neon_green,  2000 * 0.66 }
    ,   { opf::app::view::utils::colors::neon_yellow,   2000 * 0.33 }
    });
    gau_flv_bnd->setDegreeRange(s, e);
    gau_flv_bnd->setValueRange(0, 2000);

    gau_flv_ndl_ = ui->gau_flv->addNeedle(100);
    gau_flv_ndl_->setColor(opf::app::view::utils::colors::neon_blue);
    gau_flv_ndl_->setDegreeRange(s, e);
    gau_flv_ndl_->setNeedle(QcNeedleItem::DiamonNeedle);
    gau_flv_ndl_->setValueRange(0, 2000);

    auto gau_wgt_arc = ui->gau_wgt->addArc(95);
    gau_wgt_arc->setColor(opf::app::view::utils::colors::neon_yellow);
    gau_wgt_arc->setDegreeRange(s, e);

    auto gau_wgt_deg = ui->gau_wgt->addDegrees(88);
    gau_wgt_deg->setColor(opf::app::view::utils::colors::neon_yellow);
    gau_wgt_deg->setDegreeRange(s, e);
    gau_wgt_deg->setStep(200 / 10);
    gau_wgt_deg->setSubDegree(true);
    gau_wgt_deg->setValueRange(0, 200);

    auto gau_wgt_val =  ui->gau_wgt->addValues(60);
    gau_wgt_val->setColor(opf::app::view::utils::colors::neon_yellow);
    gau_wgt_val->setDegreeRange(s, e);
    gau_wgt_val->setFont("DS-Digital");
    gau_wgt_val->setStep(200 / 5);
    gau_wgt_val->setValueRange(0, 200);

    auto gau_wgt_bnd = ui->gau_wgt->addColorBand(90);
    gau_wgt_bnd->setColors({
        { opf::app::view::utils::colors::neon_yellow, 200 }
    ,   { opf::app::view::utils::colors::neon_red,    200 * 0.75 }
    });
    gau_wgt_bnd->setDegreeRange(s, e);
    gau_wgt_bnd->setValueRange(0, 200);

    gau_wgt_ndl_ = ui->gau_wgt->addNeedle(100);
    gau_wgt_ndl_->setColor(opf::app::view::utils::colors::neon_blue);
    gau_wgt_ndl_->setDegreeRange(s, e);
    gau_wgt_ndl_->setNeedle(QcNeedleItem::DiamonNeedle);
    gau_wgt_ndl_->setValueRange(0, 200);
}

main_view::~main_view() {
    subscriptions_.unsubscribe();

    delete ui;
}

void main_view::observe_on(const rxcpp::observe_on_one_worker& coordinator) {
    constexpr auto UPDATE_DISPLAY_DATETIME_INTERVAL { 1s };

    rxcpp::observable<>::interval(UPDATE_DISPLAY_DATETIME_INTERVAL, coordinator)
        .subscribe(subscriptions_, [this](auto) { handle_update_date(); });

    const auto obs_gps_message = gps_sensor_publisher_.get_observable().observe_on(coordinator);
    gauge_speed_subview_.update_value_from(obs_gps_message);
}

QString DATE_FMT { QStringLiteral("ddd, d MMM yyyy") };

QLocale RUS_LOCALE { QLocale::Russian, QLocale::Russia };

QString CSS { QStringLiteral(R"(
    color: %1;
    border: 2px solid #ffe400;
    border-radius: 8px;
)") };

void main_view::handle_update_date() {
    const auto now { QDateTime::currentDateTime() };

    ui->lbl_date->setText(RUS_LOCALE.toString(now, DATE_FMT));

    const auto delimeter = now.time().second() % 2;
    auto color = delimeter ? opf::app::view::utils::colors::neon_yellow.name() : opf::app::view::utils::colors::neon_red.name();
    auto ss = CSS.arg(color);
    ui->lbl_sta_last->setStyleSheet(ss);
}
