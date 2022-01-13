#ifndef IMESSAGESCACHINGSERVICE_H
#define IMESSAGESCACHINGSERVICE_H

// qt
#include <QObject>

class IMessagesCachingService : public QObject {

public:
    virtual ~IMessagesCachingService() noexcept = default;

};

#endif // IMESSAGESCACHINGSERVICE_H
