#include "state_watcher_fuelling.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Format.h"

using namespace std::chrono;

constexpr int BUFFERED_FLV_SIZE { 5 };

constexpr int FLV_PERIOD { 50 };

constexpr double FLV_WIDTH_FIX { 5.0 };

constexpr double FLV_FUELLING { 20.0 };

constexpr double FLV_FUELLING_DLT { 10.0 };

constexpr double FLV_DRAINING { 20.0 };

constexpr double FLV_DRAINGIN_DLT { 10.0 };


state_watcher_fuelling::state_watcher_fuelling(
    const flv_calibration_publisher& flv_calibration_publisher
)
    :   flv_calibration_publisher_ { flv_calibration_publisher }
    ,   current_fuelling_state_(state_changed_message { state_code::unknown })
{
    tk_.buffered_flv.resize(BUFFERED_FLV_SIZE);
}

void state_watcher_fuelling::start_working_on(
    [[maybe_unused]]const rxcpp::observe_on_one_worker& coordination
) {
    flv_calibration_publisher_
        .get_observable()
        .subscribe([this](const auto& x) { handle_flv_message(x); });
}

void state_watcher_fuelling::handle_flv_message(const flv_message& msg) {
    tk_.buffered_flv[tk_.buffered_flv_cur_idx] = msg.fuel_level;
    tk_.buffered_flv_cur_idx = (tk_.buffered_flv_cur_idx + 1) % BUFFERED_FLV_SIZE;

    const auto flv_avg = ranges::accumulate(tk_.buffered_flv, 0.0) / BUFFERED_FLV_SIZE;

    // Q!: Some magic is happening here, need to add an detailed comment
    tk_.flv_filtered = tk_.flv_filtered > 0.0
        ? tk_.flv_filtered * (FLV_PERIOD - 1) / FLV_PERIOD + flv_avg / FLV_PERIOD
        : flv_avg;

    const auto flv_delta = std::abs(flv_avg - tk_.flv_filtered);

    // TODO: Move to 'fixing' function
    if (flv_delta < FLV_WIDTH_FIX) {
        const auto need_fix =
            !tk_.is_flv_fixed
            && duration_cast<milliseconds>(steady_clock::now() - tk_.tp_flv_last_unfix) > 5s;

        if (need_fix) {
            tk_.is_flv_fixed = true;

            if (!tk_.is_state_fueling) {
                tk_.flv_fixed_tmp = flv_avg;
            }
        }
    } else {
        tk_.tp_flv_last_unfix = steady_clock::now();
        const auto need_unfix = flv_delta > 2.0 * FLV_WIDTH_FIX;

        if (need_unfix) {
            tk_.is_flv_fixed = false;
        }
    }

    // TODO: Move to 'fueling' function
    const auto fl_fueling = flv_avg - tk_.flv_filtered;
    if (fl_fueling >= FLV_FUELLING && !tk_.is_state_fueling) {
        tk_.is_state_fueling = true;
        current_fuelling_state_.get_subscriber().on_next(state_changed_message { state_code::refueling_begin });
    } else if (fl_fueling < FLV_FUELLING - FLV_FUELLING_DLT && tk_.is_state_fueling) {
        tk_.is_state_fueling = false;
        current_fuelling_state_.get_subscriber().on_next(state_changed_message { state_code::refueling_finish });
    }

    // TODO: Move to 'draining' function
    const auto fl_draingin = tk_.flv_filtered - flv_avg;
    if (fl_draingin >= FLV_DRAINING && !tk_.is_state_draining) {
        tk_.is_state_draining = true;
        current_fuelling_state_.get_subscriber().on_next(state_changed_message { state_code::discharging_begin });
    } else if (fl_draingin < FLV_DRAINING - FLV_DRAINGIN_DLT && tk_.is_state_draining) {
        tk_.is_state_draining = false;
        current_fuelling_state_.get_subscriber().on_next(state_changed_message { state_code::discharging_finish });
    }
}

rxcpp::observable<state_changed_message> state_watcher_fuelling::get_observable() const {
    return current_fuelling_state_.get_observable();
}
