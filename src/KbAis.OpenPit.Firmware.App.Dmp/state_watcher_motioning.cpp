#include "state_watcher_motioning.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

using namespace std::chrono;

StateWatcherMotioning::StateWatcherMotioning(const IRxGpsSensorPublisher& gps_msg_pub)
    :   gps_msg_pub_(gps_msg_pub)
    ,   current_motioning_state_(MotioningState::Move)
    ,   buffered_speed_values_(buffered_speed_size_)
{

}

void StateWatcherMotioning::start_working_on() {
    gps_msg_pub_.get_observable()
        .subscribe(
            subscriptions_
        ,   [this](const GpsMessage& x) { handle_gps_message(x); }
        );
}

rxcpp::observable<MotioningState> StateWatcherMotioning::get_observable() const {
    return current_motioning_state_.get_observable();
}

void StateWatcherMotioning::handle_gps_message(const GpsMessage& gps_msg) {
    buffered_speed_values_[buffered_speed_cur_idx_] = gps_msg.speed;
    buffered_speed_cur_idx_ = (buffered_speed_cur_idx_ + 1) % buffered_speed_size_;

    const auto speed_avg = ranges::accumulate(buffered_speed_values_, 0.0) / buffered_speed_size_;

    // Проверка на то, есть ли сейчас движение согласно показателем с GPS-антенны
    if (speed_avg > settings_.min_speed_to_move) {
        last_move_time = steady_clock::now();
    }

    const auto state = current_motioning_state_.get_value();

    if (state == MotioningState::Move && is_in_stop_state()) {
        // Был зафиксирован переходи из состояния движения в состояние остановки
        current_motioning_state_.get_subscriber().on_next(MotioningState::Stop);
        return;
    }

    if (state == MotioningState::Stop) {
        last_stop_time = steady_clock::now();

        if (is_in_park_state()) {
            current_motioning_state_.get_subscriber().on_next(MotioningState::Park);
            last_stop_time = prev_stop_time;
        }

        if (is_in_move_state()) {
            current_motioning_state_.get_subscriber().on_next(MotioningState::Move);
            prev_stop_time = last_stop_time;
        }

        return;
    }

    if (state == MotioningState::Park) {
        last_park_time = steady_clock::now();
        if (!is_in_move_state()) return;
        // Был зафиксирован переход из состояния парковки в состояние движения
        current_motioning_state_.get_subscriber().on_next(MotioningState::Move);
    }
}

bool StateWatcherMotioning::is_in_move_state() const {
    return steady_clock::now() - last_move_time < settings_.stop_threshold;
}

bool StateWatcherMotioning::is_in_stop_state() const {
    return steady_clock::now() - last_move_time >= settings_.stop_threshold;
}

bool StateWatcherMotioning::is_in_park_state() const {
    return steady_clock::now() - last_move_time >= settings_.park_threshold;
}
