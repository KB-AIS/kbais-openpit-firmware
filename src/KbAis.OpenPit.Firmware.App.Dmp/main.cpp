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
        ,   nlohmann::json {
                { "dns", "" }
            ,   { "gateway", "10.214.1.1" }
            ,   { "ip", "10.214.1.205" }
            ,   { "manual_enable", true }
            ,   { "mask", "255.255.0.0" }
            }
        );

        configurationManager.getChangeObservable("ethernet")
           .subscribe([&](Configuration configuration) {
               PLOGD << fmt::format("Got a new configuration: \n{}", configuration.value.dump(4));
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

    PLOGI << "Startup DMP application";
    return app.exec();
}
