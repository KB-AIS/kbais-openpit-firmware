#ifndef COMPOSITIONROOTFACTORY_H
#define COMPOSITIONROOTFACTORY_H

// oss
#include <boost/di.hpp>

// cfw::infra::legacy::agtp::usb
#include "ModuleAgtpService.h"
// cfw::infra::sensors::gps
#include "SerialRxGpsSensorPublisher.h"
// cfw::inter::views::dmp
#include "ViewWrapper.h"
// cfw::infra::eventbus
#include "RxEventBus.h"

#include "Caching/BlockingMessagesCachingService.h"
#include "Messaging/Collectors/MessagesCollectorsAdapter.h"
#include "Messaging/DmpImmediateMessagesMapService.h"
#include "Messaging/DmpRecurrentMessagesMapService.h"
#include "System/Services/SetupDatetimeHandler.h"

class CompositionRootFactory {

public:
    CompositionRootFactory() = delete;

    static inline auto create() {
        return boost::di::make_injector(
            createModuleAgtpService(),
            boost::di::bind<IRxGpsSensorPublisher>()
                .to<SerialRxGpsSensorPublisher>()
                .in(boost::di::singleton),
            boost::di::bind<DmpImmediateMessagesMapService>()
                .in(boost::di::singleton),
            boost::di::bind<DmpRecurrentMessagesMapService>()
                .in(boost::di::singleton),
            boost::di::bind<ImmediateMessagesCollector>()
                .in(boost::di::singleton),
            boost::di::bind<RecurrentMessagesCollector>()
                .in(boost::di::singleton),
            boost::di::bind<MessagesCollectorsAdapter>()
                .in(boost::di::singleton),
            boost::di::bind<IMessagesCachingService>()
                .to<BlockingMessagesCachingService>()
                .in(boost::di::singleton),
            boost::di::bind<RxSetupDateTimeService>()
                .in(boost::di::singleton),
            boost::di::bind<RxEventBus>()
                .in(boost::di::singleton),
            boost::di::bind<ViewWrapper>()
                .in(boost::di::singleton)
        );
    }

};

#endif // COMPOSITIONROOTFACTORY_H
