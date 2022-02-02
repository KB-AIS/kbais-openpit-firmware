#ifndef THREADWORKERNETWORKING_H
#define THREADWORKERNETWORKING_H

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "TcpMessageSendersManager.h"

class ThreadWorkerNetworking {

public:
    ThreadWorkerNetworking(TcpMessageSendersManager& manager);

private:
    rxqt::RunLoopThread m_working_thread;

    TcpMessageSendersManager& m_manager;

};

#endif // THREADWORKERNETWORKING_H
