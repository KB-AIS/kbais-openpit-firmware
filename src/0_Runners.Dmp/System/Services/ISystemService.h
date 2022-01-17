#ifndef ISYSTEMSERVICE_H
#define ISYSTEMSERVICE_H

class ISystemService {

public:
    virtual ~ISystemService() noexcept = default;

    virtual void start() = 0;

    virtual void stop() = 0;

};

#endif // ISYSTEMSERVICE_H
