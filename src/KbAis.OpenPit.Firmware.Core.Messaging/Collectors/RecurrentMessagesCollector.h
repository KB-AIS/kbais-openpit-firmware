#ifndef RECURRENTMESSAGESCOLLECTOR_H
#define RECURRENTMESSAGESCOLLECTOR_H

// qt
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QVector>

#include "Messaging/DmpRecurrentMessagesMapService.h"
#include "Messaging/Message.h"

class RecurrentMessagesCollector : public QObject {
    Q_OBJECT

public:
    RecurrentMessagesCollector(
        const DmpRecurrentMessagesMapService& messagesMapSerivce
    );

    QVector<Message> getMessages();

private:
    rxcpp::composite_subscription mSubs;

    QHash<QString, Message> mCollectedMessages;

    QMutex mMtxCollectedMessages;

};

#endif // RECURRENTMESSAGESCOLLECTOR_H
