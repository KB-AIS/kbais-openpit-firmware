// qt
#include <QApplication>
// oss
#include <fmt/core.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

// Core.Persisting
#include "Configuration/DatabaseConfigurator.h"
// Utils.TrdParty.BoostDi
#include "BoostDiExtensions.h"

#include "InjectorFactory.h"

void
configureLogging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> consoleAppender;

    init(verbose).addAppender(&consoleAppender);
}

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

        configurationManager.getChangeObservable("ethernet")
           .subscribe([&](AppConfiguration configuration) {
               PLOGD << fmt::format("Got a new configuration: \n{}", configuration.j_value.dump(4));
           });
    }
};

int main(int argc, char* argv[]) {
    configureLogging();

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    DatabaseConfigurator::configure();

    auto injector = InjectorFactory::create();
    eagerSingletons(injector);

    using ConfigurationBootstraperSingleton_t = std::shared_ptr<ConfigurationBootstraper>;
    boost::di::create<ConfigurationBootstraperSingleton_t>(injector);

    boost::di::create<std::shared_ptr<TcpMessageSendersManager>>(injector);

    PLOGI << "Startup DMP application";
    return app.exec();
}
