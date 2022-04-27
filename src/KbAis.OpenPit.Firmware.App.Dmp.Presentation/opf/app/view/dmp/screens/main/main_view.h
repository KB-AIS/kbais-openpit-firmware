#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

// qt
#include <QtCore/qtimer.h>
#include <QtWidgets/qwidget.h>

#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "IRxGpsSensorPublisher.h"
#include "RxQt.h"

#include "opf/app/view/dmp/nav/nav_controller.h"
#include "opf/app/view/dmp/screens/main/main_view_dialog_nav.h"
#include "opf/app/view/dmp/screens/main/subviews/gauge_speed_subview.h"

class i_main_view : public QWidget {

public:
    virtual ~i_main_view() noexcept = default;

    virtual void observe_on(const rxcpp::observe_on_one_worker& coordinator) = 0;
};

namespace Ui { class main_view; }

class main_view : public i_main_view {
    Q_OBJECT

    Ui::main_view* ui;

    QcLabelItem*  gau_flv_lbl_;

    QcNeedleItem* gau_flv_ndl_;

    QcLabelItem*  gau_wgt_lbl_;

    QcNeedleItem* gau_wgt_ndl_;

    gauge_speed_subview gauge_speed_subview_;

    main_view_dialog_nav dialog_nav_;

    const i_gps_sensor_publisher& gps_sensor_publisher_;

    const nav_controller& nav_controller_;

    rxcpp::composite_subscription subscriptions_;

    void handle_update_date();

public:
    main_view(
        const i_gps_sensor_publisher& gps_sensor_publisher
    ,   const nav_controller& nav_controller
    );

    ~main_view();

    void observe_on(const rxcpp::observe_on_one_worker& coordination) override;

};

#endif // MAIN_VIEW_H
