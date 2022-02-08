#define FMT_HEADER_ONLY

// qt
#include <QApplication>
// oss
#include <fmt/format.h>

#include "BoostDiExtensions.h"
#include "CompositionRootModule.h"
#include "ConfigurationsManager.h"
#include "ConfiguratorCommandLine.h"
#include "Core/Persisting/Configuration/DatabaseConfigurator.h"

struct ConfigurationBootstraper {
    ConfigurationBootstraper(ConfigurationManager& configurationManager) {
        configurationManager.registerConfiguration(
            "ethernet"
        ,   R"(
                {
                  "dns": "",
                  "gateway": "10.214.1.1",
                  "ip": "10.214.1.205",
                  "manual_enable": true,
                  "mask": "255.255.0.0"
                }
            )"_json
        );

        configurationManager.registerConfiguration(
            "networking"
        ,   R"(
                {
                  "primary_message_server": -1,
                  "servers": [
                    {
                      "enabled": true,
                      "protocol": 0,
                      "reserve_enabled": false,
                      "reserve_server_name": "",
                      "reserve_server_port": 0,
                      "send_interval": 10,
                      "server_name": "10.73.212.191",
                      "server_port": 47653
                    }
                  ],
                  "version": "1.0"
                }
            )"_json
        );
    }
};

int main(int argc, char* argv[]) {
    CLI::App commandLine;

    ConfigureCommandLine(commandLine);

    CLI11_PARSE(commandLine, argc, argv);

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    DatabaseConfigurator::configure();

    auto injector = CompositionRootModule();

    using ConfigurationBootstraperSingleton_t = std::shared_ptr<ConfigurationBootstraper>;
    boost::di::create<ConfigurationBootstraperSingleton_t>(injector);

    eagerSingletons(injector);

    PLOGI << "Startup DMP application";
    return app.exec();
}
