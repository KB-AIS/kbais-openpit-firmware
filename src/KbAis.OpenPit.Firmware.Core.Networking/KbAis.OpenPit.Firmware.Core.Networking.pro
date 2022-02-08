PRJDIR = $$PWD/..

TARGET = OpenPitCoreNetworking

TEMPLATE = lib

QT += core network sql \

CONFIG += staticlib conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration/ \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Persisting/ \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions/ \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.JsonQt/ \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt/ \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Core/Networking/Communicators/Swom/SwomProtocolCommunicator.h \
    Core/Networking/Communicators/Swom/SwomProtocolFormatter.h \
    Core/Networking/Communicators/IProtocolCommunicator.h \
    Core/Networking/Confguration/TcpMessageSenderConfiguration.h \
    Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h \
    Core/Networking/MessageSenders/IMessageSendersManager.h \
    Core/Networking/MessageSenders/TcpMessageSender.h \
    Core/Networking/MessageSenders/TcpMessageSendersManager.h \
    Core/Networking/ModuleBootstraperNetworking.h \
    Core/Networking/ModuleFactoryNetworking.h \

SOURCES += \
    Core/Networking/Communicators/Swom/SwomProtocolCommunicator.cpp \
    Core/Networking/Communicators/Swom/SwomProtocolFormatter.cpp \
    Core/Networking/MessageSenders/TcpMessageSender.cpp \
    Core/Networking/MessageSenders/TcpMessageSendersManager.cpp \
    Core/Networking/ModuleBootstraperNetworking.cpp

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
