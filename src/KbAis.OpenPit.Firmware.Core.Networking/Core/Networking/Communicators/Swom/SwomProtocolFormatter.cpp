#include "SwomProtocolFormatter.h"

// qt
#include <QDataStream>
#include <QIODevice>
#include <QtEndian>
// oss
#include <range/v3/all.hpp>

#include "Crc16Alogs.h"
#include "JsonQt.h"
#include "QtExtensions/QByteArrayExt.h"

using json = nlohmann::json;

QDataStream& operator<<(QDataStream& out, const json& payload) {
    return out << fromStdVector(json::to_msgpack(payload));
}

DecodeAckFrameResult_t
SwomProtocolFormatter::DecodeAckFrame(QByteArray& bytes) {
    auto offset { 0 };
    const auto frameLen = qFromLittleEndian<qint32>(bytes.mid(offset, 4));
    offset += 4;

    if (bytes.size() < frameLen + offset) {
        return nonstd::make_unexpected("Frame is too short");
    }

    const auto frameCrc = qFromLittleEndian<quint16>(bytes.mid(offset + frameLen, 2));
    if (frameCrc != calcCrc16CcittFalse(bytes.mid(0, offset + frameLen))) {
        return nonstd::make_unexpected("Crc is invalid");
    }

    std::vector<SwomAckPacket> packets;
    while (offset != frameLen + 4) {
        const auto packetType = static_cast<quint8>(bytes[offset]);
        offset += 1;

        if (packetType != static_cast<quint8>(SwomPacketType::Ack)) {
            return nonstd::make_unexpected("Unexpected packet type");
        }

        const auto packetUuid = QUuid::fromRfc4122(bytes.mid(offset, 16));
        offset += 16;

        qFromLittleEndian<qint32>(bytes.mid(offset, 4));
        offset += 4;

        const auto packetAckUuid = QUuid::fromRfc4122(bytes.mid(offset, 16));
        offset += 16;

        const SwomAckPacket::AckResultCode packetAckResult { static_cast<quint8>(bytes[offset]) };
        offset += 1;

        packets.push_back({ packetUuid, packetAckUuid, packetAckResult });
    }

    return packets;
}

QByteArray
SwomProtocolFormatter::EncodeAthPacket(const QUuid& uuid, const QString& equipmentId) {
    QByteArray encodedPacket;
    QDataStream out { &encodedPacket, QIODevice::WriteOnly };
    out.setByteOrder(QDataStream::LittleEndian);

    out << static_cast<quint8>(SwomPacketType::Ath)
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

    out << static_cast<quint8>(SwomPacketType::Tel)
        << uuid;

    ranges::for_each(messageBatch.messages, [&](const MessageDto& x) {
        out << fromStdVector(json::to_msgpack(x.moniker))
            << x.payload;
    });

    auto payloadLen = encodedPacket.length() - PAYLOAD_LEN_SHIFT;

    QByteArray payloadLenBytes(reinterpret_cast<const char*>(&payloadLen), sizeof(int));
    encodedPacket.insert(17, payloadLenBytes);

    return encodedPacket;
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
