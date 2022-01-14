#include "LoggerConfigurator.h"

using namespace plog;

void
LoggerConfigurator::configure() {
    static ConsoleAppender<TxtFormatter> consoleAppender;

    init(debug).addAppender(&consoleAppender);
}
