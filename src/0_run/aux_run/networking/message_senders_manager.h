#ifndef MESSAGESENDERSMANAGER_H
#define MESSAGESENDERSMANAGER_H

// Qt
#include <QObject>

namespace kbais::cfw::networking {

class MessageSendersManager : public QObject{
    Q_OBJECT

public:
    explicit MessageSendersManager(QObject* parent = nullptr);

};

} // kbais::cfw::networking

#endif // MESSAGESENDERSMANAGER_H
