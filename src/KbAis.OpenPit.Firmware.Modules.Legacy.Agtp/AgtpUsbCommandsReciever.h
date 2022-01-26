#ifndef AGTPUSBCOMMANDSRECIEVER_H
#define AGTPUSBCOMMANDSRECIEVER_H

// qt
#include <QObject>
#include <QSerialPort>

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "IAgtpRequestsReciever.h"
#include "IAgtpRequestsMediator.h"

class AgtpUsbRequestsReciever : public IAgtpRequetsReciever {
    Q_OBJECT

public:
    explicit AgtpUsbRequestsReciever(const IAgtpRequestsMediator& mediator);

    ~AgtpUsbRequestsReciever();

    Q_SLOT void start() override;

    Q_SIGNAL void processingFinished();

private:
    const IAgtpRequestsMediator& mMediator;

    QSerialPort* mSpUsbDevice;

    rxcpp::composite_subscription mSubs;

    void setupTerminal();

    void resetTerminal();

};

#endif // AGTPUSBCOMMANDSRECIEVER_H
