#ifndef MODULEFACTORYNETWORKING_H
#define MODULEFACTORYNETWORKING_H

// oss
#include <boost/di.hpp>

#include "TcpMessageSendersManager.h"
#include "ThreadWorkerNetworking.h"

inline auto createDiModuleNetworking() noexcept {
    return boost::di::make_injector(
        boost::di::bind<
            IMessageSendersManager
        ,   IRxMessageSendersStatusPublisher
        ,   TcpMessageSendersManager
        >
            .to<TcpMessageSendersManager>()
            .in(boost::di::singleton)
    ,   boost::di::bind<ThreadWorkerNetworking>()
            .in(boost::di::singleton)
    );
}

#endif // MODULEFACTORYNETWORKING_H
