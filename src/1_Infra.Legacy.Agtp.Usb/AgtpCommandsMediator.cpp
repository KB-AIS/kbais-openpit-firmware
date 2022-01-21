#include "AgtpCommandsMediator.h"

#include "Handlers/FetchDeviceInfoCmdHandler.h"
#include "Handlers/FetchDeviceSpecCmdHandler.h"
#include "Handlers/FetchDeviceModulesCmdHandler.h"

AgtpCommandsMediator::AgtpCommandsMediator(
    std::set<std::shared_ptr<IAgtpCommandHandler>> handlers
) {
    std::transform(
        handlers.begin()
    ,   handlers.end()
    ,   std::inserter(mHandlers, mHandlers.end())
    ,   [](auto x) {
            return std::make_pair(x->getCommand(), x);
        }
    );
}

AgtpCommandResult AgtpCommandsMediator::handle(const AgtpCommand& command) const {
    const auto itrFindCmd = mHandlers.find(command.command);

    if (itrFindCmd == mHandlers.end()) {
        // TODO: throw exception on unknown command
        return FetchDeviceSpecCmdHandler{}.handle(command);
    }

    return itrFindCmd->second->handle(command);
}
