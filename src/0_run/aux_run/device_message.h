#ifndef DEVICE_MESSAGE_H
#define DEVICE_MESSAGE_H

// Qt
#include <QDateTime>
#include <QMetaType>
#include <QString>

struct DeviceMessage {

    /*!
     * Unique name use to group messages of same type.
     */
    QString moniker;

    /*!
     * Serialized content of the message.
     */
    QString payload;

    /*!
     * Time when message has been created by a device.
     */
    QDateTime createdAt;

};

Q_DECLARE_METATYPE(DeviceMessage);

#endif // DEVICE_MESSAGE_H
