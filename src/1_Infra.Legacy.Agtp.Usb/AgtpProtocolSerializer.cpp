#include "AgtpProtocolSerializer.h"

// std
#include <numeric>
// qt
#include <QDataStream>
#include <QIODevice>

#include "CrcCalculators.h"

constexpr quint8 AGTP_FRAME_B = 0x3E;
constexpr quint8 AGTP_FRAME_E = 0x0A;
constexpr quint8 AGTP_PROTOCOL_V2 = 0x02;
constexpr quint8 AGTP_PACKET_ACK_CODE = 127;

QVector<AgtpCommand>
AgtpProtocolSerializer::deserialize(QByteArray&& bytes) {
    QVector<AgtpCommand> commands;

    if (bytes.at(0) != AGTP_FRAME_B) {
        return commands;
    }

    const auto framePtr = bytes.data();

    const auto frameLength = *reinterpret_cast<uint32_t*>(framePtr + 1);

    if (static_cast<uint32_t>(bytes.size()) < frameLength) {
        return commands;
    }

    if (bytes.at(5) != AGTP_PROTOCOL_V2) {
        return commands;
    }

    constexpr int FRAME_MIN_LENGTH = 12;

    auto bytesLeftUnscanned = frameLength - FRAME_MIN_LENGTH;

    constexpr int CMDS_START_POS = 10;

    auto contentBytes = bytes.mid(10, bytesLeftUnscanned);
    auto cmdsPtr = contentBytes.data();

    constexpr int PACKET_LEN_SHIFT { 1 };
    constexpr int PACKET_UID_SHIFT { 5 };
    constexpr int PACKET_PLD_SHIFT { 7 };

    while (bytesLeftUnscanned > 0) {
        const auto cmdType = *cmdsPtr;
        const auto cmdLen = *reinterpret_cast<quint32*>(cmdsPtr + PACKET_LEN_SHIFT);
        const auto cmdUid = *reinterpret_cast<quint16*>(cmdsPtr + PACKET_UID_SHIFT);
        const auto cmdCrc = *reinterpret_cast<quint16*>(cmdsPtr + cmdLen - 2);

        const auto cmdPld = QString::fromUtf8(cmdsPtr + PACKET_PLD_SHIFT, cmdLen - 9);

        commands.push_back({ cmdUid, cmdPld, cmdType });

        cmdsPtr += cmdLen;
        bytesLeftUnscanned -= cmdLen;
    }

    return commands;
}

