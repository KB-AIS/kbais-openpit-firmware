#ifndef LCS_SENSOR_PUBLISHER_H
#define LCS_SENSOR_PUBLISHER_H

// qt
#include <QObject>
#include <QSerialPort>

#include "RxQt.h"

struct lcs_sensor_message {

    double weight { 0.0 };

    double weight_raw { 0.0 };

    bool is_valid { false };

};

class i_lcs_sensor_publisher {

public:
    virtual ~i_lcs_sensor_publisher() noexcept = default;

    virtual rxcpp::observable<lcs_sensor_message> get_observable(
        const rxcpp::observe_on_one_worker& coordination
    ) const = 0;
};

class lcs_sensor_publisher : public QObject, public i_lcs_sensor_publisher {
    QSerialPort serial_port_lcs_;

    void config_serialport_connection();

    void handle_ready_read();

public:
    lcs_sensor_publisher();

    void start_work_on(
        const rxcpp::observe_on_one_worker& coordination
    );

    rxcpp::observable<lcs_sensor_message> get_observable(
        const rxcpp::observe_on_one_worker& coordination
    ) const override;

};

#endif // LCS_SENSOR_PUBLISHER_H
