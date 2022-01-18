#ifndef LEGACYCONFIGSERVICE_H
#define LEGACYCONFIGSERVICE_H

// qt
#include <QObject>
#include <QSerialPort>

// cfw::trdparty
#include "RxQt/RxQt.h"

class LegacyConfigService : public QObject {
    Q_OBJECT

public:
    explicit LegacyConfigService();

    ~LegacyConfigService();

    Q_SLOT void start();

    Q_SIGNAL void finished();

private:
    QSerialPort* terminal;

    rxcpp::composite_subscription subs;

    void setupTerminal();

    void resetTerminal();

};

#endif // LEGACYCONFIGSERVICE_H
