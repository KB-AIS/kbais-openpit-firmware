#ifndef FETCHDEVICEINFOCOMMANDHANDLER_H
#define FETCHDEVICEINFOCOMMANDHANDLER_H

#include "Handlers/IAgtpCommandHandler.h"

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

class FetchDeviceInfoCommandHandler {

public:
    FetchDeviceInfoCommandHandler();

    AgtpCommandResult handle(const AgtpCommand &command);

private:
    DeviceInfo mDeviceInfo;

};

#endif // FETCHDEVICEINFOCOMMANDHANDLER_H
