#ifndef COMPOSITIONROOTFACTORY_H
#define COMPOSITIONROOTFACTORY_H

// oss
#include <boost/di.hpp>

#include "AgtpServiceModuleFactory.h"
#include "App/Dmp/Presentation/ModuleFactoryPresentation.h"
#include "Core/Networking/ModuleFactoryNetworking.h"
#include "Core/Persisting/blocking_message_caching_service.h"
#include "Core/Persisting/runner_core_persisting.h"
#include "SerialRxGpsSensorPublisher.h"
#include "ThreadWorkerMessaging.h"
#include "messaging/immediate_message_mapper_dmp.h"
#include "messaging/recurrent_message_mapper_dmp.h"
#include "modules/sensors/serials/module_factory_sensors_serials.h"
#include "presentation/dmp/state_changed_publisher.h"
#include "simulating/fake_gps_sensor_publisher.h"
#include "simulating/fake_lcs_sensor_publisher.h"
#include "state_watching/state_watcher_motioning.h"
#include "state_watching/state_watcher_loading.h"
#include "system/module_factory_system_services.h"

inline auto create_core_configuration() {
    return boost::di::make_injector(
        boost::di::bind<
            i_app_configuration_provider
        ,   i_app_configuration_publisher
        ,   app_configuration_manager
        >()
            .to<app_configuration_manager>().in(boost::di::singleton)
    );
}

inline auto create_core_messaging() {
    return boost::di::make_injector(
        boost::di::bind<IRxImmediateMessageMapper*[]>()
            .to<DmpImmediateMessageMapper>()
    ,   boost::di::bind<IRxRecurrentMessageMapper*[]>()
            .to<DmpRecurrentMessageMapper>()
    ,   boost::di::bind<ThreadWorkerMessaging>()
            .in(boost::di::singleton)
    );
}

inline auto create_core_persisting() {
    return boost::di::make_injector(
        boost::di::bind<i_message_caching_service>()
            .to<blocking_message_caching_service>().in(boost::di::singleton)
    ,   boost::di::bind<runner_core_persisting>()
            .in(boost::di::singleton)
    );
}

inline auto create_module_sensors_gps() {
    return boost::di::make_injector(
        boost::di::bind<i_gps_sensor_publisher>()
            .to<gps_sensor_publisher>().in(boost::di::singleton)
    );
}

inline auto create_module_state_watching() {
    return boost::di::make_injector(
        boost::di::bind<i_state_changed_publisher, state_watcher_motioning>()
            .to<state_watcher_motioning>().in(boost::di::singleton)
    ,   boost::di::bind<i_state_loading_changed_publisher, state_watcher_loading>()
            .to<state_watcher_loading>().in(boost::di::singleton)

    ,   boost::di::bind<
            i_gps_sensor_publisher
        ,   i_scenario_executor
        ,   fake_gps_sensor_publisher
        >()
            .to<fake_gps_sensor_publisher>().in(boost::di::singleton) [boost::di::override]
    ,   boost::di::bind<
            i_lcs_sensor_publisher
        ,   i_scenario_executor
        ,   fake_lcs_sensor_publisher
        >()
            .to<fake_lcs_sensor_publisher>().in(boost::di::singleton) [boost::di::override]
    );
}

inline auto create_composition_root() noexcept {
    return boost::di::make_injector(         
        create_core_configuration()
//    ,   create_core_networking()
//    ,   create_core_messaging()
//    ,   create_core_persisting()
//    ,   create_module_legacy_agtp()
//    ,   create_module_presentation_dmp()
    ,   create_module_sensors_gps()
//    ,   create_module_sensors_serials()
    ,   create_module_system()
//    ,   create_module_state_watching()
    );
};

#endif // COMPOSITIONROOTFACTORY_H
