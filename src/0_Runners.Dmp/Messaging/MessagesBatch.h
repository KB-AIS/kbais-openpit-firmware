#ifndef MESSAGESBATCH_H
#define MESSAGESBATCH_H

// qt
#include <QDateTime>
#include <QMetaType>
#include <QVector>

#include "Messaging/Message.h"

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

#endif // MESSAGESBATCH_H
