PRJDIR = $${PWD}/..

TARGET = cfw_utils

QT -= qt

TEMPLATE = lib

CONFIG += 17

HEADERS += \
    Extensions/QByteArrayExt.h \
    Casting.h \
    Crc16Alogs.h \
    Crc8Alogs.h \

SOURCES += \
    Crc16Alogs.cpp \
    Crc8Alogs.cpp \

DESTDIR     = $${PRJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

target.path += /usr/lib
INSTALLS += target
