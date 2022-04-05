#ifndef SETUP_ETHERNET_SERVICE_H
#define SETUP_ETHERNET_SERVICE_H

#include "IRxConfigurationChangePublisher.h"

struct EthernetConfiguration {

    std::string dns;

    std::string ip;

    std::string mask;

    std::string gateway;

    bool is_manual_enabled;

};

/*!
 * \brief Сервис записи конфигурации сетевого подключения устройства.
 */
class SetupEthernetService {

    using EthernetConfigurationOpt_t = std::optional<EthernetConfiguration>;

    rxcpp::composite_subscription subscriptions_;

    static EthernetConfigurationOpt_t map_ethernet_configuration(const AppConfiguration& conf);

    static void handle_new_ethernet_configuration(const EthernetConfigurationOpt_t& conf_opt);

public:
    SetupEthernetService(IRxConfigurationChangePublisher& conf_pub);

    ~SetupEthernetService();

};

#endif // SETUP_ETHERNET_SERVICE_H
