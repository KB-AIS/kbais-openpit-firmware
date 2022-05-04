#ifndef RUNNER_CORE_PERSISTING_H
#define RUNNER_CORE_PERSISTING_H

#include "Core/Persisting/i_message_caching_service.h"
#include "rxqt.h"

class runner_core_persisting : QObject {

    QThread thr_worker_;

    std::unique_ptr<rxqt::run_loop> thr_run_loop_;

public:
    runner_core_persisting(i_message_caching_service& messages_caching_service);

};

#endif // RUNNER_CORE_PERSISTING_H
