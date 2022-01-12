#ifndef MESSAGES_BATCH_H
#define MESSAGES_BATCH_H

// qt
#include <QDateTime>
#include <QMetaType>
#include <QVector>

#include <messaging/message.h>

struct MessagesBatch {

    /*!
     * \brief Messages has been collected to the batch.
     */
    QVector<Message> messages;

    /*!
     * \brief Time when messages has beent collected to the batch.
     */
    QDateTime collectedAt;

};

Q_DECLARE_METATYPE(MessagesBatch);

#endif // MESSAGES_BATCH_H
