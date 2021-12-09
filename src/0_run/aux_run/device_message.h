#ifndef DEVICE_MESSAGE_H
#define DEVICE_MESSAGE_H

// Qt
#include <QByteArray>
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
    QByteArray payload;

};

Q_DECLARE_METATYPE(DeviceMessage);

#endif // DEVICE_MESSAGE_H
