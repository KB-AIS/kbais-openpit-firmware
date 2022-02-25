#include "RxFuelMessagePublisher.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Format.h"

constexpr int LIT = 0, ADC = 1;
static LlsCalibrationTable_t calibration_table { { 0, 0 }, { 100, 800 }, { 2000, 2600 } };

RxFuelMessagePublisher::RxFuelMessagePublisher(
    const SerialRxLlsSensorPublisher& lls_sensor_publisher
)
    :   m_lls_sensor_publisher(lls_sensor_publisher)
    ,   m_subject_fuel_message(FuelMessage { })
{

}

void
RxFuelMessagePublisher::start_publish_on(const rxcpp::observe_on_one_worker& coordination) const {
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

    auto fuel = get_fuel_level_by_calibration_table(static_cast<double>(lls_data.Lvl));

    PLOGD << fmt::format("Got fuel value after calibration: {:f}", fuel, 4);

    subscriber.on_next(FuelMessage { fuel, true });
}

double
RxFuelMessagePublisher::get_fuel_level_by_calibration_table(double lls_level) {
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
