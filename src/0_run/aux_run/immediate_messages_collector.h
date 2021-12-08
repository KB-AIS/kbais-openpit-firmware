#ifndef IMMEDIATEMESSAGESCOLLECTOR_H
#define IMMEDIATEMESSAGESCOLLECTOR_H

#include "device_message.h"
// Qt
#include <QList>
#include <QMutex>
#include <QObject>
#include <QTimer>

class ImmediateMessagesCollector : public QObject {
    Q_OBJECT

public:
    explicit ImmediateMessagesCollector(QObject *parent = nullptr);

    QVector<DeviceMessage> popMessages();

    Q_SLOT void handleMessageReceived(const DeviceMessage& msg);

    Q_SIGNAL void notifyMessageCollected();

private:
    void setupFireMessageCollectorTimer();

    QVector<DeviceMessage> collectedMsgs;

    QMutex internalStoreMtx;

    QTimer fireMessageCollectedTimer;

};

#endif // IMMEDIATEMESSAGESCOLLECTOR_H
