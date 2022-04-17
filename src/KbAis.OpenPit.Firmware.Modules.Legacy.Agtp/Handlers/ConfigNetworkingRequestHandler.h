#ifndef CONFIGNETWORKINGREQUESTHANDLER_H
#define CONFIGNETWORKINGREQUESTHANDLER_H

// Core.Configuration
#include "core/configuration/app_configuration_manager.h"

#include "IAgtpRequestHandler.h"

class ConfigNetworkingRequestHandler : public IAgtpRequestHandler {

public:
    ConfigNetworkingRequestHandler(app_configuration_manager& configurationProvider);

    AgtpResponse handle(const AgtpRequest &request);

    QString get_request_name() const;

private:
    app_configuration_manager& mConfigurationManager;
};

#endif // CONFIGNETWORKINGREQUESTHANDLER_H
