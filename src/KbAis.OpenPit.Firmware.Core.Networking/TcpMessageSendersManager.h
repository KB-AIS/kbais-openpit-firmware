#ifndef TCPMESSAGESENDERSMANAGER_H
#define TCPMESSAGESENDERSMANAGER_H

// qt
#include <QObject>

// Core.Configuration
#include "IRxConfigurationChangePublisher.h"

#include "IMessageSendersManager.h"
#include "IRxMessageSendersStatusPublisher.h"

class TcpMessageSendersManager
    :   public QObject
    ,   public IMessageSendersManager
    ,   public IRxMessageSendersStatusPublisher
{
    Q_OBJECT

public:
    explicit TcpMessageSendersManager(
        const IRxConfigurationChangePublisher& configurationChangePublisher
    );

    void applyConfiguration(const MessageSenderConfiguration& configuration) override;

    rxcpp::observable<QVector<MessageSenderStatus>> getObservable() const override;

private:
    void restartMessageSendersWithError();

};

#endif // TCPMESSAGESENDERSMANAGER_H
