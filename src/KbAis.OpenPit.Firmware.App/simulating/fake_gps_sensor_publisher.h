#ifndef FAKE_GPS_SENSOR_PUBLISHER_H
#define FAKE_GPS_SENSOR_PUBLISHER_H

#include "simulating/scenario_executor.h"
#include "state_watching/state_watcher_motioning.h"

#include "opf/modules/sensors/gps/gps_sensor_publisher.h"

class fake_gps_sensor_publisher : public i_gps_sensor_publisher, public i_scenario_executor {
    rxcpp::connectable_observable<gps_sensor_message> obs_gps_sensor_messages_;
    rxcpp::rxsub::behavior<gps_sensor_message>        sub_gps_sensor_messages_;

public:
    fake_gps_sensor_publisher() noexcept;

    const rxcpp::observable<gps_sensor_message> get_observable() const override;

    void setup_scenario(const rxcpp::observe_on_one_worker& c) override;

    void start_scenario() override;

};

#endif // FAKE_GPS_SENSOR_PUBLISHER_H
