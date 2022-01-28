PRJDIR = $$PWD/..

TARGET = OpenPitModulesSensorsGps

QT += core serialport

TEMPLATE = lib

QMAKE_CXXFLAGS += -Wno-psabi

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \

include($$PRJDIR/../conanbuildinfo.pri)

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

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
