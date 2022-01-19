#ifndef FETCHSUPPORTMODULESCOMMANDHANDLER_H
#define FETCHSUPPORTMODULESCOMMANDHANDLER_H

#include "IAgtpCommandHandler.h"

class FetchSupportModulesCommandHandler {

public:
    FetchSupportModulesCommandHandler();

    AgtpCommandResult handle(const AgtpCommand& command);

};

#endif // FETCHSUPPORTMODULESCOMMANDHANDLER_H
