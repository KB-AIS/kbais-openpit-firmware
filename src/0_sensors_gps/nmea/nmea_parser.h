#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

#include <nmea/nmea_sentence.h>

// std
#include <memory>
#include <vector>
// qt
#include <QDateTime>
#include <QIODevice>

namespace Sensors::Gps::Nmea {

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
 * \brief process_input
 * \param device
 * \param output_sentences
 */
void process_input(QIODevice& device, std::vector<std::shared_ptr<NmeaSentence>>& output_sentences);

}

#endif // NMEA_PARSER_H
