#include "state_watcher_motioning.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

StateWatcherMotioning::StateWatcherMotioning(const IRxGpsSensorPublisher& gps_msg_pub)
    :   gps_msg_pub_(gps_msg_pub)
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

void StateWatcherMotioning::handle_gps_message(const GpsMessage& gps_msg) {
    buffered_speed_values_[buffered_speed_cur_idx_] = gps_msg.speed;
    buffered_speed_cur_idx_ = (buffered_speed_cur_idx_ + 1) % buffered_speed_size_;

    const auto speed_avg = ranges::accumulate(buffered_speed_values_, 0.0) / buffered_speed_size_;

    PLOGD << "Average speed is " << speed_avg;

    const auto is_moving = speed_avg > settings_.min_speed_to_move;

    PLOGD << "Equipment is moving " << is_moving;

    constexpr auto is_move = []() -> bool { return true; };

    constexpr auto is_stop = []() -> bool { return true; };

    constexpr auto is_park = []() -> bool { return true; };

}
