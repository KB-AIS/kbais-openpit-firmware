#include "shift_controller.h"

// std
#include <chrono>
// oss
#include <fmt/chrono.h>
#include <plog/Log.h>
#include <range/v3/all.hpp>

using namespace std::chrono;
using namespace std::literals::chrono_literals;

shift_controller::shift_controller()
    :   config_ {
            .shift_intervals = {
                { {  8,  0,  0 }, { 19, 59, 59 } }
            ,   { { 21,  0,  0 }, {  7, 59, 59 } }
            }
        }
{}

void shift_controller::start_working_on(
    [[maybe_unused]] const rxcpp::observe_on_one_worker& coordination
) {
    PLOGD << "Setup work of service";

    // TODO: observe configuration changes
    rxcpp::observable<>::just(1, coordination)
        .tap([](auto&) { PLOGD << "Got a new configuration"; })
        .subscribe(
            subs_
        ,   [this](const auto&) { handle_setup_shift_close_timer(); }
        );
}

void shift_controller::handle_dt_sys_change() { }

void shift_controller::handle_config_change() { }

void shift_controller::handle_setup_shift_close_timer() {
    const auto tp_now { system_clock::now() };

    auto time_now { QDateTime::currentDateTimeUtc().time() };

    // TODO: calulate
    const auto duration_shift_close_in { 30s };

    auto f = ranges::find_if(config_.shift_intervals, [&](const shift_controller_config::shift_interval_t& x) -> bool {

        return x.first < x.second && time_now >= x.first && time_now < x.second;
    });

    PLOGD << f->first.msec();

    PLOGD << fmt::format("Set to close shift in {:%H:%M:%S}", duration_shift_close_in);

    rxcpp::observable<>::timer(duration_shift_close_in)
        .subscribe(subs_timer_, [this](auto) { handle_shift_close(); });

}

void shift_controller::handle_shift_start() {

}

void shift_controller::handle_shift_close() {
    PLOGD << "Performing shift close";
}
