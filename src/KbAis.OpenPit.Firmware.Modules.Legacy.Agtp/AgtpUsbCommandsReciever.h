#ifndef AGTPUSBCOMMANDSRECIEVER_H
#define AGTPUSBCOMMANDSRECIEVER_H

// qt
#include <QObject>
#include <QSerialPort>

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "IAgtpRequestsMediator.h"
#include "IAgtpRequestsReciever.h"

class AgtpUsbRequestsReciever : public IAgtpRequetsReciever {
    Q_OBJECT

public:
    explicit AgtpUsbRequestsReciever(const IAgtpRequestsMediator& mediator);

    ~AgtpUsbRequestsReciever();

    Q_SLOT void Start() override;

    Q_SIGNAL void ProcessingFinished();

private:
    const IAgtpRequestsMediator& m_mediator;

    QSerialPort* m_spUsbDevice;

    rxcpp::composite_subscription m_subscriptions;

    void SetupTerminal();

    void ResetTerminal();

};

#endif // AGTPUSBCOMMANDSRECIEVER_H
