#ifndef SETUP_ETHERNET_SERVICE_H
#define SETUP_ETHERNET_SERVICE_H

#include "core/configuration/app_configuration_manager.h"

struct ethnet_config {

    std::string dns;

    std::string ip;

    std::string mask;

    std::string gateway;

    bool is_manual_enabled;

};

/*!
 * \brief Сервис записи конфигурации сетевого подключения устройства.
 */
class setup_ethernet_service {

    using ethnet_config_opt_t = std::optional<ethnet_config>;

    rxcpp::composite_subscription subscriptions_;

    static ethnet_config_opt_t map_ethernet_configuration(const app_configuration& config);

    static void handle_new_ethnet_config(const ethnet_config_opt_t& config_opt);

public:
    setup_ethernet_service(const i_app_configuration_publisher& app_configuraiton_publisher);

    ~setup_ethernet_service();

};

#endif // SETUP_ETHERNET_SERVICE_H
