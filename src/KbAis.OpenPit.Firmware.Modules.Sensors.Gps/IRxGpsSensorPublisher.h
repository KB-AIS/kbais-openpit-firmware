#ifndef IRXGPSSENSORPUBLISHER_H
#define IRXGPSSENSORPUBLISHER_H

// qt
#include <QObject>
// oss
#include <rxcpp/rx.hpp>

#include "GpsMessage.h"

class i_gps_sensor_publisher : public QObject {

public:
    virtual ~i_gps_sensor_publisher() noexcept = default;

    virtual const rxcpp::observable<GpsMessage> get_observable() const = 0;

};

#endif // IRXGPSSENSORPUBLISHER_H
