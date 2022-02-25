#ifndef CONFIGSCALEREQUESTHANDLER_H
#define CONFIGSCALEREQUESTHANDLER_H

#include "ConfigurationsManager.h"
#include "IAgtpRequestHandler.h"

class ConfigScaleRequestHandler : public IAgtpRequestHandler {

    ConfigurationManager& m_configuration_manager;

public:
    ConfigScaleRequestHandler(ConfigurationManager& configuration_manager);

    AgtpResponse handle(const AgtpRequest& request);

    QString getRequestName() const;
};

#endif // CONFIGSCALEREQUESTHANDLER_H
