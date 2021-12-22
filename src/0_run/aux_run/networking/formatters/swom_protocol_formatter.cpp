#include "swom_protocol_formatter.h"

// qt
#include <QDataStream>
// oss
#include <nlohmann/json.hpp>

#include "utils/json_qt_support.h"

using json = nlohmann::json;

void to_json(json& j, const AthData& d) {
    j = json {
        { "EquipmentId", d.equipmentId }
    };
}

void from_json(const json& j, AthData& d) {
    j.at("EquipmentId").get_to(d.equipmentId);
}

// TODO: Refactor
QByteArray
SwomProtocolFormatter::encodeAthFrame(const QString& equipmentId) const {
    constexpr quint16 BASE_FRAME_LENGTH = 21;


    auto f = json::to_msgpack(AthData { equipmentId });

    QByteArray bytes;
    QDataStream bStream(&bytes, QIODevice::WriteOnly);
    auto foo = BASE_FRAME_LENGTH + f.size();
    bStream << foo;
    bStream << static_cast<quint8>(SwomFrameType::Ath);
    auto uuid = QUuid::createUuid();
    bStream << uuid;
    bStream.writeRawData(reinterpret_cast<const char*>(f.data()), f.size());


    return bytes;
}

QByteArray
SwomProtocolFormatter::encodeTelFrame(const QList<MessagesBatchDto> &messageBatches) const {
    QByteArray bytes;
    QDataStream bytesStream(&bytes, QIODevice::WriteOnly);

    bytesStream << static_cast<quint8>(SwomFrameType::Tel);
    for (const auto& messagesBatch : messageBatches) {
        for (const auto& message: messagesBatch.messages) {
            auto monkier = json::to_msgpack(message.moniker);

            bytesStream
                << QByteArray(reinterpret_cast<const char*>(monkier.data()), monkier.size())
                << message.payload;
        }
    }

    return bytes;
}
