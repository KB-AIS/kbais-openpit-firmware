#include "SwomProtocolFormatter.h"

// qt
#include <QDataStream>
#include <QIODevice>
// oss
#include <range/v3/all.hpp>

#include "Crc16Alogs.h"
#include "JsonQt.h"
#include "QtExtensions/QByteArrayExt.h"

using json = nlohmann::json;

QDataStream& operator<<(QDataStream& out, const json& payload) {
    return out << fromStdVector(json::to_msgpack(payload));
}

void
SwomProtocolFormatter::DecodeFrame(const QByteArray &encodedFrame) {
    //auto uuid = QUuid::fromRfc4122(byteToDecode);
}

QByteArray
SwomProtocolFormatter::EncodeFrame(const QByteArray &encodedPackets) {
    QByteArray encodedFrame;
    QDataStream out { &encodedFrame, QIODevice::WriteOnly };
    out.setByteOrder(QDataStream::LittleEndian);

    out << encodedPackets.length();
    out.writeRawData(encodedPackets.data(), encodedPackets.length());
    out << calcCrc16CcittFalse(encodedFrame);

    return encodedFrame;
}

QByteArray
SwomProtocolFormatter::EncodeAthPacket(const QUuid& uuid, const QString& equipmentId) {
    QByteArray encodedPacket;
    QDataStream out { &encodedPacket, QIODevice::WriteOnly };
    out.setByteOrder(QDataStream::LittleEndian);

    out << static_cast<quint8>(SwomFrameType::Ath)
        << uuid
        << json { { "EquipmentId", equipmentId } };

    return encodedPacket;
}

QByteArray
SwomProtocolFormatter::EncodeTelPacket(const QUuid& uuid, const MessagesBatchDto& messageBatch) {
    constexpr int PAYLOAD_LEN_SHIFT { 21 };

    QByteArray encodedPacket;
    QDataStream out { &encodedPacket, QIODevice::WriteOnly };
    out.setByteOrder(QDataStream::LittleEndian);

    out << static_cast<quint8>(SwomFrameType::Tel)
        << uuid;

    ranges::for_each(messageBatch.messages, [&](const MessageDto& x) {
        out << fromStdVector(json::to_msgpack(x.moniker))
            << x.payload;
    });

    auto payloadLen = encodedPacket.length() - PAYLOAD_LEN_SHIFT;

    QByteArray payloadLenBytes(reinterpret_cast<const char *>(&payloadLen), sizeof(int));
    encodedPacket.insert(17, payloadLenBytes);

    return encodedPacket;
}
