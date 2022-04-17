#ifndef CONFIGETHERNETREQUESTHANDLER_H
#define CONFIGETHERNETREQUESTHANDLER_H

// Core.Configuration
#include "core/configuration/app_configuration_manager.h"

#include "IAgtpRequestHandler.h"

class ConfigEthernetRequestHandler : public IAgtpRequestHandler {

public:
    ConfigEthernetRequestHandler(app_configuration_manager& configurationService);

    AgtpResponse handle(const AgtpRequest& request) override;

    QString get_request_name() const override;

private:
    app_configuration_manager& mConfigurationService;

    void updateConfiguration(const QString& configurationValue);

    QString fetchConfiguration() const;
};

#endif // CONFIGETHERNETREQUESTHANDLER_H
