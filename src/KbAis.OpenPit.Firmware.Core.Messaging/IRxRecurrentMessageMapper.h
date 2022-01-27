#ifndef IRXRECURRENTMESSAGEMAPPER_H
#define IRXRECURRENTMESSAGEMAPPER_H

// oss
#include <rxcpp/rx.hpp>

#include "Message.h"

class IRxRecurrentMessageMapper {

public:
    ~IRxRecurrentMessageMapper() noexcept = default;

    virtual rxcpp::observable<Message> getObservable() const = 0;

};

#endif // IRXRECURRENTMESSAGEMAPPER_H
