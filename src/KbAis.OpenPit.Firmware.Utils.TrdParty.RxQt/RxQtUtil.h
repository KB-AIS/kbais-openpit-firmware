#ifndef RXQTUTIL_H
#define RXQTUTIL_H

#include <QObject>
#include <rxcpp/rx.hpp>

namespace rxqt {

struct add_to {

    explicit add_to(const QObject* qobject)
        : qobject(qobject) {};

    rxcpp::composite_subscription operator()(rxcpp::composite_subscription source) const {
        QObject::connect(qobject, &QObject::destroyed, [=]() {
            source.unsubscribe();
        });
        return source;
    }

private:
    const QObject* qobject;

};

inline auto operator|(rxcpp::composite_subscription source, const add_to& func) {
    return func(source);
}

} // rxqt

#endif // RXQTUTIL_H
