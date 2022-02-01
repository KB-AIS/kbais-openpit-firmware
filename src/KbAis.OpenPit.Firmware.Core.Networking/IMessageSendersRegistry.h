#ifndef IMESSAGESENDERSREGISTRY_H
#define IMESSAGESENDERSREGISTRY_H

// qt
#include <QString>
#include <QVector>

struct MessageSenderDto {

    const QString name;

    quint64 lastSentMessageBatchId;

};

class IMessageSendersRegistry {

public:
    virtual ~IMessageSendersRegistry() noexcept = default;

    virtual QVector<MessageSenderDto> getAll() = 0;

    virtual void addOrUpdate() = 0;

};

#endif // IMESSAGESENDERSREGISTRY_H
