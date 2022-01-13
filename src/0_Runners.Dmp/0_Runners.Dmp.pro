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
    Messaging/Collectors/ImmediateMessagesCollector.h \
    Messaging/Collectors/MessagesCollectorsAdapter.h \
    Messaging/Collectors/RecurrentMessagesCollector.h \
    Messaging/DmpImmediateMessagesMapService.h \
    Messaging/Mappers/JsonMappers.h \
    Messaging/DmpRecurrentMessagesMapService.h \
    Messaging/Message.h \
    Messaging/MessagesBatch.h \
    Utils/BoostDiExtensions.h \

SOURCES += \
    Caching/BlockingMessagesCachingService.cpp \
    Messaging/Collectors/ImmediateMessagesCollector.cpp \
    Messaging/Collectors/MessagesCollectorsAdapter.cpp \
    Messaging/Collectors/RecurrentMessagesCollector.cpp \
    Messaging/DmpImmediateMessagesMapService.cpp \
    Messaging/DmpRecurrentMessagesMapService.cpp \
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
