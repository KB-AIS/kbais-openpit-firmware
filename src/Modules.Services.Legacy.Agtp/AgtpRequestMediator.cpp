#include "AgtpRequestMediator.h"

// qt
#include <QRegularExpression>
// oss
#include <pipes/operator.hpp>
#include <pipes/insert.hpp>
#include <pipes/transform.hpp>
#include <plog/Log.h>
#include <fmt/core.h>

const QRegularExpression RE_REQUEST_NAME_CAPTURE { "([\\w]+)" };

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
    PLOGD << fmt::format("AGTP service got a request: {}", request.payload.toStdString());

    const auto reRequestNameMatch = RE_REQUEST_NAME_CAPTURE.match(request.payload);

    if (!reRequestNameMatch.hasMatch()) {
        throw std::out_of_range("Request has wrong format");
    }

    const auto itrFindHandler = mHandlers.find(reRequestNameMatch.captured(1));

    if (itrFindHandler == mHandlers.end()) {
        throw std::out_of_range("Request is not supported");
    }

    return itrFindHandler->second->handle(request);
}
