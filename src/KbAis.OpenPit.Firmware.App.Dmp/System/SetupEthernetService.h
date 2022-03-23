#ifndef SETUPETHERNETSERVICE_H
#define SETUPETHERNETSERVICE_H

// qt
#include <QMutexLocker>

#include "IRxConfigurationChangePublisher.h"

class SetupEthernetService {

    rxcpp::composite_subscription m_subscriptions;

    QMutex m_mtx_update_connman_profile;

    void handle(const AppConfiguration& new_config);

public:
    SetupEthernetService(IRxConfigurationChangePublisher& service_configs);

    ~SetupEthernetService();

};

#endif // SETUPETHERNETSERVICE_H
