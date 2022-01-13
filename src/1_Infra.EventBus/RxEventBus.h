#ifndef RXEVENTBUS_H
#define RXEVENTBUS_H

// std
#include <functional>
// qt
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QString>
// oss
#include <rxcpp/rx.hpp>

class RxEvent {

public:
    RxEvent(const QString& name);

    const QString& getName() const;

private:
    const QString name;

};

typedef std::function<void(const RxEvent&)> EventHandler;

class RxEventBus {

public:
    RxEventBus(const QString& name);

    // TODO: Add as_dynamic type deduction
    void post(const RxEvent& event) const;

    const QString& getName() const;

    const rxcpp::observable<RxEvent> getObservable() const;

private:
    const QString name;

    rxcpp::subjects::subject<RxEvent> subject;

};

class RxEventModule {

public:
    RxEventModule(const RxEventBus& bus);

    ~RxEventModule() noexcept;

    void attach(const RxEventBus& bus);

    void detach();

    void subscribe(const QString& eventName, EventHandler handler);

    void unsubscribe(const QString& eventName);

private:
    rxcpp::composite_subscription subscription;

    QMap<QString, EventHandler> handlers;

};

#endif // RXEVENTBUS_H
