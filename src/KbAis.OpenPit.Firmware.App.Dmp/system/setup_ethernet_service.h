#ifndef SETUP_ETHERNET_SERVICE_H
#define SETUP_ETHERNET_SERVICE_H

#include "core/configuration/app_configuration_manager.h"

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

    static EthernetConfigurationOpt_t map_ethernet_configuration(const app_configuration& c);

    static void handle_new_ethernet_configuration(const EthernetConfigurationOpt_t& c_opt);

public:
    SetupEthernetService(const i_app_configuration_publisher& app_configuraiton_publisher);

    ~SetupEthernetService();

};

#endif // SETUP_ETHERNET_SERVICE_H
