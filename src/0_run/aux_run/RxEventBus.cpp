#include "RxEventBus.h"

RxEvent::RxEvent(const QString &eventName) : eventName(eventName) {}

const QString&
RxEvent::getEventName() const {
    return eventName;
}

void
RxEventBus::post(const RxEvent& event) const {
    subject.get_subscriber().on_next(event);
}

const rxcpp::observable<RxEvent>&
RxEventBus::getObservable() const {
    return subject.get_observable();
}

void
RxEventModule::attach(RxEventBus& bus) {
    detach();

    subscription = rxcpp::composite_subscription();

    auto handle = [&](const RxEvent& event) {
        handlers[event.getEventName()](event);
    };

    auto filter = [&](const RxEvent& event) {
        auto itr = handlers.constFind(event.getEventName());

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
RxEventModule::subscribe(const QString &eventName, EventHandler handler) {
    handlers.insert(eventName, handler);
}

void
RxEventModule::unsubscribe(const QString& eventName) {
    handlers.remove(eventName);
}
