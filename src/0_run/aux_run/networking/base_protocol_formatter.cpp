#include "base_protocol_formatter.h"

// qt
#include <QDataStream>
#include <QIODevice>
// oss
#include <json.hpp>
#include <plog/Log.h>

using json = nlohmann::json;

namespace KbAis::Cfw::Networking {

void to_json(json& j, const AthData& d) {
    j = json {
        { "EquipmentId", d.equipmentId.toStdString() }
    };
}

void from_json(const json& j, AthData& d) {
    std::string equipmentIdStr;

    j.at("EquipmentId").get_to(equipmentIdStr);
    d.equipmentId = QString::fromStdString(equipmentIdStr);
}

void to_json(json& j, const AcknData& d) {
    auto uuidBytes = d.uuid.toByteArray();

    j = json {
        { "Uuid", std::vector<unsigned char> { uuidBytes.begin(), uuidBytes.end() } },
        { "Result", d.result }
    };
}

void from_json(const json& j, AcknData& d) {
    std::vector<unsigned char> uuidBytes;

    j.at("Uuid").get_to(uuidBytes);
    d.uuid = QUuid::fromRfc4122(
        QByteArray(reinterpret_cast<const char*>(uuidBytes.data()), uuidBytes.size())
    );
    j.at("Result").get_to(d.result);
}

QByteArray
SwomProtocolFromatter::encodeAuthPackage(const QString &equipmentId) {
    constexpr quint16 BASE_FRAME_LENGTH = 21;

    auto f= json::to_msgpack(AthData { equipmentId });

    QByteArray bytes;
    QDataStream bStream(&bytes, QIODevice::WriteOnly);
    auto foo = BASE_FRAME_LENGTH + f.size();
    bStream << foo;
    bStream << static_cast<quint8>(DataType::ATH);
    auto uuid = QUuid::createUuid();
    bStream << uuid;
    bStream.writeRawData(reinterpret_cast<const char*>(f.data()), f.size());

    PLOGD << uuid.toString();

    return bytes;
}

}
