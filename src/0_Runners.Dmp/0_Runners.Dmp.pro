PRJDIR = $${PWD}/..

TARGET = cfw_runners_dmp

QT += core network sql widgets

CONFIG += c++11 c++14 c++17 conan_basic_setup

INCLUDEPATH += \
    $${PRJDIR}/1_Infra.Sensors.Gps \
    $${PRJDIR}/TrdParty.RxQt \

include($${PRJDIR}/../conanbuildinfo.pri)

HEADERS += \
    Messaging/Collectors/MessagesCollectorsAdapter.h \
    Messaging/Collectors/RecurrentMessagesCollector.h \
    Messaging/DmpRecurrentMessagesMapService.h \
    Messaging/Mappers/JsonMappers.h \
    Messaging/Message.h \
    Utils/BoostDiExtensions.h \
    Utils/NlohmannJsonQtSupport.h \

SOURCES += \
    Messaging/Collectors/MessagesCollectorsAdapter.cpp \
    Messaging/Collectors/RecurrentMessagesCollector.cpp \
    Messaging/DmpRecurrentMessagesMapService.cpp \
    main.cpp \

LIBS += -L$${PRJDIR}/binaries \
    -lcfw_infra_sensors_gps \
    -ltrdparty_rxqt \

DESTDIR     = $${PWD}/../binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

target.path += /usr/sbin
INSTALLS += target
