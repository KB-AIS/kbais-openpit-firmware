#ifndef CONIFGURATOR_CLI_PROCESSOR_H
#define CONIFGURATOR_CLI_PROCESSOR_H

// oss
#include <CLI/CLI.hpp>
#include <plog/Severity.h>

using CliProcessor_t = CLI::App;

struct LoggerOptions {
    plog::Severity severity_level { plog::Severity::none };
};

void configure_cli_processor(CliProcessor_t& cli);

void run_setup_logger_options(const LoggerOptions& options);

#endif // CONIFGURATOR_CLI_PROCESSOR_H
