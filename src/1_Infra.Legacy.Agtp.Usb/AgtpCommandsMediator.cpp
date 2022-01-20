#include "AgtpCommandsMediator.h"

#include "Handlers/FetchDeviceInfoCommandHandler.h"
#include "Handlers/FetchFullDeviceInfoCommandHandler.h"
#include "Handlers/FetchSupportModulesCommandHandler.h"

AgtpCommandsMediator::AgtpCommandsMediator() {

}

AgtpCommandResult AgtpCommandsMediator::handle(const AgtpCommand& command) const {
    // TODO: Move to mediator
    if (command.command == "DEVICEINFO=?;") {
        return FetchDeviceInfoCommandHandler{}.handle(command);
    } else if (command.command == "SUPPORT_MODULES=?;") {
        return  FetchSupportModulesCommandHandler{}.handle(command);
    } else {
        return FetchFullDeviceInfoCommandHandler{}.handle(command);
    }
}
