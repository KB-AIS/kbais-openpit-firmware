#ifndef IAGTPREQUESTSRECIEVER_H
#define IAGTPREQUESTSRECIEVER_H

// qt
#include <QObject>

class IAgtpRequetsReciever : public QObject {

public:
    virtual ~IAgtpRequetsReciever() noexcept = default;

    virtual void start() = 0;

};

#endif // IAGTPREQUESTSRECIEVER_H
