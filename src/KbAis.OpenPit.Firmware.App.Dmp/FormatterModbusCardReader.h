#ifndef FORMATTERMODBUSCARDREADER_H
#define FORMATTERMODBUSCARDREADER_H

// qt
#include <QByteArray>
// oss
#include <nonstd/expected.hpp>

struct ModbusResponseMeta {

    quint8 device_address  { 0x00 };

    quint8 functional_code { 0x00 };

    quint8 payload_size    { 0x00 };

};

class FormatterModbusCardReader {

public:
    enum class DecodeRspError {
        NotEnoughBytes
    ,   MismatchChecksum
    };

    using BytesExam_t = qint64;

    using CardNumber_t = quint32;

    using DecodeResult_t = std::pair<BytesExam_t, nonstd::expected<CardNumber_t, DecodeRspError>>;

    FormatterModbusCardReader() = delete;

    static QByteArray encode_req_read_card_number(quint8 device_address);

    static DecodeResult_t decode_rsp(const QByteArray& bytes);

};

#endif // FORMATTERMODBUSCARDREADER_H
