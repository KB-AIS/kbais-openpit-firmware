#ifndef SPDLOG_QT_SUPPORT_H
#define SPDLOG_QT_SUPPORT_H

// qt
#include <QString>

// oss
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

std::ostream &operator<<(std::ostream& os, QString string);

#endif // SPDLOG_QT_SUPPORT_H
