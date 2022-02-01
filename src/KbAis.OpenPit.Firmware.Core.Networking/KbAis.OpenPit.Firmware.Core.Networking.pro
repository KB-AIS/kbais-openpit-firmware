PRJDIR = $$PWD/..

TARGET = OpenPitCoreNetworking

TEMPLATE = lib

QMAKE_CXXFLAGS += -Wno-psabi

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

QT += core network

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration/ \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    IMessageSendersManager.h \
#    IProtocolCommunicator.h \
#    MessagesBatchesSendQueue.h \
#    MessageSender.h \
#    SwomProtocolCommunicator.h \
#    SwomProtocolFormatter.h \
    IMessageSendersRegistry.h \
    IRxMessageSendersStatusPublisher.h \
    MessageSenderConfiguration.h \
    MessageSenderStatus.h \
    TcpMessageSendersManager.h

SOURCES += \
#    MessagesBatchesSendQueue.cpp \
#    MessageSender.cpp \
#    SwomProtocolCommunicator.cpp \
#    SwomProtocolFormatter.cpp \
    TcpMessageSendersManager.cpp

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
