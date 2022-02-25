#include "ConfigScaleRequestHandler.h"

#include "Handlers/Utils.h"

const QString REQUEST_NAME { QStringLiteral("SCALE_PARAMS") };

ConfigScaleRequestHandler::ConfigScaleRequestHandler(
    ConfigurationManager& configuration_manager
)
    :   m_configuration_manager(configuration_manager)
{

}

AgtpResponse
ConfigScaleRequestHandler::handle(const AgtpRequest& request) {
    const auto config_opt = extract_configuration(request.payload);

    if (config_opt) {
        m_configuration_manager.updateConfiguration("scale", *config_opt);
    }

    const auto config = m_configuration_manager.getConfiguration("scale").j_object;

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
ConfigScaleRequestHandler::getRequestName() const {
    return REQUEST_NAME;
}
