#include "state_watcher_motioning.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>
#include <fmt/format.h>

using namespace rx;
using namespace std::chrono;

state_watcher_motioning::state_watcher_motioning(
    const i_gps_sensor_publisher& gps_sensor_publisher
)
    :   gps_sensor_publisher_(gps_sensor_publisher)
    ,   current_motioning_state_(state_code_motioning::staying)
    ,   buffered_speed_values_(buffered_speed_size_)
{
}

void state_watcher_motioning::start_working_on(const rxcpp::observe_on_one_worker& coordination) {
    gps_sensor_publisher_.get_observable()
        .observe_on(coordination)
        .subscribe(subscriptions_, [this](const GpsMessage& x) { handle_gps_message(x); });
}

rxcpp::observable<state_code_motioning> state_watcher_motioning::get_observable(
    const rxcpp::observe_on_one_worker& coordination
) const {
    return current_motioning_state_.get_observable().subscribe_on(coordination);
}

void state_watcher_motioning::handle_gps_message(const GpsMessage& gps_message) {
    buffered_speed_values_[buffered_speed_cur_idx_] = gps_message.speed;
    buffered_speed_cur_idx_ = (buffered_speed_cur_idx_ + 1) % buffered_speed_size_;

    const auto speed_avg = ranges::accumulate(buffered_speed_values_, 0.0) / buffered_speed_size_;

    // Проверка на то, есть ли сейчас движение согласно показателем с GPS-антенны
    if (speed_avg > configuration_.min_speed_to_move) {
        last_move_time = steady_clock::now();
    }

    const auto state = current_motioning_state_.get_value();

    if (state == state_code_motioning::moving && is_in_stop_state()) {
        // Был зафиксирован переходи из состояния движения в состояние остановки
        current_motioning_state_.get_subscriber().on_next(state_code_motioning::staying);
        return;
    }

    if (state == state_code_motioning::staying) {
        last_stop_time = steady_clock::now();

        if (is_in_park_state()) {
            current_motioning_state_.get_subscriber().on_next(state_code_motioning::parking);
            last_stop_time = prev_stop_time;
        }

        if (is_in_move_state()) {
            current_motioning_state_.get_subscriber().on_next(state_code_motioning::moving);
            prev_stop_time = last_stop_time;
        }

        return;
    }

    if (state == state_code_motioning::parking) {
        last_park_time = steady_clock::now();
        if (!is_in_move_state()) return;
        // Был зафиксирован переход из состояния парковки в состояние движения
        current_motioning_state_.get_subscriber().on_next(state_code_motioning::moving);
    }
}

bool state_watcher_motioning::is_in_move_state() const {
    return steady_clock::now() - last_move_time < configuration_.stop_threshold;
}

bool state_watcher_motioning::is_in_stop_state() const {
    return steady_clock::now() - last_move_time >= configuration_.stop_threshold;
}

bool state_watcher_motioning::is_in_park_state() const {
    return steady_clock::now() - last_move_time >= configuration_.park_threshold;
}
