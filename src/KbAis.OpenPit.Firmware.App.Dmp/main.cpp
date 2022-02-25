#define FMT_HEADER_ONLY

// qt
#include <QApplication>
// oss
#include <CLI/CLI.hpp>
#include <fmt/format.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "BoostDiExtensions.h"
#include "CompositionRootModule.h"
#include "ConfigurationsManager.h"
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
                  "mask": "255.255.0.0",
                  "version":"1.0"
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

        configurationManager.registerConfiguration(
            "lls_device"
        ,   R"(
                {
                  "interface":1,
                  "interval_poll":0,
                  "sensors":[
                    { "address":1, "enabled":true, "name":"1" },
                    { "address":1, "enabled":true, "name":"2" },
                    { "address":1, "enabled":true, "name":"3" },
                    { "address":1, "enabled":true, "name":"4" },
                    { "address":1, "enabled":true, "name":"5" },
                    { "address":1, "enabled":true, "name":"6" },
                    { "address":1, "enabled":true, "name":"7" },
                    { "address":1, "enabled":true, "name":"8" }
                  ]
                }
            )"_json
        );

        configurationManager.registerConfiguration(
            "scale"
        ,   R"(
                {"Scales":[
                  {"Divisor":1,"MaxScale":1000,"Percent":20,"Sens":[
                    {"Koeff":1,"Parametrs":0,"Removal":0,"Tar":[
                      {"ADC":0,"Litrs":0},{"ADC":243,"Litrs":150},
                      {"ADC":587,"Litrs":300},{"ADC":1485,"Litrs":700},
                      {"ADC":2272,"Litrs":1050},{"ADC":3274,"Litrs":1500},
                      {"ADC":3968,"Litrs":1800}],"TypeCounter":0,"TypeSens":1,"U_ADC":0},
                    {"Koeff":0,"Parametrs":0,"Removal":0,"Tar":[],"TypeCounter":0,"TypeSens":0,"U_ADC":0},
                    {"Koeff":0,"Parametrs":0,"Removal":0,"Tar":[],"TypeCounter":0,"TypeSens":0,"U_ADC":0},
                    {"Koeff":0,"Parametrs":0,"Removal":0,"Tar":[],"TypeCounter":0,"TypeSens":0,"U_ADC":0}]}],"version":"1.0"
                }
            )"_json
        );
    }
};

using namespace plog;

int main(int argc, char* argv[]) {
    CLI::App commandLine;
    commandLine.allow_extras();

    std::map<std::string, Severity> plogSeverityLevels {
        { "v", Severity::verbose }
    ,   { "d", Severity::debug }
    ,   { "i", Severity::info }
    ,   { "w", Severity::warning }
    ,   { "e", Severity::error }
    ,   { "f", Severity::fatal }
    };

    auto logSeverityLevel = Severity::none;

    commandLine.add_option("-l,--level", logSeverityLevel, "Log severity level")
        -> transform(CLI::CheckedTransformer(plogSeverityLevels, CLI::ignore_case));

    commandLine.callback([&]() {
        static ConsoleAppender<TxtFormatter> consoleAppender;

        init(logSeverityLevel).addAppender(&consoleAppender);
    });

    CLI11_PARSE(commandLine, argc, argv);

    QLocale appDefaultLocale { QLocale::Russian, QLocale::Russia };
    QLocale::setDefault(appDefaultLocale);

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
