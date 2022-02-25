#ifndef RXFUELMESSAGEPUBLISHER_H
#define RXFUELMESSAGEPUBLISHER_H

// TODO: Use module boundary contract
#include "Modules/Sensors/Serials/SerialRxLlsSensorPublisher.h"
#include "IRxConfigurationChangePublisher.h"

using LlsCalibrationTable_t = std::vector<std::tuple<qint32, qint32>>;

struct FuelMessage {
    double cur_fuel_level { 0 };

    double max_fuel_level { 0 };

    bool is_value_valid { false };
};

class RxFuelMessagePublisher {

    IRxConfigurationChangePublisher& m_config_publisher;

    SerialRxLlsSensorPublisher& m_lls_sensor_publisher;

    rxcpp::composite_subscription m_subscription;

    rxcpp::rxsub::behavior<FuelMessage> m_subject_fuel_message;

    LlsCalibrationTable_t m_calibration_table { };

    void handle_fuel_calibration(const LlsDeviceMessage& message) const;

    double get_fuel_level_by_calibration_table(double lls_value) const;

public:
    RxFuelMessagePublisher(
        IRxConfigurationChangePublisher& config_publisher
    ,   SerialRxLlsSensorPublisher& lls_sensor_publisher
    );

    void start_publish_on(const rxcpp::observe_on_one_worker& coordination);

    rxcpp::observable<FuelMessage> get_obeservable_fuel_message() const;

};

#endif // RXFUELMESSAGEPUBLISHER_H
