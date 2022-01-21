#ifndef MODULEAGTPSERVICE_H
#define MODULEAGTPSERVICE_H

// oss
#include <boost/di.hpp>

// cfw::trdparty
#include "RxQt/RxQt.h"

#include "Handlers/FetchDeviceInfoCmdHandler.h"
#include "Handlers/FetchDeviceModulesCmdHandler.h"
#include "Handlers/FetchDeviceSpecCmdHandler.h"
#include "Handlers/FetchDeviceStateCmdHandler.h"
#include "Handlers/IAgtpCommandHandler.h"
#include "AgtpUsbCommandsReciever.h"
#include "DeviceStateCollector.h"

//using InjectorAgtpModule = boost::di::injector<
//    AgtpUsbCommandsReciever*
//>;

inline auto createModuleAgtpService() noexcept {
    return boost::di::make_injector(
        boost::di::bind<DeviceStateCollector>().in(boost::di::singleton),
        boost::di::bind<AgtpUsbCommandsReciever>(),
        boost::di::bind<IAgtpCommandHandler*[]>().to<
            FetchDeviceInfoCmdHandler,
            FetchDeviceModulesCmdHandler,
            FetchDeviceSpecCmdHandler,
            FetchDeviceStateCmdHandler
        >()
    );
}

#endif // MODULEAGTPSERVICE_H
