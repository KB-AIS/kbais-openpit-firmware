PRJDIR = $${PWD}/..

QT += core serialport

CONFIG += 11 14 17 conan_basic_setup

TEMPLATE = lib

include($${PRJDIR}/../conanbuildinfo.pri)
include($${PRJDIR}/TrdParty/TrdParty.pri)

HEADERS += \
    Handlers/FetchDeviceInfoCommandHandler.h \
    Handlers/FetchFullDeviceInfoCommandHandler.h \
    Handlers/FetchSupportModulesCommandHandler.h \
    Handlers/IAgtpCommandHandler.h \
    AgtpProtocolSerializer.h \
    CrcCalculators.h \
    IAgtpCommandsMediator.h \
    IAgtpCommandsReciever.h \
    UsbAgtpCommandsReciever.h \

SOURCES += \
    Handlers/FetchDeviceInfoCommandHandler.cpp \
    Handlers/FetchFullDeviceInfoCommandHandler.cpp \
    Handlers/FetchSupportModulesCommandHandler.cpp \
    AgtpProtocolSerializer.cpp \
    CrcCalculators.cpp \
    UsbAgtpCommandsReciever.cpp \
