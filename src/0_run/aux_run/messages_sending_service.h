#ifndef MESSAGESSENDINGSERVICE_H
#define MESSAGESSENDINGSERVICE_H

#include "device_message.h"
// Qt
#include <QList>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
// OSS
#include <rigtorp/MPMCQueue.h>

using MessagesQueue = rigtorp::mpmc::Queue<DeviceMessage>;

using SktState = QAbstractSocket::SocketState;

using SktError = QAbstractSocket::SocketError;

class MessagesSendingService : public QObject {
    Q_OBJECT

public:
    explicit MessagesSendingService(QObject *parent = nullptr);

    void sendMessages(QList<DeviceMessage> messages);

private:
    QTcpSocket _socket;

    void handleSocketError(SktError error);

};

#endif // MESSSAGESSENDINGSERVICE_H
