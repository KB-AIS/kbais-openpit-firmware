#include "fake_gps_sensor_publisher.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

using namespace ranges;
using namespace rx;
using namespace std::chrono;
using namespace std::chrono_literals;

constexpr auto GPS_MESSAGE_SEND_INTERVAL { 1000ms };

std::mt19937 random_engine;

fake_gps_sensor_publisher::fake_gps_sensor_publisher() noexcept
    :   subject_gps_messages_ { GpsMessage { } }
{
}

const rxcpp::observable<GpsMessage> fake_gps_sensor_publisher::get_observable() const {
    return subject_gps_messages_.get_observable();
}

void fake_gps_sensor_publisher::setup_scenario(
    const rxcpp::observe_on_one_worker& coordination
) {
    constexpr auto make_gps_message = [](double speed) -> GpsMessage {
        return {
            .datetime = { }, .is_valid = true, .latitude  = 0.0, .longitude = 0.0
        ,   .speed = speed
        };
    };

    constexpr auto gen_gps_messages_w_const_spd = [mkr = make_gps_message](
        double speed
    ,   duration<int64_t, std::milli> duration
    ) -> std::vector<GpsMessage> {
        const auto msgs_num = duration / GPS_MESSAGE_SEND_INTERVAL;

        return
            views::generate([&]() -> GpsMessage { return mkr(speed); })
        |   views::take(msgs_num)
        |   to<std::vector>();
    };

    constexpr auto gen_gps_messages_w_smooth_spd = [mkr = make_gps_message](
        double speed_s, double speed_e
    ,   duration<int64_t, std::milli> duration
    ) -> std::vector<GpsMessage> {
        const auto msgs_num = duration / GPS_MESSAGE_SEND_INTERVAL;

        return
            views::linear_distribute(speed_s, speed_e, msgs_num)
        |   views::transform([&](double x) { return mkr(x); })
        |   to<std::vector>();
    };

    constexpr auto gen_gps_messages_w_shuffle_spd = [mkr = make_gps_message](
        double speed_s, double speed_e
    ,   duration<int64_t, std::milli> duration
    ) -> std::vector<GpsMessage> {
        const auto msgs_num = duration / GPS_MESSAGE_SEND_INTERVAL;

        return
            views::linear_distribute(speed_s, speed_e, msgs_num)
        |   views::transform([&](double x) { return mkr(x); })
        |   to<std::vector>()
        |   actions::shuffle(random_engine);
    };

    auto delay_every =
        concat_map([](auto x) { return just(x) | delay(GPS_MESSAGE_SEND_INTERVAL); });

    observable_gps_messages_ =
        from(
            coordination
        ,   iterate(gen_gps_messages_w_const_spd(0.0, 18s))
        ,   iterate(gen_gps_messages_w_smooth_spd(0.0, 30.0, 8s))
        ,   iterate(gen_gps_messages_w_shuffle_spd(28.8, 32.3, 30s))
        ,   iterate(gen_gps_messages_w_smooth_spd(0.0, 30.0, 8s) | actions::reverse)
        ,   iterate(gen_gps_messages_w_shuffle_spd(0.0, 4.3, 22s))
        ,   iterate(gen_gps_messages_w_const_spd(25.0, 30s))
        )
    |   concat() | delay_every | repeat() | multicast(subject_gps_messages_);
}

void fake_gps_sensor_publisher::start_scenario() {
    observable_gps_messages_.connect();
}
