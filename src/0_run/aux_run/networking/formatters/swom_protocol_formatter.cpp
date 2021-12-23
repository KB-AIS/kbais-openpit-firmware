#include "swom_protocol_formatter.h"

// qt
#include <QDataStream>
// oss
#include <nlohmann/json.hpp>

#include "utils/json_qt_support.h"

using json = nlohmann::json;

QDataStream& operator<<(QDataStream& out, const json& payload) {
    auto bytes = json::to_msgpack(payload);

    return out << QByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}


// TODO: Refactor
QByteArray
SwomProtocolFormatter::encodeAthFrame(const QString& equipmentId) const {
    constexpr quint16 BASE_FRAME_LENGTH = 21;

    QByteArray bytes;
    QDataStream bytesStream(&bytes, QIODevice::WriteOnly);

    auto uuid = QUuid::createUuid();

    json payload;
    payload["EquipmentId"] = equipmentId;

    bytesStream << static_cast<quint8>(SwomFrameType::Ath) << uuid << payload;

    return bytes;
}

QByteArray
SwomProtocolFormatter::encodeTelFrame(const QList<MessagesBatchDto> &messageBatches) const {
    QByteArray bytes;
    QDataStream bytesStream(&bytes, QIODevice::WriteOnly);

    auto uuid = QUuid::createUuid();

    bytesStream << static_cast<quint8>(SwomFrameType::Tel) << uuid;
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
