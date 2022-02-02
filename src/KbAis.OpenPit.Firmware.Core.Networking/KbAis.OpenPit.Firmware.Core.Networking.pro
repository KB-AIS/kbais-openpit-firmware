PRJDIR = $$PWD/..

TARGET = OpenPitCoreNetworking

TEMPLATE = lib

QT += core network \

CONFIG += conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration/ \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.JsonQt/ \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    IMessageSendersManager.h \
    IProtocolCommunicator.h \
#    MessagesBatchesSendQueue.h \
#    SwomProtocolCommunicator.h \
#    SwomProtocolFormatter.h \
    IMessageSendersRegistry.h \
    IRxMessageSendersStatusPublisher.h \
    MessageSenderConfiguration.h \
    ModuleFactoryNetworking.h \
    TcpMessageSender.h \
    TcpMessageSendersManager.h

SOURCES += \
#    MessagesBatchesSendQueue.cpp \
#    SwomProtocolCommunicator.cpp \
#    SwomProtocolFormatter.cpp \
    TcpMessageSender.cpp \
    TcpMessageSendersManager.cpp

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
