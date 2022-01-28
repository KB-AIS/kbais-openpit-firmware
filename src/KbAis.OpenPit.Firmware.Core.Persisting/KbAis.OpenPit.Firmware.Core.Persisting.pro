PRJDIR = $$PWD/..

TARGET = OpenPitCorePersisting

QT += core sql

TEMPLATE = lib

QMAKE_CXXFLAGS += -Wno-psabi

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

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
