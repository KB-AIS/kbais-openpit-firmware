#ifndef FETCHFULLDEVICEINFOCOMMANDHANDLER_H
#define FETCHFULLDEVICEINFOCOMMANDHANDLER_H

#include "IAgtpCommandHandler.h"

class FetchFullDeviceInfoCommandHandler {

public:
    FetchFullDeviceInfoCommandHandler();

    AgtpCommandResult handle(const AgtpCommand &command);

};

#endif // FETCHFULLDEVICEINFOCOMMANDHANDLER_H
