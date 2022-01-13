#ifndef RECURRENTMESSAGESCOLLECTOR_H
#define RECURRENTMESSAGESCOLLECTOR_H

// qt
#include <QHash>
#include <QList>
#include <QMutex>
#include <QObject>

#include "Messaging/Message.h"

class RecurrentMessagesCollector : public QObject {
    Q_OBJECT

public:
    RecurrentMessagesCollector();

    QVector<Message> getMessages();

    void placeMessage(const Message& message);

private:
    QHash<QString, Message> collectedMessages;

    QMutex mtxCollectedMessages;

};

#endif // RECURRENTMESSAGESCOLLECTOR_H
