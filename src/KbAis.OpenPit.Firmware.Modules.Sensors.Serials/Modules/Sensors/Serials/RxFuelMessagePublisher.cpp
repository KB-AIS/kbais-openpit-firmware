#include "RxFuelMessagePublisher.h"

// oss
#include <plog/Log.h>

#include "Format.h"

RxFuelMessagePublisher::RxFuelMessagePublisher(
    const SerialRxLlsSensorPublisher& lls_sensor_publisher
)
    :   m_lls_sensor_publisher { lls_sensor_publisher }
    ,   m_subject_fuel_message { FuelMessage { } }
{

}

void
RxFuelMessagePublisher::start_publish_on() {
    m_lls_sensor_publisher.GetObservableMessage()
        //.observe_on(main_thread)
        .subscribe(
            m_subscription
        ,   [this](const LlsDeviceMessage message) { handle_calibration(message); }
    );
}

rxcpp::observable<FuelMessage>
RxFuelMessagePublisher::get_obeservable_fuel_message() const {
    return m_subject_fuel_message.get_observable();
}

void
RxFuelMessagePublisher::handle_calibration(const LlsDeviceMessage& message) {
    // Hardcoded solution, refactor later
    if (message.data.empty()) return;

    const auto find_itr = message.data.find(0x01);
    if (find_itr == message.data.cend()) return;

    if (!find_itr->second) return;

    // Take value of first sensor
    const auto& data = find_itr->second.value();

    auto fuel = perform_some_magic(data.Lvl);
    PLOGD << fmt::format("Got fuel value after calibration: {:d}", fuel);

    m_subject_fuel_message.get_subscriber().on_next(FuelMessage { fuel, true });
}

quint32
RxFuelMessagePublisher::perform_some_magic(quint16 lls_level) {
    int n_tabl;
    double xd;
    ItemSensor sensor_config { { 0, 100, 2000 }, { 0, 800, 2600 } };

    n_tabl = sensor_config.adc.size();
    xd = static_cast<double>(lls_level);
    int start = 0, end = n_tabl - 2;
    while (start < end) {
        int half = (start + end + 1) >> 1;
        if (xd < sensor_config.adc[half]) {
            end = half - 1;
        } else {
            start = half;
        }
    }
    double y =
        sensor_config.litrs[start]
        + (
            (xd - static_cast<double>(sensor_config.adc[start]))
            * (sensor_config.litrs[start+1] - sensor_config.litrs[start])
          )
        / (sensor_config.adc[start+1] - sensor_config.adc[start]);

    return static_cast<quint32>(y);
}
