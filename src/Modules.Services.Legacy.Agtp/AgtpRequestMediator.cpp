#include "AgtpRequestMediator.h"

// oss
#include <pipes/operator.hpp>
#include <pipes/insert.hpp>
#include <pipes/transform.hpp>
#include <plog/Log.h>
#include <fmt/core.h>

AgtpRequestsMediator::AgtpRequestsMediator(
    std::set<std::shared_ptr<IAgtpRequestHandler>> handlers
) {
    handlers
        >>= pipes::transform([](auto x) {
                return std::make_pair(x->getRequestName(), x);
            })
        >>= pipes::insert(mHandlers);
}

AgtpResponse
AgtpRequestsMediator::handle(const AgtpRequest& request) const {
    const auto itrFindHandler = mHandlers.find(request.payload);

    PLOGD << fmt::format("AGTP Service got a request: {}", request.payload.toStdString());

    if (itrFindHandler == mHandlers.end()) {
        throw std::out_of_range("Request is not supported");
    }

    return itrFindHandler->second->handle(request);
}
