#include "FetchDeviceStateRequsetHandler.h"

// oss
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <plog/Log.h>

using json = nlohmann::json;

const QString CMD_NAME { QStringLiteral("STATE") };

FetchDeviceStateRequestHandler::FetchDeviceStateRequestHandler(
    const DeviceStateCollector& deviceStateCollector
)
    : mDeviceStateCollector { deviceStateCollector }
{

}

QString
FetchDeviceStateRequestHandler::getRequestName() const {
    return CMD_NAME;
}

AgtpResponse
FetchDeviceStateRequestHandler::handle(const AgtpRequest& command) {
    AgtpResponse result;

    const auto gpsMessage = mDeviceStateCollector.getGpsMessage();

    const auto payload = json {
        {
            "gps",
            json {
                { "sat", gpsMessage.satellites },
                { "valid", gpsMessage.isValid },
                { "lat", gpsMessage.latitude },
                { "lon", gpsMessage.longitude },
            }
        }
    };

    return {
        command.uid,
        QString::fromStdString(fmt::format("STATE:{}", payload.dump())).toUtf8(),
        0x00
    };
}
