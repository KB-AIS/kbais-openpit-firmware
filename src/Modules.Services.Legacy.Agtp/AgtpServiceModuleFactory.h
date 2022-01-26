#ifndef AGTPSERVICEMODULEFACTORY_H
#define AGTPSERVICEMODULEFACTORY_H

// oss
#include <boost/di.hpp>

// cfw::trdparty
#include "RxQt/RxQt.h"

#include "Handlers/ConfigEthernetRequestHandler.h"
#include "Handlers/FetchDeviceInfoRequestHandler.h"
#include "Handlers/FetchDeviceModulesRequestHandler.h"
#include "Handlers/FetchDeviceSpecRequestHandler.h"
#include "Handlers/FetchDeviceStateRequsetHandler.h"
#include "Handlers/IAgtpRequestHandler.h"
#include "AgtpRequestMediator.h"
#include "AgtpThreadWorker.h"
#include "AgtpUsbCommandsReciever.h"
#include "DeviceStateCollector.h"

inline auto createAgtpServiceModule() noexcept {
    return boost::di::make_injector(
        boost::di::bind<AgtpThreadWorker>()
            .in(boost::di::singleton)

    ,   boost::di::bind<IAgtpRequetsReciever>()
            .to<AgtpUsbRequestsReciever>()
            .in(boost::di::singleton)

    ,   boost::di::bind<IAgtpRequestsMediator>
            .to<AgtpRequestsMediator>()

    ,   boost::di::bind<DeviceStateCollector>()
            .in(boost::di::singleton)

    ,   boost::di::bind<IAgtpRequestHandler*[]>()
            .to<
                FetchConfigEthernetHandler
            ,   FetchDeviceInfoRequestHandler
            ,   FetchDeviceModulesRequestHandler
            ,   FetchDeviceSpecRequestHandler
            ,   FetchDeviceStateRequestHandler
            >()
    );
}

#endif // AGTPSERVICEMODULEFACTORY_H
