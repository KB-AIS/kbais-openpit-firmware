#ifndef FETCHDEVICESTATECMDHANDLER_H
#define FETCHDEVICESTATECMDHANDLER_H

// cfw::trdparty
#include "RxQt/RxQt.h"

#include "IAgtpRequestHandler.h"
#include "DeviceStateCollector.h"

class FetchDeviceStateCmdHandler : public IAgtpRequestHandler {

public:
    FetchDeviceStateCmdHandler(const DeviceStateCollector& deviceStateCollector);

    QString getRequestName() const override;

    AgtpResponse handle(const AgtpRequest& command) override;

private:
    const DeviceStateCollector& mDeviceStateCollector;

};

#endif // FETCHDEVICESTATECMDHANDLER_H
