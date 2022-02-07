#ifndef AGTPREQUESTMEDIATOR_H
#define AGTPREQUESTMEDIATOR_H

// std
#include <map>
#include <memory>
#include <set>

#include "Handlers/IAgtpRequestHandler.h"
#include "IAgtpRequestsMediator.h"

class AgtpRequestsMediator : public IAgtpRequestsMediator {

    using AgtpCommandHandler_t = std::shared_ptr<IAgtpRequestHandler>;

    using AgtpCommandHandlers_t = std::map<QString, AgtpCommandHandler_t>;

public:
    AgtpRequestsMediator(std::set<AgtpCommandHandler_t> handlers);

    AgtpResponse handle(const AgtpRequest& request) const override;

private:
    AgtpCommandHandlers_t m_RequestHandlers;

};

#endif // AGTPREQUESTMEDIATOR_H
