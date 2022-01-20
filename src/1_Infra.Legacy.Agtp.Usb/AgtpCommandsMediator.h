#ifndef AGTPCOMMANDSMEDIATOR_H
#define AGTPCOMMANDSMEDIATOR_H


#include "Handlers/IAgtpCommandHandler.h"

class AgtpCommandsMediator {

public:
    AgtpCommandsMediator();

    AgtpCommandResult handle(const AgtpCommand& command) const;

};

#endif // AGTPCOMMANDSMEDIATOR_H
