#ifndef DEVICE_MESSAGE_H
#define DEVICE_MESSAGE_H

// Qt
#include <QByteArray>
#include <QDateTime>
#include <QMetaType>
#include <QString>

struct DeviceMessage {

    /*!
     * \brief Unique name use to group messages of same type.
     */
    QString moniker;

    /*!
     * \brief Serialized content of the message in MessagePack format.
     */
    QByteArray payload;

    /*!
     * \brief Time when message has been actually produced.
     */
    QDateTime producedAt;

};

struct DeviceMessageBatch {

    /*!
     * \brief A unique identifier of a batch.
     */
    quint64 id;

    /*!
     * \brief Time when messages has beent collected to the batch.
     */
    QDateTime collectedAt;

    /*!
     * \brief Messages has been collected to the batch.
     */
    QList<DeviceMessage> messages;

};

Q_DECLARE_METATYPE(DeviceMessage);

Q_DECLARE_METATYPE(DeviceMessageBatch);

#endif // DEVICE_MESSAGE_H
