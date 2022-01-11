#ifndef RXEVENTBUS_H
#define RXEVENTBUS_H

// std
#include <functional>
// qt
#include <QMap>
#include <QString>
// oss
#include <rxcpp/rx.hpp>

class RxEvent {

public:
    RxEvent(const QString& eventName);

    const QString& getEventName() const;

private:
    const QString eventName;

};

typedef std::function<void(const RxEvent&)> EventHandler;

class RxEventBus {

public:
    void post(const RxEvent& event) const;

    const rxcpp::observable<RxEvent>& getObservable() const;

private:
    rxcpp::subjects::subject<RxEvent> subject;

};

class RxEventModule {

public:
    void attach(RxEventBus& bus);

    void detach();

    void subscribe(const QString& eventName, EventHandler handler);

    void unsubscribe(const QString& eventName);

private:
    rxcpp::composite_subscription subscription;

    QMap<QString, EventHandler> handlers;

};

#endif // RXEVENTBUS_H
