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
include($${PRJDIR}/../conanbuildinfo.pri)

HEADERS += \
    RxEventBus.h \
    caching/messages_caching_service.h \
    messaging/aux_immediate_messages_map_service.h \
    messaging/aux_recurrent_messages_map_service.h \
    messaging/collectors/immediate_messages_collector.h \
    messaging/collectors/messages_collectors_adapter.h \
    messaging/collectors/recurrent_messages_collector.h \
    messaging/mappers/mapper_gps_update.h \
    messaging/message.h \
    messaging/messages_batch.h \
    networking/communicators/base_protocol_communicator.h \
    networking/communicators/messages_batches_queue.h \
    networking/communicators/swom_protocol_communicator.h \
    networking/communicators/swom_protocol_formatter.h \
    networking/senders/base_message_senders_manager.h \
    networking/senders/message_sender.h \
    networking/senders/tcp_message_senders_manager.h \
    persisting/commands/save_messages_batch_command.h \
    persisting/commands/set_last_sent_messages_batch_id_command.h \
    persisting/configuration/database_configuration.h \
    persisting/queries/dtos.h \
    persisting/queries/get_messages_batches_query.h \
    system/notification_handlers/setup_datetime_handler.h \
    utils/boost_di_extensions.h \
    utils/json_qt_support.h

SOURCES += \
    RxEventBus.cpp \
    caching/messages_caching_service.cpp \
    main.cpp \
    messaging/aux_immediate_messages_map_service.cpp \
    messaging/aux_recurrent_messages_map_service.cpp \
    messaging/collectors/immediate_messages_collector.cpp \
    messaging/collectors/messages_collectors_adapter.cpp \
    messaging/collectors/recurrent_messages_collector.cpp \
    networking/communicators/messages_batches_queue.cpp \
    networking/communicators/swom_protocol_communicator.cpp \
    networking/communicators/swom_protocol_formatter.cpp \
    networking/senders/message_sender.cpp \
    networking/senders/tcp_message_senders_manager.cpp \
    persisting/commands/save_messages_batch_command.cpp \
    persisting/commands/set_last_sent_messages_batch_id_command.cpp \
    persisting/configuration/database_configuration.cpp \
    persisting/queries/get_messages_batches_query.cpp \
    system/notification_handlers/setup_datetime_handler.cpp

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
