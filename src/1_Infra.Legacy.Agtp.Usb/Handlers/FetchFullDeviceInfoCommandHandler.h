#ifndef FETCHFULLDEVICEINFOCOMMANDHANDLER_H
#define FETCHFULLDEVICEINFOCOMMANDHANDLER_H

#include "IAgtpCommandHandler.h"

class FetchFullDeviceInfoCommandHandler : public IAgtpCommandHandler {

public:
    FetchFullDeviceInfoCommandHandler();

    AgtpCommandResult handle(const AgtpCommand &command) override;

};

#endif // FETCHFULLDEVICEINFOCOMMANDHANDLER_H
