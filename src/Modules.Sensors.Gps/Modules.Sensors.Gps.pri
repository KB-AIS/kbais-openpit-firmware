
CONFIG += 11 14 17

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/Nmea/NmeaParser.h \
    $$PWD/Nmea/NmeaSentence.h \
    $$PWD/GpsMessage.h \
    $$PWD/IRxGpsSensorPublisher.h \
    $$PWD/SerialRxGpsSensorPublisher.h \

SOURCES += \
    $$PWD/Nmea/NmeaParser.cpp \
    $$PWD/Nmea/NmeaSentence.cpp \
    $$PWD/SerialRxGpsSensorPublisher.cpp \
