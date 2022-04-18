#include "shift_controller.h"

// std
#include <chrono>

using namespace std::chrono;

shift_controller::shift_controller()
{
}


template<class coordination_t>
void shift_controller::start_working_on([[maybe_unused]] const coordination_t& coordination) {
    const auto shifts = std::vector<shift> {
        shift { .shift_start_time = {  8,  0,  0 }, .shift_close_time = { 19, 59, 59 } }
    ,   shift { .shift_start_time = { 21,  0,  0 }, .shift_close_time = {  7, 59, 59 } }
    };

    const auto tp_next_shift_start = std::chrono::steady_clock::now() + 30s;

    rxcpp::observable<>::timer(tp_next_shift_start, coordination)
        .subscribe(
            subscriptions_
        ,   [](auto) {

            }
        );
}


void shift_controller::handle_shift_start() {

}

void shift_controller::hanlde_shift_close() {

}
