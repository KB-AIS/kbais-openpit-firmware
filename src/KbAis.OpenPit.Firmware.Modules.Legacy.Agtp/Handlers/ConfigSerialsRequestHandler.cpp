#include "ConfigSerialsRequestHandler.h"

// oss
#include <nlohmann/json.hpp>

using namespace nlohmann;

ConfigSerialsRequestHandler::ConfigSerialsRequestHandler() { }

AgtpResponse
ConfigSerialsRequestHandler::handle(const AgtpRequest& request) {
    const auto j = R"(
        {
            "interfaces": [
                { "baud_rate": 19200, "format": 0, "name": "RS485-1" },
                { "baud_rate": 19200, "format": 0, "name": "RS485-2" },
                { "baud_rate": 19200, "format": 0, "name": "RS485-3" },
                { "baud_rate": 19200, "format": 0, "name": "RS485-4" },
                { "baud_rate": 19200, "format": 0, "name": "RS485-5" },
                { "baud_rate": 19200, "format": 0, "name": "RS485-6" },
                { "baud_rate": 19200, "format": 0, "name": "RS232-1" },
                { "baud_rate": 19200, "format": 0, "name": "RS232-2" },
                { "baud_rate": 19200, "format": 0, "name": "RS232-3" },
                { "baud_rate": 19200, "format": 0, "name": "RS232-4" }
            ],
            "version": "1.0"
        }
    )"_json;

    return {
        request.uid
    ,   QString("%1:%2")
            .arg("INTERFACE_SETTINGS")
            .arg(QString::fromStdString(j.dump()))
            .toUtf8()
    ,   0x00
    };
}

QString
ConfigSerialsRequestHandler::get_request_name() const {
    return QStringLiteral("INTERFACE_SETTINGS");
}

AgtpResponse
ConfigSerialsTklsRequestHandler::handle(const AgtpRequest& request) {
    const auto j = R"(
        {
            "interface": 1,
            "interval_poll": 0,
            "sensors": [
                { "address": 1, "enabled": true, "name": "1" },
                { "address": 2, "enabled": false, "name": "2" },
                { "address": 3, "enabled": false, "name": "3" },
                { "address": 4, "enabled": false, "name": "4" },
                { "address": 5, "enabled": false, "name": "5" },
                { "address": 6, "enabled": false, "name": "6" },
                { "address": 7, "enabled": false, "name": "7" },
                { "address": 8, "enabled": false, "name": "8" }
            ],
            "version": "1.0"
        }
    )"_json;

    return {
        request.uid
    ,   QString("%1:%2")
            .arg("TKLS_SETTINGS")
            .arg(QString::fromStdString(j.dump()))
            .toUtf8()
    ,   0x00
    };
}

QString
ConfigSerialsTklsRequestHandler::get_request_name() const {
    return QStringLiteral("TKLS_SETTINGS");
}

AgtpResponse
ConfigSerialsTpmsRequestHandler::handle(const AgtpRequest& request) {
    const auto j = R"({"interval_poll":0,"sensors":[],"version":"1.0"})"_json;

    return {
        request.uid
    ,   QString("%1:%2")
            .arg("TPMS_SETTINGS")
            .arg(QString::fromStdString(j.dump()))
            .toUtf8()
    ,   0x00
    };
}

QString
ConfigSerialsTpmsRequestHandler::get_request_name() const {
    return QStringLiteral("TPMS_SETTINGS");
}

AgtpResponse
ConfigSerialsLcsRequestHandler::handle(const AgtpRequest& request) {
    const auto j = R"(
        {
            "address": 11,
            "delay_fix_weight": 15,
            "delay_parking": 30,
            "delay_stop": 5,
            "delta_weight": 5,
            "interface": 0,
            "interval_poll": 30,
            "scale_fuel": 0,
            "start_speed": 5,
            "threshold_weight": 20,
            "version": "1.0",
            "weight_averaging_period": 60
        }
    )"_json;

    return {
        request.uid
    ,   QString("%1:%2")
            .arg("LCS_SETTINGS")
            .arg(QString::fromStdString(j.dump()))
            .toUtf8()
    ,   0x00
    };
}

QString
ConfigSerialsLcsRequestHandler::get_request_name() const {
    return QStringLiteral("LCS_SETTINGS");
}

AgtpResponse
ConfigSerialsStscpRequestHandler::handle(const AgtpRequest& request) {
    const auto j = R"({"disable_buzzer_parking":0,"interface":0,"motion_disable":0,"version":"1.0"})"_json;

    return {
        request.uid
    ,   QString("%1:%2")
            .arg("STSCP_SETTINGS")
            .arg(QString::fromStdString(j.dump()))
            .toUtf8()
    ,   0x00
    };
}

QString
ConfigSerialsStscpRequestHandler::get_request_name() const {
    return QStringLiteral("STSCP_SETTINGS");
}

AgtpResponse
ConfigSerialsTkcrRequestHandler::handle(const AgtpRequest& request) {
    const auto j = R"(
        {
            "interface": 0,
            "network_address": 240,
            "repeated_read_filter_interval": 15000,
            "version": "1.0"
        }
    )"_json;

    return {
        request.uid
    ,   QString("%1:%2")
            .arg("TKCR_SETTINGS")
            .arg(QString::fromStdString(j.dump()))
            .toUtf8()
    ,   0x00
    };
}

QString
ConfigSerialsTkcrRequestHandler::get_request_name() const {
    return QStringLiteral("TKCR_SETTINGS");
}
