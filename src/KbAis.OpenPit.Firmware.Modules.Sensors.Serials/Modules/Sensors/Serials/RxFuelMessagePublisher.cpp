#include "RxFuelMessagePublisher.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Format.h"

using namespace std::chrono_literals;
using namespace nlohmann;

constexpr int LIT = 0, ADC = 1;
static LlsCalibrationTable_t calibration_table { { 0, 0 }, { 100, 800 }, { 2000, 2600 } };

RxFuelMessagePublisher::RxFuelMessagePublisher(
    IRxConfigurationChangePublisher& config_publisher
,   SerialRxLlsSensorPublisher& lls_sensor_publisher
)
    :   m_config_publisher(config_publisher)
    ,   m_lls_sensor_publisher(lls_sensor_publisher)
    ,   m_subject_fuel_message(FuelMessage { })
{

}

void
RxFuelMessagePublisher::start_publish_on(const rxcpp::observe_on_one_worker& coordination) {
    m_config_publisher.getChangeObservable("scale")
        .sample_with_time(500ms, coordination)
        .subscribe(
            m_subscription
        ,   [this](const AppConfiguration scale_config) {
                // Parce calibration table
                // TODO: Get calibration table for all sensors
                m_calibration_table = scale_config.j_object.at("/Scales/0/Sens/0/Tar"_json_pointer)
                |   ranges::view::transform([](const json& x) {
                        return std::make_tuple(
                            x.at("ADC").get<qint32>()
                        ,   x.at("Litrs").get<qint32>()
                        );
                    }
                )
                |   ranges::to<LlsCalibrationTable_t>();
            }
        );

    m_lls_sensor_publisher.GetObservableMessage()
        .observe_on(coordination)
        .subscribe(
            m_subscription
        ,   [this](const LlsDeviceMessage message) { handle_fuel_calibration(message); }
        );
}

rxcpp::observable<FuelMessage>
RxFuelMessagePublisher::get_obeservable_fuel_message() const {
    return m_subject_fuel_message.get_observable();
}

void
RxFuelMessagePublisher::handle_fuel_calibration(const LlsDeviceMessage& message) const {
    const auto subscriber = m_subject_fuel_message.get_subscriber();

    if (m_calibration_table.empty()) {
        return subscriber.on_next(FuelMessage { });
    }

    // TODO: Refactor, hardcoded solution only for first sensor
    if (message.data.empty()) {
        return subscriber.on_next(FuelMessage { });
    }

    const auto find_itr = message.data.find(0x01);
    if (find_itr == message.data.cend()) {
        return subscriber.on_next(FuelMessage { });
    }

    if (!find_itr->second) {
        return subscriber.on_next(FuelMessage { });
    }

    const auto& lls_data = find_itr->second.value();

    auto fuel = get_fuel_level_by_calibration_table(lls_data.Lvl);

    PLOGD << fmt::format("Got fuel value after calibration: {:f}", fuel, 4);

    // TODO: Get max from settings
    subscriber.on_next(FuelMessage { fuel, 2000, true });
}

void
RxFuelMessagePublisher::handle_new_scale_configuration(const AppConfiguration &scale_config) {

}

double
RxFuelMessagePublisher::get_fuel_level_by_calibration_table(double lls_level) const {
    int s_idx = 0, e_idx = calibration_table.size() - 2;

    while (s_idx < e_idx) {
        int bound_idx = (s_idx + e_idx + 1) >> 1;

        if (lls_level < std::get<ADC>(calibration_table[bound_idx])) {
            e_idx = bound_idx - 1;
        } else {
            s_idx = bound_idx;
        }
    }

    auto [lit_lb, adc_lb] = calibration_table[s_idx];
    auto [lit_ub, adc_ub] = calibration_table[s_idx + 1];
    auto fuel_level = lit_lb + ((lls_level - adc_lb) * (lit_ub - lit_lb)) / (adc_ub - adc_lb);

    return static_cast<quint32>(fuel_level);
}
