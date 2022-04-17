#ifndef MODULE_FACTORY_SENSORS_SERIALS_H
#define MODULE_FACTORY_SENSORS_SERIALS_H

// oss
#include <boost/di.hpp>

#include "modules/sensors/serials/lls/flv_calibration_publisher.h"
#include "modules/sensors/serials/runner_module_sensors_serials.h"

inline auto create_module_sensors_serials() noexcept {
    return boost::di::make_injector(
        boost::di::bind<flv_calibration_publisher>
            .in(boost::di::singleton)
    ,   boost::di::bind<runner_module_sensors_serials>()
            .in(boost::di::singleton)
    );
}

#endif // MODULE_FACTORY_SENSORS_SERIALS_H