QByteArray
AgtpProtocolSerializer::serialize(const QVector<AgtpCommandResult> &results) {
    constexpr int FRAME_BASE_LENGTH = 12;
    constexpr int PACKET_BASE_LENGHT = 12;

    const auto packetsLength = std::accumulate(
        results.constBegin(), results.constEnd(),
        0,
        [](int a, AgtpCommandResult x) {
            return a + PACKET_BASE_LENGHT + x.bytes.size();
        }
    );

    QByteArray bytes;
    QDataStream sBytes { &bytes, QIODevice::ReadWrite };
//    auto bytesPtr = reinterpret_cast<quint8*>(bytes.data());

//    bytesPtr[0] = AGTP_FRAME_B;
//    *reinterpret_cast<quint32*>(&bytesPtr[1]) = packetsLength + 12;
//    bytesPtr[5] = AGTP_PROTOCOL_V2;
//    *reinterpret_cast<quint32*>(&bytesPtr[6]) = static_cast<quint32>(70100007);
//    quint8 *ptr_container = &bytesPtr[10];
//    for (auto commandResult : results) {
//        *ptr_container = 127;
//        *reinterpret_cast<quint32*>(ptr_container + 1) = commandResult.bytes.size() + 12;
//        *reinterpret_cast<quint16*>(ptr_container + 5) = 0;
//        *reinterpret_cast<quint16*>(ptr_container + 7) = commandResult.uid;
//        ptr_container[9] = 0;
//        memcpy(ptr_container + 10, reinterpret_cast<quint8*>(commandResult.bytes.data()), commandResult.bytes.size());
//        *reinterpret_cast<quint16*>(ptr_container + commandResult.bytes.size() + 10) = CRC16(ptr_container, commandResult.bytes.size() + 10, 0xFFFF);
//        ptr_container += commandResult.bytes.size() + 12;
//    }
//    bytesPtr[packetsLength + 10] = CRC_1wire(&bytesPtr[0], static_cast<uint>(packetsLength + 10), 0);
//    bytesPtr[packetsLength + 11] = AGTP_FRAME_E;
    sBytes.setByteOrder(QDataStream::LittleEndian);
    sBytes << AGTP_FRAME_B;
    sBytes << static_cast<quint32>(packetsLength + FRAME_BASE_LENGTH);
    sBytes << AGTP_PROTOCOL_V2;
    sBytes << static_cast<quint32>(70100007);


    auto packetPtr = bytes.data() + 10;

    for (auto commandResult : results) {
        quint32 packetLength = commandResult.bytes.size() + PACKET_BASE_LENGHT;

        sBytes << AGTP_PACKET_ACK_CODE;
        sBytes << packetLength;
        sBytes << static_cast<quint16>(0);
        sBytes << commandResult.uid;
        sBytes << static_cast<quint8>(0);
        sBytes.writeRawData(commandResult.bytes.data(), commandResult.bytes.size());
        sBytes << CRC16(reinterpret_cast<quint8*>(packetPtr), commandResult.bytes.size() + 10, 0xFFFF);

        packetPtr = bytes.data() + 12 + commandResult.bytes.size();
    }
    sBytes << CRC_1wire(reinterpret_cast<quint8*>(bytes.data()), packetsLength + 10, 0);
    sBytes << AGTP_FRAME_E;
//    auto frameIdx = 0;
//    // FRAME BEGINING
//    bytes[frameIdx] = AGTP_FRAME_B;
//    frameIdx += 1;

//    // FRAME LENGTH
//    bytes[frameIdx] = packetsLength + FRAME_BASE_LENGTH;
//    frameIdx += 4;

//    // FRAME PROTOCOL VERSION
//    bytes[frameIdx] = AGTP_PROTOCOL_V2;
//    frameIdx += 1;

//    // FRAME DEVICE SERIAL NUMBER
//    bytes[frameIdx] = static_cast<quint32>(70100007);
//    frameIdx += 4;

//    auto packetPtr = bytes.data() + frameIdx;

//    for (auto commandResult : results) {
//        // PACKET ACK CODE
//        bytes[frameIdx] = AGTP_PACKET_ACK_CODE;
//        frameIdx += 1;

//        // PACKET LENGTH
//        unsigned int packetLength = commandResult.bytes.size() + PACKET_BASE_LENGHT;
//        bytes[frameIdx] = packetLength;
//        frameIdx += 4;

//        // PACKET UID
//        bytes[frameIdx] = static_cast<unsigned short>(0);
//        frameIdx += 2;

//        // PACKET RESPOND UID
//        bytes[frameIdx] = commandResult.uid;
//        frameIdx += 2;

//        // PACKET PROCESSING ERROR
//        bytes[frameIdx] = commandResult.error;
//        frameIdx += 1;

//        // PACKET PAYLOAD
//        // bytes.insert(frameIdx, commandResult.bytes);
//        memcpy(bytes.data() + frameIdx, commandResult.bytes.data(), commandResult.bytes.size());
//        frameIdx += commandResult.bytes.size();

//        // PAKCET CRC
//        bytes[frameIdx] = CRC16(reinterpret_cast<unsigned char*>(packetPtr), packetLength, 0xFFFF);
//        frameIdx += 2;

//        packetPtr = bytes.data() + frameIdx;
//    }

//    // FRAME CRC
//    bytes[frameIdx] = CRC_1wire(reinterpret_cast<unsigned char*>(bytes.data()), frameIdx, 0);
//    frameIdx += 1;

//    // FRAME ENDING
//    bytes[frameIdx] = AGTP_FRAME_E;

    return bytes;
}
