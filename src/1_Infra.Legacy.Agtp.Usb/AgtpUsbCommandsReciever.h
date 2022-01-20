#ifndef AGTPUSBCOMMANDSRECIEVER_H
#define AGTPUSBCOMMANDSRECIEVER_H


// qt
#include <QObject>
#include <QSerialPort>

// cfw::trdparty
#include "RxQt/RxQt.h"

#include "AgtpCommandsMediator.h"

class AgtpUsbCommandsReciever : public QObject {
    Q_OBJECT

public:
    explicit AgtpUsbCommandsReciever(const AgtpCommandsMediator& mediator);

    AgtpUsbCommandsReciever(const AgtpUsbCommandsReciever& other) : QObject(), mMediator { other.mMediator } {};

    ~AgtpUsbCommandsReciever();

    Q_SLOT void startProcessing();

    Q_SIGNAL void processingFinished();

private:
    const AgtpCommandsMediator& mMediator;

    QSerialPort* mSpUsbDevice;

    rxcpp::composite_subscription mSubs;

    void setupTerminal();

    void resetTerminal();

};

#endif // AGTPUSBCOMMANDSRECIEVER_H
