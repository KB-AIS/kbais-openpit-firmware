#include "NmeaSentence.h"

constexpr char FIELDS_SEP { ',' };

constexpr char CHKSUM_SEP { '*' };

QString TIME_FORMAT { "HHmmss.z" };

QString DATE_FORMAT { "ddMMyy" };

// Utility functions
std::pair<double, double> parse_lat_lon(
    const QByteArray& lat_bytes,
    char lat_dir,
    const QByteArray& lon_bytes,
    char lon_dir
) {
    constexpr double MINUTES_DIV { 60.0 };

    bool is_conv_ok;

    auto const lat_deg = lat_bytes.left(2).toDouble(&is_conv_ok);
    auto const lat_min = lat_bytes.mid(2).toDouble(&is_conv_ok) / MINUTES_DIV;
    double lat { (lat_deg + lat_min) * (lat_dir == 'S' ? -1 : 1) };

    auto const lon_deg = lon_bytes.left(3).toDouble(&is_conv_ok);
    auto const lon_min = lon_bytes.mid(3).toDouble(&is_conv_ok) / MINUTES_DIV;
    double lon { (lon_deg + lon_min) * (lon_dir == 'W' ? -1 : 1) };

    return { lat, lon };
}

GgaSentence::GgaSentence() {}

GgaSentence::GgaSentence(
    const QTime& time,
    double lat, double lon, short qal, short sat
) : time { time },
    latitude { lat }, longitude { lon }, gpsQauality { qal }, satellites { sat } {}

std::shared_ptr<GgaSentence> parseGgaSentence(const QByteArray& sentenceBytes) {
    constexpr int FP_TIME               { 1 };
    constexpr int FP_LAT                { 2 };
    constexpr int FP_LAT_DIR            { 3 };
    constexpr int FP_LON                { 4 };
    constexpr int FP_LON_DIR            { 5 };
    constexpr int FP_GPS_QUALITY        { 6 };
    constexpr int FP_SATELLITES         { 7 };

    bool is_conv_ok;
    const auto fields_chksum = sentenceBytes.split(CHKSUM_SEP);
    const auto fields = fields_chksum[0].split(FIELDS_SEP);

    const auto pos_time = QTime::fromString(fields[FP_TIME], TIME_FORMAT);

    const auto gps_quality = fields[FP_GPS_QUALITY].toShort(&is_conv_ok);

    if (gps_quality == 0) {
        return std::make_shared<GgaSentence>();
    }

    const auto [lat, lon] = parse_lat_lon(
        fields[FP_LAT], fields[FP_LAT_DIR].at(0),
        fields[FP_LON], fields[FP_LON_DIR].at(0)
    );

    const auto satellites  = fields[FP_SATELLITES].toShort(&is_conv_ok);

    return std::make_shared<GgaSentence>(GgaSentence {
        pos_time, lat, lon, gps_quality, satellites,
    });
}

RmcSentence::RmcSentence() {}

RmcSentence::RmcSentence(
    const QDateTime& datetime,
    double lat, double lon, double spd
) : datetime { datetime }, isValid { true },
    latitude { lat }, longitude { lon }, speed { spd } {}

std::shared_ptr<RmcSentence> parseRmcSentence(const QByteArray& sentenceBytes) {
    constexpr int FP_TIME               { 1 };
    constexpr int FP_STATUS             { 2 };
    constexpr int FP_LAT                { 3 };
    constexpr int FP_LAT_DIR            { 4 };
    constexpr int FP_LON                { 5 };
    constexpr int FP_LON_DIR            { 6 };
    constexpr int FP_SPEED              { 7 };
    constexpr int FP_DATE               { 9 };

    bool is_conv_ok;
    const auto fields_chksum = sentenceBytes.split(CHKSUM_SEP);
    const auto fields = fields_chksum[0].split(FIELDS_SEP);

    const auto& time = QTime::fromString(fields[FP_TIME], TIME_FORMAT);

    // Validity status A = Valid, V = Warning
    const auto is_valid = fields[FP_STATUS].at(0) == 'A';

    if (!is_valid) {
        return std::make_shared<RmcSentence>();
    }

    const auto [lat, lon] = parse_lat_lon(
        fields[FP_LAT], fields[FP_LAT_DIR].at(0),
        fields[FP_LON], fields[FP_LON_DIR].at(0)
    );

    const auto speed = fields[FP_SPEED].toDouble(&is_conv_ok);

    const auto date = QDate::fromString(fields[FP_DATE], DATE_FORMAT).addYears(100);

    return std::make_shared<RmcSentence>(RmcSentence {
        QDateTime(date, time, Qt::UTC), lat, lon, speed
    });
}
