#ifndef CONFIGETHERNETREQUESTHANDLER_H
#define CONFIGETHERNETREQUESTHANDLER_H

#include "ConfigurationsManager.h"

#include "IAgtpRequestHandler.h"

class FetchConfigEthernetHandler : public IAgtpRequestHandler {

public:
    FetchConfigEthernetHandler(ConfigurationManager& configuration_service);

    AgtpResponse handle(const AgtpRequest &request) override;

    QString getRequestName() const override;

private:
    ConfigurationManager& m_configuration_service;

    void updateConfiguration(const QString& configuration_value);

    QString fetchConfiguration() const;
};

#endif // CONFIGETHERNETREQUESTHANDLER_H
