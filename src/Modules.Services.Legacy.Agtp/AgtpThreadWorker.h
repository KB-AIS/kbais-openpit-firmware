#ifndef AGTPTHREADWORKER_H
#define AGTPTHREADWORKER_H

// Common::ThirdParty
#include <RxQt/RxQt.h>

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
