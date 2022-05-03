#ifndef MODULE_FACTORY_SYSTEM_SERVICES_H
#define MODULE_FACTORY_SYSTEM_SERVICES_H

// oss
#include <boost/di.hpp>

#include "system/setup_datetime_service.h"
#include "system/setup_ethernet_service.h"

inline auto create_module_system() noexcept {
    return boost::di::make_injector(
        boost::di::bind<setup_datetime_service>().in(boost::di::singleton)
    ,   boost::di::bind<setup_ethernet_service>().in(boost::di::singleton)
    );
}

#endif // MODULE_FACTORY_SYSTEM_SERVICES_H
