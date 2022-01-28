#ifndef CRC8ALOGS_H
#define CRC8ALOGS_H

// qt
#include <QByteArray>
#include <QtGlobal>

quint8 calcCrc8Maxim(const QByteArray bytes, quint8 crc8 = 0);

#endif // CRC8ALOGS_H
