#ifndef FETCHSUPPORTMODULESCOMMANDHANDLER_H
#define FETCHSUPPORTMODULESCOMMANDHANDLER_H


#include "IAgtpCommandHandler.h"

class FetchSupportModulesCommandHandler : public IAgtpCommandHandler {

public:
    FetchSupportModulesCommandHandler();

    AgtpCommandResult handle(const AgtpCommand& command) override;

};

#endif // FETCHSUPPORTMODULESCOMMANDHANDLER_H
