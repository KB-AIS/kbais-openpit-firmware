#include "host_window.h"
#include "ui_host_window.h"

// oss
#include <range/v3/all.hpp>

using namespace std::chrono_literals;

constexpr qint32 MAIN_VIEW_IDX { 0 }
               , DIAG_VIEW_IDX { 1 }
               , INFO_VIEW_IDX { 2 }
               , STOP_VIEW_IDX { 3 };

QString TIME_EVEN_FMT { QStringLiteral("hh:mm") }, TIME_FMT { QStringLiteral("hh mm") };

QString STYLE_INDICATOR_VALID { R"(
    color: #0bff01;
)" };

QString STYLE_INDICATOR_INVALID { R"(
    color: #fe0000;
)" };

constexpr auto UPDATE_DISPLAY_DATETIME_INTERVAL { 1s };

host_window::host_window(
    const i_gps_sensor_publisher& gps_sensor_publisher
,   const IRxMessageSendersDiagPub& net_module_publisher
,   i_main_view& main_view
,   diag_view& diag_view
,   info_view& info_view
,   stop_view& stop_view
,   nav_controller& nav_controller
)
    :   QMainWindow()
    ,   ui_ { new Ui::host_window }
    ,   gps_sensor_publisher_ { gps_sensor_publisher }
    ,   net_module_publisher_ { net_module_publisher }
    ,   main_view_ { main_view }
    ,   diag_view_ { diag_view }
    ,   info_view_ { info_view }
    ,   stop_view_ { stop_view }
    ,   nav_controller_ { nav_controller }
{
    ui_->setupUi(this);

    setup_screen_stack();
    setup_appbar();

    main_view_.observe_on(run_loop_.observe_on_run_loop());
}

host_window::~host_window() {
    subscriptions_.unsubscribe();
    delete ui_;
}

void host_window::setup_screen_stack() {
    main_view_.setParent(ui_->sw_nav);
    ui_->sw_nav->insertWidget(MAIN_VIEW_IDX, &main_view_);

    diag_view_.setParent(ui_->sw_nav);
    ui_->sw_nav->insertWidget(DIAG_VIEW_IDX, &diag_view_);

    info_view_.setParent(ui_->sw_nav);
    ui_->sw_nav->insertWidget(INFO_VIEW_IDX, &info_view_);

    stop_view_.setParent(ui_->sw_nav);
    ui_->sw_nav->insertWidget(STOP_VIEW_IDX, &stop_view_);

    ui_->sw_nav->setCurrentIndex(MAIN_VIEW_IDX);

    nav_controller_.get_observable()
        .subscribe(subscriptions_, [&](int screen_id) {
            if (screen_id == MAIN_VIEW_IDX) {
                ui_->lbl_screen_title->setText(QString {});
            }

            if (screen_id == DIAG_VIEW_IDX) {
                ui_->lbl_screen_title->setText(QStringLiteral("ДИАГНОСТИКА"));
            }

            if (screen_id == INFO_VIEW_IDX) {
                ui_->lbl_screen_title->setText(QStringLiteral("ИНФОРМАЦИЯ"));
            }

            // TODO: Typesafe navigation
            ui_->sw_nav->setCurrentIndex(screen_id);
        });
}

void host_window::setup_appbar() {
    const auto coordination = run_loop_.observe_on_run_loop();

    gps_sensor_publisher_.get_observable().observe_on(coordination)
        .subscribe(
            subscriptions_
        ,   [&](const GpsMessage& x) {
                ui_->lbl_indicatorGps->setStyleSheet(x.is_valid ? STYLE_INDICATOR_VALID : STYLE_INDICATOR_INVALID);
            }
        );

    net_module_publisher_.GetObservableDiagInfo().observe_on(coordination)
        .subscribe(
            subscriptions_
        ,   [&](const std::vector<MessageSenderDiagInfo>& x) {
                auto isConnected = ranges::contains(x, QString { "ConnectedState" }, &MessageSenderDiagInfo::stateText);

                ui_->lbl_indicatorSrv->setStyleSheet(isConnected ? STYLE_INDICATOR_VALID : STYLE_INDICATOR_INVALID);
        });

    rxqt::from_signal(&timer_update_display_time, &QTimer::timeout)
        .subscribe(subscriptions_, [&](auto) { handle_update_display_time(); });

    timer_update_display_time.start(UPDATE_DISPLAY_DATETIME_INTERVAL);
}

void host_window::handle_update_display_time() {
    const auto now { QDateTime::currentDateTime() };
    const auto delimeter = now.time().second() % 2;

    ui_->lbl_time->setText(delimeter ? now.toString(TIME_EVEN_FMT) : now.toString(TIME_FMT));
}
