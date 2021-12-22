PRJDIR = $${PWD}/../../
OSSDIR = $${PRJDIR}/../oss/

TARGET = aux-run

QT += core network sql widgets

CONFIG += c++11 c++14 c++17 conan_basic_setup

INCLUDEPATH += \
    # cfw
    $${PRJDIR}/1_infra/gps_sensor_sp \
    $${PRJDIR}/1_inter/aux_view \

include($${OSSDIR}/QDeferred/src/qdeferred.pri)
include($${OSSDIR}/QDeferred/src/qlambdathreadworker.pri)
include($${OSSDIR}/libgitlevtbus/src/ebus.pri)
include($${PRJDIR}/../conanbuildinfo.pri)

HEADERS += \
    caching/messages_caching_service.h \
    database/commands/save_messages_batch_command.h \
    database/configuration/database_configuration.h \
    database/queries/get_messages_batches_query.h \
    messaging/aux_immediate_messages_map_service.h \
    messaging/aux_recurrent_messages_map_service.h \
    messaging/collectors/immediate_messages_collector.h \
    messaging/collectors/messages_collectors_adapter.h \
    messaging/collectors/recurrent_messages_collector.h \
    messaging/mappers/mapper_gps_update.h \
    messaging/message.h \
    messaging/messages_batch.h \
#    networking/communicators/base_protocol_communicator.h \
#    networking/communicators/swom_protocol_communicator.h \
#    networking/formatters/base_protocol_formatter.h \
#    networking/formatters/swom_protocol_formatter.h \
#    networking/senders/base_message_senders_manager.h \
#    networking/senders/message_sender.h \
#    networking/senders/message_senders_manager.h \
    system/notification_handlers/setup_time_handler.h \
    utils/boost_di_extensions.h \
    utils/mapper_qt_types.h \
    utils/spdlog_qt_support.h

SOURCES += \
    caching/messages_caching_service.cpp \
    database/commands/save_messages_batch_command.cpp \
    database/configuration/database_configuration.cpp \
    database/queries/get_messages_batches_query.cpp \
    main.cpp \
    messaging/aux_immediate_messages_map_service.cpp \
    messaging/aux_recurrent_messages_map_service.cpp \
    messaging/collectors/immediate_messages_collector.cpp \
    messaging/collectors/messages_collectors_adapter.cpp \
    messaging/collectors/recurrent_messages_collector.cpp \
#    networking/communicators/swom_protocol_communicator.cpp \
#    networking/formatters/swom_protocol_formatter.cpp \
#    networking/senders/message_sender.cpp \
#    networking/senders/message_senders_manager.cpp \
    system/notification_handlers/setup_time_handler.cpp \
    utils/spdlog_qt_support.cpp

LIBS += -L$${PRJDIR}/binaries \
    -laux-view \
    -lgps-sensor-sp \

message(0_run/aux_run project dir: $${PWD})

DESTDIR     = $${PRJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

message(0_run/aux_run destination dir: $${DESTDIR})

target.path += /usr/sbin
INSTALLS += target
