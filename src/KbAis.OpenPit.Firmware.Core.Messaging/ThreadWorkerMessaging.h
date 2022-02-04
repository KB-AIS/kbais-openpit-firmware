#ifndef THREADWORKERMESSAGING_H
#define THREADWORKERMESSAGING_H

// Utils.TrdPary.RxQt
#include <RxQt.h>

#include "Collectors/MessagesCollectorsAdapter.h"

class ThreadWorkerMessaging {

public:
    ThreadWorkerMessaging(MessagesCollectorsAdapter& messagesCollectorsAdapter);

private:
    rxqt::RunLoopThread m_workingThread;

};

#endif // THREADWORKERMESSAGING_H
