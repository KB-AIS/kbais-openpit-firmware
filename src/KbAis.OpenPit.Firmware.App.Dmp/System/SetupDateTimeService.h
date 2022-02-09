#ifndef SETUPDATETIMESERVICE_H
#define SETUPDATETIMESERVICE_H

#include "IRxGpsSensorPublisher.h"

class SetupDateTimeService {

public:
    SetupDateTimeService(const IRxGpsSensorPublisher& gpsSensorPublisher);

    ~SetupDateTimeService() noexcept;

private:
    rxcpp::composite_subscription m_subscriptions;

    void handle(const GpsMessage& gpsMessage);

};

#endif // SETUPDATETIMESERVICE_H
