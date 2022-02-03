#ifndef NETWORKINGMODULEFACTORY_H
#define NETWORKINGMODULEFACTORY_H

// oss
#include <boost/di.hpp>

#include "Networking/MessageSenders/TcpMessageSendersManager.h"
#include "Networking/NetworkingThreadWorker.h"

inline auto create_di_module_networking() noexcept {
    return boost::di::make_injector(
        boost::di::bind<
            IMessageSendersManager
        ,   IRxMessageSendersDiagPub
        ,   TcpMessageSendersManager
        >
            .to<TcpMessageSendersManager>()
            .in(boost::di::singleton)
    ,   boost::di::bind<NetworkingThreadWorker>()
            .in(boost::di::singleton)
    );
}

#endif // NETWORKINGMODULEFACTORY_H
