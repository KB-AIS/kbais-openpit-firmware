#ifndef COMPOSITIONROOTFACTORY_H
#define COMPOSITIONROOTFACTORY_H

// oss
#include <boost/di.hpp>

// App.Dmp.Presentation
#include "ViewWrapper.h"
// Core.Messaging
#include "Collectors/MessagesCollectorsAdapter.h"
#include "ThreadWorkerMessaging.h"
#include "ModuleFactoryNetworking.h"
// Core.Persisiting
#include "BlockingMessagesCachingService.h"
// Modules.Legacy.Agtp
#include "AgtpServiceModuleFactory.h"
// Modules.Sensors.Gps
#include "SerialRxGpsSensorPublisher.h"

#include "Messaging/DmpImmediateMessageMapper.h"
#include "Messaging/DmpRecurrentMessageMapper.h"

class InjectorFactory {

public:
    InjectorFactory() = delete;

    static inline auto create() {
        return boost::di::make_injector(
            // Configure App.Dmp.Presentation
            boost::di::bind<ViewWrapper>()
                .in(boost::di::singleton)
            // Configure Core.Configuration
        ,   boost::di::bind<
                IConfigurationProvider
            ,   IRxConfigurationChangePublisher
            ,   ConfigurationManager
            >()
                .to<ConfigurationManager>()
                .in(boost::di::singleton)
        ,   create_di_module_networking()
        ,   createAgtpServiceModule()
            // Configure Modules.Sensors.Gps
        ,   boost::di::bind<IRxGpsSensorPublisher>()
                .to<SerialRxGpsSensorPublisher>()
                .in(boost::di::singleton)
            // Configure Core.Messaging
        ,   boost::di::bind<IRxImmediateMessageMapper*[]>
                .to<DmpImmediateMessageMapper>()
        ,   boost::di::bind<IRxRecurrentMessageMapper*[]>
                .to<DmpRecurrentMessageMapper>()
        ,   boost::di::bind<ThreadWorkerMessaging>()
                .in(boost::di::singleton)
            // Configure Core.Persisting
        ,   boost::di::bind<IMessagesCachingService>()
                .to<BlockingMessagesCachingService>()
                .in(boost::di::singleton)
        );
    }

};

#endif // COMPOSITIONROOTFACTORY_H
