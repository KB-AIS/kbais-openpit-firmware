﻿#include "AgtpProtocolPareser.h"

// std
#include <numeric>
// qt
#include <QDataStream>
#include <QIODevice>

// cfw::utils
#include "Crc16Alogs.h"
#include "Crc8Alogs.h"

constexpr quint8 AGTP_FRM_BEG = 0x3E;
constexpr quint8 AGTP_FRM_END = 0x0A;
constexpr quint8 AGTP_PROTOCOL_V2 = 0x02;
constexpr quint8 AGTP_PCK_ACK_CODE_OK = 0x7F;

constexpr qint32 AGTP_FRM_BASE_LEN = 12;

nonstd::expected<std::vector<AgtpRequest>, bool>
AgtpProtocolParser::parseRequest(QByteArray&& bytes) {
    if (bytes.at(0) != AGTP_FRM_BEG) {
        return nonstd::make_unexpected(false);
    }

    const auto frmPtr = bytes.data();

    const auto frmLen = *reinterpret_cast<uint32_t*>(frmPtr + 1);

    if (static_cast<uint32_t>(bytes.size()) < frmLen) {
        return nonstd::make_unexpected(false);
    }

    if (bytes.at(5) != AGTP_PROTOCOL_V2) {
        return nonstd::make_unexpected(false);
    }

    auto bytesLeftUnscanned = frmLen - AGTP_FRM_BASE_LEN;

    constexpr int CMDS_START_POS = 10;

    auto contentBytes = bytes.mid(10, bytesLeftUnscanned);
    auto cmdsPtr = contentBytes.data();

    constexpr int PCK_LEN_SHIFT { 1 };
    constexpr int PCK_UID_SHIFT { 5 };
    constexpr int PCK_PLD_SHIFT { 7 };

    std::vector<AgtpRequest> requests { };

    while (bytesLeftUnscanned > 0) {
        const auto cmdType = *cmdsPtr;
        const auto cmdLen = *reinterpret_cast<quint32*>(cmdsPtr + PCK_LEN_SHIFT);
        const auto cmdUid = *reinterpret_cast<quint16*>(cmdsPtr + PCK_UID_SHIFT);
        const auto cmdCrc = *reinterpret_cast<quint16*>(cmdsPtr + cmdLen - 2);

        const auto cmdPld = QString::fromUtf8(cmdsPtr + PCK_PLD_SHIFT, cmdLen - 9);

        requests.push_back({ cmdUid, cmdPld, cmdType });

        cmdsPtr += cmdLen;
        bytesLeftUnscanned -= cmdLen;
    }

    return requests;
}

QByteArray
AgtpProtocolParser::createResponse(const std::vector<AgtpResponse>& commandResults) {
    constexpr int AGTP_PCK_BASE_LEN = 12;

    const auto pcksLength = std::accumulate(
        commandResults.cbegin(), commandResults.cend(), 0,
        [](int a, AgtpResponse x) {
            return a + AGTP_PCK_BASE_LEN + x.bytes.size();
        }
    );

    QByteArray bytes { pcksLength + AGTP_FRM_BASE_LEN, 0x00 };
    QDataStream out { &bytes, QIODevice::ReadWrite };
    out.setByteOrder(QDataStream::LittleEndian);

    out << AGTP_FRM_BEG;
    // FRM HEADER: length
    out << static_cast<quint32>(pcksLength + AGTP_FRM_BASE_LEN);
    out << AGTP_PROTOCOL_V2;
    // FRM HEADER: device serial number
    // TODO: Placeholder
    out << static_cast<quint32>(70100007);

    constexpr int AGTP_FRM_HEAD_LEN = 10;

    auto nextPckSft = AGTP_FRM_HEAD_LEN;
    for (auto&& commandResult : commandResults) {
        quint32 pldLength = commandResult.bytes.size();
        quint32 pckLength = pldLength + AGTP_PCK_BASE_LEN;

        // PCK HEADER:
        out << AGTP_PCK_ACK_CODE_OK;
        // PCK HEADER: length
        out << pckLength;
        // PCK HEADER: uid
        out << static_cast<quint16>(0);
        // PCK HEADER: acknowledage uid
        out << commandResult.uid;
        // PCK HEADER: status code
        out << commandResult.status;
        // PCK: payload
        out.writeRawData(commandResult.bytes.data(), pldLength);
        // PCK TAIL: checksum
        out << calcCrc16CcittFalse(bytes.mid(nextPckSft, pldLength + AGTP_FRM_HEAD_LEN));

        nextPckSft = nextPckSft + pckLength;
    }

    out << calcCrc8Maxim(bytes.left(pcksLength + AGTP_FRM_HEAD_LEN));
    out << AGTP_FRM_END;

    return bytes;
}
