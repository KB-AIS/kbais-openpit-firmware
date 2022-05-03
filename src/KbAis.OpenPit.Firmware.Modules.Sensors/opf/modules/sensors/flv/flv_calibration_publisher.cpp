#include "flv_calibration_publisher.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Format.h"

using namespace std::chrono_literals;
using namespace nlohmann;

flv_calibration_publisher::flv_calibration_publisher(
    i_app_configuration_publisher& app_configuration_publisher
,   lls_sensor_publisher& lls_sensor_publisher
)
    :   app_configuration_publisher_ { app_configuration_publisher }
    ,   lls_sensor_publisher_ { lls_sensor_publisher }
    ,   last_flv_message_ { flv_message { } }
{

}

void
flv_calibration_publisher::start_publish_on(const rxcpp::observe_on_one_worker& coord) {
    app_configuration_publisher_.get_observable("scale")
        .sample_with_time(500ms, coord)
        .subscribe(
            subscriptions_
        ,   [this](const app_configuration x) { handle_new_scale_config(x); }
        );

    lls_sensor_publisher_.get_observable()
        .observe_on(coord)
        .subscribe(
            subscriptions_
        ,   [this](const lls_sensor_message x) { handle_fuel_calibration(x); }
        );
}

rxcpp::observable<flv_message>
flv_calibration_publisher::get_observable(const rxcpp::observe_on_one_worker& c) const {
    return last_flv_message_.get_observable();
}

void
flv_calibration_publisher::handle_new_scale_config(const app_configuration& config) {
    // Parce calibration table
    // TODO: Get calibration table for all sensors
    calibration_table_ = config.value.at("/Scales/0/Sens/0/Tar"_json_pointer)
    |   ranges::views::transform([](const json& x) {
            return std::make_tuple(
                x.at("ADC").get<qint32>()
            ,   x.at("Litrs").get<qint32>()
            );
        }
    )
    |   ranges::to<LlsCalibrationTable_t>();

    max_fuel_level_ = config.value.at("/Scales/0/MaxScale"_json_pointer).get<double>();
}

void
flv_calibration_publisher::handle_fuel_calibration(const lls_sensor_message& message) const {
    const auto subscriber = last_flv_message_.get_subscriber();

    if (calibration_table_.empty()) {
        return subscriber.on_next(flv_message { });
    }

    // TODO: Refactor, hardcoded solution only for first sensor
    if (message.data.empty()) {
        return subscriber.on_next(flv_message { });
    }

    const auto find_itr = message.data.find(0x01);
    if (find_itr == message.data.cend()) {
        return subscriber.on_next(flv_message { });
    }

    if (!find_itr->second) {
        return subscriber.on_next(flv_message { });
    }

    const auto& lls_data = find_itr->second.value();

    auto fuel = get_fuel_level_by_calibration_table(lls_data.Lvl);

    PLOGD << fmt::format("Got fuel value after calibration: {:f}", fuel, 4);

    subscriber.on_next(flv_message { fuel, max_fuel_level_, true });
}

double
flv_calibration_publisher::get_fuel_level_by_calibration_table(double lls_level) const {
    constexpr int LIT = 0, ADC = 1;

    int s_idx = 0, e_idx = calibration_table_.size() - 2;

    while (s_idx < e_idx) {
        int bound_idx = (s_idx + e_idx + 1) >> 1;

        if (lls_level < std::get<ADC>(calibration_table_[bound_idx])) {
            e_idx = bound_idx - 1;
        } else {
            s_idx = bound_idx;
        }
    }

    auto [lit_lb, adc_lb] = calibration_table_[s_idx];
    auto [lit_ub, adc_ub] = calibration_table_[s_idx + 1];
    auto fuel_level = lit_lb + ((lls_level - adc_lb) * (lit_ub - lit_lb)) / (adc_ub - adc_lb);

    return static_cast<quint32>(fuel_level);
}
