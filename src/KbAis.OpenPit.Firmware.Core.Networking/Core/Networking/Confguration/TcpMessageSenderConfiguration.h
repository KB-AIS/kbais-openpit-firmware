#ifndef TCPMESSAGESENDERCONFIGURATION_H
#define TCPMESSAGESENDERCONFIGURATION_H

// std
#include <chrono>
// qt
#include <QString>

enum class MessageSenderProtocol {
    Swom
};

struct TcpMessageSenderConfiguration {

    QString host;

    quint16 port;

    MessageSenderProtocol protocol;

    bool isEnabled;

    std::chrono::milliseconds sendInterval;

    QString GetMessageSenderName() const {
        return QString("%1:%2").arg(host).arg(port);
    };

};

#endif // TCPMESSAGESENDERCONFIGURATION_H
