#include "fake_flv_sensor_publisher.h"

// std
#include <vector>
// oss
#include <range/v3/all.hpp>

using namespace ranges;
using namespace rx;
using namespace std::chrono;
using namespace std::chrono_literals;

fake_flv_sensor_publisher::fake_flv_sensor_publisher() noexcept
    :   sub_flv_messages_ { flv_message { } }
{
}

rxcpp::observable<flv_message> fake_flv_sensor_publisher::get_observable() const {

}

void fake_flv_sensor_publisher::setup_scenario(const rxcpp::observe_on_one_worker& c) {
    auto delay_every =
        concat_map([](auto x) { return just(x) | delay(1000ms); });

    obs_flv_messages_ =
        from(
            c
        ,   iterate(std::vector<flv_message> {
                { .fuel_level = 1759.0, .max_fuel_level = 2000.0, .is_valid = true }
            ,   { .fuel_level = 1813.0, .max_fuel_level = 2000.0, .is_valid = true }
            })
        )
    |   concat() | delay_every | repeat() | multicast(sub_flv_messages_);
}

void fake_flv_sensor_publisher::start_scenario() {

}
