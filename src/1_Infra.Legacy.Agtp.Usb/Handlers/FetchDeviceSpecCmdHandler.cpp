#include "FetchDeviceSpecCmdHandler.h"

// oss
#include <fmt/core.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

FetchDeviceSpecCmdHandler::FetchDeviceSpecCmdHandler() {

}

AgtpCommandResult
FetchDeviceSpecCmdHandler::handle(const AgtpCommand& command) {
    AgtpCommandResult result;

    auto const payload = json {
        { "serial", "70100007" },
        {
            "app",
            json {
                { "version", "0.1.14" }
            }
        },
        {
            "hardware",
            json {
                { "name", "123" } ,
                { "revision", "123" }
            }
        },
        {
            "os",
            json {
                { "name", "linux-kbais" },
                { "build_id", "20200619064616" },
                { "build_version", "1.0.14" }
            }
        },
    };

    result.bytes = QString::fromStdString(fmt::format("INFO:{}", payload.dump())).toUtf8();
    result.uid = command.uid;

    return result;
}

QString FetchDeviceSpecCmdHandler::getCommand() const {
    return QStringLiteral("INFO?;");
}
