PRJDIR = $${PWD}/../../
OSSDIR = $${PRJDIR}/../oss/

TARGET = gps-sensor-sp

QT += core serialport

TEMPLATE = lib

CONFIG += c++17

INCLUDEPATH += \
    # cfw
    $${PRJDIR}/_utils \
    # oss
    $${OSSDIR}/plog/include

HEADERS += \
    gps_device_controller.h \
    gps_update.h \
    nmea/nmea_parser.h \
    nmea/nmea_sentence.h

SOURCES += \
    gps_device_controller.cpp \
    nmea/nmea_parser.cpp \
    nmea/nmea_sentence.cpp

LIBS += -L$${PRJDIR}/binaries \
    -lutils

message(1_infra/gps_sensor_sp proj dir: $${PWD})

DESTDIR     = $${PRJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

message(1_infra/gps_sensor_sp dest dir: $${DESTDIR})

target.path += /usr/lib
INSTALLS += target
