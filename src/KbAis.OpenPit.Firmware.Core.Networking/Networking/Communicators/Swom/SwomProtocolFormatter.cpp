#include "SwomProtocolFormatter.h"

// qt
#include <QDataStream>
#include <QIODevice>
#include <QUuid>
// oss
#include <range/v3/all.hpp>

#include "JsonQt.h"
#include "QtExtensions/QByteArrayExt.h"

using json = nlohmann::json;

QDataStream& operator<<(QDataStream& out, const json& payload) {
    return out << fromStdVector(json::to_msgpack(payload));
}

QByteArray
SwomProtocolFormatter::EncodeAckFrame(const QString& clientId) {
    QByteArray bytes;
    QDataStream out { &bytes, QIODevice::WriteOnly };

    out << static_cast<quint8>(0x00)
        << QUuid::createUuid();
    out << json { { "EquipmentId", clientId } };

    return bytes;
}

QByteArray
SwomProtocolFormatter::EncodeTelFrame(const QVector<MessagesBatchDto>& messageBatches) {
    QByteArray bytes;
    QDataStream out { &bytes, QIODevice::WriteOnly };

    out << static_cast<quint8>(0x01)
        << QUuid::createUuid();

    ranges::for_each(
        messageBatches
    |   ranges::views::transform([](MessagesBatchDto x) { return x.messages; })
    |   ranges::actions::join
    ,   [&out](const MessageDto& x) {
            out << fromStdVector(json::to_msgpack(x.moniker))
                << x.payload;
        }
    );

    return bytes;
}
