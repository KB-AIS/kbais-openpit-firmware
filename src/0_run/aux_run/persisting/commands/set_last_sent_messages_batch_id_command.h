#ifndef SET_LAST_SENT_MESSAGES_BATCH_ID_COMMAND_H
#define SET_LAST_SENT_MESSAGES_BATCH_ID_COMMAND_H

// qt
#include <QtGlobal>

class SetLastSentMessagesBatchIdCommand {

public:
    void handle(quint64 messagesBatchId) const;

};

#endif // SET_LAST_SENT_MESSAGES_BATCH_ID_COMMAND_H
