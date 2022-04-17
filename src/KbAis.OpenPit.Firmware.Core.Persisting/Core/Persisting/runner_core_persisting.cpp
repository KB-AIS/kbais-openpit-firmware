#include "runner_core_persisting.h"

#include "Core/Persisting/Configuration/DatabaseConfigurator.h"
#include <plog/Log.h>

QString WORKER_THREAD_NAME { QStringLiteral("opf.core.persisting") };

runner_core_persisting::runner_core_persisting(
    i_message_caching_service& message_caching_service
)
    :   thr_worker_ { this }
{
    DatabaseConfigurator::configure();

    thr_worker_.setObjectName(WORKER_THREAD_NAME);

    QObject::connect(&thr_worker_, &QThread::started, [&]() {
        thr_run_loop_ = std::make_unique<rxqt::run_loop>();

        const auto coordination = thr_run_loop_->observe_on_run_loop();

        message_caching_service.start_working_on(coordination);
    });

    QObject::connect(&thr_worker_, &QThread::finished, [&]() {
        thr_run_loop_ = nullptr;
    });

    QObject::connect(&thr_worker_, &QThread::finished, &thr_worker_, &QThread::deleteLater);

    thr_worker_.start();
}
