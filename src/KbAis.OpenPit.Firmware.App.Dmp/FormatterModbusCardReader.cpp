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

    auto offset { 0 };

    if (bytes.size() < MIN_EXPECTED_RESPONSE_LENGTH) {
        return make_unexpected(FormatterModbusCardReader::DecodeRspError::NotEnoughBytes);
    }

    const quint8 device_address = bytes.at(offset++);
    const quint8 fuctional_code = bytes.at(offset++);
    const quint8 payload_size   = bytes.at(offset++);

    // TODO: Check payload size

    const auto crc = qFromLittleEndian<quint16>(bytes.mid(offset + payload_size, 2));
    if (crc != calc_crc16_modbus(bytes.left(offset + payload_size))) {
        return make_unexpected(FormatterModbusCardReader::DecodeRspError::MismatchChecksum);
    }

    auto card_number = qFromLittleEndian<quint32>(bytes.mid(4, 3));

    return card_number;
}
