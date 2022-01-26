#ifndef UPDATESENDERCMD_H
#define UPDATESENDERCMD_H

// qt
#include <QtGlobal>

class UpdateSenderCmd {

public:
    void handle(quint64 messagesBatchId) const;

};

#endif // UPDATESENDERCMD_H
