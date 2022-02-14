#ifndef UPDATESENDERCMD_H
#define UPDATESENDERCMD_H

// qt
#include <QSqlQuery>

class MessageSenderUpdateCmd {

public:
    MessageSenderUpdateCmd();

    ~MessageSenderUpdateCmd();

    void handle(quint64 messagesBatchId) const;

private:
    QSqlQuery m_qryUpdateSenders;

};

#endif // UPDATESENDERCMD_H
