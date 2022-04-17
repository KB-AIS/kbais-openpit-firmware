#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

// qt
#include <QtCore/qtimer.h>
#include <QtWidgets/qwidget.h>

#include "App/Dmp/Presentation/Utils/NavController.h"
#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "IRxGpsSensorPublisher.h"
#include "RxQt.h"
#include "Utils/Widgets/QcGauge/QcGaugeWidget.h"
#include "modules/sensors/serials/lcs/lcs_sensor_publisher.h"
#include "modules/sensors/serials/lls/flv_calibration_publisher.h"
#include "presentation/dmp/state_changed_publisher.h"

class i_main_view : public QWidget {

public:
    virtual ~i_main_view() noexcept = default;

    virtual void observe_on(const rxcpp::observe_on_one_worker& coordinator) = 0;
};

namespace Ui { class main_view; }

class main_view : public i_main_view {
    Q_OBJECT

    Ui::main_view* ui;

    const i_gps_sensor_publisher& gps_sensor_publisher_;

    const i_lcs_sensor_publisher& lcs_sensor_publisher_;

    const i_state_changed_publisher& state_changed_publisher_;

    const i_state_loading_changed_publisher& state_loading_changed_publisher_;

    const nav_controller& nav_controller_;

    rxcpp::composite_subscription subscriptions_;

    QcNeedleItem*  gauge_speed_needle_;

    QcLabelItem*   gauge_speed_label_;

    void setup_gauge_speed();

    void update_display_spd(const GpsMessage& msg);

    void update_display_wgt(const lcs_sensor_message& msg);

    void update_display_date();

public:
    main_view(
        const i_gps_sensor_publisher& gps_sensor_publisher
    ,   const i_lcs_sensor_publisher& lcs_sensor_publisher
    ,   const i_state_changed_publisher& state_changed_publisher
    ,   const i_state_loading_changed_publisher& state_loading_changed_publisher
    ,   const nav_controller& nav_controller
    );

    ~main_view();

    void observe_on(const rxcpp::observe_on_one_worker& coordinator) override;

    Q_SIGNAL void notify_test_user_event();

};

#endif // MAIN_VIEW_H
