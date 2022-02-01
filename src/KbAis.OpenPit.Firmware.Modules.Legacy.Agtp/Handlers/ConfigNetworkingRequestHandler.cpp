#include "ConfigNetworkingRequestHandler.h"

#include "Handlers/Utils.h"

const QString REQUEST_NAME { QStringLiteral("SERVER_PARAM") };

ConfigNetworkingRequestHandler::ConfigNetworkingRequestHandler(
    ConfigurationManager& configurationProvider
)
    : mConfigurationManager(configurationProvider)
{

}

AgtpResponse
ConfigNetworkingRequestHandler::handle(const AgtpRequest& request) {
    const auto opt_configuration = extract_configuration(request.payload);

    if (opt_configuration.has_value()) {
        mConfigurationManager.updateConfiguration("networking", *opt_configuration);
    }

    const auto configuration = mConfigurationManager.getConfiguration("networking").value;

    return {
        request.uid
    ,   QString("%1:%2")
            .arg(REQUEST_NAME)
            .arg(QString::fromStdString(configuration.dump()))
            .toUtf8()
    ,   0x00
    };
}

QString
ConfigNetworkingRequestHandler::getRequestName() const {
    return REQUEST_NAME;
}
