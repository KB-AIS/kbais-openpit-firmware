#ifndef MESSAGESENDERCONFIGURATION_H
#define MESSAGESENDERCONFIGURATION_H

// std
#include <chrono>
// qt
#include <QString>

struct MessageSenderConfiguration {

    QString name;

    QString host;

    quint16 port;

    QString protocol;

    bool isEnabled;

    std::chrono::milliseconds sendInterval;

};

#endif // MESSAGESENDERCONFIGURATION_H
