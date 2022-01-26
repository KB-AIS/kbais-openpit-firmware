#ifndef QBYTEARRAYEXT_H
#define QBYTEARRAYEXT_H

// std
#include <vector>
// qt
#include <QByteArray>

inline QByteArray fromStdVector(std::vector<unsigned char>&& source) {
    return QByteArray(
        reinterpret_cast<const char*>(source.data())
    ,   source.size()
    );
}

#endif // QBYTEARRAYEXT_H
