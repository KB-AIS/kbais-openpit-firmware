#include "FormatterModbusCardReader.h"

// qt
#include <QDataStream>
#include <QtEndian>

#include "Crc16Alogs.h"

enum class ModbusFunctionalCode : quint8 {
    ReadHoldingRegisters = 0x03
,   PresetMultipleRegisters = 0x10
};

QByteArray
FormatterModbusCardReader::encode_req_read_card_number(quint8 device_address) {
    constexpr quint16 STA_REG_CARD_NUMBER { 0x00 }, NUM_REG_CARD_NUMBER { 0x02 };

    QByteArray bytes;
    QDataStream output { &bytes, QIODevice::WriteOnly };

    output.setByteOrder(QDataStream::BigEndian);
    output << static_cast<quint8>(device_address)
      << static_cast<quint8>(ModbusFunctionalCode::ReadHoldingRegisters)
      << static_cast<quint16>(STA_REG_CARD_NUMBER)
      << static_cast<quint16>(NUM_REG_CARD_NUMBER);

    output.setByteOrder(QDataStream::LittleEndian);
    output << calc_crc16_modbus(bytes);

    return bytes;
}

FormatterModbusCardReader::DecodeResult_t
FormatterModbusCardReader::decode_rsp(const QByteArray& bytes) {
    using namespace nonstd;

    constexpr quint8 MIN_EXPECTED_RESPONSE_LENGTH { 5 };
    constexpr quint8 CRC_FIELD_LENGTH { 2 };

    const auto consumed = bytes.size();

    if (consumed < MIN_EXPECTED_RESPONSE_LENGTH) {
        return std::make_pair(0, make_unexpected(FormatterModbusCardReader::DecodeRspError::NotEnoughBytes));
    }

    auto offset { 0 };

    const auto response_meta = ModbusResponseMeta {
        .device_address  = bytes.at(offset++),
        .functional_code = bytes.at(offset++),
        .payload_size    = bytes.at(offset++),
    };

    const auto bytes_to_crc { offset + response_meta.payload_size };

    const auto bytes_in_rsp { bytes_to_crc + CRC_FIELD_LENGTH };

    if (consumed < bytes_in_rsp) {
        return std::make_pair(0, make_unexpected(FormatterModbusCardReader::DecodeRspError::NotEnoughBytes));
    }

    const auto crc { qFromLittleEndian<quint16>(bytes.mid(bytes_to_crc, CRC_FIELD_LENGTH)) };

    if (crc != calc_crc16_modbus(bytes.left(bytes_to_crc))) {
        return std::make_pair(offset, make_unexpected(FormatterModbusCardReader::DecodeRspError::MismatchChecksum));
    }

    // WA: Hard-coded number parsing, maybe should support more cards types...
    auto cdn = qFromLittleEndian<quint32>(bytes.mid(4, 3));
    return std::make_pair(bytes_in_rsp, cdn);
}
