#ifndef IRXGPSSENSORPUBLISHER_H
#define IRXGPSSENSORPUBLISHER_H

// qt
#include <QObject>
// oss
#include <rxcpp/rx.hpp>

#include "GpsMessage.h"

class IRxGpsSensorPublisher : public QObject {

public:
    virtual const rxcpp::observable<GpsMessage> get_observable() const = 0;

};

#endif // IRXGPSSENSORPUBLISHER_H
