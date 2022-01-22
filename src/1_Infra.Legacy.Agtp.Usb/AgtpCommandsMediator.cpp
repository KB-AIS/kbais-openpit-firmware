#include "AgtpCommandsMediator.h"

// oss
#include <pipes/operator.hpp>
#include <pipes/insert.hpp>
#include <pipes/transform.hpp>

#include "Handlers/FetchDeviceInfoCmdHandler.h"
#include "Handlers/FetchDeviceSpecCmdHandler.h"
#include "Handlers/FetchDeviceModulesCmdHandler.h"

AgtpCommandsMediator::AgtpCommandsMediator(
    std::set<std::shared_ptr<IAgtpCommandHandler>> handlers
) {
    // Map
    handlers
        >>= pipes::transform([](auto x) { return std::make_pair(x->getCommand(), x); })
        >>= pipes::insert(mHandlers);
}

AgtpCommandResult
AgtpCommandsMediator::handle(const AgtpCommand& command) const {
    const auto itrFindCmd = mHandlers.find(command.command);

    if (itrFindCmd == mHandlers.end()) {
        // TODO: throw exception on unknown command
        return FetchDeviceSpecCmdHandler{}.handle(command);
    }

    return itrFindCmd->second->handle(command);
}
