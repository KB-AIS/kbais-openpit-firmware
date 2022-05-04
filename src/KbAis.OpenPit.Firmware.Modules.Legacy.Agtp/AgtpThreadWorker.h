#ifndef AGTPTHREADWORKER_H
#define AGTPTHREADWORKER_H

// Utils.TrdParty.RxQt
#include <rxqt.h>

#include "DeviceStateCollector.h"
#include "IAgtpRequestsReciever.h"

class AgtpThreadWorker {

public:
    AgtpThreadWorker(
        IAgtpRequetsReciever& requestsReciever
    ,   DeviceStateCollector& deviceStateCollector
    );

private:
    IAgtpRequetsReciever& mRequestsReciever;

    DeviceStateCollector& mDeviceStateCollector;

    rxqt::RunLoopThread mWorker;

};

#endif // AGTPTHREADWORKER_H
