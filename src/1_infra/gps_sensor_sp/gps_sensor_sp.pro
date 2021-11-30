PROJDIR = $${PWD}/../../

TARGET = gps-sensor-sp

QT += serialport

TEMPLATE = lib

CONFIG += c++17

INCLUDEPATH += \
    # carrier-firmware project
    $${PROJDIR}/2_app/gps_sensor \
    $${PROJDIR}/_utils \
    # plog
    $${PROJDIR}/../oss/plog/include

HEADERS += \
    gps_device_controller.h \
    nmea/nmea_parser.h \
    nmea/nmea_sentence.h

SOURCES += \
    gps_device_controller.cpp \
    nmea/nmea_parser.cpp \
    nmea/nmea_sentence.cpp

LIBS += -L$${PROJDIR}/binaries \
    -lutils \
    -lgps-sensor

message(1_infra/gps_sensor_sp proj dir: $${PWD})

DESTDIR     = $${PROJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

message(1_infra/gps_sensor_sp dest dir: $${DESTDIR})

target.path += /usr/lib
INSTALLS += target
