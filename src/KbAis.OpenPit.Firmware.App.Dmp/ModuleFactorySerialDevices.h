#ifndef MODULEFACTORYSERIALDEVICES_H
#define MODULEFACTORYSERIALDEVICES_H

// oss
#include <boost/di.hpp>

#include "ModuleBootstraperSerialDevices.h"

inline auto CreateInjectorSerialDevices() noexcept {
    return boost::di::make_injector(
        boost::di::bind<ModuleBootstraperSerialDevices>()
            .in(boost::di::singleton)
    );
}

#endif // MODULEFACTORYSERIALDEVICES_H
