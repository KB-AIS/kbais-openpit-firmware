#ifndef MODULEAGTPSERVICE_H
#define MODULEAGTPSERVICE_H


// oss
#include <boost/di.hpp>

#include "Handlers/FetchDeviceInfoCommandHandler.h"
#include "Handlers/FetchFullDeviceInfoCommandHandler.h"
#include "Handlers/FetchSupportModulesCommandHandler.h"
#include "Handlers/IAgtpCommandHandler.h"
#include "AgtpUsbCommandsReciever.h"

using InjectorAgtpModule = boost::di::injector<AgtpUsbCommandsReciever*>;

inline InjectorAgtpModule createModuleAgtpService() noexcept {
    return boost::di::make_injector(
        boost::di::bind<AgtpUsbCommandsReciever>()
    ,   boost::di::bind<IAgtpCommandHandler*[]>().to<
            FetchDeviceInfoCommandHandler
        ,   FetchFullDeviceInfoCommandHandler
        ,   FetchSupportModulesCommandHandler
        >()
    );
}

#endif // MODULEAGTPSERVICE_H
