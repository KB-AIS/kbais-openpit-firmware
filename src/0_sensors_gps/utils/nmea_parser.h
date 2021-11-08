#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

// Qt
#include <QDateTime>
#include <QIODevice>
#include <QVector>

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
 * \brief The ISentence struct
 */
struct ISentence { };

/*!
 * \brief The GgaSentence struct
 */
struct GgaSentence : ISentence {

public:
    /*!
     * \brief UTC of position report.
     */
    QDateTime position_time;

};

/*!
 * \brief The RmcSentence struct
 */
struct RmcSentence : ISentence {

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

/*!
 * \brief process_input
 * \param device
 * \param output
 */
void process_input(QIODevice& device, QVector<ISentence>& output_sentences);

}

#endif // NMEA_PARSER_H
