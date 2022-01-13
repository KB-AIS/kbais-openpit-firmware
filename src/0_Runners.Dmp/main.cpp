// qt
#include <QApplication>
#include <QMetaType>
// oss
#include <boost/di.hpp>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>
#include <readerwriterqueue/readerwriterqueue.h>

// cfw::infra::sensors::gps
#include "SerialRxGpsSensorPublisher.h"
// cfw::trdparty
#include "RxQt/RxQt.h"
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"

#include "Messaging/Collectors/MessagesCollectorsAdapter.h"
#include "Messaging/DmpImmediateMessagesMapService.h"
#include "Messaging/DmpRecurrentMessagesMapService.h"
#include "Utils/BoostDiExtensions.h"

namespace di = boost::di;

void setupLogging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> consoleAppender;

    init(debug).addAppender(&consoleAppender);
}

int main(int argc, char* argv[]) {
    setupLogging();

    PLOGI << "Setup AUX application";
    QApplication app(argc, argv);

    rxqt::run_loop rxQtRunLoop;

    const auto services = di::make_injector(
        di::bind<DmpImmediateMessagesMapService>()
            .in(di::singleton),
        di::bind<DmpRecurrentMessagesMapService>()
            .in(di::singleton),

        di::bind<MessagesCollectorsAdapter>()
            .in(di::singleton),

        di::bind<IRxGpsSensorPublisher>()
            .to<SerialRxGpsSensorPublisher>()
            .in(di::singleton)
    );

    eagerSingletons(services);

    PLOGI << "Startup AUX application";
    return app.exec();
}
