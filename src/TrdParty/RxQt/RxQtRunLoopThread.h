#ifndef RXQTRUNLOOPTHREAD_H
#define RXQTRUNLOOPTHREAD_H

#include "RxQt/RxQtRunLoop.h"

namespace rxqt {

class RunLoopThread : public QThread {

public:
    using QThread::QThread;

    rxqt::run_loop* runLoop() const {
        return m_runLoop.get();
    }

protected:
    void run() override {
        m_runLoop = std::make_unique<rxqt::run_loop>();
        QThread::run();
        m_runLoop = nullptr;
    }

private:
    std::unique_ptr<rxqt::run_loop> m_runLoop;

};

} // rxqt

#endif // RXQTRUNLOOPTHREAD_H
