#include "TcpMessageSendersManager.h"

TcpMessageSendersManager::TcpMessageSendersManager(
    const IRxConfigurationChangePublisher& configurationChangePublisher
)
    :   QObject()
{

}

void
TcpMessageSendersManager::applyConfiguration(const MessageSenderConfiguration& configuration) {

}

rxcpp::observable<QVector<MessageSenderStatus>>
TcpMessageSendersManager::getObservable() const {

}

void
TcpMessageSendersManager::restartMessageSendersWithError() {

}
