#ifndef MESSAGESENDERSMANAGER_H
#define MESSAGESENDERSMANAGER_H

#include "networking/base_protocol_formatter.h"
// Qt
#include <QHostAddress>
#include <QObject>

namespace kbais::cfw::networking {

class MessageSendersManager : public QObject{
    Q_OBJECT

public:
    explicit MessageSendersManager(QObject* parent = nullptr);

private:
    void registerSender();

};

} // kbais::cfw::networking

#endif // MESSAGESENDERSMANAGER_H
