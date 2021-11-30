TARGET = utils

QT -= qt

TEMPLATE = lib

CONFIG += c++17

message(_utils proj dir: $${PWD})

DESTDIR     = $${PWD}/../binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

message(_utils dest dir: $${DESTDIR})

HEADERS += \
    casting.h

target.path += /usr/lib
INSTALLS += target
