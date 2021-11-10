#include "nmea_parser.h"

// std
#include <functional>
#include <map>
// plog
#include <plog/Log.h>

namespace Sensors::Gps::Nmea {

const int PEEK_SIZE { 1024 };

const int CMD_LEN { 3 };

const int CMD_POS { 3 };

typedef std::function<shared_ptr<ISentence>(const QByteArray&)> sentence_parser;

static shared_ptr<ISentence> parse_gga_sentence(const QByteArray& sentence_bytes) {
    PLOGD << "Consumed sentence: " << sentence_bytes;

    return std::make_shared<GgaSentence>(GgaSentence { });
}

static shared_ptr<ISentence>parse_rmc_sentence(const QByteArray& sentence_bytes) {
    PLOGD << "Consumed sentence: " << sentence_bytes;

    return std::make_shared<RmcSentence>(RmcSentence { });
}

static const std::map<QString, sentence_parser> sentence_parsers {
    { "GGA", parse_gga_sentence },
    { "RMC", parse_rmc_sentence },
};

// FUNCTIONS
void process_input(QIODevice& device, QVector<shared_ptr<ISentence>>& output_sentences) {
    const auto bytes = device.peek(PEEK_SIZE);

    unsigned int bytes_scaned { 0 };

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

        // TODO: Process invalid senetence
        if (sentence_fin_pos <= sentence_beg_pos) {
            PLOGE << "Meet invalid state during processing NMEA input";
            throw 0;
        }

        bytes_scaned = sentence_fin_pos;

        const auto sentence_len = sentence_fin_pos - sentence_beg_pos;
        const auto sentence_bytes = bytes.mid(sentence_beg_pos, sentence_len);

        const auto cmd_name = sentence_bytes.mid(CMD_POS, CMD_LEN);
        const auto itr = sentence_parsers.find(cmd_name);

        if (itr != sentence_parsers.end()) {
            auto sentence = itr->second(sentence_bytes);

            output_sentences.append(sentence);
        }
    }

    // Mark scaned bytes as processed and move device's buffer forward.
    auto bytes_to_read = bytes.length() - bytes_scaned;
    device.read(bytes_to_read);
};

} // Sensors::Gps::Nmea
