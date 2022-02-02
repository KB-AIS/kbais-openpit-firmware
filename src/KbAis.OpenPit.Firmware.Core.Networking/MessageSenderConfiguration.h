#ifndef MESSAGESENDERCONFIGURATION_H
#define MESSAGESENDERCONFIGURATION_H

// std
#include <chrono>
// qt
#include <QString>

struct MessageSenderConfiguration {

    QString host;

    quint16 port;

    QString protocol;

    bool is_enabled;

    std::chrono::milliseconds send_interval;

    QString get_name() const {
        return QString("%1:%2").arg(host).arg(port);
    };

};

#endif // MESSAGESENDERCONFIGURATION_H
