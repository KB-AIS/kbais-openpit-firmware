#include "FetchDeviceSpecRequestHandler.h"

// oss
#include <fmt/format.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

FetchDeviceSpecRequestHandler::FetchDeviceSpecRequestHandler() {

}

AgtpResponse
FetchDeviceSpecRequestHandler::handle(const AgtpRequest& command) {
    AgtpResponse result;

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

QString FetchDeviceSpecRequestHandler::get_request_name() const {
    return QStringLiteral("INFO");
}
