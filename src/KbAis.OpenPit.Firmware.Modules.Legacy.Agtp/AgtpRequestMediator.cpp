#include "AgtpRequestMediator.h"

// qt
#include <QRegularExpression>
// oss
#include <fmt/format.h>
#include <plog/Log.h>
#include <range/v3/all.hpp>

const QRegularExpression RE_REQUEST_NAME_CAPTURE { "([\\w]+)" };

AgtpRequestsMediator::AgtpRequestsMediator(
    std::set<AgtpCommandHandler_t> handlers
) {
    const auto v = ranges::view::transform([](const auto& x) {
        return x->getRequestName();
    });
    m_RequestHandlers = ranges::views::zip(handlers | v, handlers)
        | ranges::to<AgtpCommandHandlers_t>();
}

AgtpResponse
AgtpRequestsMediator::handle(const AgtpRequest& request) const {
    PLOGV << fmt::format("AGTP service got a request: {}", request.payload.toStdString());

    const auto reRequestNameMatch = RE_REQUEST_NAME_CAPTURE.match(request.payload);

    if (!reRequestNameMatch.hasMatch()) {
        throw std::out_of_range("Request has wrong format");
    }

    const auto itrFindHandler = m_RequestHandlers.find(reRequestNameMatch.captured(1));

    if (itrFindHandler == m_RequestHandlers.end()) {
        throw std::out_of_range("Request is not supported");
    }

    return itrFindHandler->second->handle(request);
}
