#include "OmnicommLlsProtocolFomratter.h"

// qt
#include <QtEndian>
// oss
#include <range/v3/all.hpp>

#include "Crc8Alogs.h"

constexpr quint8 LLS_REQ_PREFIX = 0x31;

constexpr quint8 LLS_REP_PREFIX = 0x3E;

constexpr quint8 LLS_OP_CODE_SINGLEREAD     = 0x06;
constexpr int    LLS_OP_REPL_SINGLEREAD_LEN = 9;

OmnicommLlsProtocolFomratter::DecodeResult_t
OmnicommLlsProtocolFomratter::DecodeFirstReply(const QByteArray& bytes) {
    constexpr int LLS_MINIMAL_REP_LEN { 3 };

    if (bytes.length() <= LLS_MINIMAL_REP_LEN) {
        return ReturnUnexpected(0, DecodeReplyError::NotEnoughData);
    }

    // Keep track of scanned bytes in recived bytes.
    int scanned { 0 };

    // Find the start of the first reply in bytes.
    const auto msgStartIdx = bytes.indexOf(LLS_REP_PREFIX, scanned);

    if (msgStartIdx == -1) {
        // Mark all bytes as scanned because there is no reply data.
        return ReturnUnexpected(bytes.length(), DecodeReplyError::NoPrefix);
    }

    scanned = msgStartIdx + 1;
    const auto bytesPtr = bytes.constData();

    const auto llsAdr = *reinterpret_cast<const quint8*>(bytesPtr + msgStartIdx + 1);
    const auto llsOpc = *reinterpret_cast<const quint8*>(bytesPtr + msgStartIdx + 2);

    scanned += 2;

    if (llsOpc != LLS_OP_CODE_SINGLEREAD) {
        return ReturnUnexpected(scanned, DecodeReplyError::UnsupportedReply);
    }

    // Check if publisher got enough data to perfrom parsing
    if (bytes.mid(msgStartIdx).length() < LLS_OP_REPL_SINGLEREAD_LEN) {
        return ReturnUnexpected(0, DecodeReplyError::NotEnoughData);
    }

    const auto llsCrc = *reinterpret_cast<const quint8*>(bytesPtr + msgStartIdx + 8);
    if (calcCrc8Maxim(bytes.mid(msgStartIdx, 8)) != llsCrc) {
        return ReturnUnexpected(scanned, DecodeReplyError::CrcNotMatch);
    };

    return std::make_pair(scanned + 6, LlsReplyReadData {
        llsAdr
    ,   bytes.at(msgStartIdx + 3)
    ,   qFromLittleEndian<quint16>(bytesPtr + msgStartIdx + 4)
    ,   qFromLittleEndian<quint16>(bytesPtr + msgStartIdx + 6)
    });
}

QByteArray
OmnicommLlsProtocolFomratter::EncodeOpSingleRead(const std::vector<quint8>& addresses) {
    constexpr int LLS_PFX_SHIFT { 0 };
    constexpr int LLS_ADR_SHIFT { 1 };
    constexpr int LLS_OPC_SHIFT { 2 };
    constexpr int LLS_CRC_SHIFT { 3 };

    constexpr int LLS_REQ_LEN { 4 };

    QByteArray bytes;
    int currentByteIndex = 0;

    ranges::for_each(addresses, [&b = bytes, &i = currentByteIndex](auto adr) {
        b[i + LLS_PFX_SHIFT] = LLS_REQ_PREFIX;
        b[i + LLS_ADR_SHIFT] = adr;
        b[i + LLS_OPC_SHIFT] = LLS_OP_CODE_SINGLEREAD;
        b[i + LLS_CRC_SHIFT] = calcCrc8Maxim(b.mid(i, 3));
        i += LLS_REQ_LEN;
    });

    return bytes;
}

OmnicommLlsProtocolFomratter::DecodeResult_t
OmnicommLlsProtocolFomratter::ReturnUnexpected(int scaned, DecodeReplyError error) {
    return std::make_pair(scaned, nonstd::make_unexpected(error));
}
