#ifndef CONFIGFUELREQUESTHANDLER_H
#define CONFIGFUELREQUESTHANDLER_H

#include "ConfigurationsManager.h"
#include "IAgtpRequestHandler.h"

class ConfigFuelRequestHandler : public IAgtpRequestHandler {

    ConfigurationManager& m_configuration_manager;

public:
    ConfigFuelRequestHandler(ConfigurationManager& configuration_manager);

    AgtpResponse handle(const AgtpRequest& request);

    QString get_request_name() const;
};

#endif // CONFIGFUELREQUESTHANDLER_H
