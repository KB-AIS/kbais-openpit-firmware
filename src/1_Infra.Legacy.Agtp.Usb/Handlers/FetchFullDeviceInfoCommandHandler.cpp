#include "FetchFullDeviceInfoCommandHandler.h"

FetchFullDeviceInfoCommandHandler::FetchFullDeviceInfoCommandHandler() {

}

AgtpCommandResult
FetchFullDeviceInfoCommandHandler::handle(const AgtpCommand& command) {
    AgtpCommandResult result;

    result.uid = command.uid;

    return result;
}
