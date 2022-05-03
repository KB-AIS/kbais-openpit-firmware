#include "crd_formatter_tkcardreader_modbus.h"

// qt
#include <QDataStream>
#include <QtEndian>

#include "Crc16Alogs.h"

enum class functional_code : quint8 {
    ReadHoldingRegisters = 0x03
,   PresetMultipleRegisters = 0x10
};

struct response_meta {

    quint8 device_address  { 0x00 };

    quint8 functional_code { 0x00 };

    quint8 payload_size    { 0x00 };

};

QByteArray
crd_formatter_tkcardreader_modbus::encode_request_card_number(quint8 device_address) {
    constexpr quint16 STA_REG_CARD_NUMBER { 0x00 }, NUM_REG_CARD_NUMBER { 0x02 };

    QByteArray bytes;
    QDataStream output { &bytes, QIODevice::WriteOnly };

    output.setByteOrder(QDataStream::BigEndian);
    output << static_cast<quint8>(device_address)
      << static_cast<quint8>(functional_code::ReadHoldingRegisters)
      << static_cast<quint16>(STA_REG_CARD_NUMBER)
      << static_cast<quint16>(NUM_REG_CARD_NUMBER);

    output.setByteOrder(QDataStream::LittleEndian);
    output << calc_crc16_modbus(bytes);

    return bytes;
}

crd_formatter_tkcardreader_modbus::DecodeResult_t
crd_formatter_tkcardreader_modbus::decode_response(const QByteArray& bytes) {
    using namespace nonstd;

    constexpr quint8 MIN_EXPECTED_RESPONSE_LENGTH { 5 };
    constexpr quint8 CRC_FIELD_LENGTH { 2 };

    const auto consumed = bytes.size();

    if (consumed < MIN_EXPECTED_RESPONSE_LENGTH) {
        return std::make_pair(0, make_unexpected(DecodeResponseError::NotEnoughBytes));
    }

    auto offset { 0 };

    const response_meta response_meta {
        .device_address  = bytes.at(offset++)
    ,   .functional_code = bytes.at(offset++)
    ,   .payload_size    = bytes.at(offset++)
    };

    const auto bytes_to_crc { offset + response_meta.payload_size };

    const auto bytes_in_response { bytes_to_crc + CRC_FIELD_LENGTH };

    if (consumed < bytes_in_response) {
        return std::make_pair(0, make_unexpected(DecodeResponseError::NotEnoughBytes));
    }

    const auto crc { qFromLittleEndian<quint16>(bytes.mid(bytes_to_crc, CRC_FIELD_LENGTH)) };

    if (crc != calc_crc16_modbus(bytes.left(bytes_to_crc))) {
        // Отмечаем только байты из заголовка как проверенные
        return std::make_pair(offset, make_unexpected(DecodeResponseError::MismatchChecksum));
    }

    const crd_response_cardnumber rsp {
        .device_address = response_meta.device_address
        // WA: Hard-coded number parsing, maybe should support more cards types...
    ,   .card_number = qFromLittleEndian<quint32>(bytes.mid(4, 3))
    };

    return std::make_pair(bytes_in_response, std::move(rsp));
}
