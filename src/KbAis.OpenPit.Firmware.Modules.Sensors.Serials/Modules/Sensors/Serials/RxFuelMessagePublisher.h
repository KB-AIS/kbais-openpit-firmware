#ifndef RXFUELMESSAGEPUBLISHER_H
#define RXFUELMESSAGEPUBLISHER_H

// TODO: Use module boundary contract
#include "Modules/Sensors/Serials/SerialRxLlsSensorPublisher.h"

using LlsCalibrationTable_t = std::vector<std::tuple<qint32, qint32>>;

struct FuelMessage {
    double fuel_level { 0 };

    bool is_value_valid { false };
};

struct ItemSensor {
    std::vector<qint32> litrs;
    std::vector<qint32> adc;
};

class RxFuelMessagePublisher {

    const SerialRxLlsSensorPublisher& m_lls_sensor_publisher;

    rxcpp::composite_subscription m_subscription;

    rxcpp::rxsub::behavior<FuelMessage> m_subject_fuel_message;

    void handle_fuel_calibration(const LlsDeviceMessage& message) const;

    static double get_fuel_level_by_calibration_table(double lls_value);

public:
    RxFuelMessagePublisher(const SerialRxLlsSensorPublisher& lls_sensor_publisher);

    void start_publish_on(const rxcpp::observe_on_one_worker& coordination) const;

    rxcpp::observable<FuelMessage> get_obeservable_fuel_message() const;

};

#endif // RXFUELMESSAGEPUBLISHER_H
