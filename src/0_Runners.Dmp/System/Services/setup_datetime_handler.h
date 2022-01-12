#ifndef SETSYSTEMTIMECMDHANDLER_H
#define SETSYSTEMTIMECMDHANDLER_H

// oss
#include <rxcpp/rx.hpp>

#include "IRxGpsSensorPublisher.h"
#include "GpsUpdateDto.h"

class SetupDateTimeHandler {

public:
    static void handle(const GpsUpdateDto& gpsUpdate);

};

class ISetupDateTimeService {

public:
    virtual void start() = 0;

};

class RxSetupDateTimeService : public ISetupDateTimeService {

public:
    RxSetupDateTimeService(const IRxGpsSensorPublisher& gpsSensorPublisher);

    ~RxSetupDateTimeService();

    void start();

    void stop();

private:
    rxcpp::observable<GpsUpdateDto> gpsUpdateObservable;

    rxcpp::composite_subscription subGpsSensorPublisher;

};

#endif // SETSYSTEMTIMECMDHANDLER_H
