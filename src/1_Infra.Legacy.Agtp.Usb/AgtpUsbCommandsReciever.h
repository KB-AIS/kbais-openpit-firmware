#ifndef AGTPUSBCOMMANDSRECIEVER_H
#define AGTPUSBCOMMANDSRECIEVER_H


// qt
#include <QObject>
#include <QSerialPort>

// cfw::trdparty
#include "RxQt/RxQt.h"

#include "IAgtpCommandsReciever.h"
#include "AgtpRequestSender.h"

class AgtpUsbCommandsReciever : public IAgtpCommandsReciever {
    Q_OBJECT

public:
    explicit AgtpUsbCommandsReciever(const AgtpRequestsSender& mediator);

    ~AgtpUsbCommandsReciever();

    Q_SLOT void startProcessing() override;

    Q_SIGNAL void processingFinished();

private:
    const AgtpRequestsSender& mMediator;

    QSerialPort* mSpUsbDevice;

    rxcpp::composite_subscription mSubs;

    void setupTerminal();

    void resetTerminal();

};

#endif // AGTPUSBCOMMANDSRECIEVER_H
