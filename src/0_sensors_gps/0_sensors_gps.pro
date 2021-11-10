TARGET = sensors-gps

TEMPLATE = lib

QT += serialport

CONFIG += c++17

INCLUDEPATH += \
    # project
    ../_utils \
    # oss
    ../_oss/plog/include \
    ../_oss/qdef/src

HEADERS += \
    gps_sensor_thread.h \
    nmea/nmea_parser.h

SOURCES += \
    nmea/nmea_parser.cpp \
    gps_sensor_thread.cpp

LIBS += -L$$PWD/../binaries \
    -lutils

message(0_sensors_gps project dir: $${PWD})

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

message(0_sensors_gps output dir: $${DESTDIR})

target.path += /usr/lib
INSTALLS += target
