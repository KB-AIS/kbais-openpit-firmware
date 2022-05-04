#ifndef RUNNER_MODULE_SENSORS_H
#define RUNNER_MODULE_SENSORS_H

#include "rxqt.h"

#include "opf/modules/sensors/crd/crd_sensor_publisher.h"
#include "opf/modules/sensors/lcs/lcs_sensor_publisher.h"
#include "opf/modules/sensors/lls/lls_sensor_publisher.h"

class runner_module_sensors : public QObject {

    crd_sensor_publisher& crd_sensor_publisher_;

    lcs_sensor_publisher& lcs_sensor_publisher_;

    lls_sensor_publisher& lls_sensor_publisher_;

    QThread thr_worker_;

    std::unique_ptr<rxqt::run_loop> thr_run_loop_;

public:
    runner_module_sensors(
        crd_sensor_publisher& crd_sensor_publisher
    ,   lcs_sensor_publisher& lcs_sensor_publisher
    ,   lls_sensor_publisher& lls_sensor_publisher
    );

};

#endif // RUNNER_MODULE_SENSORS_H
