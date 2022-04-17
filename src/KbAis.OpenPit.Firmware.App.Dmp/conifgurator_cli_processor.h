#ifndef CONIFGURATOR_CLI_PROCESSOR_H
#define CONIFGURATOR_CLI_PROCESSOR_H

// oss, cli11
#include <CLI/CLI.hpp>
// oss, plog
#include <plog/Severity.h>

using cli_processor_t = CLI::App;

void configure_cli_processor(cli_processor_t& cli);

struct logging_options {

    plog::Severity level { plog::Severity::none };

};

void run_use_logging(logging_options& opts);

struct simulating_options {

    std::vector<std::string> scenarios;

};

void run_use_simulating(simulating_options& opts);

#endif // CONIFGURATOR_CLI_PROCESSOR_H
