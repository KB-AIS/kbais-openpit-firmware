#ifndef IAGTPREQUESTHANDLER_H
#define IAGTPREQUESTHANDLER_H

// std
#include <vector>
// qt
#include <QString>

struct AgtpRequest {

    quint16 uid { 0 };

    QString payload;

    quint8 type { 0 };

};

struct AgtpResponse {

    quint16 uid { 0 };

    QByteArray bytes;

    quint8 status { 0 };
};

class IAgtpRequestHandler {

public:
    virtual ~IAgtpRequestHandler() noexcept = default;

    virtual AgtpResponse handle(const AgtpRequest& request) = 0;

    virtual QString getRequestName() const = 0;

};

#endif // IAGTPREQUESTHANDLER_H
