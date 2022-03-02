#ifndef IPROTOCOLCOMMUNICATOR_H
#define IPROTOCOLCOMMUNICATOR_H

// qt
#include <QIODevice>

#include "RxQt.h"

struct ProtocolViolationNotif {
    std::string message;
};

class IProtocolCommunicator {

public:   
    using NotificatorProtocolViolation_t = rxcpp::observable<ProtocolViolationNotif>;

    virtual ~IProtocolCommunicator() noexcept = default;

    virtual void InitCommunication(QIODevice& device, const QString& equipment_id) = 0;

    virtual void StopCommunication() = 0;

    virtual NotificatorProtocolViolation_t GetObservableProtocolViolation() const = 0;

};

#endif // IPROTOCOLCOMMUNICATOR_H
