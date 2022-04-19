#ifndef SHIFT_CONTROLLER_H
#define SHIFT_CONTROLLER_H

// qt
#include <QtCore/QDateTime>
// oss
#include <rxcpp/rx.hpp>

struct shift_start_message {

};

struct shift_close_message {

};

struct shift_controller_config {

    using shift_interval_t  = std::pair<QTime, QTime>;

    using shift_intervals_t = std::vector<shift_interval_t>;

    shift_intervals_t shift_intervals;

    bool use_manual_shifts { false };

};

class shift_controller_config_mapper {

};

class shift_controller {

    struct shift_agregated_data {

    };

    shift_controller_config config_;

    rxcpp::composite_subscription subs_;

    rxcpp::composite_subscription subs_timer_;

    void handle_dt_sys_change();

    void handle_config_change();

    void handle_setup_shift_close_timer();

    void handle_shift_start();

    void handle_shift_close();

public:
    explicit shift_controller();

    void start_working_on(const rxcpp::observe_on_one_worker& coordination);

};

#endif // SHIFT_CONTROLLER_H
