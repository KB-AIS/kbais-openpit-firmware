PRJDIR = $${PWD}/..

TARGET = cfw_infra_sensors_gps

QT += core serialport

TEMPLATE = lib

CONFIG += 11 14 17 conan_basic_setup

INCLUDEPATH += \
    $${PRJDIR}/Utils \

include($${PRJDIR}/../conanbuildinfo.pri)
include($${PRJDIR}/Utils/Utils.pri)

HEADERS += \
    Nmea/NmeaParser.h \
    Nmea/NmeaSentence.h \
    GpsMessage.h \
    IRxGpsSensorPublisher.h \
    SerialRxGpsSensorPublisher.h \

SOURCES += \
    Nmea/NmeaParser.cpp \
    Nmea/NmeaSentence.cpp \
    SerialRxGpsSensorPublisher.cpp \

LIBS += -L$${PRJDIR}/binaries

DESTDIR     = $${PRJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

target.path += /usr/lib
INSTALLS += target
