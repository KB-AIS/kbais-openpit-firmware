#ifndef IRXMESSAGESENDERSDIAGPUB_H
#define IRXMESSAGESENDERSDIAGPUB_H

// qt
#include <QVector>
// oss
#include <rxcpp/rx.hpp>

struct MessageSenderDiagInfo {

    QString messageSenderName { };

    QString stateText { };

    std::optional<QString> errorText { std::nullopt };

    friend bool operator==(const MessageSenderDiagInfo& a, const MessageSenderDiagInfo& b) {
        return std::tie(a.messageSenderName, a.stateText, a.errorText)
            == std::tie(b.messageSenderName, b.stateText, b.errorText);
    }

};

class IRxMessageSendersDiagPub {

public:
    using MessageSenderDiagInfos_t = std::vector<MessageSenderDiagInfo>;

    virtual ~IRxMessageSendersDiagPub() noexcept = default;

    virtual rxcpp::observable<MessageSenderDiagInfos_t> GetObservableDiagInfo() const = 0;

};

#endif // IRXMESSAGESENDERSDIAGPUB_H
