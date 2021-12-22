// qt
#include <QApplication>
// oss
#include <boost/di.hpp>
#include <readerwriterqueue/readerwriterqueue.h>
#include <spdlog/spdlog.h>

#include "caching/messages_caching_service.h"
#include "database/configuration/database_configuration.h"
#include "gps_device_controller.h"
#include "host_wrapper.h"
#include "messaging/aux_immediate_messages_map_service.h"
#include "messaging/aux_recurrent_messages_map_service.h"
#include "messaging/collectors/immediate_messages_collector.h"
#include "messaging/collectors/messages_collectors_adapter.h"
#include "messaging/collectors/recurrent_messages_collector.h"
#include "messaging/messages_batch.h"
#include "utils/boost_di_extensions.h"

namespace di = boost::di;

using namespace KbAis::Cfw::Sensors::Gps;

int main(int argc, char* argv[]) {
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^%L%$] [thread %t] %v");
    spdlog::set_level(spdlog::level::trace);

    spdlog::info("Setup AUX application");
    QApplication app(argc, argv);

    Caching::Configuration::configureConnection();

    const auto services = di::make_injector(
        di::bind<BaseGpsDeviceController>()
            .to<SerialPortGpsDeviceController>().in(di::singleton),

        di::bind<HostWrapper>(),

        di::bind<moodycamel::BlockingReaderWriterQueue<MessagesBatch>>().in(di::singleton),

        di::bind<ImmediateMessagesCollector>().in(di::singleton),
        di::bind<RecurrentMessagesCollector>().in(di::singleton),
        di::bind<MessagesCollectorsAdapter>().in(di::singleton),
        di::bind<AuxImmediateMessagesMapService>().in(di::singleton),
        di::bind<AuxRecurrentMessagesMapService>().in(di::singleton),

        di::bind<IMessagesCachingService>()
            .to<MessagesCachingService>().in(di::singleton)
    );

    eagerSingletons(services);

    spdlog::info("Startup AUX application");
    return app.exec();
}
