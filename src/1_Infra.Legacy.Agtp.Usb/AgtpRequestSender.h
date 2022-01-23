#ifndef AGTPREQUESTSENDER_H
#define AGTPREQUESTSENDER_H

// std
#include <map>
#include <memory>
#include <set>

#include "Handlers/IAgtpRequestHandler.h"
#include "IAgtpCommandsMediator.h"

class AgtpRequestsSender : public IAgtpCommandsMediator {

using AgtpCommandHandlers = std::map<QString, std::shared_ptr<IAgtpRequestHandler>>;

public:
    AgtpRequestsSender(std::set<std::shared_ptr<IAgtpRequestHandler>> handlers);

    AgtpResponse handle(const AgtpRequest& command) const override;

private:
    AgtpCommandHandlers mHandlers;

};

#endif // AGTPREQUESTSENDER_H
