#define FMT_HEADER_ONLY

// qt
#include <QtWidgets/QApplication>
// oss
#include <plog/Log.h>

#include "app_configuration_initializer_dmp.h"
#include "boost_di_extensions.h"
#include "composition_root_factory.h"
#include "conifgurator_cli_processor.h"
#include "state_watching/shift_controller.h"

QApplication create_application() {
    int                app_argc { 3 };
    std::vector<char*> app_argv { (char*)"opf", (char*)"-platform", (char*)"linuxfb", nullptr };

    return { app_argc, app_argv.data() };
}

int main(int argc, char* argv[]) {
    cli_processor_t cli_processor { "OpenPit Firmware: a program to fetch equipment telemetry data", "opf" };
    configure_cli_processor(cli_processor);

    CLI11_PARSE(cli_processor, argc, argv);

    {
        QLocale appDefaultLocale { QLocale::Russian, QLocale::Russia };
        QLocale::setDefault(appDefaultLocale);
    }

    PLOGI << "Setup application";
    const auto application { create_application() };
    QApplication::setApplicationName("opf");

    {
        const auto injector = boost::di::make_injector(create_composition_root());

        boost::di::create<std::shared_ptr<app_configuration_initializer_dmp>>(injector);

        const auto s = rxcpp::observe_on_new_thread().create_coordinator().get_scheduler();
        const auto t = rxcpp::observe_on_one_worker(s);

        {
            auto gps_sensor_publisher = injector.create<std::shared_ptr<fake_gps_sensor_publisher>>();
            gps_sensor_publisher->setup_scenario(t);

            const auto shift_controller_ = injector.create<std::shared_ptr<shift_controller>>();
            shift_controller_->start_working_on(t);

            gps_sensor_publisher->start_scenario();
        }

        create_singletons(injector);
    }

    PLOGI << "Startup application";
    return application.exec();
}
