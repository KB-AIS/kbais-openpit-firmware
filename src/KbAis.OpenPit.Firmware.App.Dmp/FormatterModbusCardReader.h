#ifndef FORMATTERMODBUSCARDREADER_H
#define FORMATTERMODBUSCARDREADER_H

// qt
#include <QByteArray>
// oss
#include <nonstd/expected.hpp>

class FormatterModbusCardReader {

public:
    enum class DecodeRspError {
        NotEnoughBytes
    ,   MismatchChecksum
    };

    using CardNumber_t = quint32;

    using DecodeResult_t = nonstd::expected<CardNumber_t, DecodeRspError>;

    FormatterModbusCardReader() = delete;

    static QByteArray encode_req_read_card_number(quint8 device_address);

    static DecodeResult_t decode_rsp(const QByteArray& bytes);

};

#endif // FORMATTERMODBUSCARDREADER_H
