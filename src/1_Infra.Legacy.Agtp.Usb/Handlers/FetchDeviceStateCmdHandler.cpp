#include "FetchDeviceStateCmdHandler.h"

// oss
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <plog/Log.h>

using json = nlohmann::json;

const QString CMD_NAME { QStringLiteral("STATE:?") };

FetchDeviceStateCmdHandler::FetchDeviceStateCmdHandler(
    const DeviceStateCollector& deviceStateCollector
)
    : mDeviceStateCollector { deviceStateCollector }
{

}

QString
FetchDeviceStateCmdHandler::getCommand() const {
    return CMD_NAME;
}

AgtpCommandResult
FetchDeviceStateCmdHandler::handle(const AgtpCommand& command) {
    AgtpCommandResult result;

    const auto gpsMessage = mDeviceStateCollector.mCachedGpsMessage;

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
