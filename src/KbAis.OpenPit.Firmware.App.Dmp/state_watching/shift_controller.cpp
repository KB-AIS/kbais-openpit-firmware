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

shift_controller::shift_controller()
    :   config_ {
            .shifts = {
                { time_range<seconds> { {  8h +  0min +  0s }, {  8h + 00min + 59s } } }
            ,   { time_range<seconds> { {  8h +  1min +  0s }, { 16h + 59min + 59s } } }
            ,   { time_range<seconds> { { 21h + 30min +  0s }, { 22h + 59min + 59s } } }
            ,   { time_range<seconds> { { 23h + 15min +  0s }, {  1h + 59min + 59s } } }
            ,   { time_range<seconds> { {  2h +  0min +  0s }, {  7h + 59min + 59s } } }
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
    using days_t = std::chrono::duration
            <int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

    constexpr auto get_seconds_since_last_midnight = []() {
        const auto ns_point = system_clock::now();
        const auto days_point = floor<days_t>(ns_point);

        return duration_cast<seconds>(ns_point - days_point);
    };

    constexpr auto find_current_shift = [](
        seconds time
    ,   const std::vector<time_range<seconds>>& shifts
    ) -> std::optional<time_range<seconds>> {
        const auto shift_itr = ranges::find_if(
            shifts
        ,   [&t = time](const time_range<seconds>& x) -> bool {
                return x.is_in_range(t);
            }
        );

        return shift_itr == ranges::cend(shifts)
        ?   std::nullopt
        :   std::make_optional(*shift_itr);
    };

    constexpr auto find_upcomming_shift = [](
        seconds time
    ,   const std::vector<time_range<seconds>>& shifts
    ) -> std::optional<time_range<seconds>> {
        auto shifts_filterd_and_sorted = shifts
        |   ranges::views::filter(
                [&t = time](const time_range<seconds>& x) -> bool {
                    return !x.is_in_range(t);
                }
            )
        |   ranges::to<std::vector>()
        |   ranges::actions::sort(
                std::less<seconds> { }
            ,   [&t = time](const time_range<seconds>& x) -> seconds {
                    return x.get_duration_to_lower(t);
                }
            );

        return ranges::empty(shifts_filterd_and_sorted)
        ?   std::nullopt
        :   std::make_optional(ranges::front(shifts_filterd_and_sorted));
    };

    const auto seconds_since_midnight = 8h + 58s;//get_seconds_since_last_midnight();

    auto current_shift = find_current_shift(seconds_since_midnight, config_.shifts);

    if (current_shift) {
        auto d = current_shift->get_duration_to_upper(seconds_since_midnight);
        auto s = rxcpp::observable<>::timer(d).subscribe([this](auto) { handle_shift_close(); });
        subs_t_.add(s);
    }

    auto upcomming_shift = find_upcomming_shift(seconds_since_midnight, config_.shifts);

    if (upcomming_shift) {
        auto d = upcomming_shift->get_duration_to_lower(seconds_since_midnight);
        auto s =rxcpp::observable<>::timer(d).subscribe([this](auto) { handle_shift_start(); });
        subs_t_.add(s);
    }
}

void shift_controller::handle_shift_start() {
    PLOGD << "Performing shift start";
}

void shift_controller::handle_shift_close() {
    PLOGD << "Performing shift close";
}
