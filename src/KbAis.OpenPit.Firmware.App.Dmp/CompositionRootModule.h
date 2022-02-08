#ifndef COMPOSITIONROOTFACTORY_H
#define COMPOSITIONROOTFACTORY_H

// oss
#include <boost/di.hpp>

// TODO: Clean up mess
#include "AgtpServiceModuleFactory.h"
#include "Collectors/MessagesCollectorsAdapter.h"
#include "Core/Networking/ModuleFactoryNetworking.h"
#include "Core/Persisting/BlockingMessagesCachingService.h"
#include "Core/Persisting/CachingBootstrapper.h"
#include "Messaging/DmpImmediateMessageMapper.h"
#include "Messaging/DmpRecurrentMessageMapper.h"
#include "SerialRxGpsSensorPublisher.h"
#include "ThreadWorkerMessaging.h"
#include "ViewWrapper.h"

inline auto CompositionRootModule() noexcept {
    return boost::di::make_injector(
        CreateInjectorCoreNetworking()

        // Configure App.Dmp.Presentation
    ,   boost::di::bind<ViewWrapper>()
            .in(boost::di::singleton)
        // Configure Core.Configuration
    ,   boost::di::bind<
            IConfigurationProvider
        ,   IRxConfigurationChangePublisher
        ,   ConfigurationManager
        >()
            .to<ConfigurationManager>()
            .in(boost::di::singleton)
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
    ,   boost::di::bind<CachingBootstrapper>()
            .in(boost::di::singleton)
    );
};

#endif // COMPOSITIONROOTFACTORY_H
