#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

// qt
#include <QtCore/qtimer.h>
#include <QtWidgets/qwidget.h>

#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "rxqt.h"

#include "opf/app/view/dmp/nav/nav_controller.h"
#include "opf/app/view/dmp/nav/ntf_controller.h"
#include "opf/app/view/dmp/screens/main/main_view_dialog_nav.h"
#include "opf/app/view/dmp/screens/main/subviews/gauge_flv_subview.h"
#include "opf/app/view/dmp/screens/main/subviews/gauge_spd_subview.h"
#include "opf/app/view/dmp/screens/main/subviews/gauge_wgt_subview.h"
#include "opf/app/view/dmp/screens/stacked_view.h"
#include "opf/modules/sensors/gps/gps_sensor_publisher.h"
#include "opf/modules/sensors/lcs/lcs_sensor_publisher.h"

class i_main_view : public QWidget {

public:
    virtual ~i_main_view() noexcept = default;

    virtual void observe_on(const rxcpp::observe_on_one_worker& coordinator) = 0;

};

namespace Ui { class main_view; }

class main_view : public i_main_view, public i_stacked_view {
    Q_OBJECT

    Ui::main_view* ui_;

    const nav_controller& nav_controller_;
    const ntf_controller& ntf_controller_;

    gauge_flv_subview gauge_flv_subview_;
    gauge_spd_subview gauge_spd_subview_;
    gauge_wgt_subview gauge_wgt_subview_;
    main_view_dialog_nav dialog_nav_;

    const i_gps_sensor_publisher& gps_sensor_publisher_;
    const i_lcs_sensor_publisher& lcs_sensor_publisher_;
    rxcpp::composite_subscription subscriptions_;

    void handle_update_date();

public:
    main_view(
        const i_gps_sensor_publisher& gps_sensor_publisher
    ,   const i_lcs_sensor_publisher& lcs_sensor_publisher
    ,   const nav_controller& nav_controller
    ,   const ntf_controller& ntf_controller
    );

    ~main_view();

    void observe_on(const rxcpp::observe_on_one_worker& c) override;

    int get_view_idx() const noexcept override;

};

#endif // MAIN_VIEW_H
