#ifndef SETUPETHERNETSERVICE_H
#define SETUPETHERNETSERVICE_H

#include "IRxConfigurationChangePublisher.h"

class SetupEthernetService {

public:
    SetupEthernetService(IRxConfigurationChangePublisher& configurationPublisher);

    ~SetupEthernetService();

private:
    rxcpp::composite_subscription m_subsBag;

    void handle(const AppConfiguration& newConfiguration);

};

#endif // SETUPETHERNETSERVICE_H
