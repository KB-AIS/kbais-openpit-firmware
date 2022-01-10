#include "nmea_parser.h"

// std
#include <functional>
#include <map>
// qt
#include <QDebug>
#include <QTime>

using std::shared_ptr, std::vector;

namespace KbAis::Cfw::Sensors::Gps::Nmea {

constexpr int PEEK_SIZE { 8096 };

constexpr int CMD_LEN { 3 };

constexpr int CMD_POS { 3 };

typedef std::function<shared_ptr<NmeaSentence>(const QByteArray&)> sentence_parser;

static const std::map<QString, sentence_parser> sentence_parsers {
    { "GGA", parseGgaSentence },
    { "RMC", parseRmcSentence },
};

void process_input(QIODevice& device, vector<shared_ptr<NmeaSentence>>& output_sentences) {
    const auto bytes = device.peek(PEEK_SIZE);

    unsigned int bytes_scaned { 0u };

    // TODO: Add explicit break condition. Possible infinite loop with scaning same bytes.
    forever {
        // Try to find the begining of a sentence in peeking byte fragment.
        const auto sentence_beg_pos = bytes.indexOf(SENTENCE_BEG, bytes_scaned);
        if (sentence_beg_pos == -1) break;

        // In case of success move index of scaned bytes to beginig of the new sentence.
        bytes_scaned = sentence_beg_pos;

        // Repeat same steps with sentence's end.
        const auto sentence_fin_pos = bytes.indexOf(SENTENCE_FIN, bytes_scaned);
        if (sentence_fin_pos == -1) break;

        if (sentence_fin_pos <= sentence_beg_pos) {
            qFatal("Invalid state during processing NMEA input");

            throw 0;
        }

        bytes_scaned = sentence_fin_pos;

        const auto sentence_bytes_len = sentence_fin_pos - sentence_beg_pos;
        const auto sentence_bytes = bytes.mid(sentence_beg_pos, sentence_bytes_len);

        // Get sentence name and select parser for it.
        const auto sentence_name = sentence_bytes.mid(CMD_POS, CMD_LEN);
        const auto itr = sentence_parsers.find(sentence_name);

        if (itr != sentence_parsers.end()) {
            output_sentences.push_back(itr->second(sentence_bytes));
        }
    }
    // Mark scaned bytes as processed and move device's buffer forward.
    device.read(bytes_scaned);
};

}
