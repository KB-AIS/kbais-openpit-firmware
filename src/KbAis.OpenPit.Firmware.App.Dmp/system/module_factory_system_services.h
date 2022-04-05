#ifndef MODULE_FACTORY_SYSTEM_SERVICES_H
#define MODULE_FACTORY_SYSTEM_SERVICES_H

// oss
#include <boost/di.hpp>

#include "system/setup_datetime_service.h"
#include "system/setup_ethernet_service.h"

inline auto create_injector_system_services() noexcept {
    return boost::di::make_injector(
        boost::di::bind<SetupDateTimeService>().in(boost::di::singleton)
    ,   boost::di::bind<SetupEthernetService>().in(boost::di::singleton)
    );
}

#endif // MODULE_FACTORY_SYSTEM_SERVICES_H
