PROJDIR = $${PWD}/../../
OSSDIR = $${PROJDIR}/../oss/

TARGET = aux-run

QT += core widgets

CONFIG += c++17

INCLUDEPATH += \
    # carrier-firmware
    $${PROJDIR}/1_infra/gps_sensor_sp \
    $${PROJDIR}/1_inter/aux_view \
    $${PROJDIR}/2_app/gps_sensor \
    # Plog
    $${OSSDIR}/plog/include

include($${OSSDIR}/QDeferred/src/qdeferred.pri)
include($${OSSDIR}/QDeferred/src/qlambdathreadworker.pri)

HEADERS += \
    aux_immediate_event_mapper.h \
    aux_recurrent_event_mapper.h \
    commands/setup_time_handler.h \
    event.h \
    event_collectors_adapter.h \
    immediate_event_collector.h \
    recurrent_event_collector.h

SOURCES += \
    aux_immediate_event_mapper.cpp \
    aux_recurrent_event_mapper.cpp \
    commands/setup_time_handler.cpp \
    event_collectors_adapter.cpp \
    immediate_event_collector.cpp \
    main.cpp \
    recurrent_event_collector.cpp

LIBS += -L$${PROJDIR}/binaries \
    -laux-view \
    -lgps-sensor \
    -lgps-sensor-sp

message(0_run/aux_run proj dir: $${PWD})

DESTDIR     = $${PROJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

message(0_run/aux_run dest dir: $${DESTDIR})

target.path += /usr/sbin
INSTALLS += target
