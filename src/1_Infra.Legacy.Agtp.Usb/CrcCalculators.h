#ifndef CRCCALCULATORS_H
#define CRCCALCULATORS_H

// qt
#include <QtGlobal>

unsigned short CRC16(unsigned char * pcBlock, unsigned int len, unsigned short crc);

unsigned char CRC_1wire(const unsigned char* crcbuff, quint32 crcLen, unsigned char startvalue);

#endif // CRCCALCULATORS_H
