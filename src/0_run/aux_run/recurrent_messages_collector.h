#ifndef RECURRENTMESSAGESCOLLECTOR_H
#define RECURRENTMESSAGESCOLLECTOR_H

#include "device_message.h"
// Qt
#include <QHash>
#include <QList>
#include <QMutex>
#include <QObject>

class RecurrentMessagesCollector : public QObject {
    Q_OBJECT

public:
    explicit RecurrentMessagesCollector(QObject *parent = nullptr);

    QVector<DeviceMessage> popMessages();

    Q_SLOT void handleMessageReceived(const DeviceMessage& msg);

private:
    QHash<QString, DeviceMessage> collectedMsgs;

    QMutex internalStoreMtx;

};

#endif // RECURRENTMESSAGESCOLLECTOR_H
