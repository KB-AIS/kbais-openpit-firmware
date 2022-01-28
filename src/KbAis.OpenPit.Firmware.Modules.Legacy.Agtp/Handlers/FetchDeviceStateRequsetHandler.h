#ifndef FETCHDEVICESTATEREQUSETHANDLER_H
#define FETCHDEVICESTATEREQUSETHANDLER_H

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "IAgtpRequestHandler.h"
#include "DeviceStateCollector.h"

class FetchDeviceStateRequestHandler : public IAgtpRequestHandler {

public:
    FetchDeviceStateRequestHandler(const DeviceStateCollector& deviceStateCollector);

    QString getRequestName() const override;

    AgtpResponse handle(const AgtpRequest& command) override;

private:
    const DeviceStateCollector& mDeviceStateCollector;

};

#endif // FETCHDEVICESTATEREQUSETHANDLER_H
