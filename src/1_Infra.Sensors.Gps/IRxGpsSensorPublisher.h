#ifndef IRXGPSSENSORPUBLISHER_H
#define IRXGPSSENSORPUBLISHER_H

// qt
#include <QObject>
// oss
#include <rxcpp/rx.hpp>

#include "GpsUpdateDto.h"

class IRxGpsSensorPublisher : public QObject {

public:
    virtual const rxcpp::observable<GpsUpdateDto> getObservable() const = 0;

};

#endif // IRXGPSSENSORPUBLISHER_H
