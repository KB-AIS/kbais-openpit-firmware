#ifndef RXFUELMESSAGEPUBLISHER_H
#define RXFUELMESSAGEPUBLISHER_H

// TODO: Use module boundary contract
#include "Modules/Sensors/Serials/SerialRxLlsSensorPublisher.h"

struct ItemSensor {
    std::vector<qint32> litrs;
    std::vector<qint32> adc;
};

struct FuelMessage {
    quint32 fuel_level { 0 };

    bool is_value_valid { false };
};

class RxFuelMessagePublisher {

    const SerialRxLlsSensorPublisher& m_lls_sensor_publisher;

    rxcpp::composite_subscription m_subscription;

    rxcpp::rxsub::behavior<FuelMessage> m_subject_fuel_message;

    void handle_calibration(const LlsDeviceMessage& message);

    static quint32 perform_some_magic(quint16 lls_value);

public:
    RxFuelMessagePublisher(const SerialRxLlsSensorPublisher& lls_sensor_publisher);

    void start_publish_on();

    rxcpp::observable<FuelMessage> get_obeservable_fuel_message() const;

};

#endif // RXFUELMESSAGEPUBLISHER_H
