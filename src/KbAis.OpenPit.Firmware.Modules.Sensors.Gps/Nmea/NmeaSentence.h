#ifndef NMEASENTENCE_H
#define NMEASENTENCE_H

// std
#include <functional>
#include <memory>
// qt
#include <QDateTime>

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

    const QTime time;

    const double latitude { 0.0 };

    const double longitude { 0.0 };

    const short gpsQauality { 0 };

    const short satellites { 0 };

};

struct RmcSentence : public NmeaSentence {

    RmcSentence();

    RmcSentence(const QDateTime&, double, double, double);

    const QDateTime datetime;

    const bool isValid { false };

    const double latitude { 0.0 };

    const double longitude { 0.0 };

    /*!
     * \brief Speed over ground in knots
     */
    const double speed { 0.0 };

};

std::shared_ptr<GgaSentence> parseGgaSentence(const QByteArray& sentenceBytes);

std::shared_ptr<RmcSentence> parseRmcSentence(const QByteArray& sentenceBytes);

#endif // NMEASENTENCE_H
