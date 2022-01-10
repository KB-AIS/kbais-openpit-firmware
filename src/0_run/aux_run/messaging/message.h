#ifndef MESSAGE_H
#define MESSAGE_H

// Qt
#include <QByteArray>
#include <QDateTime>
#include <QMetaType>
#include <QString>

struct Message {

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

Q_DECLARE_METATYPE(Message);

#endif // MESSAGE_H
