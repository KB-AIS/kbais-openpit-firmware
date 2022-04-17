#ifndef MODULEFACTORYNETWORKING_H
#define MODULEFACTORYNETWORKING_H

// oss
#include <boost/di.hpp>

#include "Core/Networking/MessageSenders/TcpMessageSendersManager.h"
#include "Core/Networking/ModuleBootstraperNetworking.h"

inline auto create_core_networking() noexcept {
    return boost::di::make_injector(
        boost::di::bind<IMessageSendersManager, IRxMessageSendersDiagPub>
            .to<TcpMessageSendersManager>()
            .in(boost::di::singleton)
    ,   boost::di::bind<ModuleBootstraperNetworking>()
            .in(boost::di::singleton)
    );
}

#endif // MODULEFACTORYNETWORKING_H
