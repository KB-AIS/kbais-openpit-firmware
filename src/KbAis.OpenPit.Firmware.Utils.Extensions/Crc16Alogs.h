#ifndef CRC16ALOGS_H
#define CRC16ALOGS_H

// qt
#include <QByteArray>
#include <QtGlobal>

quint16 calcCrc16CcittFalse(const QByteArray bytes, quint16 crc16 = 0xFFFF);

#endif // CRC16ALOGS_H
