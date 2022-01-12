#include "MessagesCollectorsAdapter.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QVector>
// oss
#include <plog/Log.h>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds TIMER_COLLECT_MESSAGES_INTERVAL { 10s };

MessagesCollectorsAdapter::MessagesCollectorsAdapter(
    //ImmediateMessagesCollector& immediateMessagesCollector,
    RecurrentMessagesCollector& recurrentMessagesCollector/*,
    MessagesBatchesQueue& queue*/
) : QObject(),
    //immediateMessagesCollector { immediateMessagesCollector },
    recurrentMessagesCollector { recurrentMessagesCollector }/*,
    queue { queue }*/ {
//    QTimer timer { this };

//    timer.setInterval(TIMER_COLLECT_MESSAGES_INTERVAL);

    subs = rxcpp::composite_subscription();

    rxcpp::observable<>::interval(TIMER_COLLECT_MESSAGES_INTERVAL, rxcpp::observe_on_event_loop())
        .subscribe(subs, [&](auto x) {
            handleCollectMessages();
            PLOGD << "triggered";
        });

//    connect(
//        &immediateMessagesCollector, &ImmediateMessagesCollector::notifyMessageCollected,

//        this, [&] { threadWorker.execInThread([&] { handleCollectMessages(); }); }
//    );

//    auto loopCollectMessages = threadWorker.startLoopInThread(
//        [&] { handleCollectMessages(); }, TIMER_COLLECT_MESSAGES_INTERVAL.count()
//    );
//    Q_UNUSED(loopCollectMessages)
}

void
MessagesCollectorsAdapter::handleCollectMessages() {
    QMutexLocker lock(&mtxCollectMessagses);

    QVector<Message> messages;
    messages
//        << immediateMessagesCollector.getMessages()
        << recurrentMessagesCollector.getMessages();

    if (messages.isEmpty()) {
        return;
    }

//    auto messagesCollectedAt { QDateTime::currentDateTimeUtc() };

//    auto messagesBatchPlaced = queue.enqueue({ messages, messagesCollectedAt });
//    Q_UNUSED(messagesBatchPlaced)
}
