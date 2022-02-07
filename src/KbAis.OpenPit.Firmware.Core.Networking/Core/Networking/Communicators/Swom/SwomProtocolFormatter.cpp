#include "SwomProtocolFormatter.h"

// qt
#include <QDataStream>
#include <QIODevice>
// oss
#include <range/v3/all.hpp>

#include "JsonQt.h"
#include "QtExtensions/QByteArrayExt.h"

using json = nlohmann::json;

QDataStream& operator<<(QDataStream& out, const json& payload) {
    return out << fromStdVector(json::to_msgpack(payload));
}

QByteArray
SwomProtocolFormatter::EncodeAthFrame(const QUuid& uuid, const QString& equipmentId) {
    QByteArray bytes;
    QDataStream out { &bytes, QIODevice::WriteOnly };

    out << static_cast<quint8>(SwomFrameType::Ath)
        << uuid
        << json { { "EquipmentId", equipmentId } };

    return bytes;
}

QByteArray
SwomProtocolFormatter::EncodeTelFrame(const QUuid& uuid, const MessagesBatchDto& messageBatch) {
    QByteArray bytes;
    QDataStream out { &bytes, QIODevice::WriteOnly };

    out << static_cast<quint8>(SwomFrameType::Tel)
        << uuid;

    ranges::for_each(
        messageBatch.messages
    ,   [&out](const MessageDto& x) {
            out << fromStdVector(json::to_msgpack(x.moniker))
                << x.payload;
        }
    );

    return bytes;
}
