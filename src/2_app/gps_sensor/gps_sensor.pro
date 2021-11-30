PROJDIR = $${PWD}/../../

TARGET = gps-sensor

QT += core

TEMPLATE = lib

CONFIG += c++17

HEADERS += \
    gps_update.h

message(2_app/gps_sensor proj dir: $${PWD})

DESTDIR     = $${PROJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

message(2_app/gps_sensor dest dir: $${DESTDIR})

target.path += /usr/lib
INSTALLS += target
