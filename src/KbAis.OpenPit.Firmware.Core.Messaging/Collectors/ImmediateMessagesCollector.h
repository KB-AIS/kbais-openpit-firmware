#ifndef IMMEDIATEMESSAGESCOLLECTOR_H
#define IMMEDIATEMESSAGESCOLLECTOR_H

// qt
#include <QMutex>
#include <QObject>
#include <QTimer>
#include <QVector>

#include "Message.h"

class ImmediateMessagesCollector : public QObject {
    Q_OBJECT

public:
    ImmediateMessagesCollector();

    QVector<Message> getMessages();

    void placeMessage(const Message& message);

    Q_SIGNAL void messageCollected();

private:
    QVector<Message> collectedMessages;

    QMutex mtxCollectedMessages;

    QTimer timerMessageCollected;

};

#endif // IMMEDIATEMESSAGESCOLLECTOR_H
