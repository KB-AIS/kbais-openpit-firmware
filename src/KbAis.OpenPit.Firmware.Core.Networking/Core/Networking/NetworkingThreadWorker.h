#ifndef NETWORKINGTHREADWORKER_H
#define NETWORKINGTHREADWORKER_H

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "Core/Networking/MessageSenders/TcpMessageSendersManager.h"

class NetworkingThreadWorker {

public:
    NetworkingThreadWorker(TcpMessageSendersManager& manager);

private:
    rxqt::RunLoopThread m_working_thread;

    TcpMessageSendersManager& m_manager;

};

#endif // NETWORKINGTHREADWORKER_H
