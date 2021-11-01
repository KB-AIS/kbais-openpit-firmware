QT += gui serialport

TARGET = sensors-gps

TEMPLATE = lib

CONFIG += c++17

INCLUDEPATH += \
    # OSS
    # plog        -- logging library
    ../_oss/plog/include \
    # qdeferred   -- async code execution
    ../_oss/qdef/src

HEADERS += \
    gps_sensor_thread.h \
    utils/nmea_parser.h

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

SOURCES += \
    gps_sensor_thread.cpp \
    utils/nmea_parser.cpp
