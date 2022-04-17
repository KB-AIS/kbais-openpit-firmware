#ifndef I_MESSAGE_CACHING_SERVICE_H
#define I_MESSAGE_CACHING_SERVICE_H

// oss
#include <rxcpp/rx.hpp>

class i_message_caching_service {

public:
    virtual ~i_message_caching_service() noexcept = default;

    virtual void start_working_on(const rxcpp::observe_on_one_worker& coordination) = 0;

};

#endif // I_MESSAGE_CACHING_SERVICE_H
