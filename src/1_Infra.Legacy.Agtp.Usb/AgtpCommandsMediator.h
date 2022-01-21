#ifndef AGTPCOMMANDSMEDIATOR_H
#define AGTPCOMMANDSMEDIATOR_H

// std
#include <map>
#include <memory>
#include <set>

#include "Handlers/IAgtpCommandHandler.h"

class AgtpCommandsMediator {

using AgtpCommandHandlers = std::map<QString, std::shared_ptr<IAgtpCommandHandler>>;

public:
    AgtpCommandsMediator(std::set<std::shared_ptr<IAgtpCommandHandler>> handlers);

    AgtpCommandResult handle(const AgtpCommand& command) const;

private:
    AgtpCommandHandlers mHandlers {  };

};

#endif // AGTPCOMMANDSMEDIATOR_H
