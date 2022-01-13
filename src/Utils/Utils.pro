PRJDIR = $${PWD}/..

TARGET = cfw_utils

QT -= qt

TEMPLATE = lib

CONFIG += c++17

HEADERS += \
    Casting.h \
    Extensions/QByteArrayExt.h

DESTDIR     = $${PRJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

target.path += /usr/lib
INSTALLS += target
