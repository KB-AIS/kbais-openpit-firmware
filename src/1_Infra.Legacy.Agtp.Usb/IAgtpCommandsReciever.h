#ifndef IAGTPCOMMANDSRECIEVER_H
#define IAGTPCOMMANDSRECIEVER_H

// qt
#include <QObject>

class IAgtpCommandsReciever : public QObject {

public:
    virtual ~IAgtpCommandsReciever() noexcept = default;

    virtual void startProcessing() = 0;

};

#endif // IAGTPCOMMANDSRECIEVER_H
