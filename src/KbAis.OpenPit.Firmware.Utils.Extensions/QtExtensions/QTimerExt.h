#ifndef QTIMEREXT_H
#define QTIMEREXT_H

// std
#include <chrono>
// qt
#include <QTimer>
#include <QCoreApplication>

template <class F>
inline void wait_using_timer(
    F f
,   std::chrono::milliseconds timeout = std::chrono::milliseconds(1000)
) {
    QTimer timer;
    timer.setSingleShot(true);
    timer.setTimerType(Qt::PreciseTimer);
    timer.start(timeout.count());

    while (!f() && timer.remainingTime() >= 0) {
        qApp->processEvents();
    }
}

#endif // QTIMEREXT_H
