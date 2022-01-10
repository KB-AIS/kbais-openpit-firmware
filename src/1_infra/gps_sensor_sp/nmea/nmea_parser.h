#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

// std
#include <memory>
#include <vector>
// qt
#include <QDateTime>
#include <QIODevice>

#include "nmea/nmea_sentence.h"

namespace KbAis::Cfw::Sensors::Gps::Nmea {

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
void process_input(
    QIODevice& device,
    std::vector<std::shared_ptr<NmeaSentence>>& output_sentences
);

}

#endif // NMEA_PARSER_H
