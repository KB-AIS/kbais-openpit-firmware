#ifndef CONFIGNETWORKINGREQUESTHANDLER_H
#define CONFIGNETWORKINGREQUESTHANDLER_H

// Core.Configuration
#include "ConfigurationsManager.h"

#include "IAgtpRequestHandler.h"

class ConfigNetworkingRequestHandler : public IAgtpRequestHandler {

public:
    ConfigNetworkingRequestHandler(ConfigurationManager& configurationProvider);

    AgtpResponse handle(const AgtpRequest &request);

    QString getRequestName() const;

private:
    ConfigurationManager& mConfigurationManager;
};

#endif // CONFIGNETWORKINGREQUESTHANDLER_H