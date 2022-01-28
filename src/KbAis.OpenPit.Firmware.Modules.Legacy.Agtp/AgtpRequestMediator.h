#ifndef AGTPREQUESTMEDIATOR_H
#define AGTPREQUESTMEDIATOR_H

// std
#include <map>
#include <memory>
#include <set>

#include "Handlers/IAgtpRequestHandler.h"
#include "IAgtpRequestsMediator.h"

class AgtpRequestsMediator : public IAgtpRequestsMediator {

using AgtpCommandHandlers = std::map<QString, std::shared_ptr<IAgtpRequestHandler>>;

public:
    AgtpRequestsMediator(std::set<std::shared_ptr<IAgtpRequestHandler>> handlers);

    AgtpResponse handle(const AgtpRequest& request) const override;

private:
    AgtpCommandHandlers mHandlers;

};

#endif // AGTPREQUESTMEDIATOR_H
