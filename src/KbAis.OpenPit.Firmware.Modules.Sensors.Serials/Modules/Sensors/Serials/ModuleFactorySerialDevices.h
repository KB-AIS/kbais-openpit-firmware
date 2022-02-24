#ifndef MODULEFACTORYSERIALDEVICES_H
#define MODULEFACTORYSERIALDEVICES_H

// oss
#include <boost/di.hpp>

#include "Modules/Sensors/Serials/ModuleBootstraperSerialDevices.h"
#include "Modules/Sensors/Serials/RxFuelMessagePublisher.h"

inline auto CreateInjectorSerialDevices() noexcept {
    return boost::di::make_injector(
        boost::di::bind<RxFuelMessagePublisher>
            .in(boost::di::singleton)
    ,   boost::di::bind<ModuleBootstraperSerialDevices>()
            .in(boost::di::singleton)
    );
}

#endif // MODULEFACTORYSERIALDEVICES_H
