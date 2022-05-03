#ifndef RUNNER_STATE_WATCHER_H
#define RUNNER_STATE_WATCHER_H

#include <QtCore/QObject>

class i_work_runner {

public:
    virtual ~i_work_runner() noexcept = default;

    virtual void start() = 0;

};

class runner_state_watcher : public QObject, public i_work_runner {

public:
    runner_state_watcher();

    void start() override;
};

#endif // RUNNER_STATE_WATCHER_H
