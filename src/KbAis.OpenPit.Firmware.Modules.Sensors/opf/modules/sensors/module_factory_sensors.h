#ifndef MODULE_FACTORY_SENSORS_H
#define MODULE_FACTORY_SENSORS_H

// oss
#include <boost/di.hpp>

#include "opf/modules/sensors/flv/flv_calibration_publisher.h"
#include "opf/modules/sensors/gps/gps_sensor_publisher.h"
#include "opf/modules/sensors/runner_module_sensors.h"

inline auto create_module_sensors() noexcept {
    return boost::di::make_injector(
        boost::di::bind<i_flv_message_publisher>
            .to<flv_calibration_publisher>().in(boost::di::singleton)

    ,   boost::di::bind<i_gps_sensor_publisher>()
            .to<gps_sensor_publisher>().in(boost::di::singleton)

    ,   boost::di::bind<runner_module_sensors>()
            .in(boost::di::singleton)
    );
}

#endif // MODULE_FACTORY_SENSORS_H
