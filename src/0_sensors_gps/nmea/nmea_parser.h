#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

// std
#include <memory>
// qt
#include <QDateTime>
#include <QIODevice>
#include <QVector>

using std::shared_ptr;

// CONSTANTS
namespace Sensors::Gps::Nmea {

    /*!
     *  \brief Indicate NMEA sentence begining.
     */
    const char SENTENCE_BEG { '$' };

    /*!
     * \brief Indicate NMEA sentence finish.
     */
    const char SENTENCE_FIN[] { '\r', '\n' };

    /*!
     * \brief Max NMEA sentence length in bytes.
     */
    const int SENTENCE_MAX_LENGTH { 82 };

}

// SENTENCES
namespace Sensors::Gps::Nmea {

/*!
 * Interface to mark NMEA sentences
 */
struct ISentence {
    virtual ~ISentence() = default;
};

struct GgaSentence : public ISentence {
    QDateTime position_time;
};

struct RmcSentence : public ISentence {
    QDateTime position_time;

    bool is_valid;

    double latitude;

    double longitude;

    double speed_over_ground;
};

}

// FUNCTIONS
namespace Sensors::Gps::Nmea {

void process_input(QIODevice& device, QVector<shared_ptr<ISentence>>& output_sentences);

}

#endif // NMEA_PARSER_H
