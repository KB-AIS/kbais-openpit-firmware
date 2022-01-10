#ifndef RECURRENT_MESSAGES_COLLECTOR_H
#define RECURRENT_MESSAGES_COLLECTOR_H

// qt
#include <QHash>
#include <QList>
#include <QMutex>
#include <QObject>

#include "messaging/message.h"

class RecurrentMessagesCollector : public QObject {
    Q_OBJECT

public:
    RecurrentMessagesCollector();

    QVector<Message> getMessages();

    Q_SLOT void handleMessagePlaced(const Message& message);

private:
    QHash<QString, Message> collectedMessages;

    QMutex mtxCollectedMessages;

};

#endif // RECURRENT_MESSAGES_COLLECTOR_H
