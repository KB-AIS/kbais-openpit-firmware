PRJDIR = $$PWD/..

TARGET = OpenPitCoreMessaging

TEMPLATE = lib

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQT \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Collectors/ImmediateMessagesCollector.h \
    Collectors/MessagesCollectorsAdapter.h \
    Collectors/RecurrentMessagesCollector.h \
    Mappers/JsonMappers.h \
    Message.h \
    MessagesBatch.h \

SOURCES += \
    Collectors/ImmediateMessagesCollector.cpp \
    Collectors/MessagesCollectorsAdapter.cpp \
    Collectors/RecurrentMessagesCollector.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
