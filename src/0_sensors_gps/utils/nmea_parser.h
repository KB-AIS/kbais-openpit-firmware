#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

// Qt
#include <QDateTime>
#include <QIODevice>

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

struct GgaSentence {

public:
    /*!
     * \brief UTC of position report.
     */
    QDateTime position_time;

};

struct RmcSentence {

public:
    /*!
     * \brief UTC of position fix.
     */
    QDateTime position_time;

    bool is_valid;

    double latitude;

    double longitude;

    double speed_over_ground;

};

}


// FUNCTIONS
namespace Sensors::Gps::Nmea {

static void process_input(QIODevice& device);

}

#endif // NMEA_PARSER_H
