PRJDIR = $$PWD/..

TARGET = OpenPitCorePersisting

TEMPLATE = lib

QT += core sql \

CONFIG += staticlib depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.EventBus \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Messaging \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Commands/InsertMessagesBatchCmd.h \
    Commands/UpdateSenderCmd.h \
    Configuration/DatabaseConfigurator.h \
    Queries/Dtos.h \
    Queries/SelectMessagesBatchesQry.h \
    BlockingMessagesCachingService.h \
    IMessagesCachingService.h \

SOURCES += \
    Commands/InsertMessagesBatchCmd.cpp \
    Commands/UpdateSenderCmd.cpp \
    Configuration/DatabaseConfigurator.cpp \
    Queries/SelectMessagesBatchesQry.cpp \
    BlockingMessagesCachingService.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
