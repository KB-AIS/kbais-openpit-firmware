#ifndef FETCHDEVICEINFOREQUESTHANDLER_H
#define FETCHDEVICEINFOREQUESTHANDLER_H

#include "Handlers/IAgtpRequestHandler.h"

struct DeviceInfo {

    unsigned int serial;

    unsigned int sizeRecvBuff;

    unsigned char version;

    unsigned char imei[15];

    unsigned char imsiSim1[16];

    unsigned char imsiSim2[16];

    unsigned char iccidSim1[20];

    unsigned char iccidSim2[20];

    unsigned char versionFw[16];

    unsigned char versionBootloader[16];

    unsigned int boardVersion;

    unsigned int executionVersion;

    unsigned char reserve[120];

};

class FetchDeviceInfoRequestHandler : public IAgtpRequestHandler {

public:
    FetchDeviceInfoRequestHandler();

    AgtpResponse handle(const AgtpRequest &command) override;

    QString getRequestName() const override;

private:
    DeviceInfo mDeviceInfo;

};

#endif // FETCHDEVICEINFOREQUESTHANDLER_H
