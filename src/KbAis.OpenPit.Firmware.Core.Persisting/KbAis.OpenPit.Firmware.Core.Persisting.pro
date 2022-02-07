PRJDIR = $$PWD/..

TARGET = OpenPitCorePersisting

TEMPLATE = lib

QT += core sql \

CONFIG += staticlib depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.EventBus \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Messaging \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Core/Persisting/CachingBootstrapper.h \
    Core/Persisting/BlockingMessagesCachingService.h \
    Core/Persisting/Commands/InsertMessagesBatchCmd.h \
    Core/Persisting/Commands/UpdateSenderCmd.h \
    Core/Persisting/Configuration/DatabaseConfigurator.h \
    Core/Persisting/IMessagesCachingService.h \
    Core/Persisting/Queries/Dtos.h \
    Core/Persisting/Queries/SelectMessagesBatchesQry.h \

SOURCES += \
    Core/Persisting/CachingBootstrapper.cpp \
    Core/Persisting/BlockingMessagesCachingService.cpp \
    Core/Persisting/Commands/InsertMessagesBatchCmd.cpp \
    Core/Persisting/Commands/UpdateSenderCmd.cpp \
    Core/Persisting/Configuration/DatabaseConfigurator.cpp \
    Core/Persisting/Queries/SelectMessagesBatchesQry.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
