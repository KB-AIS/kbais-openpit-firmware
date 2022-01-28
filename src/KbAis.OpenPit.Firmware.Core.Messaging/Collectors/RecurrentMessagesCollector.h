#ifndef RECURRENTMESSAGESCOLLECTOR_H
#define RECURRENTMESSAGESCOLLECTOR_H

// qt
#include <QHash>
#include <QMutex>
#include <QVector>

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "IRxRecurrentMessageMapper.h"
#include "Message.h"

class RecurrentMessagesCollector {
    using RecurrentMessageMappers_t = std::vector<std::shared_ptr<IRxRecurrentMessageMapper>>;

public:
    RecurrentMessagesCollector(RecurrentMessageMappers_t mappers);

    ~RecurrentMessagesCollector();

    void startCollectingOn(const rxqt::run_loop& loop);

    QVector<Message> dumpMessages();

private:
    rxcpp::observable<Message> mObservable;

    rxcpp::composite_subscription mSubs;

    QHash<QString, Message> mMessages;

    QMutex mMtxMessages;

};

#endif // RECURRENTMESSAGESCOLLECTOR_H
