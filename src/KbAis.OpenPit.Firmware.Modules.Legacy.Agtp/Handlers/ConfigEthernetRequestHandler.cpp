#include "ConfigEthernetRequestHandler.h"

// qt
#include <QRegularExpression>
// oss
#include <nlohmann/json.hpp>
#include <plog/Log.h>

using json = nlohmann::json;

const QString REQUEST_NAME { QStringLiteral("ETHERNET_SETTINGS") };

const QRegularExpression REQUEST_PARAM_CAPTURE { "([\\w]+):(.*)" };

ConfigEthernetRequestHandler::ConfigEthernetRequestHandler(
    app_configuration_manager& configuration_service
)
    :   mConfigurationService { configuration_service }
{

}

AgtpResponse
ConfigEthernetRequestHandler::handle(const AgtpRequest& request) {
    const auto reRequestParamMatch = REQUEST_PARAM_CAPTURE.match(request.payload);

    const auto configuration_value = reRequestParamMatch.captured(2);
    if (configuration_value != "?") {
        updateConfiguration(configuration_value);
    }

    return {
        request.uid
    ,   QString("%1:%2").arg(REQUEST_NAME).arg(fetchConfiguration()).toUtf8()
    ,   0x00
    };
}

QString
ConfigEthernetRequestHandler::get_request_name() const {
    return REQUEST_NAME;
}

void
ConfigEthernetRequestHandler::updateConfiguration(const QString& configuration_value) {
    auto payload = nlohmann::json::parse(configuration_value.toStdString().c_str());

    mConfigurationService.set_configuration("ethernet", payload);
}

QString ConfigEthernetRequestHandler::fetchConfiguration() const {
    auto const configuration = mConfigurationService.get_configuration("ethernet");

    return QString::fromStdString(configuration.value.dump());
}
