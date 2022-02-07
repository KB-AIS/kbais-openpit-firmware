PRJDIR = $$PWD/..

TARGET = OpenPitCoreMessaging

TEMPLATE = lib

QT += core

CONFIG += staticlib depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.JsonQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Collectors/ImmediateMessagesCollector.h \
    Collectors/MessagesCollectorsAdapter.h \
    Collectors/RecurrentMessagesCollector.h \
    IRxImmediateMessageMapper.h \
    IRxRecurrentMessageMapper.h \
    Mappers/JsonMappers.h \
    Message.h \
    MessagesBatch.h \
    ThreadWorkerMessaging.h

SOURCES += \
    Collectors/ImmediateMessagesCollector.cpp \
    Collectors/MessagesCollectorsAdapter.cpp \
    Collectors/RecurrentMessagesCollector.cpp \
    ThreadWorkerMessaging.cpp

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
