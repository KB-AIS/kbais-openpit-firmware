#include "ConfigScaleRequestHandler.h"

#include "Handlers/Utils.h"

const QString REQUEST_NAME { QStringLiteral("SCALE_PARAMS") };

ConfigScaleRequestHandler::ConfigScaleRequestHandler(
    app_configuration_manager& configuration_manager
)
    :   m_configuration_manager(configuration_manager)
{

}

AgtpResponse
ConfigScaleRequestHandler::handle(const AgtpRequest& request) {
    const auto config_opt = extract_configuration(request.payload);

    if (config_opt) {
        m_configuration_manager.set_configuration("scale", *config_opt);
    }

    const auto config = m_configuration_manager.get_configuration("scale").value;

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
ConfigScaleRequestHandler::get_request_name() const {
    return REQUEST_NAME;
}
