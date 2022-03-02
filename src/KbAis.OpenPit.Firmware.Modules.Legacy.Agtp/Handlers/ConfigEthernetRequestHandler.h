#ifndef CONFIGETHERNETREQUESTHANDLER_H
#define CONFIGETHERNETREQUESTHANDLER_H

// Core.Configuration
#include "ConfigurationsManager.h"
#include <range/v3/view/transform.hpp>

#include "IAgtpRequestHandler.h"

class ConfigEthernetRequestHandler : public IAgtpRequestHandler {

public:
    ConfigEthernetRequestHandler(ConfigurationManager& configurationService);

    AgtpResponse handle(const AgtpRequest& request) override;

    QString get_request_name() const override;

private:
    ConfigurationManager& mConfigurationService;

    void updateConfiguration(const QString& configurationValue);

    QString fetchConfiguration() const;
};

#endif // CONFIGETHERNETREQUESTHANDLER_H
