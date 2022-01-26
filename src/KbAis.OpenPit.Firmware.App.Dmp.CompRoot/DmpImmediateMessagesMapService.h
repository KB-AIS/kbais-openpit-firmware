#ifndef DMPIMMEDIATEMESSAGESMAPSERVICE_H
#define DMPIMMEDIATEMESSAGESMAPSERVICE_H

// qt
#include <QObject>

// cfw::inter::views::dmp
#include "MainView.h"
// cfw::trdpary
#include "RxQt.h"

#include "Collectors/ImmediateMessagesCollector.h"
#include "Message.h"

class DmpImmediateMessagesMapService : public QObject {
    Q_OBJECT

public:
    DmpImmediateMessagesMapService(
        ImmediateMessagesCollector& messagesCollector,
        const MainView& mainView
    );

private:
    rxcpp::composite_subscription subs;

    Q_SIGNAL void messageMapped(const Message& message);
};

#endif // DMPIMMEDIATEMESSAGESMAPSERVICE_H
