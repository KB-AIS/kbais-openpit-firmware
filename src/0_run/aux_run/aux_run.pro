PRJDIR = $${PWD}/../../
OSSDIR = $${PRJDIR}/../oss/

TARGET = aux-run

QT += core network sql widgets

CONFIG += c++14 c++17 conan_basic_setup

INCLUDEPATH += \
    # cfw
    $${PRJDIR}/1_infra/gps_sensor_sp \
    $${PRJDIR}/1_inter/aux_view

include($${OSSDIR}/QDeferred/src/qdeferred.pri)
include($${OSSDIR}/QDeferred/src/qlambdathreadworker.pri)
include($${OSSDIR}/libgitlevtbus/src/ebus.pri)
include($${PRJDIR}/../conanbuildinfo.pri)

HEADERS += \
    aux_immediate_messages_mapper.h \
    aux_recurrent_messages_mapper.h \
    core/notification_handlers/setup_time_handler.h \
    database_configuration.h \
    device_message.h \
    immediate_messages_collector.h \
    messages_caching_service.h \
    messages_collectors_adapter.h \
    msgcaching/get_message_batches_query.h \
    networking/base_message_senders_manager.h \
    networking/base_protocol_communicator.h \
    networking/base_protocol_formatter.h \
    networking/message_sender.h \
    networking/message_senders_manager.h \
    networking/swom_protocol_communicator.h \
    recurrent_messages_collector.h \
    save_device_messages_command.h

SOURCES += \
    aux_immediate_messages_mapper.cpp \
    aux_recurrent_messages_mapper.cpp \
    core/notification_handlers/setup_time_handler.cpp \
    database_configuration.cpp \
    immediate_messages_collector.cpp \
    main.cpp \
    messages_caching_service.cpp \
    messages_collectors_adapter.cpp \
    msgcaching/get_message_batches_query.cpp \
    networking/base_protocol_formatter.cpp \
    networking/message_sender.cpp \
    networking/message_senders_manager.cpp \
    networking/swom_protocol_communicator.cpp \
    recurrent_messages_collector.cpp \
    save_device_messages_command.cpp

LIBS += -L$${PRJDIR}/binaries \
    -laux-view \
    -lgps-sensor-sp

message(0_run/aux_run proj dir: $${PWD})

DESTDIR     = $${PRJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

message(0_run/aux_run dest dir: $${DESTDIR})

target.path += /usr/sbin
INSTALLS += target
