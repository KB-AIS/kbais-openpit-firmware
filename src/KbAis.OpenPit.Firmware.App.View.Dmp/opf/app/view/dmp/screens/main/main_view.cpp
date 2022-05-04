#include "main_view.h"
#include "ui_main_view.h"

// std
#include <chrono>
// oss
#include <range/v3/all.hpp>
#include <plog/Log.h>

#include "opf/app/view/dmp/screens/stacked_view_idxs.h"
#include "opf/app/view/dmp/utils/neon_colors.h"

using namespace std::literals::chrono_literals;

using namespace opf::app::view::dmp;

main_view::main_view(
    const i_gps_sensor_publisher& gps_sensor_publisher
,   const i_lcs_sensor_publisher& lcs_sensor_publisher
,   const nav_controller& nav_controller
,   const ntf_controller& ntf_controller
)
    :   i_main_view(), i_stacked_view()
    ,   ui_ { new Ui::main_view }
    ,   nav_controller_(nav_controller)
    ,   ntf_controller_ { ntf_controller }
    ,   dialog_nav_(nav_controller_, this)
    ,   gps_sensor_publisher_ { gps_sensor_publisher }
    ,   lcs_sensor_publisher_ { lcs_sensor_publisher }
{
    ui_->setupUi(this);

    gauge_flv_subview_.setup_subview(ui_->gau_flv, ui_->lbl_flv_value);
    gauge_spd_subview_.setup_subview(ui_->gau_spd);
    gauge_wgt_subview_.setup_subview(ui_->gau_wgt, ui_->lbl_wgt_value);

    dialog_nav_.setWindowFlags(Qt::Popup);

    rxqt::from_signal(ui_->btn_menu, &QPushButton::released)
        .subscribe([this](auto) {
            const auto g = geometry();
            dialog_nav_.move(g.center() - dialog_nav_.rect().center());
            dialog_nav_.open();
        });

    rxqt::from_signal(ui_->btn_backlight, &QPushButton::released)
        .subscribe([this](auto) {
            ntf_controller_.req_ntf(ntf_prms {
                .primary_text = "БЫЛ ОБНАРУЖЕН ПРОСТОЙ"
            ,   .secondar y_text = "НАЖМИТЕ ДЛЯ ВВОДА ПРИЧИНЫ"
            ,   .action_opt = [this]() {
                    nav_controller_.nav_to(stacked_view_idxs::STOP);
                }
            });
        });
}

main_view::~main_view() {
    subscriptions_.unsubscribe();

    delete ui_;
}

void main_view::observe_on(const rxcpp::observe_on_one_worker& c) {
    constexpr auto UPDATE_DISPLAY_DATETIME_INTERVAL { 1s };

    rxcpp::observable<>::interval(UPDATE_DISPLAY_DATETIME_INTERVAL, c)
        .subscribe(subscriptions_, [this](auto) { handle_update_date(); });

    const auto obs_gps_message = gps_sensor_publisher_.get_observable().observe_on(c);
    gauge_spd_subview_.update_value_from(obs_gps_message);

    const auto obs_lcs_message = lcs_sensor_publisher_.get_observable(c);
    gauge_wgt_subview_.update_value_from(obs_lcs_message);
}

int main_view::get_view_idx() const noexcept {
    return opf::app::view::dmp::stacked_view_idxs::MAIN;
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

    ui_->lbl_date->setText(RUS_LOCALE.toString(now, DATE_FMT));

    const auto delimeter = now.time().second() % 2;
    auto color = delimeter ? opf::app::view::utils::colors::neon_yellow.name() : opf::app::view::utils::colors::neon_red.name();
    auto ss = CSS.arg(color);
    ui_->lbl_sta_value->setStyleSheet(ss);
}
