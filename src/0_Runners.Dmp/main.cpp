// std
#include <memory>
// qt
#include <QApplication>
#include <QMetaType>

#include "CompositionRootFactory.h"
#include "LoggerConfigurator.h"
#include "Persisting/Configuration/DatabaseConfigurator.h"
#include "Utils/BoostDiExtensions.h"

#include <nlohmann/json.hpp>
#include <ConfigurationsManager.h>

using json = nlohmann::json;

struct Test {
    Test(ConfigurationManager& configurationManager) {
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
    }
};

int main(int argc, char* argv[]) {
    LoggerConfigurator::configure();

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    DatabaseConfigurator::configure();

    auto injector = CompositionRootFactory::create();
    eagerSingletons(injector);

    injector.create<std::shared_ptr<Test>>();

    auto configuration_service = injector.create<std::shared_ptr<IRxConfigurationChangePublisher>>();

    configuration_service->getChangeObservable("ethernet")
        .subscribe([&](Configuration configuration) {
            PLOGD << configuration.value.dump(4);
        });

    PLOGI << "Startup DMP application";
    return app.exec();
}
