#include "spdlog_qt_support.h"

std::ostream &operator<<(std::ostream& os, QString string) {
   return os << string.toStdString();
}
