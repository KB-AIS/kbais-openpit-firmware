#ifndef CONFIGFUELREQUESTHANDLER_H
#define CONFIGFUELREQUESTHANDLER_H

#include "core/configuration/app_configuration_manager.h"
#include "IAgtpRequestHandler.h"

class ConfigFuelRequestHandler : public IAgtpRequestHandler {

    app_configuration_manager& m_configuration_manager;

public:
    ConfigFuelRequestHandler(app_configuration_manager& configuration_manager);

    AgtpResponse handle(const AgtpRequest& request);

    QString get_request_name() const;
};

#endif // CONFIGFUELREQUESTHANDLER_H
