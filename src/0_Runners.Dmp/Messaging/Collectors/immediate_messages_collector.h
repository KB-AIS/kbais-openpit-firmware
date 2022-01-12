#ifndef IMMEDIATE_MESSAGES_COLLECTOR_H
#define IMMEDIATE_MESSAGES_COLLECTOR_H

// qt
#include <QVector>
#include <QMutex>
#include <QObject>
#include <QTimer>

#include "messaging/message.h"

class ImmediateMessagesCollector : public QObject {
    Q_OBJECT

public:
    ImmediateMessagesCollector();

    QVector<Message> getMessages();

    Q_SLOT void handleMessagePlaced(const Message& message);

    Q_SIGNAL void notifyMessageCollected();

private:
    QVector<Message> collectedMessages;

    QMutex mtxCollectedMessages;

    QTimer timerMessageCollected;

};

#endif // IMMEDIATE_MESSAGES_COLLECTOR_H
