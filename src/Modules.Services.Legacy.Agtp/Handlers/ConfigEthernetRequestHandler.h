#ifndef CONFIGETHERNETREQUESTHANDLER_H
#define CONFIGETHERNETREQUESTHANDLER_H

#include "ConfigurationService.h"

#include "IAgtpRequestHandler.h"

class FetchConfigEthernetHandler : public IAgtpRequestHandler {

public:
    FetchConfigEthernetHandler(ConfigurationService& configuration_service);

    AgtpResponse handle(const AgtpRequest &request) override;

    QString getRequestName() const override;

private:
    ConfigurationService& m_configuration_service;

    void updateConfiguration(const QString& configuration_value);

    QString fetchConfiguration() const;
};

#endif // CONFIGETHERNETREQUESTHANDLER_H
