#include "ConfiguratorCommandLine.h"

// oss
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

using namespace plog;

void ConfigureCommandLine(CLI::App &commandLine) {
    std::map<std::string, Severity> plogSeverityLevels {
        { "v", Severity::verbose }
    ,   { "d", Severity::debug }
    ,   { "i", Severity::info }
    ,   { "w", Severity::warning }
    ,   { "e", Severity::error }
    ,   { "f", Severity::fatal }
    };

    auto logSeverityLevel = Severity::none;

    commandLine.add_option("-l,--level", logSeverityLevel, "Log severity level")
        -> transform(CLI::CheckedTransformer(plogSeverityLevels, CLI::ignore_case));

    commandLine.callback([&logSeverityLevel]() {
        static ConsoleAppender<TxtFormatter> consoleAppender;

        init(logSeverityLevel).addAppender(&consoleAppender);
    });
}
