#ifndef SHIFT_CONTROLLER_H
#define SHIFT_CONTROLLER_H

// qt
#include <QtCore/qdatetime.h>
// oss
#include <rxcpp/rx.hpp>

struct shift_start_message {

};

struct shift_close_message {

};

class shift_controller {

    struct shift {
        const QTime shift_start_time, shift_close_time;
    };

    struct shift_agregated_data {

    };

    rxcpp::composite_subscription subscriptions_;

    void handle_shift_start();

    void hanlde_shift_close();

public:
    explicit shift_controller();

    template<class coordination_t>
    void start_working_on(const coordination_t& coordination);

};

#endif // SHIFT_CONTROLLER_H
