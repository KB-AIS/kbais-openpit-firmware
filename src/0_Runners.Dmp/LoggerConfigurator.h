#ifndef LOGGERCONFIGURATOR_H
#define LOGGERCONFIGURATOR_H

#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

class LoggerConfigurator {

public:
    LoggerConfigurator() = delete;

    static void configure();

};

#endif // LOGGERCONFIGURATOR_H
