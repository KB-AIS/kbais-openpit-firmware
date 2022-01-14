PRJDIR = $${PWD}/..

TARGET = cfw_runners_dmp

QT += core network sql widgets

CONFIG += 11 14 17 conan_basic_setup

INCLUDEPATH += \
    $${PRJDIR}/1_Infra.EventBus \
    $${PRJDIR}/1_Infra.Sensors.Gps \
    $${PRJDIR}/TrdParty \
    $${PRJDIR}/Utils \

include($${PRJDIR}/../conanbuildinfo.pri)

HEADERS += \
    Caching/BlockingMessagesCachingService.h \
    Caching/IMessagesCachingService.h \
    CompositionRootFactory.h \
    LoggerConfigurator.h \
    Messaging/Collectors/ImmediateMessagesCollector.h \
    Messaging/Collectors/MessagesCollectorsAdapter.h \
    Messaging/Collectors/RecurrentMessagesCollector.h \
    Messaging/DmpImmediateMessagesMapService.h \
    Messaging/Mappers/JsonMappers.h \
    Messaging/DmpRecurrentMessagesMapService.h \
    Messaging/Message.h \
    Messaging/MessagesBatch.h \
    Networking/Communicators/IProtocolCommunicator.h \
    Networking/Communicators/MessagesBatchesSendQueue.h \
    Networking/Communicators/SwomProtocolCommunicator.h \
    Networking/Communicators/SwomProtocolFormatter.h \
    Networking/Senders/IMessageSendersManager.h \
    Networking/Senders/MessageSender.h \
    Networking/Senders/TcpMessageSendersManager.h \
    Persisting/Commands/InsertMessagesBatchCmd.h \
    Persisting/Commands/UpdateSenderCmd.h \
    Persisting/Configuration/DatabaseConfigurator.h \
    Persisting/Queries/Dtos.h \
    Persisting/Queries/SelectMessagesBatchesQry.h \
    System/Services/ISystemService.h \
    System/Services/SetupDatetimeHandler.h \
    Utils/BoostDiExtensions.h

SOURCES += \
    Caching/BlockingMessagesCachingService.cpp \
    LoggerConfigurator.cpp \
    Messaging/Collectors/ImmediateMessagesCollector.cpp \
    Messaging/Collectors/MessagesCollectorsAdapter.cpp \
    Messaging/Collectors/RecurrentMessagesCollector.cpp \
    Messaging/DmpImmediateMessagesMapService.cpp \
    Messaging/DmpRecurrentMessagesMapService.cpp \
    Networking/Communicators/MessagesBatchesSendQueue.cpp \
    Networking/Communicators/SwomProtocolCommunicator.cpp \
    Networking/Communicators/SwomProtocolFormatter.cpp \
    Networking/Senders/MessageSender.cpp \
    Networking/Senders/TcpMessageSendersManager.cpp \
    Persisting/Commands/InsertMessagesBatchCmd.cpp \
    Persisting/Commands/UpdateSenderCmd.cpp \
    Persisting/Configuration/DatabaseConfigurator.cpp \
    Persisting/Queries/SelectMessagesBatchesQry.cpp \
    System/Services/SetupDatetimeHandler.cpp \
    main.cpp \

LIBS += -L$${PRJDIR}/binaries \
    -lcfw_infra_eventbus \
    -lcfw_infra_sensors_gps \
    -ltrdparty \
    -lcfw_utils \

DESTDIR     = $${PWD}/../binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

target.path += /usr/sbin
INSTALLS += target
