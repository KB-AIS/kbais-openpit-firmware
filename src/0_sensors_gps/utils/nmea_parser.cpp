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

typedef std::function<void(const QByteArray&)> sentence_parser;

static void parse_gga_sentence(const QByteArray &sentence_bytes) {
    PLOGD << "Consumed sentence: " << sentence_bytes;
}

static void parse_rmc_sentence(const QByteArray &sentence_bytes) {
    PLOGD << "Consumed sentence: " << sentence_bytes;
}

static const std::map<QString, sentence_parser> sentence_parsers {
    { "GGA", parse_gga_sentence },
    { "RMC", parse_rmc_sentence },
};

// FUNCTIONS
static void process_input(QIODevice& device) {
    const auto bytes = device.peek(PEEK_SIZE);

    unsigned int bytes_scaned { 0 };

    // TODO: Add explicit break condition
    forever {
        const auto sentence_beg_pos = bytes.indexOf(SENTENCE_BEG, bytes_scaned);
        if (sentence_beg_pos == -1) break;

        bytes_scaned = sentence_beg_pos;

        const auto sentence_fin_pos = bytes.indexOf(SENTENCE_FIN, bytes_scaned);
        if (sentence_fin_pos == -1) break;

        bytes_scaned = sentence_fin_pos;

        const auto sentence_len = sentence_fin_pos - sentence_beg_pos;
        const auto sentence_bytes = bytes.mid(sentence_beg_pos, sentence_len);

        const auto cmd_name = sentence_bytes.mid(CMD_POS, CMD_LEN);
        const auto itr = sentence_parsers.find(cmd_name);

        if (itr != sentence_parsers.end()) {
            itr->second(sentence_bytes);
        }
    }

    auto bytes_to_read = bytes.length() - bytes_scaned;
    device.read(bytes_to_read);
};

} // Sensors::Gps::Nmea
