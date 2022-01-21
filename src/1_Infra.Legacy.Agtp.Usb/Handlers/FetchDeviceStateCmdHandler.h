#ifndef FETCHDEVICESTATECMDHANDLER_H
#define FETCHDEVICESTATECMDHANDLER_H

// cfw::trdparty
#include "RxQt/RxQt.h"

#include "IAgtpCommandHandler.h"
#include "DeviceStateCollector.h"

class FetchDeviceStateCmdHandler : public IAgtpCommandHandler {

public:
    FetchDeviceStateCmdHandler(const DeviceStateCollector& deviceStateCollector);

    QString getCommand() const override;

    AgtpCommandResult handle(const AgtpCommand& command) override;

private:
    const DeviceStateCollector& mDeviceStateCollector;

};

#endif // FETCHDEVICESTATECMDHANDLER_H
