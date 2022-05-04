#ifndef MODULEBOOTSTRAPERNETWORKING_H
#define MODULEBOOTSTRAPERNETWORKING_H

// Utils.TrdParty.RxQt
#include "rxqt.h"

#include "Core/Networking/MessageSenders/IMessageSendersManager.h"

class ModuleBootstraperNetworking {

public:
    ModuleBootstraperNetworking(IMessageSendersManager& manager);

private:
    rxqt::RunLoopThread m_thrWorker;

    IMessageSendersManager& m_manager;

};

#endif // MODULEBOOTSTRAPERNETWORKING_H
