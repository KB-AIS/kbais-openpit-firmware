#include "shift_controller.h"

// std
#include <chrono>
#include <optional>
// oss
#include <fmt/chrono.h>
#include <plog/Log.h>
#include <range/v3/all.hpp>

using namespace std::chrono;
using namespace std::chrono_literals;

shift_controller::shift_controller(
    const i_gps_sensor_publisher& gps_sensor_publisher
,   const i_state_loading_changed_publisher& state_loading_changed_publisher
)
    :   config_ {
            .shifts = {
                { time_range<seconds> { {  8h +  0min +  0s }, {  8h + 00min + 59s } } }
            ,   { time_range<seconds> { {  8h +  1min +  0s }, { 16h + 59min + 59s } } }
            ,   { time_range<seconds> { { 21h + 30min +  0s }, { 22h + 59min + 59s } } }
            ,   { time_range<seconds> { { 23h + 15min +  0s }, {  1h + 59min + 59s } } }
            ,   { time_range<seconds> { {  2h +  0min +  0s }, {  7h + 59min + 59s } } }
            }
        }
    ,   gps_sensor_publisher_ { gps_sensor_publisher }
    ,   state_loading_changed_publisher_ { state_loading_changed_publisher }
{}

constexpr double get_radians(double degrees) {
    return degrees / 180.0 * M_PI;
}

void shift_controller::start_working_on(
    [[maybe_unused]] const rxcpp::observe_on_one_worker& coordination
) {
    PLOGD << "Setup work of service";

    // TODO: observe configuration changes
    auto sub_setup_timers = rxcpp::observable<>::just(1, coordination)
        .tap([](auto&) { PLOGD << "Got a new configuration"; })
        .subscribe([this](const auto&) { handle_setup_shift_close_timer(); } );

    subs_.add(sub_setup_timers);

    auto sub_total_mileage = gps_sensor_publisher_.get_observable().observe_on(coordination)
        .filter([](const GpsMessage& x) { return x.is_valid && x.satellites >= 4; })
        .subscribe([this](const GpsMessage& x) {
            auto& y = data_.last_position;

            if (y.is_valid) {
                const auto lat1 = get_radians(x.latitude), lng1 = get_radians(x.longitude);
                const auto lat2 = get_radians(y.latitude), lng2 = get_radians(y.longitude);

                const auto sin_lat1 = std::sin(lat1), cos_lat1 = std::cos(lat1);
                const auto sin_lat2 = std::sin(lat2), cos_lat2 = std::cos(lat2);

                const auto dlt_lng = lng2 - lng1;
                const auto cos_dlt_lng = std::cos(dlt_lng), sin_dlt_lng = std::sin(dlt_lng);

                const auto d = std::atan2(
                    std::sqrt(
                        std::pow(cos_lat2 * sin_dlt_lng, 2)
                    +   std::pow(cos_lat1 * sin_lat2 - sin_lat1 * cos_lat2 * cos_dlt_lng, 2)
                    ),
                    sin_lat1 * sin_lat2 + cos_lat1 * cos_lat2 * cos_dlt_lng
                );

                constexpr double ELLIPSOID_WGS84_M = 6372795.0;

                auto distance = ELLIPSOID_WGS84_M * d;

                data_.total_mileage_m += distance;

                PLOGD << data_.total_mileage_m;
            }

            y = x;
        });

    subs_.add(sub_total_mileage);
}

void shift_controller::handle_dt_sys_change() { }

void shift_controller::handle_config_change() { }

void shift_controller::handle_setup_shift_close_timer() {
    constexpr auto get_seconds_since_last_midnight = []() {
        using days_t = duration<int, std::ratio_multiply<std::ratio<24>, hours::period>>;

        const auto ns_point = system_clock::now();
        const auto days_point = floor<days_t>(ns_point);

        return duration_cast<seconds>(ns_point - days_point);
    };

    constexpr auto find_current_shift = [](
        const seconds time
    ,   const std::vector<time_range<seconds>>& shifts
    ) -> std::optional<time_range<seconds>> {
        const auto findif_fn = [&t = time](const time_range<seconds>& x) -> bool {
            return x.is_in_range(t);
        };

        const auto shift_itr = ranges::find_if(shifts, findif_fn);

        return shift_itr == ranges::cend(shifts)
        ?   std::nullopt
        :   std::make_optional(*shift_itr);
    };

    constexpr auto find_upcomming_shift = [](
        const seconds time
    ,   const std::vector<time_range<seconds>>& shifts
    ) -> std::optional<time_range<seconds>> {
        const auto filter_fn = [&t = time](const time_range<seconds>& x) -> bool {
            return !x.is_in_range(t);
        };

        const auto projec_fn = [&t = time](const time_range<seconds>& x) -> seconds {
            return x.get_duration_to_lower(t);
        };

        const auto shifts_filterd_and_sorted = shifts
        |   ranges::views::filter(filter_fn)
        |   ranges::to<std::vector>()
        |   ranges::actions::sort(std::less<seconds> {}, projec_fn);

        return ranges::empty(shifts_filterd_and_sorted)
        ?   std::nullopt
        :   std::make_optional(ranges::front(shifts_filterd_and_sorted));
    };

    const auto seconds_since_midnight = 8h + 58s;//get_seconds_since_last_midnight();

    auto current_shift = find_current_shift(seconds_since_midnight, config_.shifts);
    if (current_shift) {
        const auto d = current_shift->get_duration_to_upper(seconds_since_midnight);
        const auto s = rxcpp::observable<>::timer(d)
            .subscribe([this](auto) { handle_shift_close(); });
        subs_shift_timers_.add(s);
    }

    auto upcomming_shift = find_upcomming_shift(seconds_since_midnight, config_.shifts);
    if (upcomming_shift) {
        const auto d = upcomming_shift->get_duration_to_lower(seconds_since_midnight);
        const auto s = rxcpp::observable<>::timer(d)
            .subscribe([this](auto) { handle_shift_start(); });
        subs_shift_timers_.add(s);
    }
}

void shift_controller::handle_shift_start() {
    PLOGD << "Performing shift start";
}

void shift_controller::handle_shift_close() {
    PLOGD << "Performing shift close";
}
