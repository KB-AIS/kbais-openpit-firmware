// std
#include <memory>
// qt
#include <QApplication>
#include <QDateTime>
#include <QHash>
#include <QHostAddress>
#include <QMetaType>
#include <QtConcurrent/QtConcurrent>
#include <QSharedPointer>
// oss
#include <gitleventbus.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>
#include <readerwriterqueue.h>

#include "aux_immediate_messages_mapper.h"
#include "aux_recurrent_messages_mapper.h"
#include "commands/setup_time_handler.h"
#include "database_configuration.h"
#include "device_message.h"
#include "host_wrapper.h"
#include "immediate_messages_collector.h"
#include "messages_caching_service.h"
#include "messages_collectors_adapter.h"
#include "networking/base_protocol_communicator.h"
#include "networking/swom_protocol_communicator.h"
#include "networking/message_sender.h"
#include "networking/message_senders_manager.h"

class ThreadRegistry {
    Q_DISABLE_COPY(ThreadRegistry)

    friend class ThreadRegistration;

    QMutex mutex;

    QList<QThread*> threads;

    static QScopedPointer<ThreadRegistry> m_instance;

    static ThreadRegistry* instance() {
        if (!m_instance) {
            m_instance.reset(new ThreadRegistry);
        }

        return m_instance.data();
    }

public:
    ThreadRegistry() {}

    ~ThreadRegistry() {
        quitThreads();
        waitThreads();
    }

    void quitThreads() {
        QMutexLocker lock(&mutex);

        QList<QThread*> & threads(threads);

        for (auto thread : threads) {
            thread->quit();
            #if QT_VERSION>=QT_VERSION_CHECK(5,2,0)
            thread->requestInterruption();
            #endif
        }
    }

    void waitThreads() {
        forever {
            QMutexLocker lock(&mutex);
            int threadCount = threads.count();
            lock.unlock();

            if (!threadCount) {
                break;
            }

            QThread::yieldCurrentThread();
        }
    }
};

class ThreadRegistration {
    Q_DISABLE_COPY(ThreadRegistration)

public:
    ThreadRegistration() {
        QMutexLocker lock(&ThreadRegistry::instance()->mutex);

        ThreadRegistry::instance()->threads << QThread::currentThread();
    }

    ~ThreadRegistration() {
        QMutexLocker lock(&ThreadRegistry::instance()->mutex);

        ThreadRegistry::instance()->threads.removeAll(QThread::currentThread());
    }
};

using namespace KbAis::Cfw::Networking;

using namespace moodycamel;

void setupLogging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> console_appender;

    init(debug).addAppender(&console_appender);
}

void setupMetaTypes() {
    qMetaTypeId<DeviceMessage>();
}

int main(int argc, char *argv[]) {
    setupLogging();
    setupMetaTypes();

    PLOGI << "Setup AUX application";
    QApplication app(argc, argv);

    Caching::Configuration::configureConnection();

    Sensors::Gps::GpsDeviceController gspController;

    SetupTimeHandler setupTimeHandler;

    HostWrapper host { gspController };

    QObject::connect(
       &gspController, &Sensors::Gps::GpsDeviceController::update_gps_data_signal,

       &setupTimeHandler, &SetupTimeHandler::handle_slot
    );

    RecurrentMessagesCollector recurrentMessagesCollector;

    AuxRecurrentEventMapper auxRecurrentEventMapper {
        recurrentMessagesCollector,
        gspController
    };

    ImmediateMessagesCollector immediateMessagesCollector { };

    AuxImmediateMessagesMapper auxImmediateEventMapper {
        immediateMessagesCollector, &host
    };

    BlockingReaderWriterQueue<DeviceMessage> msgsQueue;

    MessagesCollectorsAdapter msgsCollectorsAdapter {
        recurrentMessagesCollector,
        immediateMessagesCollector,
        msgsQueue
    };

    MessagesCachingService msgsCachingService { msgsQueue };

    MessageSendersManager msgSendersManager;
    QThread wtMsgSendersManager;

    msgSendersManager.moveToThread(&wtMsgSendersManager);

    QObject::connect(
        &wtMsgSendersManager, &QThread::started,

        &msgSendersManager, [&] {
            QList<MessageSenderConfiguration> configurations {
                {
                    "10.214.1.208",
                    9900,
                    std::chrono::milliseconds { 10000 },
                    QSharedPointer<SwomProtocolCommunicator>::create()
                }
            };

            msgSendersManager.handleConfigurationChanged(configurations);
        }
    );

    QObject::connect(
        &wtMsgSendersManager, &QThread::finished,

        &wtMsgSendersManager, &QThread::deleteLater
    );

    wtMsgSendersManager.start();

    PLOGI << "Startup AUX application";
    return app.exec();
}
