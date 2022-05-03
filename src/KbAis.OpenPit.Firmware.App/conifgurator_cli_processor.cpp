#include "conifgurator_cli_processor.h"

// oss
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>
#include <range/v3/all.hpp>

using namespace plog;

std::map<std::string, Severity> severity_level_map {
    { "fatal",   Severity::fatal }
,   { "error",   Severity::error }
,   { "warning", Severity::warning }
,   { "info",    Severity::info }
,   { "debug",   Severity::debug }
,   { "verbose", Severity::verbose }
};

void configure_cli_processor(cli_processor_t& cli) {
    auto logging_opts = std::make_shared<logging_options>();
    const auto use_logging_subcommand = cli.add_subcommand("use-logging");
    use_logging_subcommand->add_option("-l,--level", logging_opts->level, "Log severity level")
        ->transform(CLI::CheckedTransformer(severity_level_map, CLI::ignore_case));
    use_logging_subcommand->callback([logging_opts]() { run_use_logging(*logging_opts); });

    auto simulating_opts = std::make_shared<simulating_options>();
    const auto use_simulating_subcommand = cli.add_subcommand("use-simulating");
    use_simulating_subcommand->add_option("-s,--scenarios", simulating_opts->scenarios)
        ->required();
    use_simulating_subcommand->callback([simulating_opts]() { run_use_simulating(*simulating_opts); });
}

void run_use_logging(logging_options& opts) {
    static ConsoleAppender<TxtFormatter> console_appender;

    init(opts.level).addAppender(&console_appender);
}

void run_use_simulating(simulating_options& opts) {
    ranges::for_each(opts.scenarios, [&](std::string& y) {
        PLOGD << y;
    });
}
