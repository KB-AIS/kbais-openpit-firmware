#include "runner_module_sensors_serials.h"

#include "QtExtensions/QTimerExt.h"

runner_module_sensors_serials::runner_module_sensors_serials(
    crd_sensor_publisher& crd_sensor_publisher
,   lcs_sensor_publisher& lcs_sensor_publisher
,   lls_sensor_publisher& lls_sensor_publisher
)
    :   crd_sensor_publisher_ { crd_sensor_publisher }
    ,   lcs_sensor_publisher_ { lcs_sensor_publisher }
    ,   lls_sensor_publisher_ { lls_sensor_publisher }
{
    thr_worker_.setObjectName("MODULES.SENSORS.SERIAL");

    crd_sensor_publisher_.moveToThread(&thr_worker_);
    lcs_sensor_publisher_.moveToThread(&thr_worker_);
    lls_sensor_publisher_.moveToThread(&thr_worker_);

    QObject::connect(&thr_worker_, &QThread::started, [&]() {
        thr_run_loop_ = std::make_unique<rxqt::run_loop>();

        const auto coord = thr_run_loop_->observe_on_run_loop();

        crd_sensor_publisher_.start_work_on(coord);
        lcs_sensor_publisher_.start_work_on(coord);
        lls_sensor_publisher_.start_work_on(coord);
    });

    QObject::connect(&thr_worker_, &QThread::finished, &thr_worker_, &QThread::deleteLater);

    thr_worker_.start();
}
