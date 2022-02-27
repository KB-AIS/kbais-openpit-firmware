PRJDIR = $$PWD/..

TARGET = OpenPitModulesSensorsGps

TEMPLATE = lib

QT += core serialport \

CONFIG += staticlib depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    GpioUtils.h \
    Nmea/NmeaParser.h \
    Nmea/NmeaSentence.h \
    GpsMessage.h \
    IRxGpsSensorPublisher.h \
    SerialRxGpsSensorPublisher.h \

SOURCES += \
    GpioUtils.cpp \
    Nmea/NmeaParser.cpp \
    Nmea/NmeaSentence.cpp \
    SerialRxGpsSensorPublisher.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
