#ifndef NMEAPARSER_H
#define NMEAPARSER_H

// std
#include <memory>
#include <vector>
// qt
#include <QDateTime>
#include <QIODevice>

#include "opf/modules/sensors/gps/nmea/NmeaSentence.h"

/*!
 *  \brief Indicate NMEA sentence begining.
 */
constexpr char SENTENCE_BEG { '$' };

/*!
 * \brief Indicate NMEA sentence finish.
 */
constexpr char SENTENCE_FIN[] { '\r', '\n' };

/*!
 * \brief Max NMEA sentence length in bytes.
 */
constexpr int SENTENCE_MAX_LENGTH { 82 };

/*!
 * Static function to read a byte stream from IO device and parse it into set of NMEA sentences.
 *
 * \param device An IO device to read serialized NMEA byte stream from.
 *
 * \param output_sentences A reference to array which used to push parsed NMEA sentences into.
 */
void processDeviceRead(
    QIODevice& device,
    std::vector<std::shared_ptr<NmeaSentence>>& output_sentences
);

#endif // NMEAPARSER_H
