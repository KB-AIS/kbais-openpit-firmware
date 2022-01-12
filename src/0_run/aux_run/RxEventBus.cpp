#include "RxEventBus.h"

// qt
#include <QMutexLocker>
// oss
#include <plog/Log.h>

RxEvent::RxEvent(const QString &name) : name(name) {}

const QString&
RxEvent::getName() const {
    return name;
}

RxEventBus::RxEventBus(const QString& name) : name { name } {}

void
RxEventBus::post(const RxEvent& event) const {
    subject.get_subscriber().on_next(event);
}

const rxcpp::observable<RxEvent>
RxEventBus::getObservable() const {
    return subject.get_observable();
}

RxEventModule::RxEventModule(const RxEventBus& bus) {
    attach(bus);
}

RxEventModule::~RxEventModule() noexcept {
    detach();
}

void
RxEventModule::attach(const RxEventBus& bus) {
    detach();

    subscription = rxcpp::composite_subscription();

    auto handle = [&](const RxEvent& event) {
        handlers[event.getName()](event);
    };

    auto filter = [&](const RxEvent& event) {
        auto itr = handlers.constFind(event.getName());

        return itr != handlers.constEnd();
    };

    auto subscriber = rxcpp::make_subscriber<RxEvent>(subscription, handle);

    bus.getObservable().filter(filter).subscribe(subscriber);
}

void
RxEventModule::detach() {
    if (!subscription.is_subscribed()) return;

    subscription.unsubscribe();
}

void
RxEventModule::subscribe(const QString& eventName, EventHandler handler) {
    handlers.insert(eventName, handler);
}

void
RxEventModule::unsubscribe(const QString& eventName) {
    handlers.remove(eventName);
}
