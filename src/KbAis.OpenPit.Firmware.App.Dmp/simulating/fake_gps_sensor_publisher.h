#ifndef FAKE_GPS_SENSOR_PUBLISHER_H
#define FAKE_GPS_SENSOR_PUBLISHER_H

#include "IRxGpsSensorPublisher.h"
#include "modules/sensors/serials/lcs/lcs_sensor_publisher.h"
#include "simulating/scenario_executor.h"
#include "state_watching/state_watcher_motioning.h"

class fake_gps_sensor_publisher : public i_gps_sensor_publisher, public i_scenario_executor {
    rxcpp::connectable_observable<GpsMessage> observable_gps_messages_;

    rxcpp::rxsub::behavior<GpsMessage> subject_gps_messages_;

public:
    fake_gps_sensor_publisher() noexcept;

    const rxcpp::observable<GpsMessage> get_observable() const override;

    void setup_scenario(const rxcpp::observe_on_one_worker& coordination) override;

    void start_scenario() override;
};

#endif // FAKE_GPS_SENSOR_PUBLISHER_H
