#ifndef NMEA_SENTENCE_H
#define NMEA_SENTENCE_H

// std
#include <functional>
#include <memory>
// qt
#include <QDateTime>

namespace Sensors::Gps::Nmea {

/*!
 * \brief Interface to mark NMEA sentences.
 */
struct NmeaSentence {
    virtual ~NmeaSentence() = default;
};

/*!
 * \brief Global positioning system fix data.
 */
struct GgaSentence : public NmeaSentence {

    GgaSentence();

    GgaSentence(const QTime&, double, double, short, short);

    const QTime time { };

    const double lat { 0.0 };

    const double lon { 0.0 };

    const short qal { 0 };

    const short sat { 0 };

};

std::shared_ptr<GgaSentence> parse_gga_sentence(const QByteArray& sentence_bytes);

struct RmcSentence : public NmeaSentence {

    RmcSentence();

    RmcSentence(const QDateTime&, double, double, double);

    const QDateTime datetime;

    const bool is_valid { false };

    const double lat { 0.0 };

    const double lon { 0.0 };

    /*!
     * \brief Speed over ground in knots
     */
    const double spd { 0.0 };

};

std::shared_ptr<RmcSentence> parse_rmc_sentence(const QByteArray& sentence_bytes);

} // Sensors::Gps::Nmea

#endif // NMEA_SENTENCE_H
