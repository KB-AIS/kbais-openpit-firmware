#include "FetchDeviceModulesRequestHandler.h"

FetchDeviceModulesRequestHandler::FetchDeviceModulesRequestHandler() {

}

AgtpResponse
FetchDeviceModulesRequestHandler::handle(const AgtpRequest& command) {
    AgtpResponse result;

    QString modules {
        "SUPPORT_MODULES="
        "common-1.0"
        ",srvset-1.0"
//        ",display-1.0"
        ",serial_interfaces-1.0"
        ",ethernet-1.0"
//        ",can_interfaces-1.0"
//        ",write-1.0"
        ",state-1.0"
//        ",parksensor-1.0"
        ",scale-1.0"
//        ",fuel-1.0"
//        ",upgrade-1.0"
//        ",scd-1.0"
    };
    result.bytes = modules.toUtf8();
    result.uid = command.uid;

    return result;
}

QString FetchDeviceModulesRequestHandler::get_request_name() const {
    return QStringLiteral("SUPPORT_MODULES");
}
