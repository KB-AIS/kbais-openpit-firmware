#ifndef SENTENCES_H
#define SENTENCES_H

// std
#include <functional>
#include <memory>
// qt
#include <QDateTime>

namespace Sensors::Gps::Nmea {

/*!
 * \brief Interface to mark NMEA sentences.
 */
struct ISentence {
    virtual ~ISentence() = default;
};

/*!
 * \brief Global positioning system fix data.
 */
struct GgaSentence : public ISentence {

    GgaSentence() {}

    GgaSentence(
        const QTime& time,
        double lat,
        double lon,
        short gps_quality,
        short satellites) :
        time { time },
        lat { lat }, log { lon },
        gps_quality { gps_quality },
        satellites { satellites } {}

    const QTime time { };

    const double lat { 0.0 };

    const double log { 0.0 };

    const short gps_quality { 0 };

    const short satellites { 0 };

};

struct RmcSentence : public ISentence {

    RmcSentence() {}

    RmcSentence(
        const QDateTime& datetime,
        double lat,
        double lon,
        double speed) :
        datetime { datetime },
        is_valid { true },
        lat { lat }, lon { lon },
        speed { speed } {}

    const QDateTime datetime;

    const bool is_valid { false };

    const double lat { 0.0 };

    const double lon { 0.0 };

    /*!
     * \brief Speed over ground in knots
     */
    const double speed { 0.0 };

};

std::shared_ptr<GgaSentence> parse_gga_sentence(const QByteArray& sentence_bytes);

std::shared_ptr<RmcSentence> parse_rmc_sentence(const QByteArray& sentence_bytes);

}

#endif // SENTENCES_H
