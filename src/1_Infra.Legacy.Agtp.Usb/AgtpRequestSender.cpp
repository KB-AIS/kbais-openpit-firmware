#include "AgtpRequestSender.h"

// oss
#include <pipes/operator.hpp>
#include <pipes/insert.hpp>
#include <pipes/transform.hpp>

AgtpRequestsSender::AgtpRequestsSender(
    std::set<std::shared_ptr<IAgtpRequestHandler>> handlers
) {
    handlers
        >>= pipes::transform([](auto x) {
                return std::make_pair(x->getCommand(), x);
            })
        >>= pipes::insert(mHandlers);
}

AgtpResponse
AgtpRequestsSender::handle(const AgtpRequest& command) const {
    const auto itrFindCmd = mHandlers.find(command.payload);

    if (itrFindCmd == mHandlers.end()) {
        throw std::out_of_range("Command is not supported");
    }

    return itrFindCmd->second->handle(command);
}
