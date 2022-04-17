#ifndef CONFIGSCALEREQUESTHANDLER_H
#define CONFIGSCALEREQUESTHANDLER_H

#include "core/configuration/app_configuration_manager.h"
#include "IAgtpRequestHandler.h"

class ConfigScaleRequestHandler : public IAgtpRequestHandler {

    app_configuration_manager& m_configuration_manager;

public:
    ConfigScaleRequestHandler(app_configuration_manager& configuration_manager);

    AgtpResponse handle(const AgtpRequest& request);

    QString get_request_name() const;
};

#endif // CONFIGSCALEREQUESTHANDLER_H
