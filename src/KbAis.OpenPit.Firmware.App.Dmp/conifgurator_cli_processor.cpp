#include "conifgurator_cli_processor.h"

// oss
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

using namespace plog;

std::map<std::string, Severity> severity_level_map {
    { "v", Severity::verbose }, { "d", Severity::debug }, { "i", Severity::info }
,   { "w", Severity::warning }, { "e", Severity::error }, { "f", Severity::fatal }
};

void configure_cli_processor(CliProcessor_t& cli) {
    auto logger_options = std::make_shared<LoggerOptions>();

    cli.add_option("-l,--log-level", logger_options->severity_level, "Log severity level")
        ->transform(CLI::CheckedTransformer(severity_level_map, CLI::ignore_case));

    cli.callback([logger_options]() { run_setup_logger_options(*logger_options); });
}

void run_setup_logger_options(const LoggerOptions& options) {
    static ConsoleAppender<TxtFormatter> console_appender;

    init(options.severity_level).addAppender(&console_appender);
}
