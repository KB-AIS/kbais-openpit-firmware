#ifndef IAGTPREQUESTHANDLER_H
#define IAGTPREQUESTHANDLER_H

// std
#include <vector>
// qt
#include <QString>

struct AgtpRequest {

    quint16 uid { 0 };

    QString payload;

    quint8 type { 0x00 };

};

struct AgtpResponse {

    quint16 uid { 0 };

    QByteArray bytes;

    quint8 status { 0x00 };
};

class IAgtpRequestHandler {

public:
    virtual ~IAgtpRequestHandler() noexcept = default;

    virtual AgtpResponse handle(const AgtpRequest& request) = 0;

    virtual QString get_request_name() const = 0;

};

#endif // IAGTPREQUESTHANDLER_H
