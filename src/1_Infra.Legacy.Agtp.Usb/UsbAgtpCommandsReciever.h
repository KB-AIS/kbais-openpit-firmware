#ifndef USBAGTPCOMMANDSRECIEVER_H
#define USBAGTPCOMMANDSRECIEVER_H

// qt
#include <QObject>
#include <QSerialPort>

// cfw::trdparty
#include "RxQt/RxQt.h"

class UsbAgtpCommandsReciever : public QObject {
    Q_OBJECT

public:
    explicit UsbAgtpCommandsReciever();

    ~UsbAgtpCommandsReciever();

    Q_SLOT void startProcessing();

    Q_SIGNAL void processingFinished();

private:
    QSerialPort* terminal;

    rxcpp::composite_subscription subs;

    void setupTerminal();

    void resetTerminal();

};

#endif // USBAGTPCOMMANDSRECIEVER_H
