#ifndef IMESSAGESCACHINGSERVICE_H
#define IMESSAGESCACHINGSERVICE_H

// oss
#include <rxcpp/rx.hpp>

class IMessagesCachingService {

public:
    virtual ~IMessagesCachingService() noexcept = default;

    virtual void StartWorkOn(rxcpp::observe_on_one_worker& coordination) = 0;

};

#endif // IMESSAGESCACHINGSERVICE_H
