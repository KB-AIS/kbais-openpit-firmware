#ifndef MESSAGESCOLLECTORSADAPTER_H
#define MESSAGESCOLLECTORSADAPTER_H

#include "immediate_messages_collector.h"
#include "recurrent_messages_collector.h"
#include "save_device_messages_command.h"
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

    SaveDeviceMessagesCommand saveCommand;

    RecurrentMessagesCollector& recurrentCollector;

    ImmediateMessagesCollector& immediateCollector;

    QMutex collectMessagesMtx;

};

#endif // MESSAGESCOLLECTORSADAPTER_H
