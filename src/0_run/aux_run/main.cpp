// qt
#include <QApplication>
// oss
#include <boost/di.hpp>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>
#include <readerwriterqueue/readerwriterqueue.h>

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

namespace di = boost::di;

using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

using namespace KbAis::Cfw::Sensors::Gps;

template <class TDependency, class TInjector,
          std::enable_if_t<std::is_same<typename TDependency::scope, di::scopes::singleton>::value, int> = 0>
void create_singletons_eagerly_impl(const di::aux::type<TDependency>&, const TInjector& injector) {
    injector.template create<std::shared_ptr<typename TDependency::expected>>();
}

template <class TDependency, class TInjector,
          std::enable_if_t<!std::is_same<typename TDependency::scope, di::scopes::singleton>::value, int> = 0>
void create_singletons_eagerly_impl(const di::aux::type<TDependency>&, const TInjector&) { }

template <class... TDeps, class TInjector>
void create_singletons_eagerly(const di::aux::type_list<TDeps...>&, const TInjector& injector) {
    [](...) { }((create_singletons_eagerly_impl(di::aux::type<TDeps>{}, injector), 0)...);
}

template <class TInjector>
void eager_singletons(const TInjector& injector) {
    create_singletons_eagerly(typename TInjector::deps{}, injector);
}

void setupLogging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> console_appender;

    init(verbose).addAppender(&console_appender);
}

int main(int argc, char* argv[]) {
    setupLogging();

    PLOGI << "Setup AUX application";
    QApplication app(argc, argv);

    Caching::Configuration::configureConnection();

    const auto services = boost::di::make_injector(
        di::bind<BaseGpsDeviceController>()
            .to<SerialPortGpsDeviceController>().in(di::singleton),

        di::bind<HostWrapper>(),

        di::bind<MessagesBatchesQueue>().in(di::singleton),

        di::bind<ImmediateMessagesCollector>().in(di::singleton),
        di::bind<RecurrentMessagesCollector>().in(di::singleton),
        di::bind<MessagesCollectorsAdapter>().in(di::singleton),

        di::bind<AuxImmediateMessagesMapService>().in(di::singleton),
        di::bind<AuxRecurrentMessagesMapService>().in(di::singleton),

        di::bind<IMessagesCachingService>()
            .to<MessagesCachingService>().in(di::singleton)
    );

    eager_singletons(services);

    PLOGI << "Startup AUX application";
    return app.exec();
}
