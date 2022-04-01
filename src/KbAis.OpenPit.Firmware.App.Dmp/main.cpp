#define FMT_HEADER_ONLY

// qt
#include <QApplication>
// oss
#include <plog/Log.h>

#include "Core/Persisting/Configuration/DatabaseConfigurator.h"
#include "boost_di_extensions.h"
#include "composition_root_module.h"
#include "conifgurator_cli_processor.h"
#include "dmp_configuration_initializer.h"
#include "state_watcher_fuelling.h"
#include "state_watcher_motioning.h"

QApplication create_application() {
    int                app_argc { 3 };
    std::vector<char*> app_argv { (char*)"opf", (char*)"-platform", (char*)"linuxfb", nullptr };
    return { app_argc, app_argv.data() };
}

int main(int argc, char* argv[]) {
    CliProcessor_t cli_processor { "OpenPit Firmware", "OPF" };

    configure_cli_processor(cli_processor);

    CLI11_PARSE(cli_processor, argc, argv);

    {
        QLocale appDefaultLocale { QLocale::Russian, QLocale::Russia };
        QLocale::setDefault(appDefaultLocale);
    }

    PLOGI << "Setup application";
    const auto application { create_application() };

    {
        DatabaseConfigurator::configure();
    }

    {
        const auto injector { composition_root_module() };
        // Конфиги сервисов должны быть проинициализированы до самих сервисов
        boost::di::create<std::shared_ptr<DmpConfigurationInitializer>>(injector);
        // Инициализация всех остальных сервисов приложения
        create_singletons(injector);
    }

    PLOGI << "Startup application";
    return application.exec();
}
