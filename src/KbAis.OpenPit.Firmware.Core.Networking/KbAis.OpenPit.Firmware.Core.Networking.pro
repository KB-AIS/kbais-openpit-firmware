PRJDIR = $$PWD/..

TARGET = OpenPitCoreNetworking

TEMPLATE = lib

QT += core network \

CONFIG += staticlib conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration/ \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions/ \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.JsonQt/ \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt/ \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Networking/Communicators/Swom/SwomProtocolCommunicator.h \
    Networking/Communicators/Swom/SwomProtocolFormatter.h \
    Networking/Communicators/IProtocolCommunicator.h \
    Networking/Confguration/TcpMessageSenderConfiguration.h \
    Networking/Diagnosing/IRxMessageSendersDiagPub.h \
    Networking/MessageSenders/IMessageSendersManager.h \
    Networking/MessageSenders/TcpMessageSender.h \
    Networking/MessageSenders/TcpMessageSendersManager.h \
    Networking/NetworkingModuleFactory.h \
    Networking/NetworkingThreadWorker.h \

SOURCES += \
    Networking/Communicators/Swom/SwomProtocolCommunicator.cpp \
    Networking/Communicators/Swom/SwomProtocolFormatter.cpp \
    Networking/MessageSenders/TcpMessageSender.cpp \
    Networking/MessageSenders/TcpMessageSendersManager.cpp \
    Networking/NetworkingThreadWorker.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
