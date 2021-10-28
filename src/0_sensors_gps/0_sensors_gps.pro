QT += core

TARGET = sensors-gps

TEMPLATE = lib

CONFIG += c++17

HEADERS += \
    hello_world.h

message(0_sensors_gps project dir: $${PWD})

# Artifact direcotires configuration
DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

message(0_sensors_gps output dir: $${DESTDIR})

target.path += /usr/lib
INSTALLS += target
