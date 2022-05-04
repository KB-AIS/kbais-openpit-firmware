#ifndef HOST_WINDOW_H
#define HOST_WINDOW_H

// qt
#include <QtWidgets/qmainwindow.h>

#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "rxqt.h"

#include "opf/app/view/dmp/nav/nav_controller.h"
#include "opf/app/view/dmp/nav/ntf_controller.h"
#include "opf/app/view/dmp/screens/diag/diag_view.h"
#include "opf/app/view/dmp/screens/info/info_view.h"
#include "opf/app/view/dmp/screens/main/main_view.h"
#include "opf/app/view/dmp/screens/stop/stop_view.h"
#include "opf/app/view/dmp/screens/notificator_w_action.h"

#include "opf/modules/sensors/gps/gps_sensor_publisher.h"

namespace Ui { class host_window; }

class host_window : public QMainWindow {
    Q_OBJECT

    Ui::host_window* ui_;

    const i_gps_sensor_publisher& gps_sensor_publisher_;

    const IRxMessageSendersDiagPub& msg_sender_diag_publisher_;

    i_main_view& main_view_;
    diag_view& diag_view_;
    info_view& info_view_;
    stop_view& stop_view_;

    nav_controller& nav_controller_;
    ntf_controller& ntf_controller_;

    notificator_w_action notificator_w_action_;

    rxcpp::composite_subscription subscriptions_;

    rxqt::run_loop run_loop_;

    QTimer timer_update_display_time;

    void setup_screen_stack();

    void setup_appbar();

    void handle_update_display_time();

public:
    host_window(
        const i_gps_sensor_publisher& gps_sensor_publisher
    ,   const IRxMessageSendersDiagPub& msg_sender_diag_publisher
    ,   i_main_view& main_view
    ,   diag_view& diag_view
    ,   info_view& info_view
    ,   stop_view& stop_view
    ,   nav_controller& nav_controller
    ,   ntf_controller& ntf_controller
    );

    ~host_window();

};

#endif // HOST_WINDOW_H