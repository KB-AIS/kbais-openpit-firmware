#ifndef MESSAGESCOLLECTORSADAPTER_H
#define MESSAGESCOLLECTORSADAPTER_H

#include "recurrent_messages_collector.h"
#include "immediate_messages_collector.h"
// Qt
#include <QMutex>
#include <QObject>
// QDeferred
#include <QLambdaThreadWorker>

class MessagesCollectorsAdapter : public QObject {
    Q_OBJECT

public:
    explicit MessagesCollectorsAdapter(
        RecurrentMessagesCollector& recurrentCollector,
        ImmediateMessagesCollector& immediateCollector,
        QObject* parent = nullptr
    );

    Q_SIGNAL void notifyMessagesCollected();

private:
    void collectMessages();

    QLambdaThreadWorker threadWorker;

    RecurrentMessagesCollector& recurrentCollector;

    ImmediateMessagesCollector& immediateCollector;

    QMutex collectMessagesMtx;

};

#endif // MESSAGESCOLLECTORSADAPTER_H
