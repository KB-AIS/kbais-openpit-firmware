#ifndef LLS_FORMATTER_OMNICOMM_H
#define LLS_FORMATTER_OMNICOMM_H

// std
#include <vector>
// qt
#include <QByteArray>
// oss
#include <nonstd/expected.hpp>

// Using short names to save data for binary serialization
struct lls_reply_readdata {
    quint8  Adr { 0 };

    quint8  Tem { 0 };

    quint16 Lvl { 0 };

    quint16 Frq { 0 };
};

class lls_formatter_omnicomm {

public:
    enum DecodeReplyError {
        NotEnoughData
    ,   NoPrefix
    ,   UnsupportedReply
    ,   CrcNotMatch
    };

    using DecodeResult_t =
        std::pair<int, nonstd::expected<lls_reply_readdata, DecodeReplyError>>;

    lls_formatter_omnicomm() = delete;

    /*!
     * \return amount of scanned bytes and expeced decode result.
     */
    static DecodeResult_t DecodeFirstReply(const QByteArray& bytes);

    static QByteArray EncodeOpSingleRead(const std::vector<quint8>& addresses);

private:
    static DecodeResult_t ReturnUnexpected(int scaned, DecodeReplyError error);

};

#endif // LLS_FORMATTER_OMNICOMM_H
