#ifndef DTOS_H
#define DTOS_H

// qt
#include <QDateTime>
#include <QVector>

struct MessageDto {
    quint64 id;

    QString moniker;

    QByteArray payload;

    QDateTime producedAt;
};

struct MessagesBatchDto {
    quint64 id;

    QVector<MessageDto> messages;

    QDateTime collectedAt;
};

#endif // DTOS_H
