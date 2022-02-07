#ifndef IRXMESSAGESENDERSDIAGPUB_H
#define IRXMESSAGESENDERSDIAGPUB_H

// qt
#include <QVector>
// oss
#include <rxcpp/rx.hpp>

struct MessageSenderDiagInfo {

    QString message_sender_name { };

    QString state_text { };

    std::optional<QString> error_text_opt { std::nullopt };

};

class IRxMessageSendersDiagPub {

public:
    using MessageSenderDiagInfos_t = std::vector<MessageSenderDiagInfo>;

    virtual ~IRxMessageSendersDiagPub() noexcept = default;

    virtual rxcpp::observable<MessageSenderDiagInfos_t> GetObservableDiagInfo() const = 0;

};

#endif // IRXMESSAGESENDERSDIAGPUB_H
