#include "ConfigEthernetRequestHandler.h"

// qt
#include <QRegularExpression>
// oss
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <plog/Log.h>

using json = nlohmann::json;

const QString REQUEST_NAME { QStringLiteral("ETHERNET_SETTINGS") };

const QRegularExpression RE_REQUEST_PARAM_CAPTURE { "([\\w]+):(.*)" };

FetchConfigEthernetHandler::FetchConfigEthernetHandler(
    ConfigurationManager& configuration_service
)
    :   m_configuration_service { configuration_service }
{

}

AgtpResponse
FetchConfigEthernetHandler::handle(const AgtpRequest& request) {
    const auto reRequestParamMatch = RE_REQUEST_PARAM_CAPTURE.match(request.payload);

    const auto configuration_value = reRequestParamMatch.captured(2);
    if (configuration_value != "?") {
        PLOGD << configuration_value;
        updateConfiguration(configuration_value);
    }

    return {
        request.uid
    ,   QString("%1:%2").arg(REQUEST_NAME).arg(fetchConfiguration()).toUtf8()
    ,   0x00
    };
}

QString
FetchConfigEthernetHandler::getRequestName() const {
    return REQUEST_NAME;
}

void
FetchConfigEthernetHandler::updateConfiguration(const QString& configuration_value) {
    auto payload = nlohmann::json::parse(configuration_value.toStdString().c_str());

    PLOGD << payload.dump(4);

    m_configuration_service.updateConfiguration("ethernet", payload);
}

QString FetchConfigEthernetHandler::fetchConfiguration() const {
    auto const configuration = m_configuration_service.getConfiguration("ethernet");

    return QString::fromStdString(configuration.value.dump());
}
