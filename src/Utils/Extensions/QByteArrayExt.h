#ifndef QBYTEARRAYEXT_H
#define QBYTEARRAYEXT_H

// std
#include <vector>
// qt
#include <QByteArray>

inline QByteArray fromStdVector(std::vector<unsigned char>&& source) {
    auto byteArray = QByteArray::fromRawData(
        reinterpret_cast<const char*>(source.data()),
        source.size()
    );

    return byteArray;
}

#endif // QBYTEARRAYEXT_H
