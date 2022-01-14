#ifndef SETSYSTEMTIMECMDHANDLER_H
#define SETSYSTEMTIMECMDHANDLER_H

// oss
#include <rxcpp/rx.hpp>

// cfw::infra::sensors::gps
#include "GpsMessage.h"
#include "IRxGpsSensorPublisher.h"

#include "System/Services/ISystemService.h"

class RxSetupDateTimeService : public ISystemService {

public:
    RxSetupDateTimeService(const IRxGpsSensorPublisher& gpsSensorPublisher);

    ~RxSetupDateTimeService() override;

    void start() override;

    void stop() override;

private:
    rxcpp::observable<GpsMessage> gpsMessageObservable;

    rxcpp::composite_subscription subGpsSensorPublisher;

};

class SetupDateTimeHandler {

public:
    SetupDateTimeHandler() = delete;

    static void handle(const GpsMessage& gpsMessage);

};

#endif // SETSYSTEMTIMECMDHANDLER_H
