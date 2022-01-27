#ifndef IRXIMMEDIATEMESSAGEMAPPER_H
#define IRXIMMEDIATEMESSAGEMAPPER_H

// oss
#include <rxcpp/rx.hpp>

#include "Message.h"

class IRxImmediateMessageMapper {

public:
    ~IRxImmediateMessageMapper() noexcept = default;

    virtual rxcpp::observable<Message> getObservable() const = 0;

};

#endif // IRXIMMEDIATEMESSAGEMAPPER_H
