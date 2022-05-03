#ifndef CRD_FORMATTER_TKCARDREADER_MODBUS_H
#define CRD_FORMATTER_TKCARDREADER_MODBUS_H

// qt
#include <QtCore/qbytearray.h>
// oss
#include <nonstd/expected.hpp>

constexpr unsigned int card_number_empty_read { 0 };

struct crd_response_cardnumber {

    unsigned char device_address { 0x00 };

    unsigned int  card_number { card_number_empty_read };

    inline auto tie() const { return std::tie(this->device_address, this->card_number); }

    inline bool operator==(const crd_response_cardnumber& rhs) {
        return this->tie() == rhs.tie();
    }

    inline bool operator!=(const crd_response_cardnumber& rhs) {
        return !(this->tie() == rhs.tie());
    }

};

class crd_formatter_tkcardreader_modbus {

public:
    enum class DecodeResponseError { NotEnoughBytes, MismatchChecksum };

    using BytesExam_t = qint64;

    using DecodeResult_t = std::pair<BytesExam_t, nonstd::expected<crd_response_cardnumber, DecodeResponseError>>;

    crd_formatter_tkcardreader_modbus() = delete;

    static QByteArray encode_request_card_number(quint8 device_address);

    static DecodeResult_t decode_response(const QByteArray& bytes);

};

#endif // CRD_FORMATTER_TKCARDREADER_MODBUS_H
