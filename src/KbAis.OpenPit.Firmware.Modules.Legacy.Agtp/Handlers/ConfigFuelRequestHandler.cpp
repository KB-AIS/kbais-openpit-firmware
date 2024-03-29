#include "ConfigFuelRequestHandler.h"

#include "Handlers/Utils.h"

const QString REQUEST_NAME { QStringLiteral("SCALE_PARAMS") };

ConfigFuelRequestHandler::ConfigFuelRequestHandler(
    app_configuration_manager& configuration_manager
)
    :   m_configuration_manager(configuration_manager)
{

}

AgtpResponse
ConfigFuelRequestHandler::handle(const AgtpRequest& request) {
    const auto config_opt = extract_configuration(request.payload);

    if (config_opt) {
        m_configuration_manager.set_configuration("fuel", *config_opt);
    }

    const auto config = m_configuration_manager.get_configuration("fuel").value;

    return {
        request.uid
    ,   QString("%1:%2")
            .arg(REQUEST_NAME)
            .arg(QString::fromStdString(config.dump()))
            .toUtf8()
    ,   0x00
    };
}

QString
ConfigFuelRequestHandler::get_request_name() const {
    return REQUEST_NAME;
}
