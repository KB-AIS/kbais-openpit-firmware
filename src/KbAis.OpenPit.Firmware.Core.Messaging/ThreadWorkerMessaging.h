#ifndef THREADWORKERMESSAGING_H
#define THREADWORKERMESSAGING_H

// Utils.TrdPary.RxQt
#include <RxQt.h>

#include "Collectors/MessagesCollectorsAdapter.h"

class ThreadWorkerMessaging {

public:
    ThreadWorkerMessaging(
        ImmediateMessagesCollector& immediateMessagesCollector
    ,   RecurrentMessagesCollector& recurrentMessagesCollector
    ,   MessagesCollectorsAdapter& messagesCollectorsAdapter
    );

private:
    rxqt::RunLoopThread mTrdWorker;

};

#endif // THREADWORKERMESSAGING_H
