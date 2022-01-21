#include "FetchDeviceInfoCmdHandler.h"

FetchDeviceInfoCmdHandler::FetchDeviceInfoCmdHandler() {
    // !W: BEGIN BKT UNREFACTORED CODE
    memset(&mDeviceInfo, 0, sizeof(mDeviceInfo));
    mDeviceInfo.serial = 70100007;
    mDeviceInfo.sizeRecvBuff = 64000;
    mDeviceInfo.version = 1;
    memcpy(mDeviceInfo.versionFw, "AGDS-01.01", sizeof("AGDS-01.01") - 1);
    // !W: END BKT UNREFACTORED CODE
}

AgtpCommandResult
FetchDeviceInfoCmdHandler::handle(const AgtpCommand& command) {
    AgtpCommandResult result;

    // !W: BEGIN BKT UNREFACTORED CODE
    result.bytes.resize(sizeof(DeviceInfo));
    memcpy(result.bytes.data(), &mDeviceInfo, sizeof(DeviceInfo));
    // !W: END BKT UNREFACTORED CODE

    result.uid = command.uid;

    return result;
}

QString FetchDeviceInfoCmdHandler::getCommand() const {
    return QStringLiteral("DEVICEINFO=?;");
}
