#include "FetchDeviceInfoRequestHandler.h"

FetchDeviceInfoRequestHandler::FetchDeviceInfoRequestHandler() {
    // !W: BEGIN BKT UNREFACTORED CODE
    memset(&mDeviceInfo, 0, sizeof(mDeviceInfo));
    mDeviceInfo.serial = 70100007;
    mDeviceInfo.sizeRecvBuff = 64000;
    mDeviceInfo.version = 1;
    memcpy(mDeviceInfo.versionFw, "AGDS-01.01", sizeof("AGDS-01.01") - 1);
    // !W: END BKT UNREFACTORED CODE
}

AgtpResponse
FetchDeviceInfoRequestHandler::handle(const AgtpRequest& command) {
    AgtpResponse result;

    // !W: BEGIN BKT UNREFACTORED CODE
    result.bytes.resize(sizeof(DeviceInfo));
    memcpy(result.bytes.data(), &mDeviceInfo, sizeof(DeviceInfo));
    // !W: END BKT UNREFACTORED CODE

    result.uid = command.uid;

    return result;
}

QString FetchDeviceInfoRequestHandler::get_request_name() const {
    return QStringLiteral("DEVICEINFO");
}
