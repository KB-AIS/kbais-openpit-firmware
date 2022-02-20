#ifndef OMNICOMMLLSPROTOCOLFOMRATTER_H
#define OMNICOMMLLSPROTOCOLFOMRATTER_H

// std
#include <vector>
// qt
#include <QByteArray>
// oss
#include <nonstd/expected.hpp>

// Using short names to save data for binary serialization
struct LlsReplyReadData {
    char    Adr { 0 };

    char    Tem { 0 };

    quint16 Lvl { 0 };

    quint16 Frq { 0 };
};

class OmnicommLlsProtocolFomratter {

public:
    enum DecodeReplyError {
        NotEnoughData
    ,   NoPrefix
    ,   UnsupportedReply
    ,   CrcNotMatch
    };

    using DecodeResult_t =
        std::pair<int, nonstd::expected<LlsReplyReadData, DecodeReplyError>>;

    OmnicommLlsProtocolFomratter() = delete;

    /*!
     * \return amount of scanned bytes and expeced decode result.
     */
    static DecodeResult_t DecodeNextReplySingleRead(const QByteArray& bytes);

    static QByteArray EncodeOpSingleRead(const std::vector<quint8>& addresses);

private:
    static DecodeResult_t ReturnUnexpected(int scaned, DecodeReplyError error);

};

#endif // OMNICOMMLLSPROTOCOLFOMRATTER_H
