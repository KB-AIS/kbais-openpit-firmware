#ifndef CACHINGBOOTSTRAPPER_H
#define CACHINGBOOTSTRAPPER_H

#include "RxQt.h"

#include "Core/Persisting/IMessagesCachingService.h"

class CachingBootstrapper {

public:
    CachingBootstrapper(IMessagesCachingService& messagesCachingService);

private:
    rxqt::RunLoopThread m_workingThread;
};

#endif // CACHINGBOOTSTRAPPER_H
